/* type piece = {. */
/*   "role": string, */
/*   "color": string, */
/*   "promoted?": bool */
/* }; */

/* type board = JsIterable.t((int, piece)); */

/* type fenError; */

/* type setup = {. */
/*   "board": board */
/* }; */

/* [@bs.module "chessops/fen.js"] */
/* external parse: string => BsResult.t(setup, fenError) = "parseFen"; */

/* let convertSquare = (num: int): option(Chess.square) => { */
/*   let rank = num / 8; */
/*   let file = num mod 8; */
  
/*   if (file < 8 && rank < 8 && file >= 0 && rank >= 0) { */
/*     Some((file, rank)) */
/*   } else None */
/* }; */

/* let convertPiece = (p: piece): option(Chess.piece) => { */
/*   open Chess; */
/*   open Relude.Option; */
/*   open Relude.Option.Infix; */
  
/*   Color.fromString(p##color) >>= c => Role.fromString(p##role) |> map(r => (c, r)) */
/* }; */

/* let board = (s: setup): option(Chess.board) => { */
/*   open JsIterable; */
/*   open Relude.Map; */
/*   open Relude.Option; */

/*   let theBoard = ref(Chess.Board.empty); */

/*   try { */
/*     s##board */
/*       |> iter(((sqNum, piece)) => { */
/*          theBoard := theBoard^ |> set(getOrThrow(convertSquare(sqNum)), getOrThrow(convertPiece(piece))); */
/*       }); */
/*     Some(theBoard^) */
/*   } { */
/*     | _ => None */
/*   } */
/* }; */

open Chess;
open Relude.Globals;

module FenParser {
  module P = ReludeParse.Parser;
  open P.Infix;

  let piece = 
       P.anyChar
    |> P.map(Piece.fromSAN);  

  let space = 
       P.anyDigitAsInt
    |> P.map(n => Array.repeat(n, None));
  
  let rank  = 
       ((piece |> P.map(p => [| p |])) <|> space) 
    |> P.times(8) 
    |> P.map(List.toArray) 
    |> P.map(Array.flatten);
   
  let board = P.times(8, rank <* P.str("/"));
};

/* let parse = (fen: string): setup = { */
/*   let _ = ReludeParse.Parser.(runParser); */

/*   let board = FenParser.board |> runParser; */
  
/*   {...Setup.default, pieces = Board.make(board)} */
/* }; */
