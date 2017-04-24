--------------------------------------------------------------
-- addlinenumbers.lua for Notepad++ LuaScript plugin
--
-- Sometimes you need to add real line numbers into a document.
--
-- Activate it on all text in the editor with CTRL+F5
-- or select a group of lines to add numbers to.
--------------------------------------------------------------

-- helper function to split lines
function split(str, sep)
    local result = {}
    local regex = ("([^%s]+)"):format(sep)
    for each in str:gmatch(regex) do
      table.insert(result, each)
    end
    return result
end

--------------------------------------------------------------------

npp.AddShortcut("Add Line Numbers", "Ctrl+Alt+L", function()

    local result --placeholder to store the processed text for return to editor
    local SelectionStart, SelectionEnd --placeholder to store selection points

    -- Preserve Undo Capability
    editor:BeginUndoAction()

    -- Get any selected text
    local seltext = editor:GetSelText()
    local seltextlen = #seltext

    -- read selected text or entire document
    local rawtext
    if seltextlen >= 1 then
        rawtext = seltext
        --read the selection points for restore after operation
        SelectionStart = editor.SelectionStart
        SelectionEnd = editor.SelectionEnd
    else
        rawtext = editor:GetText()
    end

    -- Preserve EOL Mode when we send back the reordered lines
    local EOLchar
    if editor.EOLMode == 0 then
      EOLchar = "\r\n"
    elseif editor.EOLMode == 1 then
       EOLchar = "\r"
    elseif editor.EOLMode == 2 then
       EOLchar = "\n"
    end
    --------------------
    -- process the text
    --------------------
    
    -- split the text into an array of lines
    local rawtextlines = split(rawtext,EOLchar);

    -- add the line numbers
    for i=1,#rawtextlines do
        rawtextlines[i] = i .. " " .. rawtextlines[i]
    end
    
    -- output to result var
    result = table.concat(rawtextlines, EOLchar)
    
    --------------------
    --  end processing
    --------------------    

     -- replace selected text or entire document
    if seltextlen >= 1 then 
        editor:ReplaceSel(result)
        -- restore selection
        editor:SetSel(SelectionStart, SelectionEnd)
    else
        editor:SetText(result)	
    end
    
    editor:EndUndoAction()
    
end)
