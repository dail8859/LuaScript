-- Anytime a file is switched, check to see if it needs styled
npp.AddEventHandler("OnSwitchFile", function(filename, bufferid)
    if npp:GetExtPart() == ".csv" then
        -- Add the event handler for our custom style function
        npp.AddEventHandler("OnStyle", StyleCSV)
        
        -- Make sure to set the lexer as a custom container
        editor.Lexer = SCLEX_CONTAINER
        
        -- Set up the styles as appropriate
        editor.StyleBack[1] = 0xffffff
        editor.StyleBack[2] = 0xC0D9AF
        editor.StyleEOLFilled[1] = true
        editor.StyleEOLFilled[2] = true
        
        -- Clear any style and re-lex the entire document
        editor:ClearDocumentStyle()
        editor:Colourise(0, -1)
        
        -- Set any other options, for this styler the caret line is ugly
        editor.CaretLineVisible = false
    else
        -- Can remove the handler if it's not needed
        npp.RemoveEventHandler("OnStyle", StyleCSV)
        editor.CaretLineVisible = true
    end
end)

-- A simplified lexer - style the line according to the line number
function StyleCSV(styler)
    local lineStart = editor:LineFromPosition(styler.startPos)
    local lineEnd = editor:LineFromPosition(styler.startPos + styler.lengthDoc)
    editor:StartStyling(styler.startPos, 0 --[[ unused --]])
    for line=lineStart,lineEnd,1 do
        local lengthLine = editor:PositionFromLine(line+1) - editor:PositionFromLine(line)
        if line % 2 == 0 then
            editor:SetStyling(lengthLine, 1)
        else
            editor:SetStyling(lengthLine, 2)
        end
    end
end
