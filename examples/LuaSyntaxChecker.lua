-- Any time a lua file is saved, this compiles it and checks for
-- syntax errors. Errors are added as inline annotations

editor1.AnnotationVisible = ANNOTATION_BOXED
editor2.AnnotationVisible = ANNOTATION_BOXED

npp.AddEventHandler("OnSave", function(filename, bufferid)
    if npp:GetExtPart() ~= ".lua" then return false end

    editor:AnnotationClearAll()

    -- Try to compile the lua code
    local _, err = load(editor:GetText(0, editor.Length), npp:GetFileName(), "t")

    -- See if it worked
    if err == nil then return false end

    print(err)
    local line = tonumber(err:match(":(%d+):")) - 1
    editor.AnnotationText[line] = err
    editor.AnnotationStyle[line] = STYLE_BRACELIGHT
    editor:GotoLine(line)
    editor:VerticalCentreCaret()
    return false
end)
