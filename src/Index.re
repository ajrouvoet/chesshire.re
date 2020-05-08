open Relude.Globals;

[@bs.val] external document: Js.t({..}) = "document";

// create the mountpoint
let app = document##createElement("div")
app##id #= "app"
document##body##appendChild(app);

module Styles = {
  open Css

  // the font
  let chessMediumUrl = [url("/assets/fonts/ChessMedium.otf")]
  let chessMedium = fontFace(
    ~fontFamily="ChessMedium",
    ~src=chessMediumUrl,
    ~fontStyle=normal,
    ~fontWeight=`normal,
    (),
  )
    
  let megrim = fontFace(
    ~fontFamily="Megrim",
    ~src=[url("/assets/fonts/Megrim-Regular.ttf")],
    ~fontStyle=`normal,
    ~fontWeight=`medium,
    ()
  )
    
  // body
  global("body", [
    display(`flex),
    flexDirection(row),
    height(`percent(100.0))
  ])
    
  global("h1", [
    fontFamily(`custom(megrim))
  ])
    
  // app header
  let head = style([
    width(`percent(100.0)),

    selector("& > h1", [display(inlineBlock)])
  ])

  // app logo
  let logo = style([
    fontFamily(`custom(chessMedium)),
    display(inlineBlock),
    fontSize(pt(25)),
    padding(px(10)),
    transform(rotate(deg(150.0)))
  ])
}


module Logo {
  [@react.component]
  let make = () => {
    <span className=Styles.logo>{React.string("Q")}</span>
  }
}

module Head {
  [@react.component]
  let make = () => {
    <div className=Styles.head>
      <Logo />
      <h1>{React.string("My Best Blunders")}</h1>
    </div>
  }
}
  
module App {
  let setup:Chess.setup = Chess.Setup.default;
  let parsed =
    Fen.parse("rnbq1rk1/ppp1ppb1/3p1npp/8/3PP1P1/2N1B3/PPP1BP1P/R2QK1NR")
    |> Option.getOrElse(Chess.Setup.default);

  [@react.component]
  let make = () => {
    <>
      <Head />
      <Board setup=parsed />
    </>
  }
}

ReactDOMRe.render(
  <App />,
  app
);
