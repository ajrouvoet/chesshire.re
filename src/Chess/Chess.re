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
type squareCoord = (int, int);

module SquareComparable =
  Belt.Id.MakeComparable({
    type t = square;
    let cmp = Pervasives.compare;
  });

type squares = Belt.Set.t(square, SquareComparable.identity);

module Square = {
  let getCoord = (sq: square): squareCoord => (7 - (sq mod 8), sq / 8);
}

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
  { at: squareCoord
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
  type rank = array(occupant);

  let emptyRanks = (n) => {
    Array.repeat(n*8, None)
  };

  let (||) = (left: board, right: board): board =>
    Array.zipWith(((l, r) => r), left, right);

  let pawnRank = c => Array.repeat(8, Some((c, Pawn)));
  
  let defaultPieceRank = (c:color): rank => [|
    Some((c, Rook)),
    Some((c, Knight)),
    Some((c, Bishop)),
    Some((c, Queen)),
    Some((c, King)),
    Some((c, Bishop)),
    Some((c, Knight)),
    Some((c, Rook))
  |]

  let default:board = Array.flatten(
    [| defaultPieceRank(White)
     , pawnRank(White)
     , emptyRanks(4)
     , pawnRank(Black)
     , defaultPieceRank(Black)
     |])
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
