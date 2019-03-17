<?php

function isMobileDevice() {
	return preg_match("/(android|avantgo|blackberry|bolt|boost|cricket|docomo|fone|hiptop|mini|mobi|palm|phone|pie|tablet|up\.browser|up\.link|webos|wos)/i", $_SERVER["HTTP_USER_AGENT"]);
}

$dir = "public/images/gifs/";
if (isMobileDevice())
	$dir .= "mobile/";

$files = glob("$dir*.gif");
$age = date_diff(date_create("1998-12-22"), date_create())->y;


?><!DOCTYPE html>
<html>
	<head>
		<meta charset="UTF-8">
		<meta name="viewport" content="width=device-width, initial-scale=1">
		<title>Benoît Lormeau</title>

		<meta property="og:type" content="website">
		<meta property="og:title" content="Benoît Lormeau - IT Student">
		<meta property="og:site_name" content="Benito.io">
		<meta property="og:description" content="Hi! My name is Benoît, I'm a <?= $age ?> years old french guy, and I have been passionate about programming since I was about 10.">
		<meta property="og:image" content="http://benito.io/public/images/benoit.lormeau.jpg">
		<meta property="og:url" content="http://benito.io/">
		<meta property="og:locale" content="en_US">

		<link rel="stylesheet" href="https://fonts.googleapis.com/css?family=VT323|Noto+Sans">
		<link rel="stylesheet" href="public/css/style.css">
		<link rel="icon" href="public/images/blood-moon.ico">
	</head>
	<body class="bg">
		<main>
			<header>
				Benoît Lormeau
				<div class="lead">IT Student</div>
			</header>

			<nav>
				<a href="https://github.com/benit8" target="_blank">GitHub</a>
				<a href="https://www.linkedin.com/in/blormeau/" target="_blank">LinkedIn</a>
				<a href="mailto:benoit.lormeau@epitech.eu">Contact</a>
			</nav>

			<div id="sections">
				<section order="0,0,0">
					<h2>Who I am</h2>
					<p>
						Hi! My name is Benoît, I'm a <?= $age ?> years old french guy, and I have been passionate about programming since I was about 10.
						I started by learning web development but I turned out to be more of a software enthusiast.
					</p>
				</section>

				<section order="1,4,3">
					<h2>Studies</h2>
					<p>
						<span><abbr title="EPITECH TOEIC">TEPitech</abbr> score: 920</span>
						<table>
							<tbody>
								<tr>
									<td>2016 - Now</td>
									<td>
										Epitech Graduate Diploma in Information Technology<br>
										Epitech, Paris Graduate school of Digital Information<br>
										<small>Rennes, France</small>
									</td>
								</tr>
								<tr>
									<td>2013 - 2016</td>
									<td>
										A-Level with specialization in Sciences<br>
										Lycée Napoléon<br>
										<small>L'Aigle, France</small>
									</td>
								</tr>
							</tbody>
						</table>
					</p>
				</section>

				<section order="2,5,5">
					<h2>Professional experience</h2>
					<p>
						<table>
							<tbody>
								<tr>
									<td>Jul - Dec 2017</td>
									<td>
										Internship at <a href="https://conserto.pro/" target="_blank">Conserto</a>, Information technology consulting:
										<ul>
											<li>Cross-platform games creation with <a href="https://phaser.io" target="_blank">Phaser</a></li>
											<li>Website integration of <a href="https://lycanthrop.fr/" target="_blank">Lycanthrop</a></li>
										</ul>
									</td>
								</tr>
							</tbody>
						</table>
					</p>
				</section>

				<section order="3,1,4">
					<h2>Skills</h2>
					<p>
						<table>
							<tbody>
								<tr>
									<td>Languages</td>
									<td>
										C, C++, some Shell and NodeJS<br>
										Web languages (HTML, CSS, JS, PHP, SQL)
									</td>
								</tr>
								<tr>
									<td>Systems</td>
									<td>
										Using Linux and Windows daily. I know UNIX shell, git, and how to run servers (http, ftp, git...)
									</td>
								</tr>
							</tbody>
						</table>
					</p>
				</section>

				<section order="4,6,6">
					<h2>Personal projects</h2>
					<p>
						<table>
							<tbody>
								<tr>
									<td><a href="https://github.com/benit8/libmy-tek2" target="_blank">libmy</a></td>
									<td>
										C library full of useful functions.
										I used it in every single school project of my 2<sup>nd</sup> year at Epitech.
									</td>
								</tr>
								<tr>
									<td><a href="https://github.com/benit8/libcnet" target="_blank">libcnet</a></td>
									<td>
										C library that implements TCP sockets/listeners, HTTP requests and other network related stuff.
									</td>
								</tr>
								<tr>
									<td><a href="https://github.com/benit8/txt" target="_blank">txt</a></td>
									<td>
										PHP textboard made using my little framework <a href="https://github.com/benit8/blank" target="_blank">blank</a>.
									</td>
								</tr>
								<tr>
									<td><a href="https://github.com/benit8/blank" target="_blank">blank</a></td>
									<td>
										Simple PHP framework made using the MVC pattern.
									</td>
								</tr>
							</tbody>
						</table>
					</p>
				</section>

				<section order="5,2,1">
					<h2>Hobbies</h2>
					<p>
						Cannot spend a day without listening to music.
						I took 6 years of drum courses but I haven't really played since I left my home town.
						I also like to get lost on the Internet.
					</p>
				</section>

				<section order="6,3,2">
					<h2>Commodities</h2>
					<p>
						<table>
							<tbody>
								<tr>
									<td>Residency</td>
									<td>
										I currently live in Rennes, France.
									</td>
								</tr>
								<tr>
									<td>Commuting</td>
									<td>
										I have a car driver license as well as my own car.
									</td>
								</tr>
							</tbody>
						</table>
					</p>
				</section>
			</div>

			<footer>
				Backgrounds by <a href="http://1041uuu.tumblr.com/" target="_blank">1041uuu</a>
			</footer>
		</main>

		<img id="next-bg" style="display: none;">

		<script type="text/javascript">
			const wallpapers = <?= json_encode($files) ?>;
		</script>
		<script type="text/javascript" src="public/js/main.js"></script>
	</body>
</html>
