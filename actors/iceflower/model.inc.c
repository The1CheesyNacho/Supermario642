
Gfx iceflower_ice_flower_rgba16_aligner[] = {gsSPEndDisplayList()};
u8 iceflower_ice_flower_rgba16[] = {
	#include "actors/iceflower/ice_flower.rgba16.inc.c"
};

Vtx iceflower_000_displaylist_mesh_layer_5_vtx_0[6] = {
	{{ {1, 61, 0}, 0, {496, -16}, {0, 0, 127, 255} }},
	{{ {-22, 61, 0}, 0, {1022, -16}, {0, 0, 127, 255} }},
	{{ {-22, 0, 0}, 0, {1022, 2032}, {0, 0, 127, 255} }},
	{{ {1, 0, 0}, 0, {496, 2032}, {0, 0, 127, 255} }},
	{{ {23, 61, 0}, 0, {-30, -16}, {0, 0, 127, 255} }},
	{{ {23, 0, 0}, 0, {-30, 2032}, {0, 0, 127, 255} }},
};

Gfx iceflower_000_displaylist_mesh_layer_5_tri_0[] = {
	gsSPVertex(iceflower_000_displaylist_mesh_layer_5_vtx_0 + 0, 6, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),
	gsSP1Triangle(3, 4, 0, 0),
	gsSP1Triangle(3, 5, 4, 0),
	gsSPEndDisplayList(),
};


Gfx mat_iceflower_f3dlite_material[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0),
	gsSPClearGeometryMode(G_CULL_BACK),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPLightColor(LIGHT_1, 0xffffffff),
    gsSPLightColor(LIGHT_2, 0x7f7f7fff),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, iceflower_ice_flower_rgba16),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 2047, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 6, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 252),
	gsSPEndDisplayList(),
};

Gfx mat_revert_iceflower_f3dlite_material[] = {
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_CULL_BACK),
	gsSPEndDisplayList(),
};

Gfx iceflower_000_displaylist_mesh_layer_5[] = {
	gsSPDisplayList(mat_iceflower_f3dlite_material),
	gsSPDisplayList(iceflower_000_displaylist_mesh_layer_5_tri_0),
	gsSPDisplayList(mat_revert_iceflower_f3dlite_material),
	gsSPEndDisplayList(),
};

Gfx iceflower_material_revert_render_settings[] = {
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsSPEndDisplayList(),
};

