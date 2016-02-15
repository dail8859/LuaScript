# LuaScript
Notepad++ plugin for [Lua](http://www.lua.org/) scripting capabilities. Provides control over all of Scintilla's features and options with a light-weight, fully-functional programming language.

**Note:** This is still in early development.

## Examples
Just show me what it can do!

- Change Notepad++'s ugly marker symbols:
```lua
-- Notepad++ uses 24 internally
editor:MarkerDefine(24, SC_MARK_BOOKMARK)
editor.MarkerFore[24] = 0xFF0000
editor.MarkerBack[24] = 0xFF0000
```

- Find all instances of `image345.jpg`, `image123.png`, etc and modify the number:
```lua
for m in editor:match("(image)(\\d+)(\\.(jpg|png))", SCFIND_REGEXP) do
    i = tonumber(editor.Tag[2])
    m:replace(editor.Tag[1] .. i + 1 .. editor.Tag[3])
end
```

## Documentation
#### Globals
Global objects/functions are made available to access the editing component.
- `editor` - Refers to the current editor view. Since Notepad++ has 2 views, this controls the one currently selected.
  - `editor1` - Refers to the "first" view.
  - `editor2` - Refers to the "second" view.
- `npp` - The Notepad++ application itself.

#### Editor object
The full documentation can be found [here](/doc/editor.md). The `editor` also has a few helper functions:
- `editor:textrange(startPos, endPos)` - gets the text in the specified range
- `editor:findtext(text, [flags], [startPos, [endPos]])`
  - returns the start and end of the first match, or `nil` if no match
  - flags can be 0 (the default), or a combination of [SCFIND](http://www.scintilla.org/ScintillaDoc.html#searchFlags) constants such as `SCFIND_WHOLEWORD`, `SCFIND_MATCHCASE`, and `SCFIND_REGEXP`
- `editor:match(text, [flags], [startPos])`
  - returns a generator that allows you to loop over the matches i.e. `for m in editor:match(text, flags) do ... end`
  - the match object (i.e. the loop counter m in the above example) supports read-only properties `pos`, `len`, and `text`; and also supports a function `replace(replaceText)` to support search and replace.
  - while looping through matches, if the document is modified by any method other than the loop counter's replace method, this may cause the match generator to lose its place.
  - also, do not attempt to store the match object for later access outside the loop; it will not be useable.
- `editor:append(text)` - appends text to the end of the document
- `editor:insert(pos, text)` - inserts text at the specified position
- `editor:remove(startPos, endPos)` - removes the text in the range

#### Notepad++ object
- `npp.ConstantName(number, [hint])`
  - returns the symbolic name of a Scintilla / Notepad++ constant
  - optional `hint` parameter for the name of the appropriate constant
- `npp.MenuCommand(IDM_constant)`
  - runs a menu command using one of the `IDM_` constants defined in [menuCmdID.h](/Npp/menuCmdID.h)
- `npp.Add<Event>(function)`
  - adds a function handler for a specific event. Full documentation for events can be found [here](/doc/events.md)
- `npp.Remove<Event>(function)`
  - removes a previously added function handler for a specific event.

## Development
The code has been developed using MSVC 2013. To compile the code:

1. Open the `LuaScript.sln` file
2. Select the `Release` target
3. Press `F7` and that's it!

For convenience, MSVC automatically copies the DLL into the Notepad++ plugin directory. 

## License
This code is released under the [GNU General Public License version 2](http://www.gnu.org/licenses/gpl-2.0.txt).

#### Thanks
Special thanks to the [PythonScript](https://github.com/bruderstein/PythonScript) plugin and [SciTE](http://www.scintilla.org/SciTE.html).
