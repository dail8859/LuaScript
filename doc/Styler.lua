--- 
-- Description
-- 
-- @classmod Styler

--- Start setting styles from startPos for length with initial style initStyle.
-- @function StartStyling
-- @tparam int startPos
-- @tparam int length
-- @tparam int initStyle

--- Styling has been completed so tidy up.
-- @function EndStyling

--- Are there any more characters to process.
-- @function More
-- @treturn boolean

--- Move forward one character.
-- @function Forward

--- What is the position in the document of the current character.
-- @function Position
-- @treturn int

--- Is the current character the first on a line.
-- @function AtLineStart
-- @treturn boolean

--- Is the current character the last on a line.
-- @function AtLineEnd
-- @treturn boolean 

--- The current lexical state value.
-- @function State
-- @treturn int

--- Set the style of the current token to the current state and then change the state to the argument.
-- @function SetState

--- Combination of moving forward and setting the state.
-- Useful when the current character is a token terminator like " for a string.
-- @function ForwardSetState

--- Change the current state so that the state of the current token will be set to the argument.
-- @function ChangeState
-- @tparam int state

--- The current character.
-- @function Current
-- @treturn string

--- The next character.
-- @function Next
-- @treturn string

--- The previous character.
-- @function Previous
-- @treturn string

--- The current token.
-- @function Token
-- @treturn string

--- Is the text from the current position the same as the argument?
-- @function Match
-- @tparam string str
-- @treturn boolean

--- Convert a byte position into a line number.
-- @function Line
-- @tparam int position
-- @treturn int

--- Unsigned byte value at argument.
-- @function CharAt
-- @tparam int position
-- @treturn int

--- Style value at argument.
-- @function StyleAt
-- @tparam int position
-- @treturn int 

--- Fold level for a line.
-- @function LevelAt
-- @tparam int line
-- @treturn int

--- Set the fold level for a line.
-- @function SetLevelAt
-- @tparam int line
-- @tparam int level

--- State value for a line.
-- @function LineState
-- @tparam int line
-- @treturn int

--- Set state value for a line.
-- This can be used to store extra information from lexing, such as a current language mode, so that there is no need to look back in the document.
-- @function SetLineState
-- @tparam int line
-- @tparam int state

--- Start of the range to be lexed.
-- @tparam[readonly] int startPos

--- Length of the range to be lexed.
-- @tparam[readonly] int lengthDoc

--- Starting style.
-- @tparam[readonly] int initStyle
