'use strict';

var Css = require("bs-css-emotion/src/Css.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var Css_Core = require("bs-css/src/Css_Core.js");
var ReactDom = require("react-dom");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var Caml_int32 = require("bs-platform/lib/js/caml_int32.js");
var Relude_Int = require("relude/src/Relude_Int.bs.js");
var Relude_Map = require("relude/src/Relude_Map.bs.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var Relude_Array = require("relude/src/Relude_Array.bs.js");
var Relude_Option = require("relude/src/Relude_Option.bs.js");
var ReactDraggableMinJs = require("react-draggable/build/web/react-draggable.min.js");

var make = {
  left: 0,
  top: 0,
  right: 0,
  bottom: 0
};

var BoundingRect = {
  make: make
};

var board = Curry._1(Css.style, /* [] */0);

var whiteSquare = Curry._1(Css.style, /* :: */[
      Css_Core.SVG.fill(Css.white),
      /* [] */0
    ]);

var blackSquare = Curry._1(Css.style, /* :: */[
      Css.unsafe("fill", "url('#black-lines')"),
      /* [] */0
    ]);

var pattern = Curry._1(Css.style, /* :: */[
      Css_Core.SVG.stroke(Css.hex("777")),
      /* :: */[
        Css_Core.SVG.strokeWidth(Css.px(1)),
        /* [] */0
      ]
    ]);

var Styles = {
  board: board,
  whiteSquare: whiteSquare,
  blackSquare: blackSquare,
  pattern: pattern
};

var size = 320;

function squareFromCoordinate(pos) {
  return /* tuple */[
          pos[0] / 40 | 0,
          (size - pos[1] | 0) / 40 | 0
        ];
}

function squareFromClient(board, coord) {
  var clientY = coord[1];
  var clientX = coord[0];
  if (clientX < board.left || clientX > board.right || clientY < board.top || clientY > board.bottom) {
    return ;
  } else {
    var x = clientX - board.left | 0;
    var y = clientY - board.top | 0;
    return squareFromCoordinate(/* tuple */[
                x,
                y
              ]);
  }
}

var Settings = {
  squareSize: 40,
  size: size,
  squareFromCoordinate: squareFromCoordinate,
  squareFromClient: squareFromClient
};

function even(y) {
  return y % 2 === 0;
}

function squareColor(sq) {
  var x = sq[0];
  if (even(sq[1])) {
    if (even(x)) {
      return /* Black */1;
    } else {
      return /* White */0;
    }
  } else if (even(x)) {
    return /* White */0;
  } else {
    return /* Black */1;
  }
}

function colorClass(c) {
  if (c) {
    return blackSquare;
  } else {
    return whiteSquare;
  }
}

function Board$BlackPattern(Props) {
  return React.createElement("pattern", {
              id: "black-lines",
              height: "4",
              width: "4",
              patternUnits: "userSpaceOnUse"
            }, React.createElement("path", {
                  className: pattern,
                  d: "M-1,1 l2,-2\n             M0,4 l4,-4\n             M3,5 l2,-2"
                }));
}

var BlackPattern = {
  make: Board$BlackPattern
};

function Board$Square(Props) {
  var coord = Props.coord;
  var children = Props.children;
  var perspectiveOpt = Props.perspective;
  var perspective = perspectiveOpt !== undefined ? perspectiveOpt : /* White */0;
  var yc = coord[1];
  var xc = coord[0];
  var match = perspective ? /* tuple */[
      Caml_int32.imul(7 - xc | 0, 40).toString(),
      Caml_int32.imul(yc, 40).toString()
    ] : /* tuple */[
      Caml_int32.imul(xc, 40).toString(),
      Caml_int32.imul(7 - yc | 0, 40).toString()
    ];
  return React.createElement("g", {
              transform: "translate(" + (String(match[0]) + (", " + (String(match[1]) + ")")))
            }, children);
}

var Square = {
  make: Board$Square
};

var boardSquares = Belt_Array.concatMany(Belt_Array.map(Belt_Array.range(0, 7), (function (x) {
            return Belt_Array.map(Belt_Array.range(0, 7), (function (y) {
                          var c = squareColor(/* tuple */[
                                x,
                                y
                              ]);
                          return React.createElement(Board$Square, {
                                      coord: /* tuple */[
                                        x,
                                        y
                                      ],
                                      children: React.createElement("rect", {
                                            className: c ? blackSquare : whiteSquare,
                                            height: (40).toString(),
                                            width: (40).toString()
                                          }),
                                      key: "" + (String(x) + ("" + (String(y) + "")))
                                    });
                        }));
          })));

function image(piece) {
  var tmp;
  if (piece[0]) {
    switch (piece[1]) {
      case /* King */0 :
          tmp = "b-king.svg";
          break;
      case /* Queen */1 :
          tmp = "b-queen.svg";
          break;
      case /* Bishop */2 :
          tmp = "b-bishop.svg";
          break;
      case /* Knight */3 :
          tmp = "b-knight.svg";
          break;
      case /* Rook */4 :
          tmp = "b-rook.svg";
          break;
      case /* Pawn */5 :
          tmp = "b-pawn.svg";
          break;
      
    }
  } else {
    switch (piece[1]) {
      case /* King */0 :
          tmp = "w-king.svg";
          break;
      case /* Queen */1 :
          tmp = "w-queen.svg";
          break;
      case /* Bishop */2 :
          tmp = "w-bishop.svg";
          break;
      case /* Knight */3 :
          tmp = "w-knight.svg";
          break;
      case /* Rook */4 :
          tmp = "w-rook.svg";
          break;
      case /* Pawn */5 :
          tmp = "w-pawn.svg";
          break;
      
    }
  }
  return "/assets/pieces/" + tmp;
}

function Board$Piece$Image(Props) {
  var piece = Props.piece;
  var onMouseDown = Props.onMouseDown;
  var onMouseUp = Props.onMouseUp;
  var onTouchStart = Props.onTouchStart;
  var onTouchEnd = Props.onTouchEnd;
  var tmp = {
    height: (40).toString(),
    href: image(piece),
    width: (40).toString()
  };
  if (onMouseDown !== undefined) {
    tmp.onMouseDown = Caml_option.valFromOption(onMouseDown);
  }
  if (onMouseUp !== undefined) {
    tmp.onMouseUp = Caml_option.valFromOption(onMouseUp);
  }
  if (onTouchEnd !== undefined) {
    tmp.onTouchEnd = Caml_option.valFromOption(onTouchEnd);
  }
  if (onTouchStart !== undefined) {
    tmp.onTouchStart = Caml_option.valFromOption(onTouchStart);
  }
  return React.createElement("image", tmp);
}

var $$Image = {
  make: Board$Piece$Image
};

function Board$Piece$Overlay(Props) {
  var piece = Props.piece;
  var pos = Props.pos;
  var svgStyle = Curry._1(Css.style, /* :: */[
        Css.position(Css.absolute),
        /* :: */[
          Css.left(Css.px(pos[0] - 20 | 0)),
          /* :: */[
            Css.top(Css.px(pos[1] - 20 | 0)),
            /* [] */0
          ]
        ]
      ]);
  return ReactDom.createPortal(React.createElement("svg", {
                  className: svgStyle
                }, React.createElement(Board$Piece$Image, {
                      piece: piece
                    })), document.body);
}

var Overlay = {
  make: Board$Piece$Overlay
};

function Board$Piece(Props) {
  var piece = Props.piece;
  var onDrop = Props.onDrop;
  var match = React.useState((function () {
          return ;
        }));
  var setDragging = match[1];
  var dragging = match[0];
  var start = function (param, param$1) {
    return true;
  };
  var drag = function (param, data) {
    Curry._1(setDragging, (function (param) {
            return data;
          }));
    return true;
  };
  var stop = function (param, data) {
    Curry._1(setDragging, (function (param) {
            return ;
          }));
    Relude_Option.map((function (h) {
            return Curry._2(h, piece, data);
          }), onDrop);
    return true;
  };
  var tmp;
  if (dragging !== undefined) {
    var data = dragging;
    tmp = React.createElement(Board$Piece$Overlay, {
          piece: piece,
          pos: /* tuple */[
            data.x,
            data.y
          ]
        });
  } else {
    tmp = React.createElement(Board$Piece$Image, {
          piece: piece
        });
  }
  return React.createElement(React.Fragment, undefined, React.createElement(ReactDraggableMinJs.DraggableCore, {
                  onStart: start,
                  onDrag: drag,
                  onStop: stop,
                  children: tmp
                }));
}

var Piece = {
  image: image,
  $$Image: $$Image,
  Overlay: Overlay,
  make: Board$Piece
};

function Board$Cell(Props) {
  var cell = Props.cell;
  var onDrop = Props.onDrop;
  var tmp = {
    piece: cell.piece
  };
  if (onDrop !== undefined) {
    tmp.onDrop = Caml_option.valFromOption(onDrop);
  }
  return React.createElement(Board$Square, {
              coord: cell.at,
              children: React.createElement(Board$Piece, tmp)
            });
}

var Cell = {
  make: Board$Cell
};

function Board(Props) {
  var setup = Props.setup;
  var box = {
    contents: make
  };
  var updateBox = function (ref) {
    if (ref == null) {
      box.contents = make;
      return /* () */0;
    } else {
      box.contents = ref.getBoundingClientRect();
      return /* () */0;
    }
  };
  var onPieceDrop = function (piece, data) {
    console.log(piece);
    console.log(squareFromClient(box.contents, /* tuple */[
              data.x,
              data.y
            ]));
    return /* () */0;
  };
  return React.createElement("svg", {
              ref: updateBox,
              className: board,
              height: Relude_Int.toString(size),
              width: Relude_Int.toString(size)
            }, React.createElement("defs", undefined, React.createElement(Board$BlackPattern, { })), boardSquares, Relude_Map.foldLeft((function (acc, at, piece) {
                    return Relude_Array.cons(React.createElement(Board$Cell, {
                                    cell: {
                                      at: at,
                                      piece: piece
                                    },
                                    onDrop: onPieceDrop,
                                    key: "" + (String(at) + "")
                                  }), acc);
                  }), [], setup.pieces));
}

var make$1 = Board;

exports.BoundingRect = BoundingRect;
exports.Styles = Styles;
exports.Settings = Settings;
exports.even = even;
exports.squareColor = squareColor;
exports.colorClass = colorClass;
exports.BlackPattern = BlackPattern;
exports.Square = Square;
exports.boardSquares = boardSquares;
exports.Piece = Piece;
exports.Cell = Cell;
exports.make = make$1;
/* board Not a pure module */
