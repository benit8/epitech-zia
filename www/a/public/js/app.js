"use strict";

const $ = {
	qS: e => document.querySelector(e),
	qA: e => document.querySelectorAll(e)
};

window.addEventListener("load", function()
{

	$.qS('#alert').onclick = (e) => {
		e.preventDefault();
		alert("Hi!");
	};

});