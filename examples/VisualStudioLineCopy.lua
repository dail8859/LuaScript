-- Mimic Visual Studio's "Ctrl+C" that copies the entire line if nothing is selected
npp.AddShortcut("Copy Allow Line", "Ctrl+C", function()
    editor:CopyAllowLine()
end)

-- Mimic Visual Studio's "Ctrl+X" that cuts the line if nothing is selected
npp.AddShortcut("Cut Allow Line", "Ctrl+X", function()
    if editor.SelectionEmpty then
        editor:CopyAllowLine()
        editor:LineDelete()
    else
        editor:Cut()
    end
end)
