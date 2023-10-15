-- Mark selected or all lines with quoting char '>'


-- helper function to split lines
local function split1(str, sep)
    local result = {}
    local regex = ("([^%s]+)"):format(sep)
    for each in str:gmatch(regex) do
      table.insert(result, each)
    end
    return result
end

-- split without skip empty lines
local function split(str, delimiter)
	local result = { }
	local from  = 1
	local delim_from, delim_to = string.find( str, delimiter, from  )
	while delim_from do
		table.insert( result, string.sub( str, from , delim_from-1 ) )
		from  = delim_to + 1
		delim_from, delim_to = string.find( str, delimiter, from  )
	end
	table.insert( result, string.sub( str, from  ) )

	-- clean only space strings
	for i = 1, #result do
		if not string.find(result[i], '%S') then
			result[i] = ''
		end
	end
	return result
end


local function add_quote_char(lines)
    local res = {}
	for _, line in ipairs(lines) do
		if #line > 0 then
			line = '> ' .. line
		end
		table.insert(res, line)
	end
    return res
end

local function GetEOLchar()
	local EOLchar
	 if editor.EOLMode == 0 then
      EOLchar = "\r\n"
    elseif editor.EOLMode == 1 then
       EOLchar = "\r"
    elseif editor.EOLMode == 2 then
       EOLchar = "\n"
    end
	return EOLchar
end

--------------------------------------------------------------------


npp.AddShortcut("Make Quoting", "", function()

    local SelectionStart, SelectionEnd

    -- Preserve Undo Capability
    editor:BeginUndoAction()

    -- Get any selected text
    local text = editor:GetSelText()

    -- read selected text or entire document
    if #text >= 1 then
        --read the selection points for restore after operation
        SelectionStart = editor.SelectionStart
        SelectionEnd = editor.SelectionEnd
    else
        text = editor:GetText()
    end

    -- Preserve EOL Mode when we send back the reordered lines
    local EOLchar = GetEOLchar()

    --------------------
    -- process the text
    --------------------

    -- split the text into an array of lines
    local rawtextlines = split(text, EOLchar);

    -- randomize the lines
    rawtextlines = add_quote_char(rawtextlines)

    -- output to result var
    text = table.concat(rawtextlines, EOLchar)

    --------------------
    --  end processing
    --------------------

     -- replace selected text or entire document
    if SelectionStart then
        editor:ReplaceSel(text)
        -- restore selection
        editor:SetSel(SelectionStart, SelectionEnd)
    else
        editor:SetText(text)
    end

    editor:EndUndoAction()

end)
