// Define lists for list of level for macros. Each of the following fields are described:
// Argument 1: Internal ROM name of the level.
// Argument 2: Level enumerator for enum used to identify the level ID.
// Argument 3: Course enumerator for enum used to identify the course ID.
// Argument 4: Shorthand name of the level which should be the name of the levels/ folder of the level.
// Argument 5: The shared texture bin used.
// Argument 6: Acoustic reaches for each levels.
// Argument 7, 8, 9: Echo levels for individual areas.  < Clueless hackers should change these for their levels btw
// Argument 10: Specify dynamic music tables for levels, if specified. _ for none.
// Argument 11: Specify level camera table, if specified. _ for none.

// NOTE: Be sure to edit sZoomOutAreaMasks in camera.c, as there isnt a good way to macro those right now.
// TODO: Figure something out for sZoomOutAreaMasks?

DEFINE_LEVEL("OW1 C GROUNDS", LEVEL_CASTLE_GROUNDS, COURSE_NONE, castle_grounds, outside, 20000, 0x00, 0x00, 0x00, _, sCamCastleGrounds) 
DEFINE_LEVEL("OW2 IN CASTLE", LEVEL_CASTLE, COURSE_NONE, castle_inside, inside, 20000, 0x00, 0x00, 0x00, _, sCamCastle) 
DEFINE_LEVEL("OW4 STAR ROAD", LEVEL_SR, COURSE_SR, sr, generic, 20000, 0x00, 0x00, 0x00, _, sCamSr) 
STUB_LEVEL("", LEVEL_UNKNOWN_3, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_4, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_5, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_6, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_7, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_8, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_9, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_10, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_11, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_12, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_13, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_14, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_15, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_16, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_17, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_18, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_19, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_20, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_21, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_22, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_23, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_24, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_25, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_26, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_27, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_28, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_29, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_30, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_31, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_32, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_33, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_34, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_35, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_36, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_37, COURSE_NONE, 20000, 0x00, 0x00, 0x00, _, _) 
STUB_LEVEL("", LEVEL_UNKNOWN_38, COURSE_NONE, 20000, 0x00, 0x00, 0x00, sDynUnk38, _) 