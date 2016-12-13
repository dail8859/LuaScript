# Callbacks
Scripts can register Lua functions as a callback when certain events take place. Each event type can have multiple callbacks registered.

Scintilla related callbacks:

- `OnStyle(styler)`
- `OnChar(character)`
- `OnSavePointReached()`
- `OnSavePointLeft()`
- `OnModifyAttemptRO()`
- `OnDoubleClick(position, line, modifiers)`
- `OnUpdateUI(flags)`
- `OnModification(modType, position, length, text, linesAdded)`
- `OnMarginClick(position, margin, modifiers)`
- `OnNeedShown(position, length)`
- `OnPainted(listType, position, text)`
- `OnUserListSelection(listType, position, text)`
- `OnDwellStart(position, x, y)`
- `OnDwellEnd(position, x, y)`
- `OnZoom()`
- `OnHotSpotClick(position, modifiers)`
- `OnHotSpotDoubleClick(position, modifiers)`
- `OnHotSpotReleaseClick(position, modifiers)`
- `OnIndicatorClick(position, modifiers)`
- `OnIndicatorRelease(position, modifiers)`
- `OnCallTipClick(flag)`
- `OnAutoCSelection(position, text)`
- `OnAutoCCancelled()`
- `OnAutoCCharDeleted()`
- `OnFocusIn()`
- `OnFocusOut()`

Notepad++ related callbacks:

- `OnReady()`
- `OnBeforeOpen(filename, bufferid)`
- `OnOpen(filename, bufferid)`
- `OnSwitchFile(filename, bufferid)`
- `OnBeforeSave(filename, bufferid)`
- `OnSave(filename, bufferid)`
- `OnFileRenamed(filename, bufferid)`
- `OnFileDeleted(filename, bufferid)`
- `OnBeforeClose(filename, bufferid)`
- `OnClose(filename, bufferid)`
- `OnLangChange()`
- `OnBeforeShutdown()`
- `OnCancelShutdown()`
- `OnShutdown()`

The Notepad++ object allows adding, removing, and clearing callback functions for the events. 

- `npp.AddEventHandler(event, function)`
- `npp.RemoveEventHandler(event, function)`
- `npp.RemoveAllEventHandlers(event)`

To register for an event, first define your function that accepts the parameters (if any). You can then add the callback to the appropriate event. Although callbacks are not required to return a value, in order to ensure forwards compatibility it should return `false`. For example:

```lua
function printchar(ch)
    print("You typed " .. ch)
    return false
end

npp.AddEventHandler("OnChar", printchar)
```

Take care not to register the same callback twice. To be safe you can attempt to remove it before adding it.
