rec1<alphabet>.dat - this file contains event table for recognizer. The table contains some hashes and whatever. This table is read in evn_tab_init_prn() and gets loaded into events_treep variable. The main processing of this is in file v0compev.c, the most relevant function is seek_events.
rec2<alphabet>.dat - same as above, but variable events_tree_rtp, file v0comper.c, function  -- again seek_events.

rec6<lang>.dat - contains alphabet with marked vowels. Encoding cp1251
rec7<lang>.dat - main dictionary (some kind of tree-like structure). Encoding cp866(modified). File where used speldict.c
rec8<lang>.dat - some obscure file, seems to be ok when copied from any language.
rec9<lang>.dat - replacement rules for symbol combinations. Format is relatively simple, it is described in spelart.c Encoding cp1251. Ukrainian one is bad.



General considerations: Actual symbol recoding (cp1251->letter number) happens in text_findstat_rling and findstat using *codetable, which is filled in from rec6<lang>.dat (see InitializeAlphabet, call to dectable_init etc in spelabc.c). Therefore it seems that the alphabet in rec7<lang>.dat is redundant.

Symbol recoding from recognizer to cp1251 happens with help of the table decode_ASCII_to_ (which is e.g. called in _spell, which is called from GoodSpell).
