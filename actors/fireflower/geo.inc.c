#include "src/game/envfx_snow.h"

const GeoLayout fireflower_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_SHADOW(0, 180, 80),
		GEO_OPEN_NODE(),
			GEO_DISPLAY_LIST(LAYER_TRANSPARENT, fireflower_000_displaylist_mesh_layer_5),
		GEO_CLOSE_NODE(),
		GEO_DISPLAY_LIST(LAYER_TRANSPARENT, fireflower_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
