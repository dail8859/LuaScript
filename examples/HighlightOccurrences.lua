-- Mark occurrences of the current word under the cursor

-- Adjust these values if desired
local indicator = 12 -- not sure what one is best to use but this works
editor1.IndicStyle[indicator] = INDIC_ROUNDBOX
editor2.IndicStyle[indicator] = INDIC_ROUNDBOX
editor1.IndicAlpha[indicator] = 20
editor2.IndicAlpha[indicator] = 20
editor1.IndicOutlineAlpha[indicator] = 20
editor2.IndicOutlineAlpha[indicator] = 20

npp.AddEventHandler("OnUpdateUI", function()
    local function getRangeOnScreen()
        local firstLine = editor.FirstVisibleLine
        local lastLine = firstLine + editor.LinesOnScreen
        local startPos = editor:PositionFromLine(firstLine)
        local endPos = editor.LineEndPosition[lastLine]
        return startPos, endPos
    end

    local function clearIndicatorOnScreen()
        local s, e = getRangeOnScreen()
        editor:IndicatorClearRange(s, e - s)
    end

    editor.IndicatorCurrent = indicator

    if not editor.SelectionEmpty then
        clearIndicatorOnScreen()
        return false
    end

    local startWord = editor:WordStartPosition(editor.CurrentPos, true)
    local endWord = editor:WordEndPosition(startWord, true)

    -- Not a word
    if startWord == endWord then
        clearIndicatorOnScreen()
        return false
    end

    local word = editor:textrange(startWord, endWord)

    clearIndicatorOnScreen()

    -- for each match on screen turn on the indicator
    local startPos, endPos = getRangeOnScreen()
    local s, e = editor:findtext(word, SCFIND_WHOLEWORD | SCFIND_MATCHCASE, startPos, endPos)
    while s ~= nil do
        editor:IndicatorFillRange(s, e - s)
        s, e = editor:findtext(word, SCFIND_WHOLEWORD | SCFIND_MATCHCASE, e, endPos)
    end

    return false
end)
