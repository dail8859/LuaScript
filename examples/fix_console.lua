-- Unfold lines which copied from windows console
-- обрабатывает вставленный вывод консоли, склеивая перенесенные строки в одну

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

local function get_max_line_len(lines)
	local res
	for _, line in ipairs(lines) do
		if not res or res < #line then
			res = #line
		end
	end
	return res
end


local function merge_lines(line_len_max, lines)
	local res = {}
	local cur_line
	for i = 1, #lines+1 do
		local line = lines[i] or ''
		--print(cur_line and #cur_line, #line, line)

		if #line+1 >= line_len_max then
			if #line+1 == line_len_max then
				line = line .. ' '
			end
			cur_line = (cur_line or '') .. line
		else
			if cur_line then
				line = cur_line .. line
				cur_line = nil
			end
			table.insert(res, line)
		end
	end

	return res
end


-- =========================================

npp.AddShortcut("Fix Concole Output", "", function()
    editor:BeginUndoAction() -- Preserve Undo Capability

    local text = editor:GetSelText()     -- Get any selected text

	local SelectionStart, SelectionEnd
    if #text >= 1 then
        --read the selection points for restore after operation
        SelectionStart = editor.SelectionStart
        SelectionEnd = editor.SelectionEnd
    else
        text = editor:GetText() -- read selected text or entire document
    end

    local EOLchar = GetEOLchar()     -- Preserve EOL Mode when we send back the reordered lines

    --------------------
    -- process the text
    --------------------

    local rawtextlines = split(text, EOLchar) -- split the text into an array of lines
    local max_len = get_max_line_len(rawtextlines)
	print('max_len =', max_len)
	rawtextlines = merge_lines(max_len, rawtextlines)


    text = table.concat(rawtextlines, EOLchar) -- output to result var

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
