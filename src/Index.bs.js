'use strict';

var Css = require("bs-css-emotion/src/Css.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var ReactDom = require("react-dom");
var Relude_Option = require("relude/src/Relude_Option.bs.js");
var FenJs = require("chessops/fen.js");
var Fen$ReasonReactExamples = require("./Chess/Fen.bs.js");
var Board$ReasonReactExamples = require("./Chess/Board.bs.js");
var Chess$ReasonReactExamples = require("./Chess/Chess.bs.js");

var app = document.createElement("div");

app.id = "app";

document.body.appendChild(app);

var chessMediumUrl_000 = Css.url("/assets/fonts/ChessMedium.otf");

var chessMediumUrl = /* :: */[
  chessMediumUrl_000,
  /* [] */0
];

var chessMedium = Css.fontFace("ChessMedium", chessMediumUrl, Css.normal, /* normal */812216871, undefined, /* () */0);

var megrim = Css.fontFace("Megrim", /* :: */[
      Css.url("/assets/fonts/Megrim-Regular.ttf"),
      /* [] */0
    ], /* normal */812216871, /* medium */-20425611, undefined, /* () */0);

Curry._2(Css.$$global, "body", /* :: */[
      Css.display(/* flex */-1010954439),
      /* :: */[
        Css.flexDirection(Css.row),
        /* :: */[
          Css.height(/* `percent */[
                -119887163,
                100.0
              ]),
          /* [] */0
        ]
      ]
    ]);

Curry._2(Css.$$global, "h1", /* :: */[
      Css.fontFamily(/* `custom */[
            1066567601,
            megrim
          ]),
      /* [] */0
    ]);

var head = Curry._1(Css.style, /* :: */[
      Css.width(/* `percent */[
            -119887163,
            100.0
          ]),
      /* :: */[
        Css.selector("& > h1", /* :: */[
              Css.display(Css.inlineBlock),
              /* [] */0
            ]),
        /* [] */0
      ]
    ]);

var logo = Curry._1(Css.style, /* :: */[
      Css.fontFamily(/* `custom */[
            1066567601,
            chessMedium
          ]),
      /* :: */[
        Css.display(Css.inlineBlock),
        /* :: */[
          Css.fontSize(Css.pt(25)),
          /* :: */[
            Css.padding(Css.px(10)),
            /* :: */[
              Css.transform(Css.rotate(Css.deg(150.0))),
              /* [] */0
            ]
          ]
        ]
      ]
    ]);

var Styles = {
  chessMediumUrl: chessMediumUrl,
  chessMedium: chessMedium,
  megrim: megrim,
  head: head,
  logo: logo
};

function Index$Logo(Props) {
  return React.createElement("span", {
              className: logo
            }, "Q");
}

var Logo = {
  make: Index$Logo
};

function Index$Head(Props) {
  return React.createElement("div", {
              className: head
            }, React.createElement(Index$Logo, { }), React.createElement("h1", undefined, "My Best Blunders"));
}

var Head = {
  make: Index$Head
};

var setup = Chess$ReasonReactExamples.Setup.$$default;

var parsed = Relude_Option.getOrElse(Chess$ReasonReactExamples.Board.$$default, Fen$ReasonReactExamples.board(FenJs.parseFen("rnbq1rk1/ppp1ppb1/3p1npp/8/3PP1P1/2N1B3/PPP1BP1P/R2QK1NR w KQ - 0 1").value));

function Index$App(Props) {
  return React.createElement(React.Fragment, undefined, React.createElement(Index$Head, { }), React.createElement(Board$ReasonReactExamples.make, {
                  setup: {
                    pieces: parsed,
                    pockets: setup.pockets,
                    turn: setup.turn,
                    castling: setup.castling,
                    epSquare: setup.epSquare,
                    plies: setup.plies
                  }
                }));
}

var App = {
  setup: setup,
  parsed: parsed,
  make: Index$App
};

ReactDom.render(React.createElement(Index$App, { }), app);

exports.app = app;
exports.Styles = Styles;
exports.Logo = Logo;
exports.Head = Head;
exports.App = App;
/* app Not a pure module */
