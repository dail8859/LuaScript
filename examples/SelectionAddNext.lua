-- Select the next occurrence of the current word
npp.AddShortcut("Selection Add Next", "Ctrl+D", function()
    -- From SciTEBase.cxx
    local flags = SCFIND_MATCHCASE -- can use 0
    editor:SetTargetRange(0, editor.TextLength)
    editor.SearchFlags = flags

    -- From Editor.cxx
    if editor.SelectionEmpty or not editor.MultipleSelection then
        local startWord = editor:WordStartPosition(editor.CurrentPos, true)
        local endWord = editor:WordEndPosition(startWord, true)
        editor:SetSelection(startWord, endWord)
    else
        local i = editor.MainSelection
        local s = editor:textrange(editor.SelectionNStart[i], editor.SelectionNEnd[i])
        local searchRanges = {{editor.SelectionNEnd[i], editor.TargetEnd}, {editor.TargetStart, editor.SelectionNStart[i]}}
        for _, range in pairs(searchRanges) do
            editor:SetTargetRange(range[1], range[2])
            if editor:SearchInTarget(s) ~= -1 then
                editor:AddSelection(editor.TargetStart, editor.TargetEnd)
                editor:ScrollRange(editor.TargetStart, editor.TargetEnd)
                break
            end
        end
    end
end)
