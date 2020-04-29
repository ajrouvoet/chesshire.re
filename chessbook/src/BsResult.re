type t('a, 'e);

[@bs.get]
external isOk : t('a, 'e) => bool = "isOk";

[@bs.get]
external isErr : t('a, 'e) => bool = "isErr";

[@bs.get]
external error : t('a, 'e) => 'e = "error";

[@bs.get]
external value : t('a, 'e) => 'a = "value";

[@bs.send]
external unwrap : (t('a, 'e), 'a => 'b, 'e => 'b) => 'b = "unwrap";
