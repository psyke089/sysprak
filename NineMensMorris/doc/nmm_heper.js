// ==UserScript==
// @name        nmmorris_helper
// @namespace   nmmorris_helper
// @include     http://sysprak.priv.lab.nm.ifi.lmu.de/sysprak/NMMorris/*
// @version     1
// @grant       none
// ==/UserScript==

var infoBlock = document.getElementById('infoBlock');
if(infoBlock) {
  var ID = infoBlock.getElementsByTagName('a')[0].innerHTML;
  var throttle = document.getElementById('throttle');
  throttle.value = '0.2';
  copyToClipboard(ID);  
}

function copyToClipboard(text) {
  window.prompt("Copy to clipboard: Ctrl+C, Enter", text);
}
