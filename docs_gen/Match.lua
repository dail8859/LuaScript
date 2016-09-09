--- 
-- Used for searching and replacing text. A match generator is returned from `Editor:match` which can 
-- be used to iterate search results. While looping through matches, if the document is modified by any 
-- method other than the match object's replace method, this may cause the match generator to lose its 
-- place. Also, do not attempt to store the match object for later access outside the loop; it will not 
-- be usable.
-- 
-- @classmod Match
-- @see Editor:match
-- 
-- @usage for m in editor:match("teh") do
--     m:replace("the")
-- end

--- Beginning position of the located match.
-- @tparam[readonly] int pos

--- Length of text matched.
-- @tparam[readonly] int len

--- The actual text matched.
-- @tparam[readonly] string text

--- Replaces the matched text.
-- @function replace
-- @tparam string text replacement text

--- String representation of the match.
-- @function __tostring
