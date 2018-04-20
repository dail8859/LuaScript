--- Notepad++ wrapper.
-- Provides access to the Notepad++ application. Most of the API messages and notifications have been made available, but more can be made available as needed.
-- 
-- The global instance `npp` is used for application access.
-- 
-- @classmod Notepad

--- Activates the specified document.
-- @function ActivateDoc
-- @tparam int view either `MAIN_VIEW` or `SUB_VIEW`
-- @tparam int index2Activate index of the view to activate

--- Disable checking for auto updates.
-- @function DisableAutoUpdate

--- View another docked tab.
-- @function DmmViewOtherTab
-- @tparam string name name of the docked tab (e.g. "Function List")

--- Opens a document.
-- @function DoOpen
-- @tparam string file name
-- @treturn bool `true` if successful, else `false`

--- Enable or disable the file extension column in the doc switcher.
-- @function DocSwitcherDisableColumn
-- @tparam bool toShow whether to show or hide the column

--- Gets the BufferID located at the given position.
-- @function GetBufferIDFromPos
-- @tparam int position position of document
-- @tparam int view either `MAIN_VIEW` or `SUB_VIEW`
-- @treturn int BufferID or 0 if invalid

--- Gets the current directory of the document.
-- @function GetCurrentDirectory
-- @treturn string current directory

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

--- Gets the file name at the cursor position.
-- @function GetFileNameAtCursor
-- @treturn string file path

--- Gets the full path of the current document.
-- @function GetFullCurrentPath
-- @treturn string full path

--- Gets the full path of the provided buffer ID.
-- @function GetFullPathFromBufferID
-- @tparam int BufferID
-- @treturn string full path

--- Gets the file name (without extension) of the current document.
-- @function GetNamePart
-- @treturn string

--- Gets the number of open documents.
-- @function GetNbOpenFiles
-- @tparam int view either `ALL_OPEN_FILES`, `PRIMARY_VIEW`, or `SECOND_VIEW`
-- @treturn int

--- Gets the number of files a the session file.
-- @function GetNbSessionFiles
-- @tparam string session full path to the session file
-- @treturn int number of files

--- Gets the directory of the Notepad++ application.
-- @function GetNppDirectory
-- @treturn string

--- Gets the plugin configuration directory.
-- @function GetPluginsConfigDir
-- @treturn string

--- Gets the position information of the provided buffer ID.
-- @function GetPosFromBufferID
-- @tparam int BufferID
-- @tparam int priorityView either `MAIN_VIEW` or `SUB_VIEW`. If priorityView set to `SUB_VIEW`, then it will be searched first
-- @treturn int the two highest bits will either be `MAIN_VIEW` or `SUB_VIEW`. The lower 30 bits will be the 0-based index.

--- Hides or shows the menu.
-- @function HideMenu
-- @tparam bool hideOrNot whether or not to hide the menu
-- @treturn bool the previous state

--- Hides or shows the status bar.
-- @function HideStatusBar
-- @tparam bool hideOrNot whether or not to hide the status bar
-- @treturn bool the previous state

--- Hides or shows the tab bar.
-- @function HideTabBar
-- @tparam bool hideOrNot whether or not to hide the tab bar
-- @treturn bool the previous state

--- Hides or shows the tool bar.
-- @function HideToolBar
-- @tparam bool hideOrNot whether or not to hide the tool bar
-- @treturn bool the previous state

--- Gets whether the doc switcher is hidden or shown.
-- @function IsDocSwitcherShown
-- @treturn bool

--- Gets whether the menu is hidden or shown.
-- @function IsMenuHidden
-- @treturn bool

--- Gets whether the status bar is hidden or shown.
-- @function IsStatusBarHidden
-- @treturn bool

--- Gets whether the tab bar is hidden or shown.
-- @function IsTabBarHidden
-- @treturn bool

--- Gets whether the tool bar is hidden or shown.
-- @function IsToolBarHidden
-- @treturn bool

--- Launch the "Find in Files" dialog.
-- @function LaunchFindInFilesDlg
-- @tparam string path the directory path to search
-- @tparam string filter the filter to use (e.g. "*.c *.h")

--- Loads a session file.
-- @function LoadSession
-- @tparam string filename full path to the session file

--- Makes the current buffer marked as dirty.
-- @function MakeCurrentBufferDirty

--- Executes a menu command.
-- @function MenuCommand
-- @tparam int command one of the [IDM_xxx](https://github.com/dail8859/LuaScript/blob/master/src/Npp/menuCmdID.h) constants

--- Reloads the specified buffer ID.
-- @function ReloadBufferID
-- @tparam int BufferID
-- @tparam bool alertOrNot whether to show an alert dialog.
-- @treturn bool

--- Reloads the specified file.
-- @function ReloadFile
-- @tparam bool withAlert use an alert or not
-- @tparam string filename file name to reload

--- Saves all the opened documents.
-- @function SaveAllFiles
-- @treturn bool

--- Saves the current document.
-- @function SaveCurrentFile
-- @treturn bool

--- Saves the current document as the specified name.
-- @function SaveCurrentFileAs
-- @tparam bool asCopy flag to indicate of the file should be saved as a copy
-- @tparam string filename file name
-- @treturn bool

--- Saves a new session file of the currently opened buffers.
-- @function SaveCurrentSession
-- @tparam string session full path of the new session file

--- Enable or disable the editor's border edge
-- @function SetEditorBorderEdge
-- @tparam bool useOrNot

--- Enable or disable the use of smooth fonts.
-- @function SetSmoothFont
-- @tparam bool useOrNot

--- Enable or disable the doc switcher panel.
-- @function ShowDocSwitcher
-- @tparam bool showOrNot

--- Switch to the specified document.
-- @function SwitchToFile
-- @tparam string filename file name
-- @treturn bool `true` if successful, else `false`

--- Activate the tab bar context menu.
-- @function TriggerTabbarContextMenu
-- @tparam int view either `MAIN_VIEW` or `SUB_VIEW`
-- @tparam int index 0-based index

--- Properties
-- @section properties

--- Whether plugins are allowed to be loaded from the `%APPDATA%` directory.
-- @tparam[readonly] bool AppDataPluginsAllowed

--- Gets or sets the buffer's end of line mode (one of the `SC_EOL_xxx` constants).
-- @array BufferFormat
-- @tparam BufferID id
-- @treturn int

--- Gets or sets the buffer's language type (one of the `L_xxx` constants).
-- @array BufferLangType
-- @tparam BufferID id
-- @treturn int

--- Gets the current BufferID.
-- @tparam[readonly] BufferID CurrentBufferID

--- Gets the current column of the cursor.
-- @tparam[readonly] int CurrentColumn

--- Gets the current line number of the cursor.
-- @tparam[readonly] int CurrentLine

--- Gets the currently active view, either `MAIN_VIEW` or `SUB_VIEW`.
-- @tparam[readonly] int CurrentView

--- Gets the default background color used for documents.
-- @tparam[readonly] colour DefaultBackgroundColor

--- Gets the default foreground color used for documents.
-- @tparam[readonly] colour DefaultForegroundColor

--- Gets the description of a language.
-- @array LanguageDescription
-- @tparam[readonly] int language one of the `L_xxx` constant
-- @treturn string

--- Gets the language name.
-- @array LanguageName
-- @tparam[readonly] int language one of the `L_xxx` constant
-- @treturn string

--- Gets the encoding of Notepad++'s localization.
-- @tparam[readonly] int NativeLangEncoding

--- Sets the status bar's text.
-- `field` must be one of the following:
-- 
-- * `STATUSBAR_DOC_TYPE`
-- * `STATUSBAR_DOC_SIZE`
-- * `STATUSBAR_CUR_POS`
-- * `STATUSBAR_EOF_FORMAT`
-- * `STATUSBAR_UNICODE_TYPE`
-- * `STATUSBAR_TYPING_MODE`
-- 
-- @array StatusBar
-- @tparam int field

--- Gets the Notepad++ version.
-- The high 16 bits contain the major version. The low 16 bits contain the minor version.
-- @usage print((npp.Version >> 16) .. '.' .. (npp.Version & 0xff))
-- @tparam[readonly] int Version

--- Gets the version of the Windows operating system (one of the `WV_xxx` constants).
-- @tparam[readonly] int WindowsVersion

--- Helper Methods
-- @section helpermethods

--- Sends a message to the current editor.
-- This duplicates the functionality of the editor object, providing access to this through an interface that is more familiar to Scintilla C++ developers.
-- @function SendEditor
-- @static
-- @tparam int sci_const A `SCI_xxx` message constant
-- @param[opt=0] wparam optional parameter dependent on the specific message
-- @param[opt=0] lparam optional parameter dependent on the specific message
-- @return optional return value dependent on the specific message

--- Looks up the symbolic name of a constant.
-- @function ConstantName
-- @static
-- @tparam int const number
-- @tparam[opt] string hint the prefix of the constant to attempt to find
-- @treturn string The symbolic name of a Scintilla / Notepad++ constant. Raises error if not found.

--- Registers a Lua function to handle a given event.
-- See the topic on Callbacks
-- 
-- @function AddEventHandler
-- @static
-- @tparam string event name of the desired event (can also be an array of strings)
-- @tparam function callback function to call when the event(s) is triggered
-- @treturn bool always returns `true` currently

--- Removes previously registered Lua function for a given event.
-- @function RemoveEventHandler
-- @static
-- @tparam string event name of the desired event
-- @tparam function callback a previously registered function
-- @treturn bool `true` if the function had been previously registered, else `false`
-- @see AddEventHandler

--- Removes all registered Lua functions for a given event.
-- @function RemoveAllEventHandlers
-- @static
-- @tparam string event name of the desired event
-- @see AddEventHandler

--- Registers a Lua function for a shortcut key.
-- The callback is not passed any parameters.
-- 
-- **Note:** This can only be called during startup.
-- @function AddShortcut
-- @static
-- @tparam string name the user-friendly name of the shortcut (this is displayed in the menu)
-- @tparam string shortcut the modifier and key (e.g. "Ctrl+Alt+Shift+D"). The key must be A-Z, 0-9, or F1-F12
-- @tparam function callback function to call when the shortcut is triggered

--- Writes an error message to the console.
-- Calls `tostring` on each parameter and prints the results in the console in red text.
-- @function WriteError
-- @static
-- @param ... variable number of arguments

--- Clears the console.
-- @function ClearConsole
-- @static

--- Continuously calls a function at a set interval.
--
-- **Note:** The timer will continue to fire until `StopTimer` is called. Also, these timers run synchronously in the same thread as Notepad++, meaning that if Notepad++ is busy processing the file (such as doing search/replace) the timer event will get delayed. It is safe to access any of the globally defined LuaScript objects in the callback.
-- @function StartTimer
-- @static
-- @tparam int ms milliseconds to wait between calls
-- @tparam function callback function to be called. It will be passed a singe parameter, the opaque `timer` object
-- @return the opaque `timer` object
-- @see StopTimer
-- @usage -- A trivial example...save a copy of the current file every 10 seconds, unless the file is too big
-- npp.StartTimer(10000, function(timer)
--     if editor.Length < 1000 then
--         npp:SaveCurrentFileAs(true, [[C:\path\to\current\file.backup]])
--     else
--         npp.StopTimer(timer)
--     end
-- end)

--- Stops a previously created timer.
--
-- @function StopTimer
-- @static
-- @param timer the opaque `timer` object returned from `StartTimer` or passed into the callback
-- @see StartTimer
