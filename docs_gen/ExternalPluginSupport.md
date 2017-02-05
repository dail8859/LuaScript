# External Plugin Support
Plugin developers are able to execute Lua code from their own plugins. Notepad++ provides a mechanism for sending messages between plugins using `NPPM_MSGTOPLUGIN`.

*Note: More commands and fields can be added if needed; feel free to open an issue on GitHub.*

<br/>

Information needed for LuaScript is defined in the custom struct defined [here](https://github.com/dail8859/LuaScript/blob/master/src/NppLuaScript.h).

- `structVersion` - (reserved for future use) must always be `1`
- `flags` - (reserved for future use) must always be `0`
- `script` - UTF-8 encoded string. The meaning of this is dependent on the command executed.

<br/>

### Commands

- `LS_EXECUTE_SCRIPT` - executes a file from the hard drive. `script` specifies the full file path to execute.
- `LS_EXECUTE_STATEMENT` - executes an arbitrary string. `script` is the actual Lua code to execute.

<br/>

### Example 
The following is example C code to execute a string.

```c
void function()
{
    CommunicationInfo ci;
    LuaScriptInfo lsi;

    lsi.structVersion = 1;
    lsi.flags = 0;
    lsi.script = "for i = 1, 10 do print(i) end";

    ci.srcModuleName = TEXT("MyPlugin.dll");
    ci.internalMsg = LS_EXECUTE_STATEMENT;
    ci.info = reinterpret_cast<void *>(&lsi);

    SendMessage(nppHwnd, NPPM_MSGTOPLUGIN, reinterpret_cast<WPARAM>(TEXT("LuaScript.dll")), reinterpret_cast<LPARAM>(&ci));
}
```
