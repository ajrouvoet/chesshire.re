// open Relude;
open Belt;

type color = White | Black;

module Color {
  let fromString = (name: string): option(color) => switch name {
    | "white" => Some(White)
    | "black" => Some(Black)
    | _       => None
  };
}

type square = (int, int);

module SquareComparable =
  Belt.Id.MakeComparable({
    type t = square;
    let cmp = Pervasives.compare;
  });

type squares = Belt.Set.t(square, SquareComparable.identity);

module Squares = {
  let empty:squares = Belt.Set.make(~id=(module SquareComparable));
}

type material = 
  { pawn: int
  , knight: int
  , bishop: int
  , rook: int
  , queen: int
  , king: int
  }
  
module Material {
  let empty:material = 
    { pawn: 0
    , knight: 0
    , bishop: 0
    , rook: 0
    , queen: 0
    , king: 0
    };
}

[@bs.deriving abstract]
type byColor('a) = 
  { white: 'a
  , black: 'a
  }
  
type role = 
  | King
  | Queen
  | Bishop
  | Knight
  | Rook
  | Pawn

module Role {
  let fromString = (r: string): option(role) => switch r {
    | "king"   => Some(King)
    | "queen"  => Some(Queen)
    | "bishop" => Some(Bishop)
    | "knight" => Some(Knight)
    | "rook"   => Some(Rook)
    | "pawn"   => Some(Pawn)
    | _        => None
  }
}

type piece    = (color, role);
type occupant = option(piece)

// row major board
// type board = array(array(occupant))
type board = Belt.Map.t(square, piece, SquareComparable.identity);
type cell  = 
  { at: square
  , piece: piece
  };

type setup = 
  { pieces: board
  , pockets: byColor(material)
  , turn: color
  , castling: byColor(bool)
  , epSquare: option(square)
  , plies: int
  }

module Board {
  let empty:board = {
    Belt.Map.make(~id=(module SquareComparable))
  };

  let make = (pieces: array((square, piece))):board =>
    Belt.Map.fromArray(pieces, ~id=(module SquareComparable));
  
  let union = (left: board, right: board): board => {
    open Map;
    
    let rightBiased = (_, l, r) => switch r {
      | Some(v) => Some(v)
      | None    => l
    };
    
    merge(left, right, rightBiased)
  };

  let pawnRank = (y:int, c):board => make(Array.(
    map(range(0, 7), x => ((x, y), (c, Pawn)))
  ));
  
  let defaultPieceRank = (y:int, c:color):board => make([|
    ((0, y), (c, Rook)),
    ((1, y), (c, Knight)),
    ((2, y), (c, Bishop)),
    ((3, y), (c, Queen)),
    ((4, y), (c, King)),
    ((5, y), (c, Bishop)),
    ((6, y), (c, Knight)),
    ((7, y), (c, Rook))
  |])

  let default:board = empty
    ->union(defaultPieceRank(0, White))
    ->union(pawnRank(1, White))
    ->union(pawnRank(6, Black))
    ->union(defaultPieceRank(7, Black));
}
  
module Setup {
  let default:setup = {
    pieces   : Board.default,
    pockets  : byColor(~white=Material.empty, ~black=Material.empty),
    turn     : White,
    castling : byColor(~white=true, ~black=true),
    epSquare : None,
    plies    : 0
  };
}
