"use strict";

const $ = {
	qS: e => document.querySelector(e),
	qA: e => document.querySelectorAll(e)
};


/* Backgrounds
------------------------------------ */

let getRandomWallpaper = function()
{
	return wallpapers[Math.floor(Math.random() * wallpapers.length)];
}

let initBackgrounds = function()
{
	let bg = $.qS('.bg');
	let next = $.qS('#next-bg');  // Used to preload images

	bg.style.backgroundImage = 'url(' + getRandomWallpaper() + ')';
	next.src = getRandomWallpaper();

	setInterval(() => {
		bg.style.backgroundImage = 'url(' + next.src + ')';
		next.src = getRandomWallpaper();
	}, 1000 * 15);
}

window.addEventListener("load", initBackgrounds);


/* Section layout
------------------------------------ */

let buildLayout = function()
{
	let sectionsContainer = document.getElementById('sections');
	let cols = Math.floor(sectionsContainer.clientWidth / 500);
	cols = Math.min(Math.max(1, cols), 3);

	sectionsContainer.style.columnCount = cols;

	sortSections(cols);
}

let sortSections = function(col = 1)
{
	let sectionsContainer = document.getElementById('sections');
	let sections = sectionsContainer.childNodes;
	let sortedSections = [];

	for (let i in sections) {
		if (sections[i].nodeType != 1)
			continue;
		sortedSections.push(sections[i]);
	}

	sortedSections.sort((a, b) => {
		let ao = a.attributes.order.value.match(/(\d+)/g);
		let bo = b.attributes.order.value.match(/(\d+)/g);
		ao = (ao.length >= col) ? ao[col - 1] : ao[0];
		bo = (bo.length >= col) ? bo[col - 1] : bo[0];

		return ao - bo;
	});

	for (let i = 0; i < sortedSections.length; i++)
		sectionsContainer.appendChild(sortedSections[i]);
}

window.addEventListener("load", buildLayout);
window.addEventListener("resize", buildLayout);