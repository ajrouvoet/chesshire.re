open Relude.Globals;

type color = White | Black;

module Color {
  let fromString = (name: string): option(color) => switch name {
    | "white" => Some(White)
    | "black" => Some(Black)
    | _       => None
  };
}

type square = int;

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
  let fromChar = (r: string): option(role) => String.(switch (toLowerCase(r)) {
    | "k" => Some(King)
    | "q" => Some(Queen)
    | "b" => Some(Bishop)
    | "n" => Some(Knight)
    | "r" => Some(Rook)
    | "p" => Some(Pawn)
    | _   => None
  });

  let fromString = (r: string): option(role) => String.(switch (toLowerCase(r)) {
    | "king" => Some(King)
    | "queen" => Some(Queen)
    | "bishop" => Some(Bishop)
    | "knight" => Some(Knight)
    | "rook" => Some(Rook)
    | "pawn" => Some(Pawn)
    | _   => None
  });
}

type piece    = (color, role);

module Piece {
  let fromSAN = (p: string): option(piece) => {
    let color = String.toLowerCase(p) == p ? White : Black;
    Role.fromChar(p) |> Option.map(r => (color, r))
  }
}

type occupant = option(piece)
type board = array(occupant)

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
    Array.repeat(64, None)
  };

  let (&) = (left: board, right: board): board => {
    open Map;
    
    left |> mapWithIndex((n, p) => right |> Array.at(n) |> Option.orElse(p))
  };

  let pawnRank = (y:int, c): board => make(
    Array.makeWithIndex(7, x => ((x, y), (c, Pawn))
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
