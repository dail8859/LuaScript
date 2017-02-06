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

project = 'LuaScript <sub>v0.6.0</sub>'
title = 'LuaScript Reference'
description = 'Notepad++ plugin for [Lua](http://www.lua.org/) scripting capabilities.'
full_description = [[LuaScript allows you to customize Notepad++.

* Assign Lua functions to shortcut keys or menus
* Register callbacks for events
* Full Lua 5.3 functionality
* Interactive console with auto-completion

The startup script is automatically ran when Notepad++ is launched. This is used to register shortcuts and callbacks to extend Notepad++'s functionality.

Issues or feature requests can be made on the GitHub [issue tracker](https://github.com/dail8859/LuaScript/issues).

Make sure to check out the examples.
]]
format = 'markdown'
dir = '../docs'
style = true
template = true
no_lua_ref = true
merge = true
no_space_before_args = true
use_markdown_titles = true

topics = { -- Put any md files here
	'Callbacks.md',
	'ExternalPluginSupport.md',
}

file = { -- sourcecode to parse comments from (Lua or C)
	'Editor.lua',
	'Match.lua',
	'Styler.lua',
	'Notepad.lua',
}

examples = {
	'../examples',
}
