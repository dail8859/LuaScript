-- open base64 encoded jpg images in external viewer


local b = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'

local function base64_dec(data)
    data = string.gsub(data, '[^'..b..'=]', '')
    return (data:gsub('.', function(x)
        if (x == '=') then return '' end
        local r,f='',(b:find(x)-1)
        for i=6,1,-1 do r=r..(f%2^i-f%2^(i-1)>0 and '1' or '0') end
        return r;
    end):gsub('%d%d%d?%d?%d?%d?%d?%d?', function(x)
        if (#x ~= 8) then return '' end
        local c=0
        for i=1,8 do c=c+(x:sub(i,i)=='1' and 2^(8-i) or 0) end
        return string.char(c)
    end))
end

npp.AddShortcut("base64 to Image", "", function()
    local text = editor:GetSelText()     -- Get any selected text

    if #text ==0 then
        text = editor:GetText() -- read selected text or entire document
    end

    local data = base64_dec(text)

    local path = os.tmpname()
    if not string.find(path, ':') then
        path = os.getenv("TEMP") .. path
    end
    path = path .. '.jpg'

    local f = assert(io.open(path, 'w+b'))
    f:write(data)
    f:close()

    os.execute(path)
end)
