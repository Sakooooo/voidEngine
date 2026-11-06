 -- keep in sync with KeyboardState::string_key_map in src/engine.h
type Key =
	-- letters
	| "a"
	| "b"
	| "c"
	| "d"
	| "e"
	| "f"
	| "g"
	| "h"
	| "i"
	| "j"
	| "k"
	| "l"
	| "m"
	| "n"
	| "o"
	| "p"
	| "q"
	| "r"
	| "s"
	| "t"
	| "u"
	| "v"
	| "w"
	| "x"
	| "y"
	| "z"
	-- numbers
	| "0"
	| "1"
	| "2"
	| "3"
	| "4"
	| "5"
	| "6"
	| "7"
	| "8"
	| "9"
	-- function keys
	| "f1"
	| "f2"
	| "f3"
	| "f4"
	| "f5"
	| "f6"
	| "f7"
	| "f8"
	| "f9"
	| "f10"
	| "f11"
	| "f12"
	-- arrows
	| "up"
	| "down"
	| "left"
	| "right"
	-- modifiers
	| "lshift"
	| "rshift"
	| "lctrl"
	| "rctrl"
	| "lalt"
	| "ralt"
	| "lgui"
	| "rgui"
	-- misc
	| "space"
	| "return"
	| "escape"
	| "backspace"
	| "tab"
	| "capslock"
	| "insert"
	| "delete"
	| "home"
	| "end"
	| "pageup"
	| "pagedown"
	| "minus"
	| "equals"
	| "leftbracket"
	| "rightbracket"
	| "backslash"
	| "semicolon"
	| "apostrophe"
	| "grave"
	| "comma"
	| "period"
	| "slash"

declare input: {
    is_pressed: (key: Key) -> boolean,
}

-- the self table passed to script callbacks, see Script::load_script
type Entity = {
    id: number,
    has: (self: Entity, component: string) -> boolean,
}

declare entity: Entity

type Vector2MetaTable = {
    __add: (a: Vector2, b: Vector2) -> Vector2,
}

type Vector2 = typeof(setmetatable({} :: {
    x: number,
    y: number,
	normalize: (self: Vector2) -> Vector2
    }, {} :: Vector2MetaTable))

declare Vector2: {
    new: (x: number, y: number) -> Vector2
}

-- declare class Vector2: {
--     x: number,
--     y: number,
--     normalize: (self: Vector2) -> Vector2,
--     __add: (self: Vector2, other: Vector2) -> Vector2
-- }

-- declare Vector2: {
--     new: (x: number, y: number) -> Vector2,
--     normalize: (vector: Vector2) -> Vector2,
-- }


declare void: {
    tick: ((self: Entity, dt: number) -> ())?,
}
