#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "game/level_update.h"

#include "levels/scripts.h"

#include "actors/common1.h"

#include "make_const_nonconst.h"
#include "levels/jrb/header.h"

static const LevelScript script_func_local_1[] = {
    OBJECT_WITH_ACTS(/*model*/ MODEL_NONE,                     /*pos*/  5385, -5520,  2428, /*angle*/ 0,   0, 0, /*behParam*/ 0x00000000, /*beh*/ bhvInSunkenShip,           /*acts*/ ACT_1),
    OBJECT_WITH_ACTS(/*model*/ MODEL_NONE,                     /*pos*/  5385, -5520,  2428, /*angle*/ 0,   0, 0, /*behParam*/ 0x00000000, /*beh*/ bhvInSunkenShip2,          /*acts*/ ACT_1),
    OBJECT_WITH_ACTS(/*model*/ MODEL_UNAGI,                    /*pos*/  6048, -5381,  1154, /*angle*/ 0, 340, 0, /*behParam*/ 0x00000000, /*beh*/ bhvUnagi,                  /*acts*/ ACT_1),
    OBJECT_WITH_ACTS(/*model*/ MODEL_UNAGI,                    /*pos*/  8270, -3130,  1846, /*angle*/ 0, 285, 0, /*behParam*/ 0x01010000, /*beh*/ bhvUnagi,                  /*acts*/ ACT_2),
    OBJECT_WITH_ACTS(/*model*/ MODEL_UNAGI,                    /*pos*/  6048, -5381,  1154, /*angle*/ 0, 340, 0, /*behParam*/ 0x02020000, /*beh*/ bhvUnagi,                  /*acts*/ ACT_3 | ACT_4 | ACT_5 | ACT_6),
    OBJECT_WITH_ACTS(/*model*/ MODEL_NONE,                     /*pos*/  4988, -5221,  2473, /*angle*/ 0,   0, 0, /*behParam*/ 0x00000000, /*beh*/ bhvJetStream,              /*acts*/ ACT_2 | ACT_3 | ACT_4 | ACT_5 | ACT_6),
    OBJECT(          /*model*/ MODEL_NONE,                     /*pos*/ -1800, -2812, -2100, /*angle*/ 0,   0, 0, /*behParam*/ 0x02000000, /*beh*/ bhvTreasureChestsJrb),
    OBJECT_WITH_ACTS(/*model*/ MODEL_BOBOMB_BUDDY,             /*pos*/ -1956,  1331,  6500, /*angle*/ 0,   0, 0, /*behParam*/ 0x00000000, /*beh*/ bhvBobombBuddyOpensCannon, /*acts*/ ACT_2 | ACT_3 | ACT_4 | ACT_5 | ACT_6),
    RETURN(),
};

static const LevelScript script_func_local_2[] = {
    OBJECT(/*model*/ MODEL_NONE,                     /*pos*/    53,  2355,  2724, /*angle*/ 0,   0, 0, /*behParam*/ 0x00290000, /*beh*/ bhvPoleGrabbing),
    OBJECT(/*model*/ MODEL_NONE,                     /*pos*/   659,  2560,  3314, /*angle*/ 0,   0, 0, /*behParam*/ 0x00290000, /*beh*/ bhvPoleGrabbing),
    OBJECT(/*model*/ MODEL_NONE,                     /*pos*/  1087,  2150,  3798, /*angle*/ 0,   0, 0, /*behParam*/ 0x00290000, /*beh*/ bhvPoleGrabbing),
    OBJECT(/*model*/ MODEL_NONE,                     /*pos*/ -2535,  1075,  6113, /*angle*/ 0,   0, 0, /*behParam*/ 0x00610000, /*beh*/ bhvPoleGrabbing),
    RETURN(),
};

static const LevelScript script_func_local_3[] = {
    OBJECT_WITH_ACTS(/*model*/ MODEL_NONE,            /*pos*/ 4900,  2400,   800, /*angle*/ 0, 0, 0, /*behParam*/ 0x03000000, /*beh*/ bhvHiddenRedCoinStar, /*acts*/ ALL_ACTS),
#if defined(VERSION_JP) || defined(VERSION_SH)
    OBJECT_WITH_ACTS(/*model*/ MODEL_STAR,            /*pos*/ 1540,  2160,  2130, /*angle*/ 0, 0, 0, /*behParam*/ 0x04000000, /*beh*/ bhvStar,                 /*acts*/ ALL_ACTS),
#else
    OBJECT_WITH_ACTS(/*model*/ MODEL_EXCLAMATION_BOX, /*pos*/ 1540,  2160,  2130, /*angle*/ 0, 0, 0, /*behParam*/ 0x04080000, /*beh*/ bhvExclamationBox,      /*acts*/ ALL_ACTS),
#endif
    OBJECT_WITH_ACTS(/*model*/ MODEL_STAR,            /*pos*/ 5000, -4800,  2500, /*angle*/ 0, 0, 0, /*behParam*/ 0x05000000, /*beh*/ bhvStar,                 /*acts*/ ACT_2 | ACT_3 | ACT_4 | ACT_5 | ACT_6),
    RETURN(),
};

static const LevelScript script_func_local_4[] = {
    OBJECT(/*model*/ MODEL_NONE,            /*pos*/  400,  -350, -2700, /*angle*/ 0, 0, 0, /*behParam*/ 0x00000000, /*beh*/ bhvTreasureChestsShip),
    RETURN(),
};

static const LevelScript script_func_local_5[] = {
    RETURN(),
};

const LevelScript level_jrb_entry[] = {
    INIT_LEVEL(),
    LOAD_YAY0(        /*seg*/ 0x07, _jrb_segment_7SegmentRomStart, _jrb_segment_7SegmentRomEnd),
    LOAD_YAY0(        /*seg*/ 0x0B, _effect_yay0SegmentRomStart, _effect_yay0SegmentRomEnd),
    LOAD_YAY0_TEXTURE(/*seg*/ 0x09, _water_yay0SegmentRomStart, _water_yay0SegmentRomEnd),
    LOAD_YAY0(        /*seg*/ 0x0A, _clouds_skybox_yay0SegmentRomStart, _clouds_skybox_yay0SegmentRomEnd),
    LOAD_YAY0(        /*seg*/ 0x05, _group4_yay0SegmentRomStart, _group4_yay0SegmentRomEnd),
    LOAD_RAW(         /*seg*/ 0x0C, _group4_geoSegmentRomStart, _group4_geoSegmentRomEnd),
    LOAD_YAY0(        /*seg*/ 0x06, _group13_yay0SegmentRomStart, _group13_yay0SegmentRomEnd),
    LOAD_RAW(         /*seg*/ 0x0D, _group13_geoSegmentRomStart,  _group13_geoSegmentRomEnd),
    LOAD_YAY0(        /*seg*/ 0x08, _common0_yay0SegmentRomStart, _common0_yay0SegmentRomEnd),
    LOAD_RAW(         /*seg*/ 0x0F, _common0_geoSegmentRomStart,  _common0_geoSegmentRomEnd),
    ALLOC_LEVEL_POOL(),
    MARIO(/*model*/ MODEL_MARIO, /*behParam*/ 0x00000000, /*beh*/ bhvMario),
    LUIGI(/*model*/ MODEL_LUIGI, /*behParam*/ 0x00000001, /*beh*/ bhvLuigi),
    JUMP_LINK(script_func_global_1),
    JUMP_LINK(script_func_global_5),
    JUMP_LINK(script_func_global_14),

    AREA(/*index*/ 1, jrb_geo_000A18),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/ -6750, 2126, 1482, /*angle*/ 0, 90, 0, /*behParam*/ 0x000A0000, /*beh*/ bhvSpinAirborneWarp),
        WARP_NODE(/*id*/ 0x0A, /*destLevel*/ LEVEL_JRB, /*destArea*/ 0x01, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ 0xF3, /*destLevel*/ LEVEL_JRB, /*destArea*/ 0x02, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ 0xF0, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x35, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ 0xF1, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x67, /*flags*/ WARP_NO_CHECKPOINT),
        WHIRLPOOL(/*index*/ 0, /*acts*/ ACT_2 | ACT_3 | ACT_4 | ACT_5 | ACT_6, /*pos*/ 4979, -5222, 2482, /*strength*/ -30),
        JUMP_LINK(script_func_local_1),
        JUMP_LINK(script_func_local_2),
        JUMP_LINK(script_func_local_3),
        TERRAIN(/*terrainData*/ jrb_seg7_area_1_collision),
        MACRO_OBJECTS(/*objList*/ jrb_seg7_area_1_macro_objs),
        SET_BACKGROUND_MUSIC(/*settingsPreset*/ 0x0003, /*seq*/ SEQ_LEVEL_WATER),
        TERRAIN_TYPE(/*terrainType*/ TERRAIN_WATER),
    END_AREA(),

    AREA(/*index*/ 2, jrb_geo_000AFC),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/ 928, 1050, -1248, /*angle*/ 0, 180, 0, /*behParam*/ 0x000A0000, /*beh*/ bhvSwimmingWarp),
        WARP_NODE(/*id*/ 0x0A, /*destLevel*/ LEVEL_JRB, /*destArea*/ 0x02, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ 0xF0, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x35, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ 0xF1, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x01, /*destNode*/ 0x67, /*flags*/ WARP_NO_CHECKPOINT),
        JUMP_LINK(script_func_local_4),
        JUMP_LINK(script_func_local_5),
        TERRAIN(/*terrainData*/ jrb_seg7_area_2_collision),
        MACRO_OBJECTS(/*objList*/ jrb_seg7_area_2_macro_objs),
        SET_BACKGROUND_MUSIC(/*settingsPreset*/ 0x0003, /*seq*/ SEQ_LEVEL_WATER),
        TERRAIN_TYPE(/*terrainType*/ TERRAIN_WATER),
    END_AREA(),

    FREE_LEVEL_POOL(),
    MARIO_POS(/*area*/ 1, /*yaw*/ 90, /*pos*/ -6750, 1126, 1482),
    CALL(/*arg*/ 0, /*func*/ lvl_init_or_update),
    CALL_LOOP(/*arg*/ 1, /*func*/ lvl_init_or_update),
    CLEAR_LEVEL(),
    SLEEP_BEFORE_EXIT(/*frames*/ 1),
    EXIT(),
};
