---------------------------------------------------------------
-- randomizelines.lua for Notepad++ LuaScript plugin
--
-- This script will randomize the lines within a document.
--
-- Activate it on all text in the editor by running without a
-- selection, or select a group of lines to randomize only 
-- the selected lines.
---------------------------------------------------------------

-- helper function to split lines
local function split(str, sep)
    local result = {}
    local regex = ("([^%s]+)"):format(sep)
    for each in str:gmatch(regex) do
      table.insert(result, each)
    end
    return result
end

-- helper function to shuffle a table
local function shuffle(t)
    math.randomseed( os.time() )
    local rand = math.random 
    local iterations = #t
    local j
    
    for i = iterations, 2, -1 do
        j = rand(i)
        t[i], t[j] = t[j], t[i]
    end
    return t
end

--------------------------------------------------------------------

npp.AddShortcut("Randomize Lines", "", function()

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

    -- randomize the lines
    rawtextlines = shuffle(rawtextlines)
    
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
