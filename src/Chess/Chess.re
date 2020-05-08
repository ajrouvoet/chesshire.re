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
type sides  = NoSides | KingSide | QueenSide | BothSides;

module SquareComparable =
  Belt.Id.MakeComparable({
    type t = square;
    let cmp = Pervasives.compare;
  });

type squares = Belt.Set.t(square, SquareComparable.identity);

module Square = {
  let toCoord   = (sq: square): squareCoord => (sq mod 8, sq / 8);
  let fromCoord = ((x, y): squareCoord): square => y * 8 + x;
  let fromName  = (name: string) => {
    open Option.Infix;
    String.charCodeAt(0, name) >>= f =>
    String.charCodeAt(1, name) >>= r => 
      Some(fromCoord((f - 97 , r - 48)))
  }
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
    let color = String.toLowerCase(p) == p ? Black : White;
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
  , castling: byColor(sides)
  , epSquare: option(square)
  , halfmoves: int // since last capture or pawn advance
  , fullmoves: int
  }

module Board {
  type rank = array(occupant);

  let emptyRanks = (n) => {
    Array.repeat(n*8, None)
  };

  let (||) = (left: board, right: board): board =>
    Array.zipWith(((l, r) => Option.(orElse(r, l))), left, right);

  let pawnRank = c => Array.repeat(8, Some((c, Pawn)));
  
  let defaultPieceRank = (c:color): rank => [|
    Some((c, Rook)),
    Some((c, Knight)),
    Some((c, Bishop)),
    Some((c, King)),
    Some((c, Rook)),
    Some((c, Bishop)),
    Some((c, Knight)),
    Some((c, Rook))
  |]

  let default:board = Array.flatten(
    [| defaultPieceRank(Black)
     , pawnRank(Black)
     , emptyRanks(4)
     , pawnRank(White)
     , defaultPieceRank(White)
     |])
}
  
module Setup {
  let default:setup = {
    pieces   : Board.default,
    pockets  : byColor(~white=Material.empty, ~black=Material.empty),
    turn     : White,
    castling : byColor(~white=BothSides, ~black=BothSides),
    epSquare : None,
    halfmoves: 0,
    fullmoves: 0
  };
}
