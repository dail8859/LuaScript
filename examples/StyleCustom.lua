--[[ An example file

proc clip(int a)
« Clip into the positive zone »
if (a > 0) a
0
end 
]]

-- Define style numbers
S_DEFAULT = 0
S_IDENTIFIER = 1
S_KEYWORD = 2
S_UNICODECOMMENT = 3

-- Anytime a file is switched, check to see if it needs styled
npp.AddEventHandler("OnSwitchFile", function(filename, bufferid)
    if npp:GetExtPart() == ".abc" then
        -- Add the event handler for our custom style function
        npp.AddEventHandler("OnStyle", CustomStyle)
        
        -- Make sure to set the lexer as a custom container
        editor.Lexer = SCLEX_CONTAINER
        
        -- Set up the styles as appropriate
        editor.StyleFore[S_DEFAULT] = 0x7f007f
        editor.StyleBold[S_DEFAULT] = true
        editor.StyleFore[S_IDENTIFIER] = 0x000000
        editor.StyleFore[S_KEYWORD] = 0x800000
        editor.StyleBold[S_KEYWORD] = true
        editor.StyleFore[S_UNICODECOMMENT] = 0x008000
        editor.StyleFont[S_UNICODECOMMENT] = "Georgia"
        editor.StyleItalic[S_UNICODECOMMENT] = true
        editor.StyleSize[S_UNICODECOMMENT] = 9 
        
        -- Clear any style and re-lex the entire document
        editor:ClearDocumentStyle()
        editor:Colourise(0, -1)
    else
        -- Can remove the handler if it's not needed
        npp.RemoveEventHandler("OnStyle", CustomStyle)
    end

    return false
end)

-- Style the document. Handles UTF-8 characters
function CustomStyle(styler)
    identifierCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

    styler:StartStyling(styler.startPos, styler.lengthDoc, styler.initStyle)
    while styler:More() do
        -- Exit state if needed
        if styler:State() == S_IDENTIFIER then
            if not identifierCharacters:find(styler:Current(), 1, true) then
                identifier = styler:Token()
                if identifier == "if" or identifier == "end" then
                    styler:ChangeState(S_KEYWORD)
                end
                styler:SetState(S_DEFAULT)
            end
        elseif styler:State() == S_UNICODECOMMENT then
            if styler:Match("»") then
                styler:ForwardSetState(S_DEFAULT)
            end
        end

        -- Enter state if needed
        if styler:State() == S_DEFAULT then
            if styler:Match("«") then
                styler:SetState(S_UNICODECOMMENT)
            elseif identifierCharacters:find(styler:Current(), 1, true) then
                styler:SetState(S_IDENTIFIER)
            end
        end

        styler:Forward()
    end
    styler:EndStyling()
end
