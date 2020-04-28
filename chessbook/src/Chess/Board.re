open Chess
open Relude.Globals

[@bs.val] external document: Js.t({..}) = "document";

module Styles {
  open Css

  let board = style([
  ])
    
  let whiteSquare = style([
    SVG.fill(white)
  ])

  let blackSquare = style([
    unsafe("fill", "url('#black-lines')")
  ])
    
  let pattern = style([
    SVG.stroke(hex("777")),
    SVG.strokeWidth(px(1))
  ])
}
  
module Settings {
  let squareSize = 40
  let size       = squareSize * 8
}
  
let even = (y: int) => y mod 2 == 0
let squareColor = (sq: square): color => {
  let (x, y) = sq;
  even(y) ? (even(x) ? White : Black) : (even(x) ? Black : White)
};

let colorClass = (c: color): string => switch (c) {
  | White => Styles.whiteSquare
  | Black => Styles.blackSquare
};

module BlackPattern { 
  [@react.component]
  let make = () => {
    <pattern id="black-lines" width="4" height="4" patternUnits="userSpaceOnUse">
      <path
          d="M-1,1 l2,-2
             M0,4 l4,-4
             M3,5 l2,-2"
          className=Styles.pattern />
    </pattern>
  }
};

module Square = {
  [@react.component]
  let make = (~coord:square, ~children:React.element, ~flip=false) => {
    open Settings;
    let (xc,yc) = coord;

    let (x, y) = if(flip) {
      (Js.Int.toString(xc * squareSize), Js.Int.toString(yc * squareSize))
    } else {
      (Js.Int.toString(xc * squareSize), Js.Int.toString((7-yc) * squareSize))
    };

    <g transform={j|translate($x, $y)|j}>{children}</g>
  };
};

// 32 squares
let boardSquares = Belt.Array.(
  map(range(0, 7), x => map(range(0, 7), y => 
    <Square coord=(x, y) key={j|$x$y|j}> 
      <rect 
         width=Js.Int.toString(Settings.squareSize) 
         height=Js.Int.toString(Settings.squareSize)
         className=squareColor((x,y))->colorClass />
    </Square>
  ))->concatMany->React.array
);

module Piece {
  let image = (piece: piece) => "/assets/pieces/" ++ switch piece {
    | (White, King)   => "w-king.svg"
    | (White, Queen)  => "w-queen.svg"
    | (White, Rook)   => "w-rook.svg"
    | (White, Knight) => "w-knight.svg"
    | (White, Bishop) => "w-bishop.svg"
    | (White, Pawn)   => "w-pawn.svg"
    | (Black, King)   => "b-king.svg"
    | (Black, Queen)  => "b-queen.svg"
    | (Black, Rook)   => "b-rook.svg"
    | (Black, Knight) => "b-knight.svg"
    | (Black, Bishop) => "b-bishop.svg"
    | (Black, Pawn)   => "b-pawn.svg"
  };
  
  module Image {
    [@react.component]
    let make = (~piece: piece, ~onMouseDown=?, ~onMouseUp=?, ~onTouchStart=?, ~onTouchEnd=?) => {
      open Settings;
      <image 
        href={image(piece)} 
        width=Js.Int.toString(squareSize) 
        height=Js.Int.toString(squareSize) 
        ?onMouseDown ?onMouseUp ?onTouchStart ?onTouchEnd />
    }
  };

  module Overlay {
    [@react.component]
    let make = (~piece: piece, ~pos: (int, int)) => {
      open Settings;
      let (x, y) = pos;

      let svgStyle = Css.(style([
        position(absolute),
        left(px(x - squareSize / 2)),
        top(px(y - squareSize / 2)),
      ]))

      // render the dragged piece directly on the document body
      // so that it can go outside of the boundary of the board
      ReactDOMRe.createPortal(
        <svg className=svgStyle>
          <Image piece />
        </svg>,
        document##body
      )
    }
  };

  [@react.component]
  let make = (~piece: piece) => {
    open DraggableCore;
    let (dragging:option(draggableData), setDragging) = React.useState(() => None);

    let start = (_, _)    => { true };
    let drag  = (_, data) => { setDragging(_ => Some(data)); true };
    let stop  = (_, _)    => { setDragging(_ => None); true};

    <>
      <DraggableCore onStart=start onDrag=drag onStop=stop>
        { switch dragging {
          | None       => <Image piece />
          | Some(data) => <Overlay piece pos=(data.x, data.y) />
        }}
      </DraggableCore>
    </>

  };
}

module Cell {
  [@react.component]
  let make = (~cell:Chess.cell) => {
    <Square coord=cell.at><Piece piece=cell.piece /></Square>
  }
}

[@react.component]
let make = (~setup:Chess.setup) => {
  open Settings;

  <svg className=(Styles.board) width=Int.toString(size) height=Int.toString(size)>
    <defs>
      <BlackPattern />
    </defs>
    {boardSquares}
    {
      setup
      |>Chess.pieces
      |>Map.foldLeft(
          (acc, at, piece) => Array.cons(<Cell key={j|$at|j} cell={at, piece} />, acc),
          [||]
        )
      |>React.array
    }
  </svg>
}

/*   // compute the board coordinates of a square from its name */
/*   squareLocation(sq: SquareName): [number,number] { */
/*     let x = sq[0].charCodeAt(0) - 97 */
/*     let y = sq[1].charCodeAt(0) - 48 */

/*     return [x * SQ, DIM - y * SQ] */
/*   } */
    
/*   squareFromCoordinate(co: Position): SquareName { */
/*     let x = Math.trunc(co.x / SQ) */
/*     let y = Math.trunc((DIM - co.y) / SQ) + 1 */
      
/*     return ('abcdefgh'[x] + y.toString() as SquareName) */
/*   } */

/*   squareFromScreenCoordinate(ev: MouseEvent): SquareName { */
/*     let can = (this.canvas.current as Element).getBoundingClientRect() */
      
/*     let x = ev.clientX - can.x */
/*     let y = ev.clientY - can.y */
      
/*     return this.squareFromCoordinate({x, y}) */
/*   } */
