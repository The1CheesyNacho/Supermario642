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
#include "levels/sr/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_sr_entry[] = {
	INIT_LEVEL(),
	LOAD_MIO0(0x7, _sr_segment_7SegmentRomStart, _sr_segment_7SegmentRomEnd), 
	LOAD_MIO0(0xa, _sr_skybox_mio0SegmentRomStart, _sr_skybox_mio0SegmentRomEnd), 
    LOAD_YAY0(        /*seg*/ 0x05, _group11_yay0SegmentRomStart, _group11_yay0SegmentRomEnd),
    LOAD_RAW(         /*seg*/ 0x0C, _group11_geoSegmentRomStart,  _group11_geoSegmentRomEnd),
    LOAD_YAY0(        /*seg*/ 0x06, _group14_yay0SegmentRomStart, _group14_yay0SegmentRomEnd),
    LOAD_RAW(         /*seg*/ 0x0D, _group14_geoSegmentRomStart,  _group14_geoSegmentRomEnd),
    LOAD_YAY0(        /*seg*/ 0x08, _common0_yay0SegmentRomStart, _common0_yay0SegmentRomEnd),
    LOAD_RAW(         /*seg*/ 0x0F, _common0_geoSegmentRomStart,  _common0_geoSegmentRomEnd),
	ALLOC_LEVEL_POOL(),
    JUMP_LINK(script_func_global_1),
    JUMP_LINK(script_func_global_12),
    JUMP_LINK(script_func_global_15),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
    LOAD_MODEL_FROM_GEO(MODEL_THI_WARP_PIPE,       warp_pipe_geo),

	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, sr_area_1),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_THI_WARP_PIPE, -4015, 4653, -18563, 0, 0, 0, (0x0A << 16), bhvWarpPipe),
		OBJECT(MODEL_THI_WARP_PIPE, -3922, 4653, -13526, 0, 0, 0, (0x0A << 16), bhvWarpPipe),
		OBJECT(MODEL_THI_WARP_PIPE, 6017, 4653, -13517, 0, 0, 0, (0x0A << 16), bhvWarpPipe),
		OBJECT(MODEL_THI_WARP_PIPE, 5952, 4653, -18511, 0, 0, 0, (0x0A << 16), bhvWarpPipe),
		OBJECT(MODEL_THI_WARP_PIPE, 1011, 4653, -21260, 0, 0, 0, (0x0A << 16), bhvWarpPipe),
		OBJECT(MODEL_NONE, 1202, 368, -17421, 0, 0, 180, 0x000A0000, bhvWarp),
		OBJECT(MODEL_NONE, -1446, 4777, -14549, 0, 0, 0, 0x000A0000, bhvSpinAirborneWarp),
		MARIO_POS(0x01, 0, -1446, 4777, -14549),
		TERRAIN(sr_area_1_collision),
		MACRO_OBJECTS(sr_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_LEVEL_WATER),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),

	FREE_LEVEL_POOL(),
	MARIO_POS(0x01, 0, 1045, 7212, -16884),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};
