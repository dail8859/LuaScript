-- Get rid of Notepad++'s ugly bookmark icon

-- Notepad++ uses 24 internally
editor1:MarkerDefine(24, SC_MARK_BOOKMARK)
editor1.MarkerFore[24] = 0x0000EE
editor1.MarkerBack[24] = 0x6060F2

editor2:MarkerDefine(24, SC_MARK_BOOKMARK)
editor2.MarkerFore[24] = 0x0000EE
editor2.MarkerBack[24] = 0x6060F2
