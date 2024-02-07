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
#include "levels/tsns/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_tsns_entry[] = {
	INIT_LEVEL(),
	LOAD_MIO0(0x7, _tsns_segment_7SegmentRomStart, _tsns_segment_7SegmentRomEnd), 
	LOAD_MIO0(0xa, _bits_skybox_mio0SegmentRomStart, _bits_skybox_mio0SegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000000, bhvMario), 

	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, tsns_area_1),
		WARP_NODE( 0xF1, LEVEL_CASTLE,   0x02,   0x6C, WARP_NO_CHECKPOINT),
		WARP_NODE( 0xF0, LEVEL_CASTLE,   0x02,   0x3A, WARP_NO_CHECKPOINT),
		WARP_NODE( 0x0C, LEVEL_RR,   0x01,   0x0B, WARP_NO_CHECKPOINT),
		WARP_NODE( 0x0B, LEVEL_RR,   0x01,   0x0C, WARP_NO_CHECKPOINT),
		WARP_NODE( 0x0A, LEVEL_RR,   0x01,   0x0A, WARP_NO_CHECKPOINT),
		MARIO_POS(0x01, 0, -2097, 504, 5179),
		TERRAIN(tsns_area_1_collision),
		MACRO_OBJECTS(tsns_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_EVENT_PIRANHA_PLANT),
		TERRAIN_TYPE(TERRAIN_STONE),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),

	FREE_LEVEL_POOL(),
	MARIO_POS(0x01, 0, -2097, 504, 5179),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};
