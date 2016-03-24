# Callback Events
Scripts can register Lua functions as a callback when certain events take place. Each event type can have mulitple callbacks registered. Supported events include:

- `OnChar(character)`
- `OnUpdateUI()`
- `OnSavePointReached()`
- `OnSavePointLeft()`
- `OnBeforeOpen(filename)`
- `OnOpen(filename)`
- `OnSwitchFile(filename)`
- `OnBeforeSave(filename)`
- `OnSave(filename)`
- `OnBeforeClose(filename)`
- `OnClose(filename)`
- `OnLangChange()`

The Notepad++ object allows adding, removing, and clearing callback functions for the events. 

- `npp.Add<Event>(function)`
- `npp.Remove<Event>(function)`
- `npp.RemoveAll<Event>()`

To register for an event, first define your fuction that accepts the parameters (if any). You can then add the callback to the appropriate event. Although callbacks are not required to return a value, in order to ensure forwards compatibility it should return `false`. For example:

```lua
function printchar(ch)
    print("You typed " .. ch)
    return false
end

npp.AddOnChar(printchar)
```

Take care not to register the same callback twice. To be safe you can attempt to remove it before adding it.
