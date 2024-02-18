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
#include "levels/rr/header.h"

static const LevelScript script_func_local_1[] = {
    OBJECT(/*model*/ MODEL_NONE,                        /*pos*/     0,     0,     0, /*angle*/  0,   0,   0, /*behParam*/ 0x00000000, /*beh*/ bhvDonutPlatformSpawner),
    OBJECT(/*model*/ MODEL_PURPLE_SWITCH,               /*pos*/  4428, -1936,  6564, /*angle*/  0,   0,   0, /*behParam*/ 0x00000000, /*beh*/ bhvFloorSwitchAnimatesObject),
    OBJECT(/*model*/ MODEL_NONE,                        /*pos*/   614, -2857,  3671, /*angle*/  0,   0,   0, /*behParam*/ 0x00CC0000, /*beh*/ bhvPoleGrabbing),
    OBJECT(/*model*/ MODEL_NONE,                        /*pos*/   621, -4598,  7362, /*angle*/  0,   0,   0, /*behParam*/ 0x00750000, /*beh*/ bhvPoleGrabbing),
    OBJECT(/*model*/ MODEL_NONE,                        /*pos*/  5119,  3819,  3325, /*angle*/  0,   0,   0, /*behParam*/ 0x00610000, /*beh*/ bhvPoleGrabbing),
    OBJECT(/*model*/ MODEL_NONE,                        /*pos*/  3554,  2891, -2327, /*angle*/  0,   0,   0, /*behParam*/ 0x00C10000, /*beh*/ bhvPoleGrabbing),
    OBJECT(/*model*/ MODEL_NONE,                        /*pos*/  2680,   214,   295, /*angle*/  0,   0,   0, /*behParam*/ 0x00620000, /*beh*/ bhvPoleGrabbing),
    OBJECT(/*model*/ MODEL_NONE,                        /*pos*/  3811,  1033,   295, /*angle*/  0,   0,   0, /*behParam*/ 0x00620000, /*beh*/ bhvPoleGrabbing),
    RETURN(),
};

static const LevelScript script_func_local_2[] = {
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -5809, -1834,  5719, /*angle*/ 0, 0, 0, /*behParam*/ 0x00000000, /*beh*/ bhvFlamethrower),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -4838, -1015,  4081, /*angle*/ 0, 0, 0, /*behParam*/ 0x00000000, /*beh*/ bhvFlamethrower),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/  3301, -1834,  5617, /*angle*/ 0, 0, 0, /*behParam*/ 0x00000000, /*beh*/ bhvFlamethrower),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/  6772,  -757,  -606, /*angle*/ 0, 0, 0, /*behParam*/ 0x00000000, /*beh*/ bhvFlamethrower),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -4187,  3213, -6630, /*angle*/ 0, 0, 0, /*behParam*/ 0x00000000, /*beh*/ bhvFlamethrower),
    RETURN(),
};

static const LevelScript script_func_local_3[] = {
    OBJECT_WITH_ACTS(/*model*/ MODEL_STAR, /*pos*/  1450,  3400, -2352, /*angle*/ 0, 0, 0, /*behParam*/ 0x00000000, /*beh*/ bhvStar,                 /*acts*/ ALL_ACTS),
    OBJECT_WITH_ACTS(/*model*/ MODEL_STAR, /*pos*/ -4200,  6700, -4450, /*angle*/ 0, 0, 0, /*behParam*/ 0x01000000, /*beh*/ bhvStar,                 /*acts*/ ALL_ACTS),
    OBJECT_WITH_ACTS(/*model*/ MODEL_NONE, /*pos*/ -5150, -1400,     0, /*angle*/ 0, 0, 0, /*behParam*/ 0x02000000, /*beh*/ bhvHiddenRedCoinStar, /*acts*/ ALL_ACTS),
    OBJECT_WITH_ACTS(/*model*/ MODEL_STAR, /*pos*/ -5850,  -700,  4950, /*angle*/ 0, 0, 0, /*behParam*/ 0x03000000, /*beh*/ bhvStar,                 /*acts*/ ALL_ACTS),
    OBJECT_WITH_ACTS(/*model*/ MODEL_STAR, /*pos*/  3700,  -400,  6600, /*angle*/ 0, 0, 0, /*behParam*/ 0x04000000, /*beh*/ bhvStar,                 /*acts*/ ALL_ACTS),
    RETURN(),
};

const LevelScript level_rr_entry[] = {
    INIT_LEVEL(),
    LOAD_YAY0(        /*seg*/ 0x07, _rr_segment_7SegmentRomStart, _rr_segment_7SegmentRomEnd),
    LOAD_YAY0_TEXTURE(/*seg*/ 0x09, _sky_yay0SegmentRomStart, _sky_yay0SegmentRomEnd),
    LOAD_YAY0(        /*seg*/ 0x0A, _cloud_floor_skybox_yay0SegmentRomStart, _cloud_floor_skybox_yay0SegmentRomEnd),
    LOAD_YAY0(        /*seg*/ 0x05, _group11_yay0SegmentRomStart, _group11_yay0SegmentRomEnd),
    LOAD_RAW(         /*seg*/ 0x0C, _group11_geoSegmentRomStart,  _group11_geoSegmentRomEnd),
    LOAD_YAY0(        /*seg*/ 0x08, _common0_yay0SegmentRomStart, _common0_yay0SegmentRomEnd),
    LOAD_RAW(         /*seg*/ 0x0F, _common0_geoSegmentRomStart, _common0_geoSegmentRomEnd),
    ALLOC_LEVEL_POOL(),
    MARIO(/*model*/ MODEL_MARIO, /*behParam*/ 0x00000000, /*beh*/ bhvMario),
    LUIGI(/*model*/ MODEL_LUIGI, /*behParam*/ 0x00000001, /*beh*/ bhvLuigi),
    JUMP_LINK(script_func_global_12),
    JUMP_LINK(script_func_global_1),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_03,           rr_geo_000660),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_04,           rr_geo_000678),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_05,           rr_geo_000690),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_06,           rr_geo_0006A8),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_07,           rr_geo_0006C0),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_08,           rr_geo_0006D8),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_09,           rr_geo_0006F0),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0A,           rr_geo_000708),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0B,           rr_geo_000720),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0C,           rr_geo_000738),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0D,           rr_geo_000758),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0E,           rr_geo_000770),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_0F,           rr_geo_000788),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_10,           rr_geo_0007A0),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_11,           rr_geo_0007B8),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_12,           rr_geo_0007D0),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_13,           rr_geo_0007E8),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_14,           rr_geo_000800),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_15,           rr_geo_000818),
    LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_16,           rr_geo_000830),

    AREA(/*index*/ 1, rr_geo_0009D0),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/ 2599, -833, 2071, /*angle*/ 0, 90, 0, /*behParam*/ 0x000A0000, /*beh*/ bhvSpinAirborneWarp),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/ -7092, 2364, -63, /*angle*/ 0, 90, 0, /*behParam*/ 0x000B0000, /*beh*/ bhvFadingWarp),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/ -4213, 3379, -2815, /*angle*/ 0, 180, 0, /*behParam*/ 0x000C0000, /*beh*/ bhvFadingWarp),
        WARP_NODE(/*id*/ 0x0A, /*destLevel*/ LEVEL_RR, /*destArea*/ 0x01, /*destNode*/ 0x0A, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ 0x0B, /*destLevel*/ LEVEL_RR, /*destArea*/ 0x01, /*destNode*/ 0x0C, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ 0x0C, /*destLevel*/ LEVEL_RR, /*destArea*/ 0x01, /*destNode*/ 0x0B, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ 0xF0, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x02, /*destNode*/ 0x3A, /*flags*/ WARP_NO_CHECKPOINT),
        WARP_NODE(/*id*/ 0xF1, /*destLevel*/ LEVEL_CASTLE, /*destArea*/ 0x02, /*destNode*/ 0x6C, /*flags*/ WARP_NO_CHECKPOINT),
        JUMP_LINK(script_func_local_1),
        JUMP_LINK(script_func_local_2),
        JUMP_LINK(script_func_local_3),
        TERRAIN(/*terrainData*/ rr_seg7_collision_level),
        MACRO_OBJECTS(/*objList*/ rr_seg7_macro_objs),
        SET_BACKGROUND_MUSIC(/*settingsPreset*/ 0x0000, /*seq*/ SEQ_LEVEL_SLIDE),
        TERRAIN_TYPE(/*terrainType*/ TERRAIN_STONE),
    END_AREA(),

    FREE_LEVEL_POOL(),
    MARIO_POS(/*area*/ 1, /*yaw*/ 90, /*pos*/ 2599, -1833, 2071),
    CALL(/*arg*/ 0, /*func*/ lvl_init_or_update),
    CALL_LOOP(/*arg*/ 1, /*func*/ lvl_init_or_update),
    CLEAR_LEVEL(),
    SLEEP_BEFORE_EXIT(/*frames*/ 1),
    EXIT(),
};
