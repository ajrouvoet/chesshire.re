/*{
  allowAnyClick: boolean,
  cancel: string,
  disabled: boolean,
  enableUserSelectHack: boolean,
  offsetParent: HTMLElement,
  grid: [number, number],
  handle: string,
  onStart: DraggableEventHandler,
  onDrag: DraggableEventHandler,
  onStop: DraggableEventHandler,
  onMouseDown: (e: MouseEvent) => void,
  scale: number
}*/

type draggableData = 
  { node: Dom.element
  , x: int
  , y: int
  , deltaX: int
  , deltaY: int
  , lastX:int
  , lastY: int
  };
  
type draggableEventHandler = (ReactEvent.Mouse.t, draggableData) => bool;

[@bs.module "react-draggable/build/web/react-draggable.min.js"]
[@react.component]
external make: (
  ~onStart: draggableEventHandler,
  ~onDrag: draggableEventHandler,
  ~onStop: draggableEventHandler,
  ~children: React.element) => React.element = "DraggableCore";

