# Editor Object
- `editor` - Refers to the current editor view. Since Notepad++ has 2 views, this controls the one currently selected.
  - `editor1` - Refers to the "first" view.
  - `editor2` - Refers to the "second" view.

### Text Retrieval and Modification

**string editor:[GetText](http://www.scintilla.org/ScintillaDoc.html#SCI_GETTEXT)()** -- _Retrieve all the text in the document. Returns number of characters retrieved. Result is NUL-terminated._

**editor:[SetText](http://www.scintilla.org/ScintillaDoc.html#SCI_SETTEXT)(string text)** -- _Replace the contents of the document with the argument text._

**editor:[SetSavePoint](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSAVEPOINT)()** -- _Remember the current position in the undo history as the position at which the document was saved._

**string editor:[GetLine](http://www.scintilla.org/ScintillaDoc.html#SCI_GETLINE)(int line)** -- _Retrieve the contents of a line. Returns the length of the line._

**editor:[ReplaceSel](http://www.scintilla.org/ScintillaDoc.html#SCI_REPLACESEL)(string text)** -- _Replace the selected text with the argument text._

**bool editor.[ReadOnly](http://www.scintilla.org/ScintillaDoc.html#SCI_SETREADONLY)** -- _Set to read only or read write._

**editor:[Allocate](http://www.scintilla.org/ScintillaDoc.html#SCI_ALLOCATE)(int bytes)** -- _Enlarge the document to a particular size of text bytes._

**editor:[AddText](http://www.scintilla.org/ScintillaDoc.html#SCI_ADDTEXT)(string text)** -- _Add text to the document at current position._

**editor:[AppendText](http://www.scintilla.org/ScintillaDoc.html#SCI_APPENDTEXT)(string text)** -- _Append a string to the end of the document without changing the selection._

**editor:[InsertText](http://www.scintilla.org/ScintillaDoc.html#SCI_INSERTTEXT)(position pos, string text)** -- _Insert string at a position._

**editor:[ChangeInsertion](http://www.scintilla.org/ScintillaDoc.html#SCI_CHANGEINSERTION)(string text)** -- _Change the text that is being inserted in response to SC_MOD_INSERTCHECK_

**editor:[ClearAll](http://www.scintilla.org/ScintillaDoc.html#SCI_CLEARALL)()** -- _Delete all text in the document._

**editor:[DeleteRange](http://www.scintilla.org/ScintillaDoc.html#SCI_DELETERANGE)(position pos, int deleteLength)** -- _Delete a range of text in the document._

**editor:[ClearDocumentStyle](http://www.scintilla.org/ScintillaDoc.html#SCI_CLEARDOCUMENTSTYLE)()** -- _Set all style bytes to 0, remove all folding information._

**int editor.[CharAt](http://www.scintilla.org/ScintillaDoc.html#SCI_GETCHARAT)[position pos]** read-only

**int editor.[StyleAt](http://www.scintilla.org/ScintillaDoc.html#SCI_GETSTYLEAT)[position pos]** read-only

**editor:[ReleaseAllExtendedStyles](http://www.scintilla.org/ScintillaDoc.html#SCI_RELEASEALLEXTENDEDSTYLES)()** -- _Release all extended (>255) style numbers_

**int editor:[AllocateExtendedStyles](http://www.scintilla.org/ScintillaDoc.html#SCI_ALLOCATEEXTENDEDSTYLES)(int numberStyles)** -- _Allocate some extended (>255) style numbers and return the start of the range_

**string editor:[TargetAsUTF8](http://www.scintilla.org/ScintillaDoc.html#SCI_TARGETASUTF8)()** -- _Returns the target converted to UTF8. Return the length in bytes._

**string editor:[EncodedFromUTF8](http://www.scintilla.org/ScintillaDoc.html#SCI_ENCODEDFROMUTF8)(string utf8)** -- _Translates a UTF8 string into the document encoding. Return the length of the result in bytes. On error return 0._

**editor:[SetLengthForEncode](http://www.scintilla.org/ScintillaDoc.html#SCI_SETLENGTHFORENCODE)(int bytes)** -- _Set the length of the utf8 argument for calling EncodedFromUTF8. Set to -1 and the string will be measured to the first nul._

### Searching

**position editor.[TargetStart](http://www.scintilla.org/ScintillaDoc.html#SCI_SETTARGETSTART)** -- _Sets the position that starts the target which is used for updating the document without affecting the scroll position._

**position editor.[TargetEnd](http://www.scintilla.org/ScintillaDoc.html#SCI_SETTARGETEND)** -- _Sets the position that ends the target which is used for updating the document without affecting the scroll position._

**editor:[SetTargetRange](http://www.scintilla.org/ScintillaDoc.html#SCI_SETTARGETRANGE)(position start, position end)** -- _Sets both the start and end of the target in one call._

**editor:[TargetFromSelection](http://www.scintilla.org/ScintillaDoc.html#SCI_TARGETFROMSELECTION)()** -- _Make the target range start and end be the same as the selection range start and end._

**editor:[TargetWholeDocument](http://www.scintilla.org/ScintillaDoc.html#SCI_TARGETWHOLEDOCUMENT)()** -- _Sets the target to the whole document._

**int editor.[SearchFlags](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSEARCHFLAGS)** -- _Set the search flags used by SearchInTarget._

**int editor:[SearchInTarget](http://www.scintilla.org/ScintillaDoc.html#SCI_SEARCHINTARGET)(string text)** -- _Search for a counted string in the target and set the target to the found range. Text is counted so it can contain NULs. Returns length of range or -1 for failure in which case target is not moved._

**string editor.[TargetText](http://www.scintilla.org/ScintillaDoc.html#SCI_GETTARGETTEXT)** read-only

**int editor:[ReplaceTarget](http://www.scintilla.org/ScintillaDoc.html#SCI_REPLACETARGET)(string text)** -- _Replace the target text with the argument text. Text is counted so it can contain NULs. Returns the length of the replacement text._

**int editor:[ReplaceTargetRE](http://www.scintilla.org/ScintillaDoc.html#SCI_REPLACETARGETRE)(string text)** -- _Replace the target text with the argument text after \d processing. Text is counted so it can contain NULs. Looks for \d where d is between 1 and 9 and replaces these with the strings matched in the last search operation which were surrounded by \( and \). Returns the length of the replacement text including any change caused by processing the \d patterns._

**string editor.[Tag](http://www.scintilla.org/ScintillaDoc.html#SCI_GETTAG)[int tagNumber]** read-only

**editor:[SearchAnchor](http://www.scintilla.org/ScintillaDoc.html#SCI_SEARCHANCHOR)()** -- _Sets the current caret position to be the search anchor._

**int editor:[SearchNext](http://www.scintilla.org/ScintillaDoc.html#SCI_SEARCHNEXT)(int flags, string text)** -- _Find some text starting at the search anchor. Does not ensure the selection is visible._

**int editor:[SearchPrev](http://www.scintilla.org/ScintillaDoc.html#SCI_SEARCHPREV)(int flags, string text)** -- _Find some text starting at the search anchor and moving backwards. Does not ensure the selection is visible._

### Overtype

**bool editor.[Overtype](http://www.scintilla.org/ScintillaDoc.html#SCI_SETOVERTYPE)** -- _Set to overtype (true) or insert mode._

### Cut, Copy and Paste

**editor:[Cut](http://www.scintilla.org/ScintillaDoc.html#SCI_CUT)()** -- _Cut the selection to the clipboard._

**editor:[Copy](http://www.scintilla.org/ScintillaDoc.html#SCI_COPY)()** -- _Copy the selection to the clipboard._

**editor:[Paste](http://www.scintilla.org/ScintillaDoc.html#SCI_PASTE)()** -- _Paste the contents of the clipboard into the document replacing the selection._

**editor:[Clear](http://www.scintilla.org/ScintillaDoc.html#SCI_CLEAR)()** -- _Clear the selection._

**bool editor:[CanPaste](http://www.scintilla.org/ScintillaDoc.html#SCI_CANPASTE)()** -- _Will a paste succeed?_

**editor:[CopyAllowLine](http://www.scintilla.org/ScintillaDoc.html#SCI_COPYALLOWLINE)()** -- _Copy the selection, if selection empty copy the line with the caret_

**editor:[CopyRange](http://www.scintilla.org/ScintillaDoc.html#SCI_COPYRANGE)(position start, position end)** -- _Copy a range of text to the clipboard. Positions are clipped into the document._

**editor:[CopyText](http://www.scintilla.org/ScintillaDoc.html#SCI_COPYTEXT)(string text)** -- _Copy argument text to the clipboard._

**bool editor.[PasteConvertEndings](http://www.scintilla.org/ScintillaDoc.html#SCI_SETPASTECONVERTENDINGS)** -- _Enable/Disable convert-on-paste for line endings_

### Error Handling

**int editor.[Status](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSTATUS)** -- _Change error status - 0 = OK._

### Undo and Redo

**editor:[Undo](http://www.scintilla.org/ScintillaDoc.html#SCI_UNDO)()** -- _Undo one action in the undo history._

**bool editor:[CanUndo](http://www.scintilla.org/ScintillaDoc.html#SCI_CANUNDO)()** -- _Are there any undoable actions in the undo history?_

**editor:[Redo](http://www.scintilla.org/ScintillaDoc.html#SCI_REDO)()** -- _Redoes the next action on the undo history._

**bool editor:[CanRedo](http://www.scintilla.org/ScintillaDoc.html#SCI_CANREDO)()** -- _Are there any redoable actions in the undo history?_

**editor:[EmptyUndoBuffer](http://www.scintilla.org/ScintillaDoc.html#SCI_EMPTYUNDOBUFFER)()** -- _Delete the undo history._

**bool editor.[UndoCollection](http://www.scintilla.org/ScintillaDoc.html#SCI_SETUNDOCOLLECTION)** -- _Choose between collecting actions into the undo history and discarding them._

**editor:[BeginUndoAction](http://www.scintilla.org/ScintillaDoc.html#SCI_BEGINUNDOACTION)()** -- _Start a sequence of actions that is undone and redone as a unit. May be nested._

**editor:[EndUndoAction](http://www.scintilla.org/ScintillaDoc.html#SCI_ENDUNDOACTION)()** -- _End a sequence of actions that is undone and redone as a unit._

**editor:[AddUndoAction](http://www.scintilla.org/ScintillaDoc.html#SCI_ADDUNDOACTION)(int token, int flags)** -- _Add a container action to the undo stack_

### Selection and Information

**int editor.[TextLength](http://www.scintilla.org/ScintillaDoc.html#SCI_GETTEXTLENGTH)** read-only

**int editor.[Length](http://www.scintilla.org/ScintillaDoc.html#SCI_GETLENGTH)** read-only

**int editor.[LineCount](http://www.scintilla.org/ScintillaDoc.html#SCI_GETLINECOUNT)** read-only

**int editor.[FirstVisibleLine](http://www.scintilla.org/ScintillaDoc.html#SCI_SETFIRSTVISIBLELINE)** -- _Scroll so that a display line is at the top of the display._

**int editor.[LinesOnScreen](http://www.scintilla.org/ScintillaDoc.html#SCI_LINESONSCREEN)** read-only

**bool editor.[Modify](http://www.scintilla.org/ScintillaDoc.html#SCI_GETMODIFY)** read-only

**editor:[SetSel](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSEL)(position start, position end)** -- _Select a range of text._

**editor:[GotoPos](http://www.scintilla.org/ScintillaDoc.html#SCI_GOTOPOS)(position pos)** -- _Set caret to a position and ensure it is visible._

**editor:[GotoLine](http://www.scintilla.org/ScintillaDoc.html#SCI_GOTOLINE)(int line)** -- _Set caret to start of a line and ensure it is visible._

**position editor.[CurrentPos](http://www.scintilla.org/ScintillaDoc.html#SCI_SETCURRENTPOS)** -- _Sets the position of the caret._

**position editor.[Anchor](http://www.scintilla.org/ScintillaDoc.html#SCI_SETANCHOR)** -- _Set the selection anchor to a position. The anchor is the opposite end of the selection from the caret._

**position editor.[SelectionStart](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSELECTIONSTART)** -- _Sets the position that starts the selection - this becomes the anchor._

**position editor.[SelectionEnd](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSELECTIONEND)** -- _Sets the position that ends the selection - this becomes the currentPosition._

**editor:[SetEmptySelection](http://www.scintilla.org/ScintillaDoc.html#SCI_SETEMPTYSELECTION)(position pos)** -- _Set caret to a position, while removing any existing selection._

**editor:[SelectAll](http://www.scintilla.org/ScintillaDoc.html#SCI_SELECTALL)()** -- _Select all the text in the document._

**int editor:[LineFromPosition](http://www.scintilla.org/ScintillaDoc.html#SCI_LINEFROMPOSITION)(position pos)** -- _Retrieve the line containing a position._

**position editor:[PositionFromLine](http://www.scintilla.org/ScintillaDoc.html#SCI_POSITIONFROMLINE)(int line)** -- _Retrieve the position at the start of a line._

**position editor.[LineEndPosition](http://www.scintilla.org/ScintillaDoc.html#SCI_GETLINEENDPOSITION)[int line]** read-only

**int editor:[LineLength](http://www.scintilla.org/ScintillaDoc.html#SCI_LINELENGTH)(int line)** -- _How many characters are on a line, including end of line characters?_

**string editor:[GetSelText](http://www.scintilla.org/ScintillaDoc.html#SCI_GETSELTEXT)()** -- _Retrieve the selected text. Return the length of the text. Result is NUL-terminated._

**string editor:[GetCurLine](http://www.scintilla.org/ScintillaDoc.html#SCI_GETCURLINE)()** -- _Retrieve the text of the line containing the caret. Returns the index of the caret on the line. Result is NUL-terminated._

**bool editor.[SelectionIsRectangle](http://www.scintilla.org/ScintillaDoc.html#SCI_SELECTIONISRECTANGLE)** read-only

**int editor.[SelectionMode](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSELECTIONMODE)** -- _Set the selection mode to stream (SC_SEL_STREAM) or rectangular (SC_SEL_RECTANGLE/SC_SEL_THIN) or by lines (SC_SEL_LINES)._

**position editor:[GetLineSelStartPosition](http://www.scintilla.org/ScintillaDoc.html#SCI_GETLINESELSTARTPOSITION)(int line)** -- _Retrieve the position of the start of the selection at the given line (INVALID_POSITION if no selection on this line)._

**position editor:[GetLineSelEndPosition](http://www.scintilla.org/ScintillaDoc.html#SCI_GETLINESELENDPOSITION)(int line)** -- _Retrieve the position of the end of the selection at the given line (INVALID_POSITION if no selection on this line)._

**editor:[MoveCaretInsideView](http://www.scintilla.org/ScintillaDoc.html#SCI_MOVECARETINSIDEVIEW)()** -- _Move the caret inside current view if it's not there already._

**int editor:[WordEndPosition](http://www.scintilla.org/ScintillaDoc.html#SCI_WORDENDPOSITION)(position pos, bool onlyWordCharacters)** -- _Get position of end of word._

**int editor:[WordStartPosition](http://www.scintilla.org/ScintillaDoc.html#SCI_WORDSTARTPOSITION)(position pos, bool onlyWordCharacters)** -- _Get position of start of word._

**bool editor:[IsRangeWord](http://www.scintilla.org/ScintillaDoc.html#SCI_ISRANGEWORD)(position start, position end)** -- _Is the range start..end considered a word?_

**position editor:[PositionBefore](http://www.scintilla.org/ScintillaDoc.html#SCI_POSITIONBEFORE)(position pos)** -- _Given a valid document position, return the previous position taking code page into account. Returns 0 if passed 0._

**position editor:[PositionAfter](http://www.scintilla.org/ScintillaDoc.html#SCI_POSITIONAFTER)(position pos)** -- _Given a valid document position, return the next position taking code page into account. Maximum value returned is the last position in the document._

**position editor:[PositionRelative](http://www.scintilla.org/ScintillaDoc.html#SCI_POSITIONRELATIVE)(position pos, int relative)** -- _Given a valid document position, return a position that differs in a number of characters. Returned value is always between 0 and last position in document._

**int editor:[CountCharacters](http://www.scintilla.org/ScintillaDoc.html#SCI_COUNTCHARACTERS)(int startPos, int endPos)** -- _Count characters between two positions._

**int editor:[TextWidth](http://www.scintilla.org/ScintillaDoc.html#SCI_TEXTWIDTH)(int style, string text)** -- _Measure the pixel width of some text in a particular style. NUL terminated text argument. Does not handle tab or control characters._

**int editor:[TextHeight](http://www.scintilla.org/ScintillaDoc.html#SCI_TEXTHEIGHT)(int line)** -- _Retrieve the height of a particular line of text in pixels._

**int editor.[Column](http://www.scintilla.org/ScintillaDoc.html#SCI_GETCOLUMN)[position pos]** read-only

**int editor:[FindColumn](http://www.scintilla.org/ScintillaDoc.html#SCI_FINDCOLUMN)(int line, int column)** -- _Find the position of a column on a line taking into account tabs and multi-byte characters. If beyond end of line, return line end position._

**position editor:[PositionFromPoint](http://www.scintilla.org/ScintillaDoc.html#SCI_POSITIONFROMPOINT)(int x, int y)** -- _Find the position from a point within the window._

**position editor:[PositionFromPointClose](http://www.scintilla.org/ScintillaDoc.html#SCI_POSITIONFROMPOINTCLOSE)(int x, int y)** -- _Find the position from a point within the window but return INVALID_POSITION if not close to text._

**position editor:[CharPositionFromPoint](http://www.scintilla.org/ScintillaDoc.html#SCI_CHARPOSITIONFROMPOINT)(int x, int y)** -- _Find the position of a character from a point within the window._

**position editor:[CharPositionFromPointClose](http://www.scintilla.org/ScintillaDoc.html#SCI_CHARPOSITIONFROMPOINTCLOSE)(int x, int y)** -- _Find the position of a character from a point within the window. Return INVALID_POSITION if not close to text._

**int editor:[PointXFromPosition](http://www.scintilla.org/ScintillaDoc.html#SCI_POINTXFROMPOSITION)(position pos)** -- _Retrieve the x value of the point in the window where a position is displayed._

**int editor:[PointYFromPosition](http://www.scintilla.org/ScintillaDoc.html#SCI_POINTYFROMPOSITION)(position pos)** -- _Retrieve the y value of the point in the window where a position is displayed._

**editor:[HideSelection](http://www.scintilla.org/ScintillaDoc.html#SCI_HIDESELECTION)(bool normal)** -- _Draw the selection in normal style or with selection highlighted._

**editor:[ChooseCaretX](http://www.scintilla.org/ScintillaDoc.html#SCI_CHOOSECARETX)()** -- _Set the last x chosen value to be the caret x position._

**editor:[MoveSelectedLinesUp](http://www.scintilla.org/ScintillaDoc.html#SCI_MOVESELECTEDLINESUP)()** -- _Move the selected lines up one line, shifting the line above after the selection_

**editor:[MoveSelectedLinesDown](http://www.scintilla.org/ScintillaDoc.html#SCI_MOVESELECTEDLINESDOWN)()** -- _Move the selected lines down one line, shifting the line below before the selection_

**bool editor.[MouseSelectionRectangularSwitch](http://www.scintilla.org/ScintillaDoc.html#SCI_SETMOUSESELECTIONRECTANGULARSWITCH)** -- _Set whether switching to rectangular mode while selecting with the mouse is allowed._

### Multiple Selection and Virtual Space

**bool editor.[MultipleSelection](http://www.scintilla.org/ScintillaDoc.html#SCI_SETMULTIPLESELECTION)** -- _Set whether multiple selections can be made_

**bool editor.[AdditionalSelectionTyping](http://www.scintilla.org/ScintillaDoc.html#SCI_SETADDITIONALSELECTIONTYPING)** -- _Set whether typing can be performed into multiple selections_

**int editor.[MultiPaste](http://www.scintilla.org/ScintillaDoc.html#SCI_SETMULTIPASTE)** -- _Change the effect of pasting when there are multiple selections._

**int editor.[VirtualSpaceOptions](http://www.scintilla.org/ScintillaDoc.html#SCI_SETVIRTUALSPACEOPTIONS)** -- _Returns the position at the end of the selection._

**int editor.[RectangularSelectionModifier](http://www.scintilla.org/ScintillaDoc.html#SCI_SETRECTANGULARSELECTIONMODIFIER)** -- _On GTK+, allow selecting the modifier key to use for mouse-based rectangular selection. Often the window manager requires Alt+Mouse Drag for moving windows. Valid values are SCMOD_CTRL(default), SCMOD_ALT, or SCMOD_SUPER._

**int editor.[Selections](http://www.scintilla.org/ScintillaDoc.html#SCI_GETSELECTIONS)** read-only

**bool editor.[SelectionEmpty](http://www.scintilla.org/ScintillaDoc.html#SCI_GETSELECTIONEMPTY)** read-only

**editor:[ClearSelections](http://www.scintilla.org/ScintillaDoc.html#SCI_CLEARSELECTIONS)()** -- _Clear selections to a single empty stream selection_

**int editor:[SetSelection](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSELECTION)(int caret, int anchor)** -- _Set a simple selection_

**int editor:[AddSelection](http://www.scintilla.org/ScintillaDoc.html#SCI_ADDSELECTION)(int caret, int anchor)** -- _Add a selection_

**editor:[DropSelectionN](http://www.scintilla.org/ScintillaDoc.html#SCI_DROPSELECTIONN)(int selection)** -- _Drop one selection_

**int editor.[MainSelection](http://www.scintilla.org/ScintillaDoc.html#SCI_SETMAINSELECTION)** -- _Set the main selection_

**position editor.[SelectionNCaret](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSELECTIONNCARET)[int selection]** -- _Which selection is the main selection_

**int editor.[SelectionNCaretVirtualSpace](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSELECTIONNCARETVIRTUALSPACE)[int selection]** -- _Which selection is the main selection_

**position editor.[SelectionNAnchor](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSELECTIONNANCHOR)[int selection]** -- _Which selection is the main selection_

**int editor.[SelectionNAnchorVirtualSpace](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSELECTIONNANCHORVIRTUALSPACE)[int selection]** -- _Which selection is the main selection_

**position editor.[SelectionNStart](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSELECTIONNSTART)[int selection]** -- _Sets the position that starts the selection - this becomes the anchor._

**position editor.[SelectionNEnd](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSELECTIONNEND)[int selection]** -- _Sets the position that ends the selection - this becomes the currentPosition._

**position editor.[RectangularSelectionCaret](http://www.scintilla.org/ScintillaDoc.html#SCI_SETRECTANGULARSELECTIONCARET)** -- _Returns the position at the end of the selection._

**int editor.[RectangularSelectionCaretVirtualSpace](http://www.scintilla.org/ScintillaDoc.html#SCI_SETRECTANGULARSELECTIONCARETVIRTUALSPACE)** -- _Returns the position at the end of the selection._

**position editor.[RectangularSelectionAnchor](http://www.scintilla.org/ScintillaDoc.html#SCI_SETRECTANGULARSELECTIONANCHOR)** -- _Returns the position at the end of the selection._

**int editor.[RectangularSelectionAnchorVirtualSpace](http://www.scintilla.org/ScintillaDoc.html#SCI_SETRECTANGULARSELECTIONANCHORVIRTUALSPACE)** -- _Returns the position at the end of the selection._

**int editor.[AdditionalSelAlpha](http://www.scintilla.org/ScintillaDoc.html#SCI_SETADDITIONALSELALPHA)** -- _Set the alpha of the selection._

**colour editor.[AdditionalSelFore](http://www.scintilla.org/ScintillaDoc.html#SCI_SETADDITIONALSELFORE) write-only** -- _Set the foreground colour of additional selections. Must have previously called SetSelFore with non-zero first argument for this to have an effect._

**colour editor.[AdditionalSelBack](http://www.scintilla.org/ScintillaDoc.html#SCI_SETADDITIONALSELBACK) write-only** -- _Set the background colour of additional selections. Must have previously called SetSelBack with non-zero first argument for this to have an effect._

**colour editor.[AdditionalCaretFore](http://www.scintilla.org/ScintillaDoc.html#SCI_SETADDITIONALCARETFORE)** -- _Set the foreground colour of additional carets._

**bool editor.[AdditionalCaretsBlink](http://www.scintilla.org/ScintillaDoc.html#SCI_SETADDITIONALCARETSBLINK)** -- _Set whether additional carets will blink_

**bool editor.[AdditionalCaretsVisible](http://www.scintilla.org/ScintillaDoc.html#SCI_SETADDITIONALCARETSVISIBLE)** -- _Set whether additional carets are visible_

**editor:[SwapMainAnchorCaret](http://www.scintilla.org/ScintillaDoc.html#SCI_SWAPMAINANCHORCARET)()** -- _Swap that caret and anchor of the main selection._

**editor:[RotateSelection](http://www.scintilla.org/ScintillaDoc.html#SCI_ROTATESELECTION)()** -- _Set the main selection to the next selection._

**editor:[MultipleSelectAddNext](http://www.scintilla.org/ScintillaDoc.html#SCI_MULTIPLESELECTADDNEXT)()** -- _Add the next occurrence of the main selection to the set of selections as main. If the current selection is empty then select word around caret._

**editor:[MultipleSelectAddEach](http://www.scintilla.org/ScintillaDoc.html#SCI_MULTIPLESELECTADDEACH)()** -- _Add each occurrence of the main selection in the target to the set of selections. If the current selection is empty then select word around caret._

### Scrolling and Automatic Scrolling

**editor:[LineScroll](http://www.scintilla.org/ScintillaDoc.html#SCI_LINESCROLL)(int columns, int lines)** -- _Scroll horizontally and vertically._

**editor:[ScrollCaret](http://www.scintilla.org/ScintillaDoc.html#SCI_SCROLLCARET)()** -- _Ensure the caret is visible._

**editor:[ScrollRange](http://www.scintilla.org/ScintillaDoc.html#SCI_SCROLLRANGE)(position secondary, position primary)** -- _Scroll the argument positions and the range between them into view giving priority to the primary position then the secondary position. This may be used to make a search match visible._

**editor:[SetXCaretPolicy](http://www.scintilla.org/ScintillaDoc.html#SCI_SETXCARETPOLICY)(int caretPolicy, int caretSlop)** -- _Set the way the caret is kept visible when going sideways. The exclusion zone is given in pixels._

**editor:[SetYCaretPolicy](http://www.scintilla.org/ScintillaDoc.html#SCI_SETYCARETPOLICY)(int caretPolicy, int caretSlop)** -- _Set the way the line the caret is on is kept visible. The exclusion zone is given in lines._

**editor:[SetVisiblePolicy](http://www.scintilla.org/ScintillaDoc.html#SCI_SETVISIBLEPOLICY)(int visiblePolicy, int visibleSlop)** -- _Set the way the display area is determined when a particular line is to be moved to by Find, FindNext, GotoLine, etc._

**bool editor.[HScrollBar](http://www.scintilla.org/ScintillaDoc.html#SCI_SETHSCROLLBAR)** -- _Show or hide the horizontal scroll bar._

**bool editor.[VScrollBar](http://www.scintilla.org/ScintillaDoc.html#SCI_SETVSCROLLBAR)** -- _Show or hide the vertical scroll bar._

**int editor.[XOffset](http://www.scintilla.org/ScintillaDoc.html#SCI_SETXOFFSET)** -- _Get and Set the xOffset (ie, horizontal scroll position)._

**int editor.[ScrollWidth](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSCROLLWIDTH)** -- _Sets the document width assumed for scrolling._

**bool editor.[ScrollWidthTracking](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSCROLLWIDTHTRACKING)** -- _Sets whether the maximum width line displayed is used to set scroll width._

**bool editor.[EndAtLastLine](http://www.scintilla.org/ScintillaDoc.html#SCI_SETENDATLASTLINE)** -- _Sets the scroll range so that maximum scroll position has the last line at the bottom of the view (default). Setting this to false allows scrolling one page below the last line._

### Whitespace

**int editor.[ViewWS](http://www.scintilla.org/ScintillaDoc.html#SCI_SETVIEWWS)** -- _Make white space characters invisible, always visible or visible outside indentation._

**editor:[SetWhitespaceFore](http://www.scintilla.org/ScintillaDoc.html#SCI_SETWHITESPACEFORE)(bool useSetting, colour fore)** -- _Set the foreground colour of all whitespace and whether to use this setting._

**editor:[SetWhitespaceBack](http://www.scintilla.org/ScintillaDoc.html#SCI_SETWHITESPACEBACK)(bool useSetting, colour back)** -- _Set the background colour of all whitespace and whether to use this setting._

**int editor.[WhitespaceSize](http://www.scintilla.org/ScintillaDoc.html#SCI_SETWHITESPACESIZE)** -- _Set the size of the dots used to mark space characters._

**int editor.[ExtraAscent](http://www.scintilla.org/ScintillaDoc.html#SCI_SETEXTRAASCENT)** -- _Set extra ascent for each line_

**int editor.[ExtraDescent](http://www.scintilla.org/ScintillaDoc.html#SCI_SETEXTRADESCENT)** -- _Set extra descent for each line_

### Cursor

**int editor.[Cursor](http://www.scintilla.org/ScintillaDoc.html#SCI_SETCURSOR)** -- _Sets the cursor to one of the SC_CURSOR* values._

### Mouse Capture

**bool editor.[MouseDownCaptures](http://www.scintilla.org/ScintillaDoc.html#SCI_SETMOUSEDOWNCAPTURES)** -- _Set whether the mouse is captured when its button is pressed._

### Line endings

**int editor.[EOLMode](http://www.scintilla.org/ScintillaDoc.html#SCI_SETEOLMODE)** -- _Set the current end of line mode._

**editor:[ConvertEOLs](http://www.scintilla.org/ScintillaDoc.html#SCI_CONVERTEOLS)(int eolMode)** -- _Convert all line endings in the document to one mode._

**bool editor.[ViewEOL](http://www.scintilla.org/ScintillaDoc.html#SCI_SETVIEWEOL)** -- _Make the end of line characters visible or invisible._

**int editor.[LineEndTypesSupported](http://www.scintilla.org/ScintillaDoc.html#SCI_GETLINEENDTYPESSUPPORTED)** read-only

**int editor.[LineEndTypesAllowed](http://www.scintilla.org/ScintillaDoc.html#SCI_SETLINEENDTYPESALLOWED)** -- _Set the line end types that the application wants to use. May not be used if incompatible with lexer or encoding._

**int editor.[LineEndTypesActive](http://www.scintilla.org/ScintillaDoc.html#SCI_GETLINEENDTYPESACTIVE)** read-only

### Styling

**position editor.[EndStyled](http://www.scintilla.org/ScintillaDoc.html#SCI_GETENDSTYLED)** read-only

**editor:[StartStyling](http://www.scintilla.org/ScintillaDoc.html#SCI_STARTSTYLING)(position pos, int mask)** -- _Set the current styling position to pos and the styling mask to mask. The styling mask can be used to protect some bits in each styling byte from modification._

**editor:[SetStyling](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSTYLING)(int length, int style)** -- _Change style from current styling position for length characters to a style and move the current styling position to after this newly styled segment._

**editor:[SetStylingEx](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSTYLINGEX)(string styles)** -- _Set the styles for a segment of the document._

**int editor.[IdleStyling](http://www.scintilla.org/ScintillaDoc.html#SCI_SETIDLESTYLING)** -- _Sets limits to idle styling._

**int editor.[LineState](http://www.scintilla.org/ScintillaDoc.html#SCI_SETLINESTATE)[int line]** -- _Used to hold extra styling information for each line._

**int editor.[MaxLineState](http://www.scintilla.org/ScintillaDoc.html#SCI_GETMAXLINESTATE)** read-only

### Style Definition

**editor:[StyleResetDefault](http://www.scintilla.org/ScintillaDoc.html#SCI_STYLERESETDEFAULT)()** -- _Reset the default style to its state at startup_

**editor:[StyleClearAll](http://www.scintilla.org/ScintillaDoc.html#SCI_STYLECLEARALL)()** -- _Clear all the styles and make equivalent to the global default style._

**string editor.[StyleFont](http://www.scintilla.org/ScintillaDoc.html#SCI_STYLESETFONT)[int style]** -- _Set the font of a style._

**int editor.[StyleSize](http://www.scintilla.org/ScintillaDoc.html#SCI_STYLESETSIZE)[int style]** -- _Set the size of characters of a style._

**int editor.[StyleSizeFractional](http://www.scintilla.org/ScintillaDoc.html#SCI_STYLESETSIZEFRACTIONAL)[int style]** -- _Set the size of characters of a style. Size is in points multiplied by 100._

**bool editor.[StyleBold](http://www.scintilla.org/ScintillaDoc.html#SCI_STYLESETBOLD)[int style]** -- _Set a style to be bold or not._

**int editor.[StyleWeight](http://www.scintilla.org/ScintillaDoc.html#SCI_STYLESETWEIGHT)[int style]** -- _Set the weight of characters of a style._

**bool editor.[StyleItalic](http://www.scintilla.org/ScintillaDoc.html#SCI_STYLESETITALIC)[int style]** -- _Set a style to be italic or not._

**bool editor.[StyleUnderline](http://www.scintilla.org/ScintillaDoc.html#SCI_STYLESETUNDERLINE)[int style]** -- _Set a style to be underlined or not._

**colour editor.[StyleFore](http://www.scintilla.org/ScintillaDoc.html#SCI_STYLESETFORE)[int style]** -- _Set the foreground colour of a style._

**colour editor.[StyleBack](http://www.scintilla.org/ScintillaDoc.html#SCI_STYLESETBACK)[int style]** -- _Set the background colour of a style._

**bool editor.[StyleEOLFilled](http://www.scintilla.org/ScintillaDoc.html#SCI_STYLESETEOLFILLED)[int style]** -- _Set a style to have its end of line filled or not._

**int editor.[StyleCharacterSet](http://www.scintilla.org/ScintillaDoc.html#SCI_STYLESETCHARACTERSET)[int style]** -- _Set the character set of the font in a style._

**int editor.[StyleCase](http://www.scintilla.org/ScintillaDoc.html#SCI_STYLESETCASE)[int style]** -- _Set a style to be mixed case, or to force upper or lower case._

**bool editor.[StyleVisible](http://www.scintilla.org/ScintillaDoc.html#SCI_STYLESETVISIBLE)[int style]** -- _Set a style to be visible or not._

**bool editor.[StyleChangeable](http://www.scintilla.org/ScintillaDoc.html#SCI_STYLESETCHANGEABLE)[int style]** -- _Set a style to be changeable or not (read only). Experimental feature, currently buggy._

**bool editor.[StyleHotSpot](http://www.scintilla.org/ScintillaDoc.html#SCI_STYLESETHOTSPOT)[int style]** -- _Set a style to be a hotspot or not._

### Caret, Selection, and Hotspot Styles

**editor:[SetSelFore](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSELFORE)(bool useSetting, colour fore)** -- _Set the foreground colour of the main and additional selections and whether to use this setting._

**editor:[SetSelBack](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSELBACK)(bool useSetting, colour back)** -- _Set the background colour of the main and additional selections and whether to use this setting._

**int editor.[SelAlpha](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSELALPHA)** -- _Set the alpha of the selection._

**bool editor.[SelEOLFilled](http://www.scintilla.org/ScintillaDoc.html#SCI_SETSELEOLFILLED)** -- _Set the selection to have its end of line filled or not._

**colour editor.[CaretFore](http://www.scintilla.org/ScintillaDoc.html#SCI_SETCARETFORE)** -- _Set the foreground colour of the caret._

**bool editor.[CaretLineVisible](http://www.scintilla.org/ScintillaDoc.html#SCI_SETCARETLINEVISIBLE)** -- _Display the background of the line containing the caret in a different colour._

**colour editor.[CaretLineBack](http://www.scintilla.org/ScintillaDoc.html#SCI_SETCARETLINEBACK)** -- _Set the colour of the background of the line containing the caret._

**int editor.[CaretLineBackAlpha](http://www.scintilla.org/ScintillaDoc.html#SCI_SETCARETLINEBACKALPHA)** -- _Set background alpha of the caret line._

**bool editor.[CaretLineVisibleAlways](http://www.scintilla.org/ScintillaDoc.html#SCI_SETCARETLINEVISIBLEALWAYS)** -- _Sets the caret line to always visible._

**int editor.[CaretPeriod](http://www.scintilla.org/ScintillaDoc.html#SCI_SETCARETPERIOD)** -- _Get the time in milliseconds that the caret is on and off. 0 = steady on._

**int editor.[CaretStyle](http://www.scintilla.org/ScintillaDoc.html#SCI_SETCARETSTYLE)** -- _Set the style of the caret to be drawn._

**int editor.[CaretWidth](http://www.scintilla.org/ScintillaDoc.html#SCI_SETCARETWIDTH)** -- _Set the width of the insert mode caret._

**editor:[SetHotspotActiveFore](http://www.scintilla.org/ScintillaDoc.html#SCI_SETHOTSPOTACTIVEFORE)(bool useSetting, colour fore)** -- _Set a fore colour for active hotspots._

**colour editor:[GetHotspotActiveFore](http://www.scintilla.org/ScintillaDoc.html#SCI_GETHOTSPOTACTIVEFORE)()** -- _Get the fore colour for active hotspots._

**editor:[SetHotspotActiveBack](http://www.scintilla.org/ScintillaDoc.html#SCI_SETHOTSPOTACTIVEBACK)(bool useSetting, colour back)** -- _Set a back colour for active hotspots._

**colour editor:[GetHotspotActiveBack](http://www.scintilla.org/ScintillaDoc.html#SCI_GETHOTSPOTACTIVEBACK)()** -- _Get the back colour for active hotspots._

**bool editor.[HotspotActiveUnderline](http://www.scintilla.org/ScintillaDoc.html#SCI_SETHOTSPOTACTIVEUNDERLINE)** -- _Enable / Disable underlining active hotspots._

**bool editor.[HotspotSingleLine](http://www.scintilla.org/ScintillaDoc.html#SCI_SETHOTSPOTSINGLELINE)** -- _Limit hotspots to single line so hotspots on two lines don't merge._

**int editor.[CaretSticky](http://www.scintilla.org/ScintillaDoc.html#SCI_SETCARETSTICKY)** -- _Stop the caret preferred x position changing when the user types._

**editor:[ToggleCaretSticky](http://www.scintilla.org/ScintillaDoc.html#SCI_TOGGLECARETSTICKY)()** -- _Switch between sticky and non-sticky: meant to be bound to a key._

### Character Representations

**string editor.[Representation](http://www.scintilla.org/ScintillaDoc.html#SCI_SETREPRESENTATION)[string encodedCharacter]** -- _Set the way a character is drawn._

**editor:[ClearRepresentation](http://www.scintilla.org/ScintillaDoc.html#SCI_CLEARREPRESENTATION)(string encodedCharacter)** -- _Remove a character representation._

**int editor.[ControlCharSymbol](http://www.scintilla.org/ScintillaDoc.html#SCI_SETCONTROLCHARSYMBOL)** -- _Change the way control characters are displayed: If symbol is &lt; 32, keep the drawn way, else, use the given character._

### Margins

**int editor.[MarginTypeN](http://www.scintilla.org/ScintillaDoc.html#SCI_SETMARGINTYPEN)[int margin]** -- _Set a margin to be either numeric or symbolic._

**int editor.[MarginWidthN](http://www.scintilla.org/ScintillaDoc.html#SCI_SETMARGINWIDTHN)[int margin]** -- _Set the width of a margin to a width expressed in pixels._

**int editor.[MarginMaskN](http://www.scintilla.org/ScintillaDoc.html#SCI_SETMARGINMASKN)[int margin]** -- _Set a mask that determines which markers are displayed in a margin._

**bool editor.[MarginSensitiveN](http://www.scintilla.org/ScintillaDoc.html#SCI_SETMARGINSENSITIVEN)[int margin]** -- _Make a margin sensitive or insensitive to mouse clicks._

**int editor.[MarginCursorN](http://www.scintilla.org/ScintillaDoc.html#SCI_SETMARGINCURSORN)[int margin]** -- _Set the cursor shown when the mouse is inside a margin._

**int editor.[MarginLeft](http://www.scintilla.org/ScintillaDoc.html#SCI_SETMARGINLEFT)** -- _Sets the size in pixels of the left margin._

**int editor.[MarginRight](http://www.scintilla.org/ScintillaDoc.html#SCI_SETMARGINRIGHT)** -- _Sets the size in pixels of the right margin._

**editor:[SetFoldMarginColour](http://www.scintilla.org/ScintillaDoc.html#SCI_SETFOLDMARGINCOLOUR)(bool useSetting, colour back)** -- _Set the colours used as a chequerboard pattern in the fold margin_

**editor:[SetFoldMarginHiColour](http://www.scintilla.org/ScintillaDoc.html#SCI_SETFOLDMARGINHICOLOUR)(bool useSetting, colour fore)** -- _Set the colours used as a chequerboard pattern in the fold margin_

**string editor.[MarginText](http://www.scintilla.org/ScintillaDoc.html#SCI_MARGINSETTEXT)[int line]** -- _Set the text in the text margin for a line_

**int editor.[MarginStyle](http://www.scintilla.org/ScintillaDoc.html#SCI_MARGINSETSTYLE)[int line]** -- _Set the style number for the text margin for a line_

**string editor.[MarginStyles](http://www.scintilla.org/ScintillaDoc.html#SCI_MARGINSETSTYLES)[int line]** -- _Set the style in the text margin for a line_

**editor:[MarginTextClearAll](http://www.scintilla.org/ScintillaDoc.html#SCI_MARGINTEXTCLEARALL)()** -- _Clear the margin text on all lines_

**int editor.[MarginStyleOffset](http://www.scintilla.org/ScintillaDoc.html#SCI_MARGINSETSTYLEOFFSET)** -- _Get the start of the range of style numbers used for margin text_

**int editor.[MarginOptions](http://www.scintilla.org/ScintillaDoc.html#SCI_SETMARGINOPTIONS)** -- _Set the margin options._

### Annotations

**string editor.[AnnotationText](http://www.scintilla.org/ScintillaDoc.html#SCI_ANNOTATIONSETTEXT)[int line]** -- _Set the annotation text for a line_

**int editor.[AnnotationStyle](http://www.scintilla.org/ScintillaDoc.html#SCI_ANNOTATIONSETSTYLE)[int line]** -- _Set the style number for the annotations for a line_

**string editor.[AnnotationStyles](http://www.scintilla.org/ScintillaDoc.html#SCI_ANNOTATIONSETSTYLES)[int line]** -- _Set the annotation styles for a line_

**int editor.[AnnotationLines](http://www.scintilla.org/ScintillaDoc.html#SCI_ANNOTATIONGETLINES)[int line]** read-only

**editor:[AnnotationClearAll](http://www.scintilla.org/ScintillaDoc.html#SCI_ANNOTATIONCLEARALL)()** -- _Clear the annotations from all lines_

**int editor.[AnnotationVisible](http://www.scintilla.org/ScintillaDoc.html#SCI_ANNOTATIONSETVISIBLE)** -- _Set the visibility for the annotations for a view_

**int editor.[AnnotationStyleOffset](http://www.scintilla.org/ScintillaDoc.html#SCI_ANNOTATIONSETSTYLEOFFSET)** -- _Get the start of the range of style numbers used for annotations_

### Other Settings

**bool editor.[BufferedDraw](http://www.scintilla.org/ScintillaDoc.html#SCI_SETBUFFEREDDRAW)** -- _If drawing is buffered then each line of text is drawn into a bitmap buffer before drawing it to the screen to avoid flicker._

**int editor.[PhasesDraw](http://www.scintilla.org/ScintillaDoc.html#SCI_SETPHASESDRAW)** -- _In one phase draw, text is drawn in a series of rectangular blocks with no overlap. In two phase draw, text is drawn in a series of lines allowing runs to overlap horizontally. In multiple phase draw, each element is drawn over the whole drawing area, allowing text to overlap from one line to the next._

**bool editor.[TwoPhaseDraw](http://www.scintilla.org/ScintillaDoc.html#SCI_SETTWOPHASEDRAW)** -- _In twoPhaseDraw mode, drawing is performed in two phases, first the background and then the foreground. This avoids chopping off characters that overlap the next run._

**int editor.[Technology](http://www.scintilla.org/ScintillaDoc.html#SCI_SETTECHNOLOGY)** -- _Set the technology used._

**int editor.[FontQuality](http://www.scintilla.org/ScintillaDoc.html#SCI_SETFONTQUALITY)** -- _Choose the quality level for text from the FontQuality enumeration._

**int editor.[CodePage](http://www.scintilla.org/ScintillaDoc.html#SCI_SETCODEPAGE)** -- _Set the code page used to interpret the bytes of the document as characters. The SC_CP_UTF8 value can be used to enter Unicode mode._

**int editor.[IMEInteraction](http://www.scintilla.org/ScintillaDoc.html#SCI_SETIMEINTERACTION)** -- _Choose to display the the IME in a winow or inline._

**string editor.[WordChars](http://www.scintilla.org/ScintillaDoc.html#SCI_SETWORDCHARS)** -- _Set the set of characters making up words for when moving or selecting by word. First sets defaults like SetCharsDefault._

**string editor.[WhitespaceChars](http://www.scintilla.org/ScintillaDoc.html#SCI_SETWHITESPACECHARS)** -- _Set the set of characters making up whitespace for when moving or selecting by word. Should be called after SetWordChars._

**string editor.[PunctuationChars](http://www.scintilla.org/ScintillaDoc.html#SCI_SETPUNCTUATIONCHARS)** -- _Set the set of characters making up punctuation characters Should be called after SetWordChars._

**editor:[SetCharsDefault](http://www.scintilla.org/ScintillaDoc.html#SCI_SETCHARSDEFAULT)()** -- _Reset the set of characters for whitespace and word characters to the defaults._

**editor:[GrabFocus](http://www.scintilla.org/ScintillaDoc.html#SCI_GRABFOCUS)()** -- _Set the focus to this Scintilla widget._

**bool editor.[Focus](http://www.scintilla.org/ScintillaDoc.html#SCI_SETFOCUS)** -- _Change internal focus flag._

### Brace Highlighting

**editor:[BraceHighlight](http://www.scintilla.org/ScintillaDoc.html#SCI_BRACEHIGHLIGHT)(position pos1, position pos2)** -- _Highlight the characters at two positions._

**editor:[BraceBadLight](http://www.scintilla.org/ScintillaDoc.html#SCI_BRACEBADLIGHT)(position pos)** -- _Highlight the character at a position indicating there is no matching brace._

**editor:[BraceHighlightIndicator](http://www.scintilla.org/ScintillaDoc.html#SCI_BRACEHIGHLIGHTINDICATOR)(bool useBraceHighlightIndicator, int indicator)** -- _Use specified indicator to highlight matching braces instead of changing their style._

**editor:[BraceBadLightIndicator](http://www.scintilla.org/ScintillaDoc.html#SCI_BRACEBADLIGHTINDICATOR)(bool useBraceBadLightIndicator, int indicator)** -- _Use specified indicator to highlight non matching brace instead of changing its style._

**position editor:[BraceMatch](http://www.scintilla.org/ScintillaDoc.html#SCI_BRACEMATCH)(position pos)** -- _Find the position of a matching brace or INVALID_POSITION if no match._

### Tabs and Indentation Guides

**int editor.[TabWidth](http://www.scintilla.org/ScintillaDoc.html#SCI_SETTABWIDTH)** -- _Change the visible size of a tab to be a multiple of the width of a space character._

**editor:[ClearTabStops](http://www.scintilla.org/ScintillaDoc.html#SCI_CLEARTABSTOPS)(int line)** -- _Clear explicit tabstops on a line._

**editor:[AddTabStop](http://www.scintilla.org/ScintillaDoc.html#SCI_ADDTABSTOP)(int line, int x)** -- _Add an explicit tab stop for a line._

**int editor:[GetNextTabStop](http://www.scintilla.org/ScintillaDoc.html#SCI_GETNEXTTABSTOP)(int line, int x)** -- _Find the next explicit tab stop position on a line after a position._

**bool editor.[UseTabs](http://www.scintilla.org/ScintillaDoc.html#SCI_SETUSETABS)** -- _Indentation will only use space characters if useTabs is false, otherwise it will use a combination of tabs and spaces._

**int editor.[Indent](http://www.scintilla.org/ScintillaDoc.html#SCI_SETINDENT)** -- _Set the number of spaces used for one level of indentation._

**bool editor.[TabIndents](http://www.scintilla.org/ScintillaDoc.html#SCI_SETTABINDENTS)** -- _Sets whether a tab pressed when caret is within indentation indents._

**bool editor.[BackSpaceUnIndents](http://www.scintilla.org/ScintillaDoc.html#SCI_SETBACKSPACEUNINDENTS)** -- _Sets whether a backspace pressed when caret is within indentation unindents._

**int editor.[LineIndentation](http://www.scintilla.org/ScintillaDoc.html#SCI_SETLINEINDENTATION)[int line]** -- _Change the indentation of a line to a number of columns._

**position editor.[LineIndentPosition](http://www.scintilla.org/ScintillaDoc.html#SCI_GETLINEINDENTPOSITION)[int line]** read-only

**int editor.[IndentationGuides](http://www.scintilla.org/ScintillaDoc.html#SCI_SETINDENTATIONGUIDES)** -- _Show or hide indentation guides._

**int editor.[HighlightGuide](http://www.scintilla.org/ScintillaDoc.html#SCI_SETHIGHLIGHTGUIDE)** -- _Set the highlighted indentation guide column. 0 = no highlighted guide._

### Markers

**editor:[MarkerDefine](http://www.scintilla.org/ScintillaDoc.html#SCI_MARKERDEFINE)(int markerNumber, int markerSymbol)** -- _Set the symbol used for a particular marker number._

**editor:[MarkerDefinePixmap](http://www.scintilla.org/ScintillaDoc.html#SCI_MARKERDEFINEPIXMAP)(int markerNumber, string pixmap)** -- _Define a marker from a pixmap._

**int editor.[RGBAImageWidth](http://www.scintilla.org/ScintillaDoc.html#SCI_RGBAIMAGESETWIDTH) write-only** -- _Set the width for future RGBA image data._

**int editor.[RGBAImageHeight](http://www.scintilla.org/ScintillaDoc.html#SCI_RGBAIMAGESETHEIGHT) write-only** -- _Set the height for future RGBA image data._

**int editor.[RGBAImageScale](http://www.scintilla.org/ScintillaDoc.html#SCI_RGBAIMAGESETSCALE) write-only** -- _Set the scale factor in percent for future RGBA image data._

**editor:[MarkerDefineRGBAImage](http://www.scintilla.org/ScintillaDoc.html#SCI_MARKERDEFINERGBAIMAGE)(int markerNumber, string pixels)** -- _Define a marker from RGBA data. It has the width and height from RGBAImageSetWidth/Height_

**int editor:[MarkerSymbolDefined](http://www.scintilla.org/ScintillaDoc.html#SCI_MARKERSYMBOLDEFINED)(int markerNumber)** -- _Which symbol was defined for markerNumber with MarkerDefine_

**colour editor.[MarkerFore](http://www.scintilla.org/ScintillaDoc.html#SCI_MARKERSETFORE)[int markerNumber] write-only** -- _Set the foreground colour used for a particular marker number._

**colour editor.[MarkerBack](http://www.scintilla.org/ScintillaDoc.html#SCI_MARKERSETBACK)[int markerNumber] write-only** -- _Set the background colour used for a particular marker number._

**colour editor.[MarkerBackSelected](http://www.scintilla.org/ScintillaDoc.html#SCI_MARKERSETBACKSELECTED)[int markerNumber] write-only** -- _Set the background colour used for a particular marker number when its folding block is selected._

**editor:[MarkerEnableHighlight](http://www.scintilla.org/ScintillaDoc.html#SCI_MARKERENABLEHIGHLIGHT)(bool enabled)** -- _Enable/disable highlight for current folding bloc (smallest one that contains the caret)_

**int editor.[MarkerAlpha](http://www.scintilla.org/ScintillaDoc.html#SCI_MARKERSETALPHA)[int markerNumber] write-only** -- _Set the alpha used for a marker that is drawn in the text area, not the margin._

**int editor:[MarkerAdd](http://www.scintilla.org/ScintillaDoc.html#SCI_MARKERADD)(int line, int markerNumber)** -- _Add a marker to a line, returning an ID which can be used to find or delete the marker._

**editor:[MarkerAddSet](http://www.scintilla.org/ScintillaDoc.html#SCI_MARKERADDSET)(int line, int set)** -- _Add a set of markers to a line._

**editor:[MarkerDelete](http://www.scintilla.org/ScintillaDoc.html#SCI_MARKERDELETE)(int line, int markerNumber)** -- _Delete a marker from a line._

**editor:[MarkerDeleteAll](http://www.scintilla.org/ScintillaDoc.html#SCI_MARKERDELETEALL)(int markerNumber)** -- _Delete all markers with a particular number from all lines._

**int editor:[MarkerGet](http://www.scintilla.org/ScintillaDoc.html#SCI_MARKERGET)(int line)** -- _Get a bit mask of all the markers set on a line._

**int editor:[MarkerNext](http://www.scintilla.org/ScintillaDoc.html#SCI_MARKERNEXT)(int lineStart, int markerMask)** -- _Find the next line at or after lineStart that includes a marker in mask. Return -1 when no more lines._

**int editor:[MarkerPrevious](http://www.scintilla.org/ScintillaDoc.html#SCI_MARKERPREVIOUS)(int lineStart, int markerMask)** -- _Find the previous line before lineStart that includes a marker in mask._

**int editor:[MarkerLineFromHandle](http://www.scintilla.org/ScintillaDoc.html#SCI_MARKERLINEFROMHANDLE)(int handle)** -- _Retrieve the line number at which a particular marker is located._

**editor:[MarkerDeleteHandle](http://www.scintilla.org/ScintillaDoc.html#SCI_MARKERDELETEHANDLE)(int handle)** -- _Delete a marker._

### Indicators

**int editor.[IndicStyle](http://www.scintilla.org/ScintillaDoc.html#SCI_INDICSETSTYLE)[int indic]** -- _Set an indicator to plain, squiggle or TT._

**colour editor.[IndicFore](http://www.scintilla.org/ScintillaDoc.html#SCI_INDICSETFORE)[int indic]** -- _Set the foreground colour of an indicator._

**int editor.[IndicAlpha](http://www.scintilla.org/ScintillaDoc.html#SCI_INDICSETALPHA)[int indicator]** -- _Set the alpha fill colour of the given indicator._

**int editor.[IndicOutlineAlpha](http://www.scintilla.org/ScintillaDoc.html#SCI_INDICSETOUTLINEALPHA)[int indicator]** -- _Set the alpha outline colour of the given indicator._

**bool editor.[IndicUnder](http://www.scintilla.org/ScintillaDoc.html#SCI_INDICSETUNDER)[int indic]** -- _Set an indicator to draw under text or over(default)._

**int editor.[IndicHoverStyle](http://www.scintilla.org/ScintillaDoc.html#SCI_INDICSETHOVERSTYLE)[int indic]** -- _Set a hover indicator to plain, squiggle or TT._

**colour editor.[IndicHoverFore](http://www.scintilla.org/ScintillaDoc.html#SCI_INDICSETHOVERFORE)[int indic]** -- _Set the foreground hover colour of an indicator._

**int editor.[IndicFlags](http://www.scintilla.org/ScintillaDoc.html#SCI_INDICSETFLAGS)[int indic]** -- _Set the attributes of an indicator._

**int editor.[IndicatorCurrent](http://www.scintilla.org/ScintillaDoc.html#SCI_SETINDICATORCURRENT)** -- _Set the indicator used for IndicatorFillRange and IndicatorClearRange_

**int editor.[IndicatorValue](http://www.scintilla.org/ScintillaDoc.html#SCI_SETINDICATORVALUE)** -- _Set the value used for IndicatorFillRange_

**editor:[IndicatorFillRange](http://www.scintilla.org/ScintillaDoc.html#SCI_INDICATORFILLRANGE)(int position, int fillLength)** -- _Turn a indicator on over a range._

**editor:[IndicatorClearRange](http://www.scintilla.org/ScintillaDoc.html#SCI_INDICATORCLEARRANGE)(int position, int clearLength)** -- _Turn a indicator off over a range._

**int editor:[IndicatorAllOnFor](http://www.scintilla.org/ScintillaDoc.html#SCI_INDICATORALLONFOR)(int position)** -- _Are any indicators present at position?_

**int editor:[IndicatorValueAt](http://www.scintilla.org/ScintillaDoc.html#SCI_INDICATORVALUEAT)(int indicator, int position)** -- _What value does a particular indicator have at at a position?_

**int editor:[IndicatorStart](http://www.scintilla.org/ScintillaDoc.html#SCI_INDICATORSTART)(int indicator, int position)** -- _Where does a particular indicator start?_

**int editor:[IndicatorEnd](http://www.scintilla.org/ScintillaDoc.html#SCI_INDICATOREND)(int indicator, int position)** -- _Where does a particular indicator end?_

**editor:[FindIndicatorShow](http://www.scintilla.org/ScintillaDoc.html#SCI_FINDINDICATORSHOW)(position start, position end)** -- _On OS X, show a find indicator._

**editor:[FindIndicatorFlash](http://www.scintilla.org/ScintillaDoc.html#SCI_FINDINDICATORFLASH)(position start, position end)** -- _On OS X, flash a find indicator, then fade out._

**editor:[FindIndicatorHide](http://www.scintilla.org/ScintillaDoc.html#SCI_FINDINDICATORHIDE)()** -- _On OS X, hide the find indicator._

### Autocompletion

**editor:[AutoCShow](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCSHOW)(int lenEntered, string itemList)** -- _Display a auto-completion list. The lenEntered parameter indicates how many characters before the caret should be used to provide context._

**editor:[AutoCCancel](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCCANCEL)()** -- _Remove the auto-completion list from the screen._

**bool editor:[AutoCActive](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCACTIVE)()** -- _Is there an auto-completion list visible?_

**position editor:[AutoCPosStart](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCPOSSTART)()** -- _Retrieve the position of the caret when the auto-completion list was displayed._

**editor:[AutoCComplete](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCCOMPLETE)()** -- _User has selected an item so remove the list and insert the selection._

**editor:[AutoCStops](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCSTOPS)(string characterSet)** -- _Define a set of character that when typed cancel the auto-completion list._

**int editor.[AutoCSeparator](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCSETSEPARATOR)** -- _Change the separator character in the string setting up an auto-completion list. Default is space but can be changed if items contain space._

**editor:[AutoCSelect](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCSELECT)(string text)** -- _Select the item in the auto-completion list that starts with a string._

**int editor.[AutoCCurrent](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCGETCURRENT)** read-only

**string editor.[AutoCCurrentText](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCGETCURRENTTEXT)** read-only

**bool editor.[AutoCCancelAtStart](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCSETCANCELATSTART)** -- _Should the auto-completion list be cancelled if the user backspaces to a position before where the box was created._

**string editor.[AutoCFillUps](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCSETFILLUPS) write-only** -- _Define a set of characters that when typed will cause the autocompletion to choose the selected item._

**bool editor.[AutoCChooseSingle](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCSETCHOOSESINGLE)** -- _Should a single item auto-completion list automatically choose the item._

**bool editor.[AutoCIgnoreCase](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCSETIGNORECASE)** -- _Set whether case is significant when performing auto-completion searches._

**int editor.[AutoCCaseInsensitiveBehaviour](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCSETCASEINSENSITIVEBEHAVIOUR)** -- _Set auto-completion case insensitive behaviour to either prefer case-sensitive matches or have no preference._

**int editor.[AutoCMulti](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCSETMULTI)** -- _Change the effect of autocompleting when there are multiple selections._

**int editor.[AutoCOrder](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCSETORDER)** -- _Set the way autocompletion lists are ordered._

**bool editor.[AutoCAutoHide](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCSETAUTOHIDE)** -- _Set whether or not autocompletion is hidden automatically when nothing matches._

**bool editor.[AutoCDropRestOfWord](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCSETDROPRESTOFWORD)** -- _Set whether or not autocompletion deletes any word characters after the inserted text upon completion._

**editor:[RegisterImage](http://www.scintilla.org/ScintillaDoc.html#SCI_REGISTERIMAGE)(int type, string xpmData)** -- _Register an XPM image for use in autocompletion lists._

**editor:[RegisterRGBAImage](http://www.scintilla.org/ScintillaDoc.html#SCI_REGISTERRGBAIMAGE)(int type, string pixels)** -- _Register an RGBA image for use in autocompletion lists. It has the width and height from RGBAImageSetWidth/Height_

**editor:[ClearRegisteredImages](http://www.scintilla.org/ScintillaDoc.html#SCI_CLEARREGISTEREDIMAGES)()** -- _Clear all the registered XPM images._

**int editor.[AutoCTypeSeparator](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCSETTYPESEPARATOR)** -- _Change the type-separator character in the string setting up an auto-completion list. Default is '?' but can be changed if items contain '?'._

**int editor.[AutoCMaxHeight](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCSETMAXHEIGHT)** -- _Set the maximum height, in rows, of auto-completion and user lists. The default is 5 rows._

**int editor.[AutoCMaxWidth](http://www.scintilla.org/ScintillaDoc.html#SCI_AUTOCSETMAXWIDTH)** -- _Set the maximum width, in characters, of auto-completion and user lists. Set to 0 to autosize to fit longest item, which is the default._

### User Lists

**editor:[UserListShow](http://www.scintilla.org/ScintillaDoc.html#SCI_USERLISTSHOW)(int listType, string itemList)** -- _Display a list of strings and send notification when user chooses one._

### Call Tips

**editor:[CallTipShow](http://www.scintilla.org/ScintillaDoc.html#SCI_CALLTIPSHOW)(position pos, string definition)** -- _Show a call tip containing a definition near position pos._

**editor:[CallTipCancel](http://www.scintilla.org/ScintillaDoc.html#SCI_CALLTIPCANCEL)()** -- _Remove the call tip from the screen._

**bool editor:[CallTipActive](http://www.scintilla.org/ScintillaDoc.html#SCI_CALLTIPACTIVE)()** -- _Is there an active call tip?_

**position editor:[CallTipPosStart](http://www.scintilla.org/ScintillaDoc.html#SCI_CALLTIPPOSSTART)()** -- _Retrieve the position where the caret was before displaying the call tip._

**int editor.[CallTipPosStart](http://www.scintilla.org/ScintillaDoc.html#SCI_CALLTIPSETPOSSTART) write-only** -- _Set the start position in order to change when backspacing removes the calltip._

**editor:[CallTipSetHlt](http://www.scintilla.org/ScintillaDoc.html#SCI_CALLTIPSETHLT)(int start, int end)** -- _Highlight a segment of the definition._

**colour editor.[CallTipBack](http://www.scintilla.org/ScintillaDoc.html#SCI_CALLTIPSETBACK) write-only** -- _Set the background colour for the call tip._

**colour editor.[CallTipFore](http://www.scintilla.org/ScintillaDoc.html#SCI_CALLTIPSETFORE) write-only** -- _Set the foreground colour for the call tip._

**colour editor.[CallTipForeHlt](http://www.scintilla.org/ScintillaDoc.html#SCI_CALLTIPSETFOREHLT) write-only** -- _Set the foreground colour for the highlighted part of the call tip._

**int editor.[CallTipUseStyle](http://www.scintilla.org/ScintillaDoc.html#SCI_CALLTIPUSESTYLE) write-only** -- _Enable use of STYLE_CALLTIP and set call tip tab size in pixels._

**bool editor.[CallTipPosition](http://www.scintilla.org/ScintillaDoc.html#SCI_CALLTIPSETPOSITION) write-only** -- _Set position of calltip, above or below text._

### Key Bindings

**editor:[AssignCmdKey](http://www.scintilla.org/ScintillaDoc.html#SCI_ASSIGNCMDKEY)(keymod km, int msg)** -- _When key+modifier combination km is pressed perform msg._

**editor:[ClearCmdKey](http://www.scintilla.org/ScintillaDoc.html#SCI_CLEARCMDKEY)(keymod km)** -- _When key+modifier combination km is pressed do nothing._

**editor:[ClearAllCmdKeys](http://www.scintilla.org/ScintillaDoc.html#SCI_CLEARALLCMDKEYS)()** -- _Drop all key mappings._

**editor:[Null](http://www.scintilla.org/ScintillaDoc.html#SCI_NULL)()** -- _Null operation._

### Popup Edit Menu

**editor:[UsePopUp](http://www.scintilla.org/ScintillaDoc.html#SCI_USEPOPUP)(bool allowPopUp)** -- _Set whether a pop up menu is displayed automatically when the user presses the wrong mouse button._

### Macro Recording

**editor:[StartRecord](http://www.scintilla.org/ScintillaDoc.html#SCI_STARTRECORD)()** -- _Start notifying the container of all key presses and commands._

**editor:[StopRecord](http://www.scintilla.org/ScintillaDoc.html#SCI_STOPRECORD)()** -- _Stop notifying the container of all key presses and commands._

### Printing

**int editor.[PrintMagnification](http://www.scintilla.org/ScintillaDoc.html#SCI_SETPRINTMAGNIFICATION)** -- _Sets the print magnification added to the point size of each style for printing._

**int editor.[PrintColourMode](http://www.scintilla.org/ScintillaDoc.html#SCI_SETPRINTCOLOURMODE)** -- _Modify colours when printing for clearer printed text._

**int editor.[PrintWrapMode](http://www.scintilla.org/ScintillaDoc.html#SCI_SETPRINTWRAPMODE)** -- _Set printing to line wrapped (SC_WRAP_WORD) or not line wrapped (SC_WRAP_NONE)._

### Direct Access

**int editor.[DirectFunction](http://www.scintilla.org/ScintillaDoc.html#SCI_GETDIRECTFUNCTION)** read-only

**int editor.[DirectPointer](http://www.scintilla.org/ScintillaDoc.html#SCI_GETDIRECTPOINTER)** read-only

**int editor.[CharacterPointer](http://www.scintilla.org/ScintillaDoc.html#SCI_GETCHARACTERPOINTER)** read-only

**int editor:[GetRangePointer](http://www.scintilla.org/ScintillaDoc.html#SCI_GETRANGEPOINTER)(int position, int rangeLength)** -- _Return a read-only pointer to a range of characters in the document. May move the gap so that the range is contiguous, but will only move up to rangeLength bytes._

**position editor.[GapPosition](http://www.scintilla.org/ScintillaDoc.html#SCI_GETGAPPOSITION)** read-only

### Multiple Views

**int editor.[DocPointer](http://www.scintilla.org/ScintillaDoc.html#SCI_SETDOCPOINTER)** -- _Change the document object used._

**int editor:[CreateDocument](http://www.scintilla.org/ScintillaDoc.html#SCI_CREATEDOCUMENT)()** -- _Create a new document object. Starts with reference count of 1 and not selected into editor._

**editor:[AddRefDocument](http://www.scintilla.org/ScintillaDoc.html#SCI_ADDREFDOCUMENT)(int doc)** -- _Extend life of document._

**editor:[ReleaseDocument](http://www.scintilla.org/ScintillaDoc.html#SCI_RELEASEDOCUMENT)(int doc)** -- _Release a reference to the document, deleting document if it fades to black._

### Background Loading and Saving

**int editor:[CreateLoader](http://www.scintilla.org/ScintillaDoc.html#SCI_CREATELOADER)(int bytes)** -- _Create an ILoader*._

### Folding

**int editor:[VisibleFromDocLine](http://www.scintilla.org/ScintillaDoc.html#SCI_VISIBLEFROMDOCLINE)(int line)** -- _Find the display line of a document line taking hidden lines into account._

**int editor:[DocLineFromVisible](http://www.scintilla.org/ScintillaDoc.html#SCI_DOCLINEFROMVISIBLE)(int lineDisplay)** -- _Find the document line of a display line taking hidden lines into account._

**editor:[ShowLines](http://www.scintilla.org/ScintillaDoc.html#SCI_SHOWLINES)(int lineStart, int lineEnd)** -- _Make a range of lines visible._

**editor:[HideLines](http://www.scintilla.org/ScintillaDoc.html#SCI_HIDELINES)(int lineStart, int lineEnd)** -- _Make a range of lines invisible._

**bool editor.[LineVisible](http://www.scintilla.org/ScintillaDoc.html#SCI_GETLINEVISIBLE)[int line]** read-only

**bool editor.[AllLinesVisible](http://www.scintilla.org/ScintillaDoc.html#SCI_GETALLLINESVISIBLE)** read-only

**int editor.[FoldLevel](http://www.scintilla.org/ScintillaDoc.html#SCI_SETFOLDLEVEL)[int line]** -- _Set the fold level of a line. This encodes an integer level along with flags indicating whether the line is a header and whether it is effectively white space._

**int editor.[FoldFlags](http://www.scintilla.org/ScintillaDoc.html#SCI_SETFOLDFLAGS) write-only** -- _Set some style options for folding._

**int editor:[GetLastChild](http://www.scintilla.org/ScintillaDoc.html#SCI_GETLASTCHILD)(int line, int level)** -- _Find the last child line of a header line._

**int editor.[FoldParent](http://www.scintilla.org/ScintillaDoc.html#SCI_GETFOLDPARENT)[int line]** read-only

**editor:[ToggleFold](http://www.scintilla.org/ScintillaDoc.html#SCI_TOGGLEFOLD)(int line)** -- _Switch a header line between expanded and contracted._

**bool editor.[FoldExpanded](http://www.scintilla.org/ScintillaDoc.html#SCI_SETFOLDEXPANDED)[int line]** -- _Show the children of a header line._

**editor:[FoldLine](http://www.scintilla.org/ScintillaDoc.html#SCI_FOLDLINE)(int line, int action)** -- _Expand or contract a fold header._

**editor:[FoldChildren](http://www.scintilla.org/ScintillaDoc.html#SCI_FOLDCHILDREN)(int line, int action)** -- _Expand or contract a fold header and its children._

**editor:[FoldAll](http://www.scintilla.org/ScintillaDoc.html#SCI_FOLDALL)(int action)** -- _Expand or contract all fold headers._

**editor:[ExpandChildren](http://www.scintilla.org/ScintillaDoc.html#SCI_EXPANDCHILDREN)(int line, int level)** -- _Expand a fold header and all children. Use the level argument instead of the line's current level._

**int editor.[AutomaticFold](http://www.scintilla.org/ScintillaDoc.html#SCI_SETAUTOMATICFOLD)** -- _Set automatic folding behaviours._

**int editor:[ContractedFoldNext](http://www.scintilla.org/ScintillaDoc.html#SCI_CONTRACTEDFOLDNEXT)(int lineStart)** -- _Find the next line at or after lineStart that is a contracted fold header line. Return -1 when no more lines._

**editor:[EnsureVisible](http://www.scintilla.org/ScintillaDoc.html#SCI_ENSUREVISIBLE)(int line)** -- _Ensure a particular line is visible by expanding any header line hiding it._

**editor:[EnsureVisibleEnforcePolicy](http://www.scintilla.org/ScintillaDoc.html#SCI_ENSUREVISIBLEENFORCEPOLICY)(int line)** -- _Ensure a particular line is visible by expanding any header line hiding it. Use the currently set visibility policy to determine which range to display._

### Line Wrapping

**int editor.[WrapMode](http://www.scintilla.org/ScintillaDoc.html#SCI_SETWRAPMODE)** -- _Sets whether text is word wrapped._

**int editor.[WrapVisualFlags](http://www.scintilla.org/ScintillaDoc.html#SCI_SETWRAPVISUALFLAGS)** -- _Set the display mode of visual flags for wrapped lines._

**int editor.[WrapVisualFlagsLocation](http://www.scintilla.org/ScintillaDoc.html#SCI_SETWRAPVISUALFLAGSLOCATION)** -- _Set the location of visual flags for wrapped lines._

**int editor.[WrapIndentMode](http://www.scintilla.org/ScintillaDoc.html#SCI_SETWRAPINDENTMODE)** -- _Sets how wrapped sublines are placed. Default is fixed._

**int editor.[WrapStartIndent](http://www.scintilla.org/ScintillaDoc.html#SCI_SETWRAPSTARTINDENT)** -- _Set the start indent for wrapped lines._

**int editor.[LayoutCache](http://www.scintilla.org/ScintillaDoc.html#SCI_SETLAYOUTCACHE)** -- _Sets the degree of caching of layout information._

**int editor.[PositionCache](http://www.scintilla.org/ScintillaDoc.html#SCI_SETPOSITIONCACHE)** -- _Set number of entries in position cache_

**editor:[LinesSplit](http://www.scintilla.org/ScintillaDoc.html#SCI_LINESSPLIT)(int pixelWidth)** -- _Split the lines in the target into lines that are less wide than pixelWidth where possible._

**editor:[LinesJoin](http://www.scintilla.org/ScintillaDoc.html#SCI_LINESJOIN)()** -- _Join the lines in the target._

**int editor:[WrapCount](http://www.scintilla.org/ScintillaDoc.html#SCI_WRAPCOUNT)(int line)** -- _The number of display lines needed to wrap a document line_

### Zooming

**editor:[ZoomIn](http://www.scintilla.org/ScintillaDoc.html#SCI_ZOOMIN)()** -- _Magnify the displayed text by increasing the sizes by 1 point._

**editor:[ZoomOut](http://www.scintilla.org/ScintillaDoc.html#SCI_ZOOMOUT)()** -- _Make the displayed text smaller by decreasing the sizes by 1 point._

**int editor.[Zoom](http://www.scintilla.org/ScintillaDoc.html#SCI_SETZOOM)** -- _Set the zoom level. This number of points is added to the size of all fonts. It may be positive to magnify or negative to reduce._

### Long Lines

**int editor.[EdgeMode](http://www.scintilla.org/ScintillaDoc.html#SCI_SETEDGEMODE)** -- _The edge may be displayed by a line (EDGE_LINE) or by highlighting text that goes beyond it (EDGE_BACKGROUND) or not displayed at all (EDGE_NONE)._

**int editor.[EdgeColumn](http://www.scintilla.org/ScintillaDoc.html#SCI_SETEDGECOLUMN)** -- _Set the column number of the edge. If text goes past the edge then it is highlighted._

**colour editor.[EdgeColour](http://www.scintilla.org/ScintillaDoc.html#SCI_SETEDGECOLOUR)** -- _Change the colour used in edge indication._

### Lexer

**int editor.[Lexer](http://www.scintilla.org/ScintillaDoc.html#SCI_SETLEXER)** -- _Set the lexing language of the document._

**string editor.[LexerLanguage](http://www.scintilla.org/ScintillaDoc.html#SCI_SETLEXERLANGUAGE)** -- _Set the lexing language of the document based on string name._

**editor:[LoadLexerLibrary](http://www.scintilla.org/ScintillaDoc.html#SCI_LOADLEXERLIBRARY)(string path)** -- _Load a lexer library (dll / so)._

**editor:[Colourise](http://www.scintilla.org/ScintillaDoc.html#SCI_COLOURISE)(position start, position end)** -- _Colourise a segment of the document using the current lexing language._

**int editor:[ChangeLexerState](http://www.scintilla.org/ScintillaDoc.html#SCI_CHANGELEXERSTATE)(position start, position end)** -- _Indicate that the internal state of a lexer has changed over a range and therefore there may be a need to redraw._

**string editor:[PropertyNames](http://www.scintilla.org/ScintillaDoc.html#SCI_PROPERTYNAMES)()** -- _Retrieve a '\n' separated list of properties understood by the current lexer. Result is NUL-terminated._

**int editor:[PropertyType](http://www.scintilla.org/ScintillaDoc.html#SCI_PROPERTYTYPE)(string name)** -- _Retrieve the type of a property._

**string editor:[DescribeProperty](http://www.scintilla.org/ScintillaDoc.html#SCI_DESCRIBEPROPERTY)(string name)** -- _Describe a property. Result is NUL-terminated._

**string editor.[Property](http://www.scintilla.org/ScintillaDoc.html#SCI_SETPROPERTY)[string key]** -- _Set up a value that may be used by a lexer for some optional feature._

**string editor.[PropertyExpanded](http://www.scintilla.org/ScintillaDoc.html#SCI_GETPROPERTYEXPANDED)[string key]** read-only

**int editor.[PropertyInt](http://www.scintilla.org/ScintillaDoc.html#SCI_GETPROPERTYINT)[string key]** read-only

**string editor.[KeyWords](http://www.scintilla.org/ScintillaDoc.html#SCI_SETKEYWORDS)[int keywordSet] write-only** -- _Set up the key words used by the lexer._

**string editor:[DescribeKeyWordSets](http://www.scintilla.org/ScintillaDoc.html#SCI_DESCRIBEKEYWORDSETS)()** -- _Retrieve a '\n' separated list of descriptions of the keyword sets understood by the current lexer. Result is NUL-terminated._

**string editor.[SubStyleBases](http://www.scintilla.org/ScintillaDoc.html#SCI_GETSUBSTYLEBASES)** read-only

**int editor.[DistanceToSecondaryStyles](http://www.scintilla.org/ScintillaDoc.html#SCI_DISTANCETOSECONDARYSTYLES)** read-only

**int editor:[AllocateSubStyles](http://www.scintilla.org/ScintillaDoc.html#SCI_ALLOCATESUBSTYLES)(int styleBase, int numberStyles)** -- _Allocate a set of sub styles for a particular base style, returning start of range_

**editor:[FreeSubStyles](http://www.scintilla.org/ScintillaDoc.html#SCI_FREESUBSTYLES)()** -- _Free allocated sub styles_

**int editor.[SubStylesStart](http://www.scintilla.org/ScintillaDoc.html#SCI_GETSUBSTYLESSTART)[int styleBase]** read-only

**int editor.[SubStylesLength](http://www.scintilla.org/ScintillaDoc.html#SCI_GETSUBSTYLESLENGTH)[int styleBase]** read-only

**int editor.[StyleFromSubStyle](http://www.scintilla.org/ScintillaDoc.html#SCI_GETSTYLEFROMSUBSTYLE)[int subStyle]** read-only

**int editor.[PrimaryStyleFromStyle](http://www.scintilla.org/ScintillaDoc.html#SCI_GETPRIMARYSTYLEFROMSTYLE)[int style]** read-only

**string editor.[Identifiers](http://www.scintilla.org/ScintillaDoc.html#SCI_SETIDENTIFIERS)[int style] write-only** -- _Set the identifiers that are shown in a particular style_

### Notifications

**int editor.[Identifier](http://www.scintilla.org/ScintillaDoc.html#SCI_SETIDENTIFIER)** -- _Set the identifier reported as idFrom in notification messages._

**int editor.[ModEventMask](http://www.scintilla.org/ScintillaDoc.html#SCI_SETMODEVENTMASK)** -- _Set which document modification events are sent to the container._

**int editor.[MouseDwellTime](http://www.scintilla.org/ScintillaDoc.html#SCI_SETMOUSEDWELLTIME)** -- _Sets the time the mouse must sit still to generate a mouse dwell event._

### Deprecated Messages and Notifications

**int editor.[StyleBitsNeeded](http://www.scintilla.org/ScintillaDoc.html#SCI_GETSTYLEBITSNEEDED)** read-only
