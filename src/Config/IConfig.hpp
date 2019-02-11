/*
** EPITECH PROJECT, 2019
** zia
** File description:
** IConfig.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

class IConfig
{
public:
	virtual ~IConfig() = default;

protected:
	virtual bool loadFromFile(const std::string &filename) = 0;

	// virtual const std::string &getString(const std::string &sectionName, const std::string &key, const std::string &def = "") = 0;
	// virtual bool getBool(const std::string &sectionName, const std::string &key, bool def = false) = 0;
	// virtual int getInt(const std::string &sectionName, const std::string &key, int def = 0) = 0;
	// virtual unsigned getUnsigned(const std::string &sectionName, const std::string &key, unsigned def = 0) = 0;
	// virtual double getDouble(const std::string &sectionName, const std::string &key, double def = 0.0) = 0;

	// virtual std::vector<const std::string &> getStringArray(const std::string &sectionName, const std::string &key) = 0;
	// virtual std::vector<bool> getBoolArray(const std::string &sectionName, const std::string &key) = 0;
	// virtual std::vector<int> getIntArray(const std::string &sectionName, const std::string &key) = 0;
	// virtual std::vector<unsigned> getUnsignedArray(const std::string &sectionName, const std::string &key) = 0;
	// virtual std::vector<double> getDoubleArray(const std::string &sectionName, const std::string &key) = 0;
};