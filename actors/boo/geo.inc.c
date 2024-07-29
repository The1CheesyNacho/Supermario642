#include "src/game/envfx_snow.h"

const GeoLayout boo_armature_002_switch_option_002[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_SWITCH_CASE(0, geo_switch_mario_eyes),
		GEO_OPEN_NODE(),
			GEO_NODE_START(),
			GEO_OPEN_NODE(),
				GEO_DISPLAY_LIST(LAYER_OPAQUE, boo_002_switch_option_head__no_cap__mesh_layer_1),
				GEO_DISPLAY_LIST(LAYER_TRANSPARENT, boo_002_switch_option_head__no_cap__mesh_layer_5),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
	GEO_CLOSE_NODE(),
	GEO_RETURN(),
};
const GeoLayout boo_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_SHADOW(0, 150, 70),
		GEO_OPEN_NODE(),
			GEO_SCALE(LAYER_FORCE, 26214),
			GEO_OPEN_NODE(),
				GEO_ASM(0, geo_update_layer_transparency),
				GEO_SWITCH_CASE(2, geo_switch_anim_state),
				GEO_OPEN_NODE(),
					GEO_NODE_START(),
					GEO_OPEN_NODE(),
						GEO_DISPLAY_LIST(LAYER_OPAQUE, boo_000_displaylist_mesh_layer_1),
					GEO_CLOSE_NODE(),
					GEO_BRANCH(1, boo_armature_002_switch_option_002),
				GEO_CLOSE_NODE(),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, boo_material_revert_render_settings),
		GEO_DISPLAY_LIST(LAYER_TRANSPARENT, boo_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
