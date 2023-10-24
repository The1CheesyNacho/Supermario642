#include "src/game/envfx_snow.h"

const GeoLayout motos_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_SHADOW(1, 255, 100),
		GEO_OPEN_NODE(),
			GEO_SCALE(LAYER_OPAQUE, 16384),
			GEO_OPEN_NODE(),
				GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, NULL),
				GEO_OPEN_NODE(),
					GEO_BILLBOARD_WITH_PARAMS_AND_DL(LAYER_ALPHA, 0, 129, 0, motos_pelvis_mesh_layer_4),
					GEO_OPEN_NODE(),
						GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, NULL),
						GEO_OPEN_NODE(),
							GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, 34, -89, 6, 0, -90, -180),
							GEO_OPEN_NODE(),
								GEO_ANIMATED_PART(LAYER_ALPHA, 0, 0, 0, motos_leg_L_mesh_layer_4),
								GEO_OPEN_NODE(),
									GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 21, 0, motos_foot_L_mesh_layer_1),
								GEO_CLOSE_NODE(),
							GEO_CLOSE_NODE(),
						GEO_CLOSE_NODE(),
						GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, NULL),
						GEO_OPEN_NODE(),
							GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, -34, -89, 6, 0, 90, 180),
							GEO_OPEN_NODE(),
								GEO_ANIMATED_PART(LAYER_ALPHA, 0, 0, 0, motos_leg_R_mesh_layer_4),
								GEO_OPEN_NODE(),
									GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 21, 0, motos_foot_R_mesh_layer_1),
								GEO_CLOSE_NODE(),
							GEO_CLOSE_NODE(),
						GEO_CLOSE_NODE(),
						GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, NULL),
						GEO_OPEN_NODE(),
							GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, 76, 24, 55, 90, 0, 0),
							GEO_OPEN_NODE(),
								GEO_ANIMATED_PART(LAYER_ALPHA, 0, 0, 0, motos_arm_L_mesh_layer_4),
								GEO_OPEN_NODE(),
									GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, motos_hand_bottom_L_mesh_layer_1),
									GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, motos_hand_top_L_mesh_layer_1),
									GEO_OPEN_NODE(),
										GEO_ASM(0, geo_update_held_mario_pos),
									GEO_CLOSE_NODE(),
								GEO_CLOSE_NODE(),
							GEO_CLOSE_NODE(),
						GEO_CLOSE_NODE(),
						GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, NULL),
						GEO_OPEN_NODE(),
							GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, -76, 24, 55, 90, 0, 0),
							GEO_OPEN_NODE(),
								GEO_ANIMATED_PART(LAYER_ALPHA, 0, 0, 0, motos_arm_R_mesh_layer_4),
								GEO_OPEN_NODE(),
									GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, motos_hand_bottom_R_mesh_layer_1),
									GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, motos_hand_top_R_mesh_layer_1),
								GEO_CLOSE_NODE(),
							GEO_CLOSE_NODE(),
						GEO_CLOSE_NODE(),
						GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 77, 0, motos_head_mesh_layer_1),
					GEO_CLOSE_NODE(),
				GEO_CLOSE_NODE(),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, motos_material_revert_render_settings),
		GEO_DISPLAY_LIST(LAYER_ALPHA, motos_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
