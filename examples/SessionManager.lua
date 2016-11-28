-- A very simplistic session manager. It opens previously open files and restores
-- cursor position and scroll position. It is not perfect, for example it does 
-- not keep track of the order or in which view the files are in. This code must
-- be ran during startup. This is just a demonstration of how to use the API
-- and what can be done in a small amount of Lua.


local opened_files = {}
local is_shutting_down = false

-- If a session is found, load it. Note this cannot be done directly during
-- startup since N++ hasn't completely initialized yet.
npp.AddEventHandler("OnReady", function()
    local files = table.load(npp:GetPluginsConfigDir() .. "\\session.lua")
    if files ~= nil then
        for i,v in pairs(files) do
            npp:DoOpen(v.name)
            if v.lang then npp.BufferLangType[npp.CurrentBufferID] = v.lang end
            editor.FirstVisibleLine = v.top or 0
            editor.CurrentPos = v.pos or 0
            editor.Anchor = v.anchor or 0
        end
    end
    return false 
end)

-- Remove any files that are closed
npp.AddEventHandler("OnClose", function(f,i)
    -- If N++ is in the middle of shutting down, just ignore it
    if is_shutting_down then return false end
    opened_files[i] = nil
    return false
end)

-- This won't be called unless the user manually changes it, else it just
-- defaults to whatever value N++ decides is best
npp.AddEventHandler("OnLangChange", function()
    local i = npp.CurrentBufferID
    opened_files[i].lang = npp.BufferLangType[i]
end)

-- Keep track of some of the variables we want to restore in the file
-- This event will also catch any files being opened for the first time
npp.AddEventHandler("OnUpdateUI", function()
    local i = npp.CurrentBufferID
    if opened_files[i] == nil then
        opened_files[i] = {}
        opened_files[i].name = npp:GetFullCurrentPath()
    end
    opened_files[i].pos = editor.CurrentPos
    opened_files[i].anchor = editor.Anchor
    opened_files[i].top = editor.FirstVisibleLine

    return false
end)

npp.AddEventHandler("OnFileRenamed", function(f,i)
    opened_files[i].name = f -- update the name
    return false
end)

-- Set a flag so we know N++ is shutting down
npp.AddEventHandler("OnBeforeShutdown", function()
    is_shutting_down = true
    return false
end)

-- The user could cancel the shutdown
npp.AddEventHandler("OnCancelShutdown", function()
    is_shutting_down = false
    return false
end)

-- If it is actually shutting down, save the session file
npp.AddEventHandler("OnShutdown", function()
    -- Remove any dummy files, e.g. "new 1"
    for id, tab in pairs(opened_files) do
        if tab.name:find("^new %d+$") ~= nil then
            opened_files[id] = nil
        end
    end
    table.save(opened_files, npp:GetPluginsConfigDir() .. "\\session.lua")

    return false
end)


-- These below are merely helper fuctions to serialize tables. They were found
-- laying around on the Internet somewhere and worked for this case.

function table.save(tbl,filename )
    local function exportstring( s )
        return string.format("%q", s)
    end
    local charS,charE = "\t","\n"
    local file,err = io.open( filename, "wb" )
    if err then return err end

    -- initiate variables for save procedure
    local tables,lookup = { tbl },{ [tbl] = 1 }
    file:write( "return {"..charE )

    for idx,t in ipairs( tables ) do
        file:write( "-- Table: {"..idx.."}"..charE )
        file:write( "{"..charE )
        local thandled = {}

        for i,v in ipairs( t ) do
            thandled[i] = true
            local stype = type( v )
            -- only handle value
            if stype == "table" then
                if not lookup[v] then
                    table.insert( tables, v )
                    lookup[v] = #tables
                end
                file:write( charS.."{"..lookup[v].."},"..charE )
            elseif stype == "string" then
                file:write(  charS..exportstring( v )..","..charE )
            elseif stype == "number" then
                file:write(  charS..tostring( v )..","..charE )
            end
        end

        for i,v in pairs( t ) do
            -- escape handled values
            if (not thandled[i]) then
            
                local str = ""
                local stype = type( i )
                -- handle index
                if stype == "table" then
                    if not lookup[i] then
                        table.insert( tables,i )
                        lookup[i] = #tables
                    end
                    str = charS.."[{"..lookup[i].."}]="
                elseif stype == "string" then
                    str = charS.."["..exportstring( i ).."]="
                elseif stype == "number" then
                    str = charS.."["..tostring( i ).."]="
                end
            
                if str ~= "" then
                    stype = type( v )
                    -- handle value
                    if stype == "table" then
                        if not lookup[v] then
                            table.insert( tables,v )
                            lookup[v] = #tables
                        end
                        file:write( str.."{"..lookup[v].."},"..charE )
                    elseif stype == "string" then
                        file:write( str..exportstring( v )..","..charE )
                    elseif stype == "number" then
                        file:write( str..tostring( v )..","..charE )
                    end
                end
            end
        end
        file:write( "},"..charE )
    end
    file:write( "}" )
    file:close()
end

function table.load( sfile )
    local ftables,err = loadfile( sfile )
    if err then return _,err end
    local tables = ftables()
    for idx = 1,#tables do
        local tolinki = {}
        for i,v in pairs( tables[idx] ) do
            if type( v ) == "table" then
                tables[idx][i] = tables[v[1]]
            end
            if type( i ) == "table" and tables[i[1]] then
                table.insert( tolinki,{ i,tables[i[1]] } )
            end
        end
        -- link indices
        for _,v in ipairs( tolinki ) do
            tables[idx][v[2]],tables[idx][v[1]] =  tables[idx][v[1]],nil
        end
    end
    return tables[1]
end
