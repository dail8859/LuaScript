# LuaScript
Notepad++ plugin for [Lua](http://www.lua.org/) scripting capabilities. This provides control over all of Scintilla's features and options with a light-weight, fully-functional programming language.

Major features include:

- Assign Lua functions to shortcut keys
- Register callback functions for events
- Full Lua 5.3 functionality
- Interactive console with auto-completion
- Requires no special permissions

## Getting Started
Install the plugin using the Notepad++ Plugin Manager, or download it from the [Release](https://github.com/dail8859/LuaScript/releases) page and copy `LuaScript.dll` to your `\plugins` folder.

#### Lua Console
LuaScript provides an interactive console for running Lua commands. This can be shown by `Plugins > LuaScript > Show Console`.

#### Startup Script
LuaScript looks for the `\plugins\config\startup.lua` file and automatically runs it on Notepad++ startup. You can easily edit this file via `Plugins > LuaScript > Edit Startup Script`. You can include any commands you want to immediately execute on program startup, as well as register any additional shortcuts or callbacks.

#### Registering New Shortcuts
New functions can be added as shortcuts by using [AddShortcut](https://dail8859.github.io/LuaScript/classes/Notepad.html#Notepad.AddShortcut). The new menu items are listed under the `Plugins > LuaScript` menu. For example [this script](https://dail8859.github.io/LuaScript/examples/visualstudiolinecopy.lua.html) adds 2 new menu items.

#### Registering Callback Functions
Scripts can also register functions to be called when certain events occur, such as a file being opened, text being modified, etc. Each callback provides a different set of parameters. See the documentation for [callbacks](https://dail8859.github.io/LuaScript/topics/callbacks.md.html) to see the entire list of possible events and parameters provided.

## Documentation
The full API documentation can be found [here](http://dail8859.github.io/LuaScript/). Also be sure to check out the [examples](/examples/) directory.

## Development
The code has been developed using MSVC 2015. To compile the code:

1. Open the `LuaScript.sln` file
1. Press `F7` and that's it!

For convenience, MSVC automatically copies the DLL into the Notepad++ plugin directory. 

[![Dev Build](https://ci.appveyor.com/api/projects/status/k80bu9lfjk5n3i39?svg=true)](https://ci.appveyor.com/project/OwnageIsMagic/luascript-til21/build/artifacts)

## License
This code is released under the [GNU General Public License version 2](http://www.gnu.org/licenses/gpl-2.0.txt).

#### Thanks
Special thanks to the [PythonScript](https://github.com/bruderstein/PythonScript) plugin and [SciTE](http://www.scintilla.org/SciTE.html).
