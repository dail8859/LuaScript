-- Any time a lua file is saved, this compiles it and checks for
-- syntax errors. Errors are added as inline annotations

editor1.AnnotationVisible = ANNOTATION_INDENTED
editor2.AnnotationVisible = ANNOTATION_INDENTED

npp.AddEventHandler("OnSave", function(filename, bufferid)
    if npp:GetExtPart() ~= ".lua" then return false end

    editor:AnnotationClearAll()
    
    -- Try to compile the lua code
    local f, err = load(editor:GetText(0, editor.Length), npp:GetFileName(), "t")

    -- See if it worked
    if f ~= nil then return false end

    print(err)
    editor.AnnotationText[tonumber(err:match(":(%d+):")) - 1] = err
end)
