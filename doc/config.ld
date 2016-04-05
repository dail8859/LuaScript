-- Make a new array type. This type assumes it has a tparam tag to indicate what
-- should be used to index it
new_type("array", "Array")

alias("position", "int")
alias("colour", "int")
alias("BufferID", "int")

-- Tweak the array names to show "Name[param]"
custom_display_name_handler = function(item, default_handler)
	if item.type == 'array' then
		return item.name .. '[' .. item.params[1] .. ']'
	end
	return default_handler(item)
end

local scidocurl = 'http://www.scintilla.org/ScintillaDoc.html#%s'
custom_see_handler('^(SCI_[%w_]+)$', function(name)
    local url = scidocurl:format(name)
    return name, url
end)

project = 'LuaScript <sub>v0.3.0</sub>'
title = 'LuaScript Reference'
description = 'Notepad++ plugin for [Lua](http://www.lua.org/) scripting capabilities.'
format = 'markdown'
dir = './gh-pages'
style = true
template = true
no_lua_ref = true
merge = true
no_space_before_args = true

topics = { -- Put any md files here
	'Callbacks.md',
}

file = { -- sourcecode to parse comments from (Lua or C)
	'Editor.lua',
	'Match.lua',
	'../SciTE/LuaExtension.cpp',
	'Notepad.lua',
}

examples = {
	'../examples',
}
