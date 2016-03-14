--- 
-- @classmod Notepad

--- Activates the specified document.
-- @function ActivateDoc
-- @tparam int view either `MAIN_VIEW` or `SUB_VIEW`
-- @tparam int index2Activate

--- Opens a document.
-- @function DoOpen
-- @tparam string file name
-- @treturn bool `true` if successful, else `false`

--- Gets the BufferID located at the given position.
-- @function GetBufferIDFromPos
-- @tparam int position of document
-- @tparam int view either `MAIN_VIEW` or `SUB_VIEW`
-- @treturn int BufferID or 0 if invalid

--- Gets the current directory of the document.
-- @function GetCurrentDirectory
-- @treturn string current directory

--- Gets the BufferID located at the given position.
-- @function GetBufferIDFromPos
-- @tparam int view either `MAIN_VIEW` or `SUB_VIEW`
-- @treturn int 


--- Gets the current document index within the view.
-- @function GetCurrentDocIndex
-- @tparam int view either `MAIN_VIEW` or `SUB_VIEW`
-- @treturn int index of the current document. -1 if the requested view is not visible.

--- Selects and returns the word at the cursor's position
-- @function GetCurrentWord
-- @treturn string word

--- Gets the file extension of the current document.
-- @function GetExtPart
-- @treturn string file extension

--- Gets the file name of the current document.
-- @function GetFileName
-- @treturn string file name

--- Gets the full path of the current document.
-- @function GetFullCurrentPath
-- @treturn string full path

--- Gets the file name (without extension) of the current document.
-- @function GetNamePart
-- @treturn string file name (without extension)

--- Gets the number of open documents.
-- @function GetNbOpenFiles
-- @tparam int view either `ALL_OPEN_FILES`, `PRIMARY_VIEW`, or `SECOND_VIEW`
-- @treturn int number of documents

--- Gets the directory of the Notepad++ application.
-- @function GetNppDirectory
-- @treturn string Notepad++ directory

--- Gets the plugin configuration directory.
-- @function GetPluginsConfigDir
-- @treturn strign plugin configuration directory

--- Executes a menu command.
-- @function MenuCommand
-- @tparam int one of the `IDM_xxx` constants

--- Reloads the specified file.
-- @function ReloadFile
-- @tparam bool withAlert whether to use an alert or not
-- @tparam string filename file name to reload

--- Saves all the opened docuents.
-- @function SaveAllFiles
-- @treturn bool

--- Saves the current document.
-- @function SaveCurrentFile
-- @treturn bool

--- Saves the current document as the specified name.
-- @function SaveCurrentFileAs
-- @tparam bool asCopy
-- @tparam string file name
-- @treturn bool

--- Switch to the specified document.
-- @function SwitchToFile
-- @tparam string file name
-- @treturn bool `true` if successful, else `false`

--- Properties
-- @section properties

--- Whether plugins are allowed to be loaded from the %APPDATA% directory (read-only).
-- @tparam bool AppDataPluginsAllowed

--- Gets and sets the buffer's language type (one of the `L_xxx` constants).
-- @array BufferLangType
-- @tparam BufferID id
-- @treturn int

--- Gets the current BufferID (read-only).
-- @tparam BufferID CurrentBufferID

--- Gets the current column of the cursor (read-only).
-- @tparam int CurrentColumn

--- Gets the current line number of the cursor (read-only).
-- @tparam int CurrentLine

--- Gets the currently active view, either `MAIN_VIEW` or `SUB_VIEW` (read-only).
-- @tparam int CurrentView

--- Gets the default background color used for documents (read-only).
-- @tparam colour DefaultBackgroundColor

--- Gets the default foreground color used for documents (read-only).
-- @tparam colour DefaultForegroundColor

--- Gets the description of a language (read-only).
-- @array LanguageDescription
-- @tparam int language one of the `L_xxx` constant
-- @treturn string

--- Gets the language name (read-only).
-- @array LanguageName
-- @tparam int language one of the `L_xxx` constant
-- @treturn string

--- Gets the Notepad++ version (read-only).
-- The high 16 bits contain the major version. The low 16 bits contain the minor version.
-- @usage print((npp.Version >> 16) .. '.' .. (npp.Version & 0xff))
-- @tparam int Version

--- Gets the version of the Windows operating system (one of the `WV_xxx` constants) (read-only).
-- @tparam int WindowsVersion
