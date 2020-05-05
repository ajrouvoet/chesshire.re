'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");

function iter(f, t) {
  var iterator = t[Symbol.iterator]();
  var iterator$1 = iterator;
  var f$1 = f;
  while(true) {
    var curr = iterator$1.next();
    if (curr.done) {
      return 0;
    } else {
      var match = curr.value;
      if (match !== undefined) {
        Curry._1(f$1, Caml_option.valFromOption(match));
      }
      continue ;
    }
  };
}

exports.iter = iter;
/* No side effect */
