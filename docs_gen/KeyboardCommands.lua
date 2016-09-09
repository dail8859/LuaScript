--- Keyboard Commands
-- @section keyboardcommands

--- Move caret down one line.
-- @function LineDown

--- Move caret down one line extending selection to new caret position.
-- @function LineDownExtend

--- Move caret down one line, extending rectangular selection to new caret position.
-- @function LineDownRectExtend

--- Scroll the document down, keeping the caret visible.
-- @function LineScrollDown

--- Move caret up one line.
-- @function LineUp

--- Move caret up one line extending selection to new caret position.
-- @function LineUpExtend

--- Move caret up one line, extending rectangular selection to new caret position.
-- @function LineUpRectExtend

--- Scroll the document up, keeping the caret visible.
-- @function LineScrollUp

--- Move caret between paragraphs (delimited by empty lines).
-- @function ParaDown

--- Move caret between paragraphs (delimited by empty lines).
-- @function ParaDownExtend

--- Move caret between paragraphs (delimited by empty lines).
-- @function ParaUp

--- Move caret between paragraphs (delimited by empty lines).
-- @function ParaUpExtend

--- Move caret left one character.
-- @function CharLeft

--- Move caret left one character extending selection to new caret position.
-- @function CharLeftExtend

--- Move caret left one character, extending rectangular selection to new caret position.
-- @function CharLeftRectExtend

--- Move caret right one character.
-- @function CharRight

--- Move caret right one character extending selection to new caret position.
-- @function CharRightExtend

--- Move caret right one character, extending rectangular selection to new caret position.
-- @function CharRightRectExtend

--- Move caret left one word.
-- @function WordLeft

--- Move caret left one word extending selection to new caret position.
-- @function WordLeftExtend

--- Move caret right one word.
-- @function WordRight

--- Move caret right one word extending selection to new caret position.
-- @function WordRightExtend

--- Move caret left one word, position cursor at end of word.
-- @function WordLeftEnd

--- Move caret left one word, position cursor at end of word, extending selection to new caret position.
-- @function WordLeftEndExtend

--- Move caret right one word, position cursor at end of word.
-- @function WordRightEnd

--- Move caret right one word, position cursor at end of word, extending selection to new caret position.
-- @function WordRightEndExtend

--- Move to the previous change in capitalization.
-- @function WordPartLeft

--- Move to the previous change in capitalization extending selection to new caret position.
-- @function WordPartLeftExtend

--- Move to the change next in capitalization.
-- @function WordPartRight

--- Move to the next change in capitalization extending selection to new caret position.
-- @function WordPartRightExtend

--- Move caret to first position on line.
-- @function Home

--- Move caret to first position on line extending selection to new caret position.
-- @function HomeExtend

--- Move caret to first position on line, extending rectangular selection to new caret position.
-- @function HomeRectExtend

--- Move caret to first position on display line.
-- @function HomeDisplay

--- Move caret to first position on display line extending selection to new caret position.
-- @function HomeDisplayExtend

--- Similarly Home when word-wrap is enabled.
-- @function HomeWrap

--- Similarly HomeExtend when word-wrap is enabled.
-- @function HomeWrapExtend

--- Move caret to before first visible character on line. If already there move to first character on line.
-- @function VCHome

--- Like VCHome but extending selection to new caret position.
-- @function VCHomeExtend

--- Move caret to before first visible character on line. If already there move to first character on line. In either case, extend rectangular selection to new caret position.
-- @function VCHomeRectExtend

--- 
-- @function VCHomeWrap

--- 
-- @function VCHomeWrapExtend

--- 
-- @function VCHomeDisplay

--- 
-- @function VCHomeDisplayExtend

--- Move caret to last position on line.
-- @function LineEnd

--- Move caret to last position on line extending selection to new caret position.
-- @function LineEndExtend

--- Move caret to last position on line, extending rectangular selection to new caret position.
-- @function LineEndRectExtend

--- Move caret to last position on display line.
-- @function LineEndDisplay

--- Move caret to last position on display line extending selection to new caret position.
-- @function LineEndDisplayExtend

--- Similarly LineEnd when word-wrap is enabled.
-- @function LineEndWrap

--- Similarly LineEndExtend when word-wrap is enabled.
-- @function LineEndWrapExtend

--- Move caret to first position in document.
-- @function DocumentStart

--- Move caret to first position in document extending selection to new caret position.
-- @function DocumentStartExtend

--- Move caret to last position in document.
-- @function DocumentEnd

--- Move caret to last position in document extending selection to new caret position.
-- @function DocumentEndExtend

--- Move caret one page up.
-- @function PageUp

--- Move caret one page up extending selection to new caret position.
-- @function PageUpExtend

--- Move caret one page up, extending rectangular selection to new caret position.
-- @function PageUpRectExtend

--- Move caret one page down.
-- @function PageDown

--- Move caret one page down extending selection to new caret position.
-- @function PageDownExtend

--- Move caret one page down, extending rectangular selection to new caret position.
-- @function PageDownRectExtend

--- Move caret to top of page, or one page up if already at top of page.
-- @function StutteredPageUp

--- Move caret to top of page, or one page up if already at top of page, extending selection to new caret position.
-- @function StutteredPageUpExtend

--- Move caret to bottom of page, or one page down if already at bottom of page.
-- @function StutteredPageDown

--- Move caret to bottom of page, or one page down if already at bottom of page, extending selection to new caret position.
-- @function StutteredPageDownExtend

--- Delete the selection or if no selection, the character before the caret.
-- @function DeleteBack

--- Delete the selection or if no selection, the character before the caret. Will not delete the character before at the start of a line.
-- @function DeleteBackNotLine

--- Delete the word to the left of the caret.
-- @function DelWordLeft

--- Delete the word to the right of the caret.
-- @function DelWordRight

--- Delete the word to the right of the caret, but not the trailing non-word characters.
-- @function DelWordRightEnd

--- Delete back from the current position to the start of the line.
-- @function DelLineLeft

--- Delete forwards from the current position to the end of the line.
-- @function DelLineRight

--- Delete the line containing the caret.
-- @function LineDelete

--- Cut the line containing the caret.
-- @function LineCut

--- Copy the line containing the caret.
-- @function LineCopy

--- Switch the current line with the previous.
-- @function LineTranspose

--- Duplicate the current line.
-- @function LineDuplicate

--- Transform the selection to lower case.
-- @function LowerCase

--- Transform the selection to upper case.
-- @function UpperCase

--- Cancel any modes such as call tip or auto-completion list display.
-- @function Cancel

--- Switch from insert to overtype mode or the reverse.
-- @function EditToggleOvertype

--- Insert a new line, may use a CRLF, CR or LF depending on EOL mode.
-- @function NewLine

--- Insert a Form Feed character.
-- @function FormFeed

--- If selection is empty or all on one line replace the selection with a tab character. If more than one line selected, indent the lines.
-- @function Tab

--- Dedent the selected lines.
-- @function BackTab

--- Duplicates the current selection.
-- @function SelectionDuplicate

--- Scroll the document so that the caret is in the center of the screen.
-- @function VerticalCentreCaret

--- Move the selected lines up one line, shifting the line above after the selection.
-- @function MoveSelectedLinesUp

--- Move the selected lines down one line, shifting the line below before the selection.
-- @function MoveSelectedLinesDown

--- Scroll the document to the start without changing the selection.
-- @function ScrollToStart

--- Scroll the document to the end without changing the selection.
-- @function ScrollToEnd
