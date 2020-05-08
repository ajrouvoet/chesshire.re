open Chess;
open Relude.Globals;

module Parser {
  module P = ReludeParse.Parser;
  open P.Infix;

  let piece = 
       P.anyChar
    |> P.map(Piece.fromSAN)
    |> P.filter(p => switch p {
      | Some(_) => true
      | None    => false });  

  let space = 
       P.anyDigitAsInt
    |> P.map(n => Array.repeat(n, None));
  
  let rank  = 
       ((piece |> P.map(p => [| p |])) <|> space) 
    |> P.many
    |> P.map(List.toArray) 
    |> P.map(Array.flatten);
   
  let board = rank
    |> P.sepBy(P.str("/"))
    |> P.map(List.toArray)
    |> P.filter(a => Array.length(a) == 8)
    |> P.map(Array.flatten);

  let color =
        (P.str("w") |> P.map(_ => White))
    <|> (P.str("b") |> P.map(_ => Black))
       
  let sideOf = (s: string) => {
    switch (String.toLowerCase(s)) {
    | "kq" => BothSides
    | "k"  => KingSide
    | "q"  => QueenSide
    | _    => NoSides
    }
  };
  
  let squareName = 
    ((f, r) => Square.fromName(f ++ r))
    <$> P.anyCharInRange(97, 104) 
    <*> P.anyDigit;

  let castleOptions = {
    let sides = 
      (P.str("-") |> P.map (_ => (NoSides, NoSides))) 
      <|> P.tuple2
        ( (sideOf <$> (P.tries(P.str("KQ")) <|> P.str("K") <|> P.str("Q") <|> P.str("")))
        , (sideOf <$> (P.tries(P.str("KQ")) <|> P.str("K") <|> P.str("Q") <|> P.str("")))
        );
        
    sides |> P.map(((ws, bs)) => byColor(~white= ws, ~black=bs))
  }

  let setup = 
    ((b, t, c, ep, hc, fc) => 
     ({...Setup.default, pieces: b, turn: t, castling: c, epSquare: ep, halfmoves: hc, fullmoves: fc}))
      <$> board
      <*> (P.ws *> color)
      <*> (P.ws *> castleOptions)
      <*> (P.ws *> squareName)
      <*> (P.ws *> P.anyInt)
      <*> (P.ws *> P.anyInt);
};

let parse = (fen: string): option(setup) => {
  open ReludeParse.Parser;

  Parser.setup
    |> runParser(fen)
    |> Result.toOption
};
