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

#include "actors/common1.h"

/* Fast64 begin persistent block [includes] */
/* Fast64 end persistent block [includes] */

#include "make_const_nonconst.h"
#include "levels/castle_grounds/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_castle_grounds_entry[] = {
	INIT_LEVEL(),
	LOAD_YAY0(0x07, _castle_grounds_segment_7SegmentRomStart, _castle_grounds_segment_7SegmentRomEnd), 
	LOAD_YAY0(0x0A, _water_skybox_yay0SegmentRomStart, _water_skybox_yay0SegmentRomEnd), 
	LOAD_YAY0_TEXTURE(0x09, _outside_yay0SegmentRomStart, _outside_yay0SegmentRomEnd), 
	LOAD_YAY0(0x05, _group10_yay0SegmentRomStart, _group10_yay0SegmentRomEnd), 
	LOAD_RAW(0x0C, _group1_geoSegmentRomStart, _group1_geoSegmentRomEnd), 
	LOAD_YAY0(0x06, _group15_yay0SegmentRomStart, _group15_yay0SegmentRomEnd), 
	LOAD_RAW(0x0D, _group15_geoSegmentRomStart, _group15_geoSegmentRomEnd), 
	LOAD_YAY0(0x08, _common0_yay0SegmentRomStart, _common0_yay0SegmentRomEnd), 
	LOAD_RAW(0x0F, _common0_geoSegmentRomStart, _common0_geoSegmentRomEnd), 
	LOAD_MIO0(0x7, _castle_grounds_segment_7SegmentRomStart, _castle_grounds_segment_7SegmentRomEnd), 
	LOAD_MIO0(0xa, _water_skybox_mio0SegmentRomStart, _water_skybox_mio0SegmentRomEnd), 
	LOAD_MIO0(0x5, _group1_mio0SegmentRomStart, _group1_mio0SegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	JUMP_LINK(script_func_global_1), 
	JUMP_LINK(script_func_global_2), 
	JUMP_LINK(script_func_global_16), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_03, castle_grounds_geo_0006F4), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_BUBBLY_TREE, bubbly_tree_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_WARP_PIPE, warp_pipe_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_CASTLE_DOOR, castle_door_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_METAL_DOOR, metal_door_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_VCUTM_GRILL, castle_grounds_geo_00070C), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_FLAG, castle_grounds_geo_000660), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_CANNON_GRILL, castle_grounds_geo_000724), 

	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, castle_grounds_area_1),
		WARP_NODE(0x00, LEVEL_CASTLE, 0x01, 0x00, WARP_NO_CHECKPOINT),
		WARP_NODE(0x01, LEVEL_CASTLE, 0x01, 0x01, WARP_NO_CHECKPOINT),
		WARP_NODE(0x02, LEVEL_CASTLE, 0x01, 0x02, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_CASTLE_GROUNDS_CASTLE_DOOR, -1250, 1656, -2919, 0, 180, 0,   0x00010000, bhvDoorWarp),
		OBJECT(MODEL_CASTLE_GROUNDS_CASTLE_DOOR, -1404, 1656, -2919, 0, 0, 0,   0x00000000, bhvDoorWarp),
		OBJECT(MODEL_NONE, -3764, 851, 11447, 0, 0, 0, 0x000A0000, bhvSpinAirborneWarp),
		MARIO_POS(0x01, 0, -2800, 851, 7655),
		OBJECT(MODEL_TOAD, -4580, 734, 6980, 0, 133, 0, 0x000A0000, bhvToadMessage),
		OBJECT(MODEL_TOAD, -5296, 734, 5340, 0, 95, 0, 0x000A0000, bhvToadMessage),
		OBJECT(MODEL_TOAD, -5168, 734, 3543, 0, 56, 0, 0x000A0000, bhvToadMessage),
		OBJECT(MODEL_TOAD, -4287, 734, 2527, 0, 30, 0, 0x000A0000, bhvToadMessage),
		OBJECT(MODEL_THWIMP, -2862, 734, 2271, 0, 10, 0, 0x000A0000, bhvThwimp),
		OBJECT(MODEL_TOAD, 2317, -23, 7948, 0, 129, 0, 0x000A0000, bhvToadMessage),
		OBJECT(MODEL_CASTLE_GROUNDS_BUBBLY_TREE, 633, 305, 3556, 0, 10, 0, 0x000A0000, bhvTree),
		OBJECT(MODEL_CASTLE_GROUNDS_BUBBLY_TREE, -696, 507, 3556, 0, 10, 0, 0x000A0000, bhvTree),
		OBJECT(MODEL_CASTLE_GROUNDS_BUBBLY_TREE, 914, 488, 3037, 0, 10, 0, 0x000A0000, bhvTree),
		OBJECT(MODEL_CASTLE_GROUNDS_BUBBLY_TREE, -33, 734, 1583, 0, 10, 0, 0x000A0000, bhvTree),
		OBJECT(MODEL_CASTLE_GROUNDS_BUBBLY_TREE, 1051, 734, 1105, 0, 10, 0, 0x000A0000, bhvTree),
		OBJECT(MODEL_CASTLE_GROUNDS_BUBBLY_TREE, 1275, 734, -809, 0, 10, 0, 0x000A0000, bhvTree),
		OBJECT(MODEL_CASTLE_GROUNDS_BUBBLY_TREE, -5817, 734, 89, 0, 10, 0, 0x000A0000, bhvTree),
		OBJECT(MODEL_CASTLE_GROUNDS_BUBBLY_TREE, -3764, 1227, -500, 0, 10, 0, 0x000A0000, bhvTree),
		OBJECT(MODEL_CASTLE_GROUNDS_BUBBLY_TREE, -2935, 1227, -1010, 0, 10, 0, 0x000A0000, bhvTree),
		OBJECT(MODEL_CASTLE_GROUNDS_BUBBLY_TREE, 6909, 1014, 6757, 0, 10, 0, 0x000A0000, bhvTree),
		OBJECT(MODEL_STAR, 6957, 1097, 6868, 0, 10, 0, 0x000A0000, bhvStar),
		OBJECT(MODEL_TOAD, 9317, 733, 4736, 0, 10, 0, 0x000A0000, bhvToadMessage),
		OBJECT(MODEL_CASTLE_GROUNDS_WARP_PIPE, 5034, 740, 2783, 0, 0, 0, 0x000A0000, bhvWarpPipe),
		TERRAIN(castle_grounds_area_1_collision),
		MACRO_OBJECTS(castle_grounds_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_MOLD),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),

	FREE_LEVEL_POOL(),
	MARIO_POS(0x01, 0, -2800, 851, 7655),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};
