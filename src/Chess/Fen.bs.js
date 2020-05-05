'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var Relude_List = require("relude/src/Relude_List.bs.js");
var Relude_Array = require("relude/src/Relude_Array.bs.js");
var ReludeParse_Parser = require("relude-parse/src/ReludeParse_Parser.bs.js");
var Chess$ReasonReactExamples = require("./Chess.bs.js");

var piece = ReludeParse_Parser.map(Chess$ReasonReactExamples.Piece.fromSAN, ReludeParse_Parser.anyChar);

var space = ReludeParse_Parser.map((function (n) {
        return Relude_Array.repeat(n, undefined);
      }), ReludeParse_Parser.anyDigitAsInt);

var rank = ReludeParse_Parser.map(Relude_Array.flatten, ReludeParse_Parser.map(Relude_List.toArray, ReludeParse_Parser.times(8, Curry._2(ReludeParse_Parser.Infix.$less$pipe$great, ReludeParse_Parser.map((function (p) {
                        return [p];
                      }), piece), space))));

var board = ReludeParse_Parser.times(8, Curry._2(ReludeParse_Parser.Infix.$less$star, rank, ReludeParse_Parser.str("/")));

var FenParser = {
  P: /* alias */0,
  piece: piece,
  space: space,
  rank: rank,
  board: board
};

exports.FenParser = FenParser;
/* piece Not a pure module */
