# LuaScript
Notepad++ plugin for [Lua](http://www.lua.org/) scripting capabilities. Provides control over all of Scintilla's features and options with a light-weight, fully-functional programming language.

Some examples:
- Change Notepad++'s ugly marker symbols:
```lua
-- Notepad++ uses 24 internally
editor:MarkerDefine(24, SC_MARK_BOOKMARK)
editor.MarkerFore[24] = 0xFF0000
editor.MarkerBack[24] = 0xFF0000
```

- Find all instances of `image345.jpg`, `image123,png`, etc and modify the number:
```lua
for m in editor:match("(image)(\\d+)(\\.(jpg|png))", SCFIND_REGEXP) do
    i = tonumber(editor.Tag[2])
    m:replace(editor.Tag[1] .. i + 1 .. editor.Tag[3])
end
```


**Note:** This is still in early development.

## Development
The code has been developed using MSVC 2013. To compile the code:

1. Open the `LuaScript.sln` file
2. Select the `Unicode Release` target
3. Press `F7` and that's it!

For convenience, MSVC automatically copies the DLL into the Notepad++ plugin directory. 

## License
This code is released under the [GNU General Public License version 2](http://www.gnu.org/licenses/gpl-2.0.txt).
