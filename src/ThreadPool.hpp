/*
** EPITECH PROJECT, 2019
** zia
** File description:
** ThreadPool.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include <atomic>
#include <exception>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

namespace detail
{
	template <typename T>
	class Queue
	{
	public:
		bool push(T const &value) {
			std::unique_lock<std::mutex> lock(m_mutex);
			m_q.push(value);
			return true;
		}

		// deletes the retrieved element, do not use for non integral types
		bool pop(T &v) {
			std::unique_lock<std::mutex> lock(m_mutex);
			if (m_q.empty())
				return false;
			v = m_q.front();
			m_q.pop();
			return true;
		}

		bool empty() {
			std::unique_lock<std::mutex> lock(m_mutex);
			return m_q.empty();
		}

	private:
		std::queue<T> m_q;
		std::mutex m_mutex;
	};
}

////////////////////////////////////////////////////////////////////////////////

class ThreadPool
{
public:
	ThreadPool()
	: m_nWaiting(0)
	, m_isDone(false)
	, m_isStop(false)
	{}

	ThreadPool(int nThreads)
	: ThreadPool()
	{
		resize(nThreads);
	}

	// the destructor waits for all the functions in the queue to be finished
	~ThreadPool()
	{
		stop(true);
	}

	////////////////////////////////////////////////////////////////////////

	// get the number of running threads in the pool
	size_t size() {
		return m_threads.size();
	}

	// number of idle threads
	size_t idleCount() {
		return m_nWaiting;
	}

	std::thread &getThread(size_t i) {
		return *m_threads[i];
	}

	// change the number of threads in the pool
	// should be called from one thread, otherwise be careful to not interleave, also with stop()
	// nThreads must be >= 0
	void resize(size_t newSize)
	{
		if (m_isStop || m_isDone)
			return;

		size_t size = m_threads.size();
		if (newSize == size)
			return;

		if (newSize > size) {
			m_threads.resize(newSize);
			m_flags.resize(newSize);

			for (size_t i = size; i < newSize; ++i) {
				m_flags[i] = std::make_shared<std::atomic<bool>>(false);
				setThread(i);
			}
		}
		else {
			for (size_t i = size - 1; i >= newSize; --i) {
				*m_flags[i] = true;  // this thread will finish
				m_threads[i]->detach();
			}
			{
				// stop the detached threads that were waiting
				std::unique_lock<std::mutex> lock(m_mutex);
				m_cv.notify_all();
			}
			m_threads.resize(newSize); // safe to delete because the threads are detached
			m_flags.resize(newSize); // safe to delete because the threads have copies of shared_ptr of the flags, not originals
		}
	}

	// empty the queue
	void clearQueue()
	{
		std::function<void(int id)> *_f;
		while (m_q.pop(_f))
			delete _f; // empty the queue
	}

	// pops a functional wrapper to the original function
	std::function<void(int)> pop()
	{
		std::function<void(int id)> *_f = nullptr;
		m_q.pop(_f);
		std::unique_ptr<std::function<void(int id)>> func(_f); // at return, delete the function even if an exception occurred
		std::function<void(int)> f;
		if (_f)
			f = *_f;
		return f;
	}

	// wait for all computing threads to finish and stop all threads
	// may be called asynchronously to not pause the calling thread while waiting
	// if isWait == true, all the functions in the queue are run, otherwise the queue is cleared without running the functions
	void stop(bool isWait = false)
	{
		if (!isWait) {
			if (m_isStop)
				return;
			m_isStop = true;
			for (size_t i = 0, n = size(); i < n; ++i) {
				// command the threads to stop
				*m_flags[i] = true;
			}
			// empty the queue
			clearQueue();
		}
		else {
			if (m_isDone || m_isStop)
				return;
			// give the waiting threads a command to finish
			m_isDone = true;
		}

		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.notify_all();  // stop all waiting threads
		}

		// wait for the computing threads to finish
		for (size_t i = 0; i < m_threads.size(); ++i) {
			if (m_threads[i]->joinable())
				m_threads[i]->join();
		}

		// if there were no threads in the pool but some functors in the queue,
		// the functors are not deleted by the threads therefore delete them here
		clearQueue();
		m_threads.clear();
		m_flags.clear();
	}

	template<typename F, typename... Rest>
	auto push(F &&f, Rest&&... rest) -> std::future<decltype(f(0, rest...))>
	{
		auto pck = std::make_shared<std::packaged_task<decltype(f(0, rest...))(int)>>(
			std::bind(std::forward<F>(f), std::placeholders::_1, std::forward<Rest>(rest)...)
		);
		auto _f = new std::function<void(int id)>([pck](int id) {
			(*pck)(id);
		});
		m_q.push(_f);
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.notify_one();
		return pck->get_future();
	}

	// run the user's function that excepts argument `int id` of the running thread. returned value is templatized
	// operator returns std::future, where the user can get the result and rethrow the catched exceptions
	template<typename F>
	auto push(F &&f) -> std::future<decltype(f(0))>
	{
		auto pck = std::make_shared<std::packaged_task<decltype(f(0))(int)>>(std::forward<F>(f));
		auto _f = new std::function<void(int id)>([pck](int id) {
			(*pck)(id);
		});
		m_q.push(_f);
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.notify_one();
		return pck->get_future();
	}


private:
	ThreadPool(const ThreadPool &);// = delete;
	ThreadPool(ThreadPool &&);// = delete;
	ThreadPool & operator=(const ThreadPool &);// = delete;
	ThreadPool & operator=(ThreadPool &&);// = delete;

	void setThread(int i)
	{
		// a copy of the shared ptr to the flag
		std::shared_ptr<std::atomic<bool>> flag(m_flags[i]);

		auto f = [this, i, flag/* a copy of the shared ptr to the flag */]() {
			std::atomic<bool> &_flag = *flag;
			std::function<void(int id)> *_f;
			bool isPop = m_q.pop(_f);
			while (true) {
				// if there is anything in the queue
				while (isPop) {
					// at return, delete the function even if an exception occurred
					std::unique_ptr<std::function<void(int id)>> func(_f);
					(*_f)(i);
					if (_flag)
						return; // the thread is wanted to stop, return even if the queue is not empty yet
					else
						isPop = m_q.pop(_f);
				}
				// the queue is empty here, wait for the next command
				std::unique_lock<std::mutex> lock(m_mutex);
				m_nWaiting++;
				m_cv.wait(lock, [this, &_f, &isPop, &_flag]() {
					isPop = m_q.pop(_f);
					return isPop || m_isDone || _flag;
				});
				m_nWaiting--;
				if (!isPop)
					return; // if the queue is empty and isDone == true or *flag then return
			}
		};
		m_threads[i].reset(new std::thread(f));
	}

private:
	std::vector<std::unique_ptr<std::thread>> m_threads;
	std::vector<std::shared_ptr<std::atomic<bool>>> m_flags;
	detail::Queue<std::function<void(int id)> *> m_q;
	std::atomic<size_t> m_nWaiting;
	std::atomic<bool> m_isDone;
	std::atomic<bool> m_isStop;

	std::mutex m_mutex;
	std::condition_variable m_cv;
};