-- open corresponding header (.h, .hpp) or source (.c, .cpp) file
-- скрипт для Notepad++, открывает соотвтествующий файл заголовка или исх. кода по названию


local function get_index(tbl, val)
	for i, v in ipairs(tbl) do
		if v == val then
			return i
		end
	end
end

local function split_ext(file_path)
	return string.match(file_path, "^(.+)(%..+)$")
end

local function file_exist(file_path)
	local f = io.open(file_path, "r")
	if f ~= nil then
		io.close(f)
		return true
	else
		return false
	end
end


-- ======================================================

local KNOWN_EXTENSIONS = {'.h', '.hpp', '.cpp', '.c'}


npp.AddShortcut("Open Source", "", function()
	local file_name, file_ext = split_ext(npp:GetFullCurrentPath())

	if get_index(KNOWN_EXTENSIONS, file_ext) then
		for i, ext in ipairs(KNOWN_EXTENSIONS) do
			if ext ~= file_ext then
				local tmp_path = file_name .. ext
				-- print(tmp_path)
				if file_exist(tmp_path) then
					npp:DoOpen(tmp_path)
					break
				end
			end
		end
	end
end)
