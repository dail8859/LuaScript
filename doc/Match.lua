--- 
-- Used for searching and replacing text. A 
-- match generator is returned from `Editor:match` which can be used to iterate 
-- search results. While looping through matches, if the document is modified
-- by any method other than the match object's replace method, this may cause 
-- the match generator to lose its place. Also, do not attempt to store the 
-- match object for later access outside the loop; it will not be usable.
-- 
-- @usage for m in editor:match("teh") do m:replace("the") end
-- 
-- @classmod Match
-- @see Editor:match

--- Beginning position of the located match.
-- @tparam int pos

--- Length of text matched.
-- @tparam int len

--- The actual text matched.
-- @tparam string text

--- Replaces the matched text.
-- @function replace
-- @tparam string replaceText replacement text

--- String representation of the match.
-- @function __tostring
