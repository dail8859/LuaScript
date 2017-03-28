# LuaScript
Notepad++ plugin for [Lua](http://www.lua.org/) scripting capabilities. Provides control over all of Scintilla's features and options with a light-weight, fully-functional programming language. Download it from the [Release](https://github.com/dail8859/LuaScript/releases) page.

Major features include:

- Assign Lua functions to shortcut keys
- Register callbacks to specific events
- Full Lua 5.3 functionality
- Interactive console with auto-completion
- Requires no special permissions

## Getting Started
Install the plugin using the Notepad++ Plugin Manager, or download it from the [Release](https://github.com/dail8859/LuaScript/releases) page and copy `LuaScript.dll` to your `\plugins` folder.

#### Startup Scripts
LuaScript looks for the file `\plugins\config\startup.lua` and automatically runs it on Notepad++ startup. You can include any commands you want to immediately execute on prgram startup, as well as register any additional functions/shortcuts.

You can include additional script files within your `startup.lua` file. In the following example, LuaScript will look for a folder  `\plugins\config\lua` and automatially load all of the `.lua` script files within:

```lua
--Include other scripts from lua folder
package.path = package.path .. ";" .. npp:GetPluginsConfigDir() .. "\\lua\\?.lua"
local f = io.popen('dir "' .. npp:GetPluginsConfigDir() .. '\\lua\\*.lua" /b') 
for mod in f:lines() do require( mod:sub(0,-5) ) end
```

#### Registering New Commands
You can register new commands (with or without keyboard shortcuts) by using [AddShortcut](https://dail8859.github.io/LuaScript/classes/Notepad.html#Notepad.AddShortcut). For example [this example script](https://dail8859.github.io/LuaScript/examples/visualstudiolinecopy.lua.html) adds 2 new menu items.

The new menu items are listed under the `Plugins...LuaScript` menu. Commands can be registered without keyboard shortcuts by supplying an empty string or `nil` value in the second argument of `AddShortcut()`

## Documentation
The full API documentation can be found [here](http://dail8859.github.io/LuaScript/).

## Examples
Just show me what it can do! Also, check out the [examples](/examples/) directory.

- Change Notepad++'s ugly marker symbols:
```lua
-- Notepad++ uses 24 internally
editor:MarkerDefine(24, SC_MARK_BOOKMARK)
editor.MarkerFore[24] = 0x0000EE
editor.MarkerBack[24] = 0x6060F2
```

- Find all instances of `image345.jpg`, `image123.png`, etc and modify the number:
```lua
for m in editor:match("(image)(\\d+)(\\.(jpg|png))", SCFIND_REGEXP) do
    i = tonumber(editor.Tag[2])
    m:replace(editor.Tag[1] .. i + 1 .. editor.Tag[3])
end
```

- Get some of Visual Studio's copy and paste functionality:
```lua
-- Mimic Visual Studio's "Ctrl+C" that copies the entire line if nothing is selected
npp.AddShortcut("Copy Allow Line", "Ctrl+C", function()
	editor:CopyAllowLine()
end)

-- Mimic Visual Studio's "Ctrl+X" that cuts the line if nothing is selected
npp.AddShortcut("Cut Allow Line", "Ctrl+X", function()
	if editor.SelectionEmpty then
		editor:CopyAllowLine()
		editor:LineDelete()
	else
		editor:Cut()
	end
end)
```

## Development
The code has been developed using MSVC 2013. To compile the code:

1. Open the `LuaScript.sln` file
2. Select the `Win32` platform (x64 is currently experimental)
3. Press `F7` and that's it!

For convenience, MSVC automatically copies the DLL into the Notepad++ plugin directory. 

## License
This code is released under the [GNU General Public License version 2](http://www.gnu.org/licenses/gpl-2.0.txt).

#### Thanks
Special thanks to the [PythonScript](https://github.com/bruderstein/PythonScript) plugin and [SciTE](http://www.scintilla.org/SciTE.html).
