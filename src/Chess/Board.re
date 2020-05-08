open Chess
open Relude.Globals

[@bs.val] external document: Js.t({..}) = "document";

module BoundingRect {
  type t = {
    left: int, top: int, right: int, bottom: int
  };
  
  let make = { left: 0, top: 0, right: 0, bottom: 0 };
}

// missing from React bindings
[@bs.send] external getBoundingClientRect : (Dom.element) => BoundingRect.t = "getBoundingClientRect";

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

  // assumes the pos is in the board's dimensions
  let squareFromCoordinate = (pos: (int , int)): squareCoord => {
    let (x, y) = pos;

    (x / squareSize , (size - y) / squareSize);
  }

  // validates the coordinates
  let squareFromClient(board: BoundingRect.t, coord: (int, int)): option(squareCoord) {
    let (clientX, clientY) = coord

    if (clientX < board.left || clientX > board.right || clientY < board.top || clientY > board.bottom) {
      None
    } else {
      let x = clientX - board.left
      let y = clientY - board.top

      Some(squareFromCoordinate((x, y)))
    }
  }
}
  
let even = (y: int) => y mod 2 == 0
let squareColor = (sq: squareCoord): color => {
  let (x, y) = sq;
  even(y) ? (even(x) ? Black : White) : (even(x) ? White : Black)
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
  let make = (~coord:squareCoord, ~children:React.element, ~perspective=White) => {
    open Settings;
    let (xc,yc) = coord;

    let (x, y) = switch perspective {
      | Black => (Js.Int.toString((7-xc) * squareSize), Js.Int.toString((7-yc) * squareSize))
      | White => (Js.Int.toString(xc * squareSize), Js.Int.toString(yc * squareSize))
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

  type dropHandler = (piece, DraggableCore.draggableData) => bool;

  [@react.component]
  let make = (~piece: piece, ~onDrop=?) => {
    open DraggableCore;
    let (dragging:option(draggableData), setDragging) = React.useState(() => None);

    let start = (_, _)    => { true };
    let drag  = (_, data) => { setDragging(_ => Some(data)); true };
    let stop  = (_, data) => { 
      setDragging(_ => None); 
      ignore(onDrop|>Option.map(h => h(piece, data))); 
      true
    };

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
  let make = (~at:squareCoord, ~occupant:occupant, ~onDrop=?) => {
    <Square coord=at>
      { switch occupant {
        | Some(piece) => <Piece piece ?onDrop/>
        | None        => React.null
      }}
    </Square>
  }
}

[@react.component]
let make = (~setup:Chess.setup) => {
  open Settings;

  // keep track of the bounding box of the board, for translating
  // screen coordinates to board coordinates
  let box = ref(BoundingRect.make)
  let updateBox = (ref) => {
    switch (Js.Nullable.toOption(ref)) {
    | Some(el) => box := getBoundingClientRect(el)
    | None     => box := BoundingRect.make
    }
  };
  
  let onPieceDrop = (piece, data) => {
    open DraggableCore;
    Js.Console.log(piece);
    Js.Console.log(squareFromClient(box^, (data.x, data.y)))
  };

  <svg 
    className=(Styles.board) width=Int.toString(size) height=Int.toString(size)
    ref={ReactDOMRe.Ref.callbackDomRef(updateBox)}
    >
    <defs>
      <BlackPattern />
    </defs>
    {boardSquares}
    {
      setup.pieces
      |> Array.mapWithIndex((occ, at) =>
          <Cell key={j|$at|j}
                at=Chess.Square.getCoord(at)
                occupant=occ
                onDrop=onPieceDrop />
        )
      |> React.array
    }
  </svg>
}
