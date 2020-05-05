'use strict';

var Belt_Id = require("bs-platform/lib/js/belt_Id.js");
var Belt_Map = require("bs-platform/lib/js/belt_Map.js");
var Belt_Set = require("bs-platform/lib/js/belt_Set.js");
var Caml_obj = require("bs-platform/lib/js/caml_obj.js");
var Relude_Map = require("relude/src/Relude_Map.bs.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var Relude_Array = require("relude/src/Relude_Array.bs.js");
var Relude_Option = require("relude/src/Relude_Option.bs.js");
var Relude_String = require("relude/src/Relude_String.bs.js");

function fromString(name) {
  switch (name) {
    case "black" :
        return /* Black */1;
    case "white" :
        return /* White */0;
    default:
      return ;
  }
}

var Color = {
  fromString: fromString
};

var cmp = Caml_obj.caml_compare;

var SquareComparable = Belt_Id.MakeComparable({
      cmp: cmp
    });

var empty = Belt_Set.make(SquareComparable);

var Squares = {
  empty: empty
};

var empty$1 = {
  pawn: 0,
  knight: 0,
  bishop: 0,
  rook: 0,
  queen: 0,
  king: 0
};

var Material = {
  empty: empty$1
};

function fromChar(r) {
  var match = Relude_String.toLowerCase(r);
  switch (match) {
    case "b" :
        return /* Bishop */2;
    case "k" :
        return /* King */0;
    case "n" :
        return /* Knight */3;
    case "p" :
        return /* Pawn */5;
    case "q" :
        return /* Queen */1;
    case "r" :
        return /* Rook */4;
    default:
      return ;
  }
}

function fromString$1(r) {
  var match = Relude_String.toLowerCase(r);
  switch (match) {
    case "bishop" :
        return /* Bishop */2;
    case "king" :
        return /* King */0;
    case "knight" :
        return /* Knight */3;
    case "pawn" :
        return /* Pawn */5;
    case "queen" :
        return /* Queen */1;
    case "rook" :
        return /* Rook */4;
    default:
      return ;
  }
}

var Role = {
  fromChar: fromChar,
  fromString: fromString$1
};

function fromSAN(p) {
  var color = Relude_String.toLowerCase(p) === p ? /* White */0 : /* Black */1;
  return Relude_Option.map((function (r) {
                return /* tuple */[
                        color,
                        r
                      ];
              }), fromChar(p));
}

var Piece = {
  fromSAN: fromSAN
};

var empty$2 = Belt_Map.make(SquareComparable);

function make(pieces) {
  return Belt_Map.fromArray(pieces, SquareComparable);
}

function union(left, right) {
  var rightBiased = function (param, l, r) {
    if (r !== undefined) {
      return Caml_option.some(Caml_option.valFromOption(r));
    } else {
      return l;
    }
  };
  return Relude_Map.merge(rightBiased, left, right);
}

function pawnRank(y, c) {
  var pieces = Relude_Array.makeWithIndex(7, (function (x) {
          return /* tuple */[
                  /* tuple */[
                    x,
                    y
                  ],
                  /* tuple */[
                    c,
                    /* Pawn */5
                  ]
                ];
        }));
  return Belt_Map.fromArray(pieces, SquareComparable);
}

function defaultPieceRank(y, c) {
  return Belt_Map.fromArray([
              /* tuple */[
                /* tuple */[
                  0,
                  y
                ],
                /* tuple */[
                  c,
                  /* Rook */4
                ]
              ],
              /* tuple */[
                /* tuple */[
                  1,
                  y
                ],
                /* tuple */[
                  c,
                  /* Knight */3
                ]
              ],
              /* tuple */[
                /* tuple */[
                  2,
                  y
                ],
                /* tuple */[
                  c,
                  /* Bishop */2
                ]
              ],
              /* tuple */[
                /* tuple */[
                  3,
                  y
                ],
                /* tuple */[
                  c,
                  /* Queen */1
                ]
              ],
              /* tuple */[
                /* tuple */[
                  4,
                  y
                ],
                /* tuple */[
                  c,
                  /* King */0
                ]
              ],
              /* tuple */[
                /* tuple */[
                  5,
                  y
                ],
                /* tuple */[
                  c,
                  /* Bishop */2
                ]
              ],
              /* tuple */[
                /* tuple */[
                  6,
                  y
                ],
                /* tuple */[
                  c,
                  /* Knight */3
                ]
              ],
              /* tuple */[
                /* tuple */[
                  7,
                  y
                ],
                /* tuple */[
                  c,
                  /* Rook */4
                ]
              ]
            ], SquareComparable);
}

var $$default = union(union(union(union(empty$2, defaultPieceRank(0, /* White */0)), pawnRank(1, /* White */0)), pawnRank(6, /* Black */1)), defaultPieceRank(7, /* Black */1));

var Board = {
  empty: empty$2,
  make: make,
  union: union,
  pawnRank: pawnRank,
  defaultPieceRank: defaultPieceRank,
  $$default: $$default
};

var default_pockets = {
  white: empty$1,
  black: empty$1
};

var default_castling = {
  white: true,
  black: true
};

var $$default$1 = {
  pieces: $$default,
  pockets: default_pockets,
  turn: /* White */0,
  castling: default_castling,
  epSquare: undefined,
  plies: 0
};

var Setup = {
  $$default: $$default$1
};

exports.Color = Color;
exports.SquareComparable = SquareComparable;
exports.Squares = Squares;
exports.Material = Material;
exports.Role = Role;
exports.Piece = Piece;
exports.Board = Board;
exports.Setup = Setup;
/* SquareComparable Not a pure module */
