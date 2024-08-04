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
#include "levels/course0/header.h"
static const LevelScript script_func_local_2[] = {
    OBJECT(/*model*/ MODEL_NONE, /*pos*/     0,   900, -1710, /*angle*/ 0, 180, 0, /*behParam*/ 0x00030000, /*beh*/ bhvDeathWarp),
    OBJECT(/*model*/ MODEL_NONE, /*pos*/ -1328,   260,  4664, /*angle*/ 0, 180, 0, /*behParam*/ 0x00040000, /*beh*/ bhvSpinAirborneCircleWarp),
	OBJECT_WITH_ACTS(/*model*/ MODEL_GUMMY,    /*pos*/     -2500, 0, -1500, /*angle*/ 0, 0, 0, /*behParam*/ 0x00000000, /*beh*/ bhvGoomba,          /*acts*/ ALL_ACTS),
	OBJECT_WITH_ACTS(/*model*/ MODEL_GUMMY,    /*pos*/     0, 0, -2000, /*angle*/ 0, 0, 0, /*behParam*/ 0x00000000, /*beh*/ bhvGoomba,          /*acts*/ ALL_ACTS),
	OBJECT_WITH_ACTS(/*model*/ MODEL_GUMMY,    /*pos*/     -2500, 0, 1000, /*angle*/ 0, 0, 0, /*behParam*/ 0x00000000, /*beh*/ bhvGoomba,          /*acts*/ ALL_ACTS), 
    RETURN(),
};

const LevelScript level_course0_entry[] = {
    INIT_LEVEL(),
    LOAD_MIO0(        /*seg*/ 0x07, _course0_segment_7SegmentRomStart, _course0_segment_7SegmentRomEnd),
    LOAD_MIO0(        /*seg*/ 0x08, _common0_mio0SegmentRomStart, _common0_mio0SegmentRomEnd),
    LOAD_RAW(         /*seg*/ 0x0F, _common0_geoSegmentRomStart,  _common0_geoSegmentRomEnd),
    ALLOC_LEVEL_POOL(),
		MARIO(MODEL_MARIO, 0x00000000, bhvMario),
		LUIGI(MODEL_LUIGI, 0x00000001, bhvLuigi),

    AREA(/*index*/ 1, RCP_Stage35Scene1),
    WARP_NODE(/*id*/ 0x00, /*destLevel*/ LEVEL_COURSE1,         /*destArea*/ 1, /*destNode*/ 0x00, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x01, /*destLevel*/ LEVEL_COURSE1,         /*destArea*/ 1, /*destNode*/ 0x01, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0x02, /*destLevel*/ LEVEL_COURSE1,         /*destArea*/ 3, /*destNode*/ 0x02, /*flags*/ WARP_NO_CHECKPOINT),
    WARP_NODE(/*id*/ 0xF1, /*destLevel*/ LEVEL_COURSE1, /*destArea*/ 1, /*destNode*/ 0x03, /*flags*/ WARP_NO_CHECKPOINT),
        TERRAIN(/*terrainData*/ course0_area_1_collision),
 //       SET_BACKGROUND_MUSIC(/*settingsPreset*/ 0x0002, /*seq*/ SEQ_LEVEL_SLIDE), 
    TERRAIN_TYPE(/*terrainType*/ TERRAIN_STONE),
    JUMP_LINK(script_func_global_1),
	        JUMP_LINK(script_func_local_2),
    END_AREA(),

    FREE_LEVEL_POOL(),
    MARIO_POS(/*area*/ 1, /*yaw*/ 0, /*pos*/ 0, 0, 0),
    CALL(/*arg*/ 0, /*func*/ lvl_init_or_update),
    CALL_LOOP(/*arg*/ 1, /*func*/ lvl_init_or_update),
    CLEAR_LEVEL(),
    SLEEP_BEFORE_EXIT(/*frames*/ 1),
    EXIT(),
};
