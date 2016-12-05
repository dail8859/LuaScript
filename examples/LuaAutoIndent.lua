-- Regexs to determine when to indent or unindent
-- From: https://github.com/sublimehq/Packages/blob/master/Lua/Indent.tmPreferences
local decreaseIndentPattern = [[^\s*(elseif|else|end|\})\s*$]]
local increaseIndentPattern = [[^\s*(else|elseif|for|(local\s+)?function|if|repeat|until|while)\b((?!end).)*$|\{\s*$]]

do_increase = false

-- Get the start and end position of a specific line number
local function getLinePositions(line_num)
    local start_pos = editor:PositionFromLine(line_num)
    local end_pos = start_pos + editor:LineLength(line_num)
    return start_pos, end_pos
end

-- Check any time a character is added
local function autoIndent_OnChar(ch)
    if ch == "\n" then
        -- Get the previous line
        local line_num = editor:LineFromPosition(editor.CurrentPos) - 1
        local start_pos, end_pos = getLinePositions(line_num)
        if editor:findtext(increaseIndentPattern, SCFIND_REGEXP, start_pos, end_pos) then
            -- This has to be delayed because N++'s auto-indentation hasn't triggered yet
            do_increase = true
        end
    else
        local line_num = editor:LineFromPosition(editor.CurrentPos)
        local start_pos, end_pos = getLinePositions(line_num)
        if editor:findtext(decreaseIndentPattern, SCFIND_REGEXP, start_pos, end_pos) then
            -- The pattern matched, now check the previous line's indenation
            if line_num > 1 and editor.LineIndentation[line_num - 1] <= editor.LineIndentation[line_num] then
                editor.LineIndentation[line_num] = editor.LineIndentation[line_num] - 4
            end
        end
    end

    return false
end

-- Work around N++'s auto indentation by delaying the indentation change
local function autoIndent_OnUpdateUI(flags)
    if do_increase then
        do_increase = false
        -- Now the the indentation can be increased since N++'s auto-indentation is done by now
        editor:Tab()
    end

    return false
end

-- See if the auto indentation should be turned on or off
local function checkAutoIndent(bufferid)
    if npp.BufferLangType[bufferid] == L_LUA then
        do_increase = false
        -- Register the event handlers
        npp.AddEventHandler("OnChar", autoIndent_OnChar)
        npp.AddEventHandler("OnUpdateUI", autoIndent_OnUpdateUI)
    else
        -- Remove the event handlers
        npp.RemoveEventHandler("OnChar", autoIndent_OnChar)
        npp.RemoveEventHandler("OnUpdateUI", autoIndent_OnUpdateUI)
    end
end

-- Only turn on the auto indentation when it is actually a lua file
-- Check it when the file is switched to and if the language changes

npp.AddEventHandler("OnSwitchFile", function(filename, bufferid)
    checkAutoIndent(bufferid)
    return false
end)

npp.AddEventHandler("OnLangChange", function()
    checkAutoIndent(npp.CurrentBufferID)
    return false
end)
