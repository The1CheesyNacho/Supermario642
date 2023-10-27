#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "game/level_update.h"

#include "levels/scripts.h"


/* Fast64 begin persistent block [includes] */
/* Fast64 end persistent block [includes] */

#include "make_const_nonconst.h"
#include "levels/sr/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_sr_entry[] = {
	INIT_LEVEL(),
	LOAD_MIO0(0x7, _sr_segment_7SegmentRomStart, _sr_segment_7SegmentRomEnd), 
	LOAD_MIO0(0xa, _sr_skybox_mio0SegmentRomStart, _sr_skybox_mio0SegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 

	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, sr_area_1),
		WARP_NODE(0x0A, LEVEL_CASTLE_GROUNDS, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_THI_WARP_PIPE, -4015, 4653, -18563, 0, 0, 0, (0x0A << 16), bhvWarpPipe),
		OBJECT(MODEL_THI_WARP_PIPE, -3922, 4653, -13526, 0, 0, 0, (0x0A << 16), bhvWarpPipe),
		OBJECT(MODEL_THI_WARP_PIPE, 6017, 4653, -13517, 0, 0, 0, (0x0A << 16), bhvWarpPipe),
		OBJECT(MODEL_THI_WARP_PIPE, 5952, 4653, -18511, 0, 0, 0, (0x0A << 16), bhvWarpPipe),
		OBJECT(MODEL_THI_WARP_PIPE, 1011, 4653, -21260, 0, 0, 0, (0x0A << 16), bhvWarpPipe),
		OBJECT(MODEL_THI_WARP_PIPE, 1053, 7226, -16899, 0, 0, 0, (0x0A << 16), bhvWarpPipe),
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
	MARIO_POS(0x01, 0, -1446, 4777, -14549),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};
