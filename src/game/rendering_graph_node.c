#include <PR/ultratypes.h>

#include "area.h"
#include "engine/math_util.h"
#include "game_init.h"
#include "gfx_dimensions.h"
#include "main.h"
#include "memory.h"
#include "print.h"
#include "rendering_graph_node.h"
#include "shadow.h"
#include "sm64.h"
#include "game_init.h"
#include "puppyprint.h"
#include "debug_box.h"
#include "level_update.h"
#include "behavior_data.h"
#include "string.h"
#include "color_presets.h"
#include "emutest.h"
#include "lantern_engine.h"

#include "config.h"
#include "config/config_world.h"

/**
 * This file contains the code that processes the scene graph for rendering.
 * The scene graph is responsible for drawing everything except the HUD / text boxes.
 * First the root of the scene graph is processed when geo_process_root
 * is called from level_script.c. The rest of the tree is traversed recursively
 * using the function geo_process_node_and_siblings, which switches over all
 * geo node types and calls a specialized function accordingly.
 * The types are defined in engine/graph_node.h
 *
 * The scene graph typically looks like:
 * - Root (viewport)
 *  - Master list
 *   - Ortho projection
 *    - Background (skybox)
 *  - Master list
 *   - Perspective
 *    - Camera
 *     - <area-specific display lists>
 *     - Object parent
 *      - <group with 240 object nodes>
 *  - Master list
 *   - Script node (Cannon overlay)
 *
 */

s16 gMatStackIndex = 0;
ALIGNED16 Mat4 gMatStack[32];
ALIGNED16 Mtx *gMatStackFixed[32];
f32 sAspectRatio;
Mat4 gCameraTransform;

/**
 * Animation nodes have state in global variables, so this struct captures
 * the animation state so a 'context switch' can be made when rendering the
 * held object.
 */
struct GeoAnimState {
    /*0x00*/ u8 type;
    /*0x01*/ u8 enabled;
    /*0x02*/ s16 frame;
    /*0x04*/ f32 translationMultiplier;
    /*0x08*/ u16 *attribute;
    /*0x0C*/ s16 *data;
};

// For some reason, this is a GeoAnimState struct, but the current state consists
// of separate global variables. It won't match EU otherwise.
struct GeoAnimState gGeoTempState;

u8 gCurrAnimType;
u8 gCurrAnimEnabled;
s16 gCurrAnimFrame;
f32 gCurrAnimTranslationMultiplier;
u16 *gCurrAnimAttribute;
s16 *gCurrAnimData;

struct AllocOnlyPool *gDisplayListHeap;

/* Rendermode settings for cycle 1 for all 8 or 13 layers. */
struct RenderModeContainer renderModeTable_1Cycle[2] = { 
    [RENDER_NO_ZB] = { {
        [LAYER_FORCE] = G_RM_OPA_SURF,
        [LAYER_OPAQUE] = G_RM_AA_OPA_SURF,
        [LAYER_OPAQUE_INTER] = G_RM_AA_OPA_SURF,
        [LAYER_OPAQUE_DECAL] = G_RM_AA_OPA_SURF,
        [LAYER_ALPHA] = G_RM_AA_TEX_EDGE,
#if SILHOUETTE
        [LAYER_ALPHA_DECAL] = G_RM_AA_TEX_EDGE | ZMODE_DEC,
        [LAYER_SILHOUETTE_OPAQUE] = G_RM_AA_OPA_SURF,
        [LAYER_SILHOUETTE_ALPHA] = G_RM_AA_TEX_EDGE,
        [LAYER_OCCLUDE_SILHOUETTE_OPAQUE] = G_RM_AA_OPA_SURF,
        [LAYER_OCCLUDE_SILHOUETTE_ALPHA] = G_RM_AA_TEX_EDGE,
#endif
        [LAYER_TRANSPARENT_DECAL] = G_RM_AA_XLU_SURF,
        [LAYER_TRANSPARENT] = G_RM_AA_XLU_SURF,
        [LAYER_TRANSPARENT_INTER] = G_RM_AA_XLU_SURF,
    } },
    [RENDER_ZB] = { {
        [LAYER_FORCE] = G_RM_ZB_OPA_SURF,
        [LAYER_OPAQUE] = G_RM_AA_ZB_OPA_SURF,
        [LAYER_OPAQUE_INTER] = G_RM_AA_ZB_OPA_INTER,
        [LAYER_OPAQUE_DECAL] = G_RM_AA_ZB_OPA_DECAL,
        [LAYER_ALPHA] = G_RM_AA_ZB_TEX_EDGE,
#if SILHOUETTE
        [LAYER_ALPHA_DECAL] = G_RM_AA_ZB_TEX_EDGE | ZMODE_DEC,
        [LAYER_SILHOUETTE_OPAQUE] = G_RM_AA_ZB_OPA_SURF,
        [LAYER_SILHOUETTE_ALPHA] = G_RM_AA_ZB_TEX_EDGE,
        [LAYER_OCCLUDE_SILHOUETTE_OPAQUE] = G_RM_AA_ZB_OPA_SURF,
        [LAYER_OCCLUDE_SILHOUETTE_ALPHA] = G_RM_AA_ZB_TEX_EDGE,
#endif
        [LAYER_TRANSPARENT_DECAL] = G_RM_AA_ZB_XLU_DECAL,
        [LAYER_TRANSPARENT] = G_RM_AA_ZB_XLU_SURF,
        [LAYER_TRANSPARENT_INTER] = G_RM_AA_ZB_XLU_INTER,
    } } };

/* Rendermode settings for cycle 2 for all 13 layers. */
struct RenderModeContainer renderModeTable_2Cycle[2] = {
    [RENDER_NO_ZB] = { {
        [LAYER_FORCE] = G_RM_OPA_SURF2,
        [LAYER_OPAQUE] = G_RM_AA_OPA_SURF2,
        [LAYER_OPAQUE_INTER] = G_RM_AA_OPA_SURF2,
        [LAYER_OPAQUE_DECAL] = G_RM_AA_OPA_SURF2,
        [LAYER_ALPHA] = G_RM_AA_TEX_EDGE2,
#if SILHOUETTE
        [LAYER_ALPHA_DECAL] = G_RM_AA_TEX_EDGE2 | ZMODE_DEC,
        [LAYER_SILHOUETTE_OPAQUE] = G_RM_AA_OPA_SURF2,
        [LAYER_SILHOUETTE_ALPHA] = G_RM_AA_TEX_EDGE2,
        [LAYER_OCCLUDE_SILHOUETTE_OPAQUE] = G_RM_AA_OPA_SURF2,
        [LAYER_OCCLUDE_SILHOUETTE_ALPHA] = G_RM_AA_TEX_EDGE2,
#endif
        [LAYER_TRANSPARENT_DECAL] = G_RM_AA_XLU_SURF2,
        [LAYER_TRANSPARENT] = G_RM_AA_XLU_SURF2,
        [LAYER_TRANSPARENT_INTER] = G_RM_AA_XLU_SURF2,
    } },
    [RENDER_ZB] = { {
        [LAYER_FORCE] = G_RM_ZB_OPA_SURF2,
        [LAYER_OPAQUE] = G_RM_AA_ZB_OPA_SURF2,
        [LAYER_OPAQUE_INTER] = G_RM_AA_ZB_OPA_INTER2,
        [LAYER_OPAQUE_DECAL] = G_RM_AA_ZB_OPA_DECAL2,
        [LAYER_ALPHA] = G_RM_AA_ZB_TEX_EDGE2,
#if SILHOUETTE
        [LAYER_ALPHA_DECAL] = G_RM_AA_ZB_TEX_EDGE2 | ZMODE_DEC,
        [LAYER_SILHOUETTE_OPAQUE] = G_RM_AA_ZB_OPA_SURF2,
        [LAYER_SILHOUETTE_ALPHA] = G_RM_AA_ZB_TEX_EDGE2,
        [LAYER_OCCLUDE_SILHOUETTE_OPAQUE] = G_RM_AA_ZB_OPA_SURF2,
        [LAYER_OCCLUDE_SILHOUETTE_ALPHA] = G_RM_AA_ZB_TEX_EDGE2,
#endif
        [LAYER_TRANSPARENT_DECAL] = G_RM_AA_ZB_XLU_DECAL2,
        [LAYER_TRANSPARENT] = G_RM_AA_ZB_XLU_SURF2,
        [LAYER_TRANSPARENT_INTER] = G_RM_AA_ZB_XLU_INTER2,
    } } };

ALIGNED16 struct GraphNodeRoot *gCurGraphNodeRoot = NULL;
ALIGNED16 struct GraphNodeMasterList *gCurGraphNodeMasterList = NULL;
ALIGNED16 struct GraphNodePerspective *gCurGraphNodeCamFrustum = NULL;
ALIGNED16 struct GraphNodeCamera *gCurGraphNodeCamera = NULL;
ALIGNED16 struct GraphNodeObject *gCurGraphNodeObject = NULL;
ALIGNED16 struct GraphNodeHeldObject *gCurGraphNodeHeldObject = NULL;
u16 gAreaUpdateCounter = 0;
LookAt* gCurLookAt;

#if SILHOUETTE
// AA_EN        Enable anti aliasing (not actually used for AA in this case).
// IM_RD        Enable reading coverage value.
// CLR_ON_CVG   Don't change the color unless coverage overflows. This helps prevent triangle overlap.
// CVG_DST_WRAP Wrap the coverage value on overflow.
// CVG_X_ALPHA  Coverage and alpha will be multiplied and both will be the same. This makes texture alpha work (eg. Wing Cap wings).
// FORCE_BL     Force Blending.
#define SIL_CVG_THRESHOLD    0x3F // 32..255, 63 seems to give best results
#define SCHWA (AA_EN | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | CVG_X_ALPHA | FORCE_BL)
static const Gfx dl_silhouette_begin[] = {
    gsDPPipeSync(),
    // Set the render mode for the silhouette so that it gets its color and alpha from the fog register.
    gsDPSetRenderMode((SCHWA | GBL_c1(G_BL_CLR_FOG, G_BL_A_FOG, G_BL_CLR_MEM, G_BL_1MA)),
                      (SCHWA | GBL_c2(G_BL_CLR_FOG, G_BL_A_FOG, G_BL_CLR_MEM, G_BL_1MA))),
    // Set the silhouette's color & alpha.
    gsDPSetFogColor(0, 0, 0, SILHOUETTE),
    // Hacky way to prevent triangle overlap. 32..255. 63 seems to give best results.
    gsDPSetEnvColor(0, 0, 0, 0x3F),
    gsSPEndDisplayList(),
};

static const Gfx dl_silhouette_end[] = {
    gsDPPipeSync(),
    gsDPSetFogColor(  0,   0,   0, 255), // Reset fog color & alpha
    gsDPSetEnvColor(255, 255, 255, 255), // Reset env color & alpha
    gsSPEndDisplayList(),
};
#undef SCHWA
#endif

struct RenderPhase {
    u8 startLayer;
    u8 endLayer;
};

static struct RenderPhase sRenderPhases[] = {
#if SILHOUETTE
    [RENDER_PHASE_ZEX_BEFORE_SILHOUETTE]   = {
        .startLayer = LAYER_FIRST,
        .endLayer   = LAYER_LAST_BEFORE_SILHOUETTE,
    },

    [RENDER_PHASE_ZEX_SILHOUETTE]          = {
        .startLayer = LAYER_SILHOUETTE_FIRST,
        .endLayer   = LAYER_SILHOUETTE_LAST,
    },

    [RENDER_PHASE_ZEX_NON_SILHOUETTE]      = {
        .startLayer = LAYER_SILHOUETTE_FIRST,
        .endLayer   = LAYER_SILHOUETTE_LAST,
    },

    [RENDER_PHASE_ZEX_OCCLUDE_SILHOUETTE]  = {
        .startLayer = LAYER_OCCLUDE_SILHOUETTE_FIRST,
        .endLayer   = LAYER_OCCLUDE_SILHOUETTE_LAST,
    },

    [RENDER_PHASE_ZEX_AFTER_SILHOUETTE]    = {
        .startLayer = LAYER_NON_ZB_FIRST,
        .endLayer   = LAYER_LAST,
    },
#else
    [RENDER_PHASE_ZEX_ALL]                 = {
        .startLayer = LAYER_FIRST,
        .endLayer   = LAYER_LAST,
    },
#endif
};

extern const Gfx init_rsp[];

#define UPPER_FIXED(x) ((int)((unsigned int)((x) * 0x10000) >> 16))
#define LOWER_FIXED(x) ((int)((unsigned int)((x) * 0x10000) & 0xFFFF))

// Fixed-point identity matrix with the inverse of world scale
Mtx identityMatrixWorldScale = {{
    {UPPER_FIXED(1.0f / WORLD_SCALE) << 16, 0x00000000,
     UPPER_FIXED(1.0f / WORLD_SCALE) <<  0, 0x00000000},
    {0x00000000,                            UPPER_FIXED(1.0f / WORLD_SCALE) << 16,
     0x00000000,                            UPPER_FIXED(1.0f)               <<  0},
    {LOWER_FIXED(1.0f / WORLD_SCALE) << 16, 0x00000000,
     LOWER_FIXED(1.0f / WORLD_SCALE) <<  0, 0x00000000},
    {0x00000000,                            LOWER_FIXED(1.0f / WORLD_SCALE) << 16,
     0x00000000,                            LOWER_FIXED(1.0f)               <<  0}
}};

/**
 * Process a master list node. This has been modified, so now it runs twice, for each microcode.
 * It iterates through the first 5 layers of if the first index using F3DLX2.Rej, then it switches
 * to F3DZEX and iterates through all layers, then switches back to F3DLX2.Rej and finishes the last
 * 3. It does this, because layers 5-7 are non zbuffered, and just doing 0-7 of ZEX, then 0-7 of REJ
 * would make the ZEX 0-4 render on top of Rej's 5-7.
 */
 
 Vec3uc gGlobalFogColor;
//static Vec3f LookAtPos;

//f32 player_z_diff(f32 zLoc) {
    //f32 diff;
    //one_diff(diff, gMarioState->pos[2], zLoc);
    //return one_mag(diff);
//}

#define EVIL_FOG_DIST -7600
/*#define EVIL_FOG_DIST2 -4600
//#define EVIL_FOG_DIST3 -1600
//#define EVIL_FOG_DIST4 700
			if (gMarioState->pos[2] > EVIL_FOG_DIST3 && gMarioState->pos[2] < EVIL_FOG_DIST4) { \
				castleFOG[1] = approach_color_linear(castleFOG[1], 0xB6); \
			} else if (gMarioState->pos[2] > EVIL_FOG_DIST2 && gMarioState->pos[2] < EVIL_FOG_DIST3) { \
				castleFOG[1] = approach_color_linear(castleFOG[1], 0x79); \
			} else if (gMarioState->pos[2] > EVIL_FOG_DIST && gMarioState->pos[2] < EVIL_FOG_DIST2) { \
				if (castleFOG[0] != 0x98) castleFOG[0] = approach_color_linear(castleFOG[0], 0x98); \
				castleFOG[1] = approach_color_linear(castleFOG[1], 0x3C); \
				if (castleFOG[2] != 0xFF) castleFOG[2] = approach_color_linear(castleFOG[2], 0xFF); \
			} else if (gMarioState->pos[2] < EVIL_FOG_DIST) { \
*/
#define set_dynamic_fog() \
	switch (gCurrLevelNum) { \
		case LEVEL_PSS: \
			gSPFogPosition(tempGfxHead++, 980, 1000); \
		break; \
		case LEVEL_CASTLE_GROUNDS: \
		break; \
		case LEVEL_SA: \
		break; \
		case LEVEL_TTM: \
		break; \
		case LEVEL_WDW: \
		break; \
		case LEVEL_DDD: \
		break; \
		case LEVEL_HMC: \
		break; \
		case LEVEL_COTMC: \
		break; \
		case LEVEL_JRB: \
		break; \
    } \
	gDPSetFogColor(tempGfxHead++, gGlobalFogColor[0], gGlobalFogColor[1], gGlobalFogColor[2], 255); \
 
void geo_process_master_list_sub(struct GraphNodeMasterList *node) {
    struct RenderPhase *renderPhase;
    struct DisplayListNode *currList;
    s32 currLayer     = LAYER_FIRST;
    s32 startLayer    = LAYER_FIRST;
    s32 endLayer      = LAYER_LAST;
    s32 phaseIndex    = RENDER_PHASE_FIRST;
    s32 enableZBuffer = (node->node.flags & GRAPH_RENDER_Z_BUFFER) != 0;
    s32 finalPhase    = enableZBuffer ? RENDER_PHASE_END : 1;
    struct RenderModeContainer *mode1List = &renderModeTable_1Cycle[enableZBuffer];
    struct RenderModeContainer *mode2List = &renderModeTable_2Cycle[enableZBuffer];
    Gfx *tempGfxHead = gDisplayListHead;

    // Loop through the render phases
    for (phaseIndex = RENDER_PHASE_FIRST; phaseIndex < finalPhase; phaseIndex++) {
        if (enableZBuffer) {
            // Get the render phase information.
            renderPhase = &sRenderPhases[phaseIndex];
            startLayer  = renderPhase->startLayer;
            endLayer    = renderPhase->endLayer;
            // Enable z buffer.
            gDPPipeSync(tempGfxHead++);
            gSPSetGeometryMode(tempGfxHead++, G_ZBUFFER);
        } else {
            startLayer = LAYER_FORCE;
            endLayer = LAYER_TRANSPARENT;
        }
        // Iterate through the layers on the current render phase.
        for (currLayer = startLayer; currLayer <= endLayer; currLayer++) {
            // Set 'currList' to the first DisplayListNode on the current layer.
            currList = node->listHeads[currLayer];
#if defined(DISABLE_AA) || !SILHOUETTE
            // Set the render mode for the current layer.
            gDPSetRenderMode(tempGfxHead++, mode1List->modes[currLayer],
                                                 mode2List->modes[currLayer]);
#else
            if (phaseIndex == RENDER_PHASE_NON_SILHOUETTE) {
                // To properly cover the silhouette, disable AA.
                // The silhouette model does not have AA due to the hack used to prevent triangle overlap.
                gDPSetRenderMode(tempGfxHead++, (mode1List->modes[currLayer] & ~IM_RD),
                                                     (mode2List->modes[currLayer] & ~IM_RD));
            } else {
                // Set the render mode for the current dl.
                gDPSetRenderMode(tempGfxHead++, mode1List->modes[currLayer],
                                                     mode2List->modes[currLayer]);
            }
#endif

			set_dynamic_fog();
			
            // Iterate through all the displaylists on the current layer.
            while (currList != NULL) {
                // Add the display list's transformation to the master list.
                gSPMatrix(tempGfxHead++, VIRTUAL_TO_PHYSICAL(currList->transform),
                          (G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH));
#if SILHOUETTE
                if (phaseIndex == RENDER_PHASE_SILHOUETTE) {
                    // Add the current display list to the master list, with silhouette F3D.
                    gSPDisplayList(tempGfxHead++, dl_silhouette_begin);
                    gSPDisplayList(tempGfxHead++, currList->displayList);
                    gSPDisplayList(tempGfxHead++, dl_silhouette_end);
                } else {
                    // Add the current display list to the master list.
                    gSPDisplayList(tempGfxHead++, currList->displayList);
                }
#else
                // Add the current display list to the master list.
                gSPDisplayList(tempGfxHead++, currList->displayList);
#endif
                // Move to the next DisplayListNode.
                currList = currList->next;
            }
        }
    }

    if (enableZBuffer) {
        // Disable z buffer.
        gDPPipeSync(tempGfxHead++);
        gSPClearGeometryMode(tempGfxHead++, G_ZBUFFER);
#ifdef VISUAL_DEBUG
        // Load the world scale identity matrix
        gSPMatrix(tempGfxHead++, &identityMatrixWorldScale, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        if (surfaceView) visual_surface_loop(&tempGfxHead);
        render_debug_boxes(&tempGfxHead);
#endif
    }

    gDisplayListHead = tempGfxHead;
}

/**
 * Appends the display list to one of the master lists based on the layer
 * parameter. Look at the RenderModeContainer struct to see the corresponding
 * render modes of layers.
 */
void geo_append_display_list(void *displayList, s32 layer) {
#ifdef F3DEX_GBI_2
    gSPLookAt(gDisplayListHead++, gCurLookAt);
#endif
#if SILHOUETTE
    if (gCurGraphNodeObject != NULL) {
        if (gCurGraphNodeObject->node.flags & GRAPH_RENDER_SILHOUETTE) {
            switch (layer) {
                case LAYER_OPAQUE: layer = LAYER_SILHOUETTE_OPAQUE; break;
                case LAYER_ALPHA:  layer = LAYER_SILHOUETTE_ALPHA;  break;
            }
        }
        if (gCurGraphNodeObject->node.flags & GRAPH_RENDER_OCCLUDE_SILHOUETTE) {
            switch (layer) {
                case LAYER_OPAQUE: layer = LAYER_OCCLUDE_SILHOUETTE_OPAQUE; break;
                case LAYER_ALPHA:  layer = LAYER_OCCLUDE_SILHOUETTE_ALPHA;  break;
            }
        }
    }
#endif // F3DEX_GBI_2 || SILHOUETTE
    if (gCurGraphNodeMasterList != NULL) {
        struct DisplayListNode *listNode =
            alloc_only_pool_alloc(gDisplayListHeap, sizeof(struct DisplayListNode));

        listNode->transform = gMatStackFixed[gMatStackIndex];
        listNode->displayList = displayList;
        listNode->next = NULL;
        if (gCurGraphNodeMasterList->listHeads[layer] == NULL) {
            gCurGraphNodeMasterList->listHeads[layer] = listNode;
        } else {
            gCurGraphNodeMasterList->listTails[layer]->next = listNode;
        }
        gCurGraphNodeMasterList->listTails[layer] = listNode;
    }
}

static void inc_mat_stack() {
    Mtx *mtx = alloc_display_list(sizeof(*mtx));
    gMatStackIndex++;
    mtxf_to_mtx(mtx, gMatStack[gMatStackIndex]);
    gMatStackFixed[gMatStackIndex] = mtx;
}

static void append_dl_and_return(struct GraphNodeDisplayList *node) {
    if (node->displayList != NULL) {
        geo_append_display_list(node->displayList, GET_GRAPH_NODE_LAYER(node->node.flags));
    }
    if (node->node.children != NULL) {
        geo_process_node_and_siblings(node->node.children);
    }
    gMatStackIndex--;
}

/**
 * Process the master list node.
 */
void geo_process_master_list(struct GraphNodeMasterList *node) {
    s32 layer;

    if (gCurGraphNodeMasterList == NULL && node->node.children != NULL) {
        gCurGraphNodeMasterList = node;
        for (layer = LAYER_FIRST; layer < LAYER_COUNT; layer++) {
            node->listHeads[layer] = NULL;
        }
        geo_process_node_and_siblings(node->node.children);
        geo_process_master_list_sub(gCurGraphNodeMasterList);
        gCurGraphNodeMasterList = NULL;
    }
}

/**
 * Process an orthographic projection node.
 */
 void geo_process_ortho_projection(struct GraphNodeOrthoProjection *node) {
    if (node->node.children != NULL) {
        Mtx *mtx = alloc_display_list(sizeof(*mtx));
        f32 scale = node->scale / 2.0f;
        f32 left = (gCurGraphNodeRoot->x - gCurGraphNodeRoot->width) * scale;
        f32 right = (gCurGraphNodeRoot->x + gCurGraphNodeRoot->width) * scale;
        f32 top = (gCurGraphNodeRoot->y - gCurGraphNodeRoot->height) * scale;
        f32 bottom = (gCurGraphNodeRoot->y + gCurGraphNodeRoot->height) * scale;

        guOrtho(mtx, left, right, bottom, top, -2.0f, 2.0f, 1.0f);
        gSPPerspNormalize(gDisplayListHead++, 0xFFFF);
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(mtx), G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);

        geo_process_node_and_siblings(node->node.children);
    }
}

/**
 * Process a perspective projection node.
 */
void geo_process_perspective(struct GraphNodePerspective *node) {
    if (node->fnNode.func != NULL) {
        node->fnNode.func(GEO_CONTEXT_RENDER, &node->fnNode.node, gMatStack[gMatStackIndex]);
    }
    if (node->fnNode.node.children != NULL) {
        u16 perspNorm;
        Mtx *mtx = alloc_display_list(sizeof(*mtx));
#ifdef WIDE
        if (gConfig.widescreen && gCurrLevelNum != 0x01){
            sAspectRatio = 16.0f / 9.0f; // 1.775f
        } else {
            sAspectRatio = 4.0f / 3.0f; // 1.33333f
        }
#else
        sAspectRatio = 4.0f / 3.0f; // 1.33333f
#endif

        f32 vHalfFov = ( ((node->fov * 4096.f) + 8192.f) ) / 45.f;

        // We need to account for aspect ratio changes by multiplying by the widescreen horizontal stretch 
        // (normally 1.775).
        node->halfFovHorizontal = tans(vHalfFov * sAspectRatio);

#ifdef VERTICAL_CULLING
        node->halfFovVertical = tans(vHalfFov);
#endif

        // With low fovs, coordinate overflow can occur more easily. This slightly reduces precision only while zoomed in.
        f32 scale = node->fov < 28.0f ? remap(MAX(node->fov, 15), 15, 28, 0.5f, 1.0f): 1.0f;
        guPerspective(mtx, &perspNorm, node->fov, sAspectRatio, node->near / WORLD_SCALE, node->far / WORLD_SCALE, scale);

        gSPPerspNormalize(gDisplayListHead++, perspNorm);

        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(mtx), G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);

        gCurGraphNodeCamFrustum = node;
        geo_process_node_and_siblings(node->fnNode.node.children);
        gCurGraphNodeCamFrustum = NULL;
    }
}

static f32 get_dist_from_camera(Vec3f pos) {
    return -((gCameraTransform[0][2] * pos[0])
           + (gCameraTransform[1][2] * pos[1])
           + (gCameraTransform[2][2] * pos[2])
           +  gCameraTransform[3][2]);
}

/**
 * Process a level of detail node. From the current transformation matrix,
 * the perpendicular distance to the camera is extracted and the children
 * of this node are only processed if that distance is within the render
 * range of this node.
 */
void geo_process_level_of_detail(struct GraphNodeLevelOfDetail *node) {
#ifdef AUTO_LOD
    f32 distanceFromCam = (gEmulator & EMU_CONSOLE) ? get_dist_from_camera(gMatStack[gMatStackIndex][3]) : 50.0f;
#else
    f32 distanceFromCam = get_dist_from_camera(gMatStack[gMatStackIndex][3]);
#endif

    if ((f32)node->minDistance <= distanceFromCam
        && distanceFromCam < (f32)node->maxDistance
        && node->node.children != 0) {
        geo_process_node_and_siblings(node->node.children);
    }
}

/**
 * Process a switch case node. The node's selection function is called
 * if it is 0, and among the node's children, only the selected child is
 * processed next.
 */
void geo_process_switch(struct GraphNodeSwitchCase *node) {
    struct GraphNode *selectedChild = node->fnNode.node.children;
    s32 i;

    if (node->fnNode.func != NULL) {
        node->fnNode.func(GEO_CONTEXT_RENDER, &node->fnNode.node, gMatStack[gMatStackIndex]);
    }
    for (i = 0; selectedChild != NULL && node->selectedCase > i; i++) {
        selectedChild = selectedChild->next;
    }
    if (selectedChild != NULL) {
        geo_process_node_and_siblings(selectedChild);
    }
}


static Lights1* sSceneLight = NULL;
static Lights1* sSpecularLight = NULL;
static Lights1* sAmbientLight = NULL;
//consider looking into point lights

/**
 * Creates a displaylist to set the active lights closest to a given location
 */
 
//Gfx* createLightDl(UNUSED Vec3f pos, UNUSED f32 yOffset) {
Gfx* createLightDl(void) {
    Gfx *lightDl, *lightDlHead;
	if (gOccludeLighting) {
		lightDlHead = lightDl = alloc_display_list(sizeof(Gfx) * 6);
		gSPNumLights(lightDl++, NUMLIGHTS_3);
	} else if (!gDisableLighting) {
		// Allocate a displaylist with room for each gSPLight and the gSPEndDisplayList
		//if (!gVanillaLighting && !gFileSelect) {
		if (!gVanillaLighting) {
			lightDlHead = lightDl = alloc_display_list(sizeof(Gfx) * (3 + gNumLightColors));

			gSPNumLights(lightDl++, gNumLightColors);
		} else {
			lightDlHead = lightDl = alloc_display_list(sizeof(Gfx) * 5);

			gSPNumLights(lightDl++, NUMLIGHTS_2);
		}
	
		if (gNumLightSources > 1 || gNumLightColors > 1) gSPLight(lightDl++, &sSpecularLight->l, LIGHT_2);
		gSPLight(lightDl++, &sSceneLight->l, LIGHT_1);
		if (gNumLightSources > 2 || gNumLightColors > 2) gSPLight(lightDl++, &sAmbientLight->l, LIGHT_3);
		
		
		//gSPLight(lightDl++, &sSpecularLight->l, LIGHT_1);
		//gSPLight(lightDl++, &sSpecularLight->l, LIGHT_2);
		//gSPLight(lightDl++, &sSceneLight->l, LIGHT_3);
		//gSPLight(lightDl++, &sSceneLight->l, LIGHT_4);
		//gSPLight(lightDl++, &sSceneLight->l, LIGHT_5);
		//gSPLight(lightDl++, &sSceneLight->l, LIGHT_6);
		//gSPLight(lightDl++, &sSceneLight->l, LIGHT_7);
		//gSPLight(lightDl++, &sSceneLight->l, LIGHT_8);
		
	} else {
		lightDlHead = lightDl = alloc_display_list(sizeof(Gfx) * 3);
		gSPNumLights(lightDl++, NUMLIGHTS_0);
	}

    // Terminate the display list
    gSPEndDisplayList(lightDl);

    // Return the head of the created display list
    return lightDlHead;
}

void determine_number_of_light_sources(void) {
	//if (!gVanillaLighting && !gFileSelect) {
	if (!gVanillaLighting) {
		switch (gNumLightSources) {
			case 0:
			case 1:
				gSPSetLights1(gDisplayListHead++, (*sSceneLight)); 
			break;
			case 2:
				//gSPSetLights1(gDisplayListHead++, (*sSpecularLight)); 
				gSPSetLights2(gDisplayListHead++, (*sSceneLight));
			break;
			case 3:
				//gSPSetLights1(gDisplayListHead++, (*sSpecularLight)); 
				//gSPSetLights2(gDisplayListHead++, (*sAmbientLight));
				gSPSetLights3(gDisplayListHead++, (*sSceneLight));
			break;
		}
	} else {
		if (gOccludeLighting) {
			gSPSetLights3(gDisplayListHead++, (*sSceneLight));
		} else if (!gDisableLighting) {
			gSPSetLights2(gDisplayListHead++, (*sSceneLight));
		} else {
			gSPSetLights0(gDisplayListHead++, (*sSceneLight));
		}
	}
}

Vec3uc pixColor;

#include "buffers/framebuffers.h"
#include "game_init.h"
#include "engine/colors.h"

ALWAYS_INLINE void framebuffer_probe_color(void) {
    
	#define IMAGE_SIZE 16
	
	#define SAMPLE_SIZE 1
	
	s32 pixel;
    s32 iy, ix, sy, sx;
    s32 idy, idx, sdy;
    RGBA16 *fb = gFramebuffers[sRenderingFramebuffer];

    for ((iy = 0); (iy < IMAGE_SIZE); (iy++)) {
        idy = (SAMPLE_SIZE + 12) * iy;
        for ((ix = 0); (ix < IMAGE_SIZE); (ix++)) {
            vec3_zero(pixColor);
            idx = (SAMPLE_SIZE + 12) * ix;

            for ((sy = 0); (sy < 3); (sy++)) {
                sdy = ((SCREEN_WIDTH * (idy + sy)) + idx);
                for ((sx = 0); (sx < 3); (sx++)) {
                    pixel = fb[sdy + sx];
                    pixColor[0] += RGBA16_R(pixel);
                    pixColor[1] += RGBA16_G(pixel);
                    pixColor[2] += RGBA16_B(pixel);
                }
            }
        }
    }
}

ALWAYS_INLINE u8 approach_color_light(u8 current, u8 target) {
	s16 diff = (target - current);
	if (diff >= 0) {
		current = (diff > 1) ? current + 2: target;
	} else {
		current = (diff < -1) ? current - 2 : target;
	}
	
    return current;
}

void setup_light(Vec3c LDir, Lights1 LCol, u8 LType) {
		sSceneLight = (Lights1*)alloc_display_list(sizeof(Lights1));
		bcopy(&LCol, sSceneLight, sizeof(Lights1));
		Vec3f transformedLightDirection;
		Vec3f globalLightDirection;
		switch (LType) {
			case DIRECTIONAL_CAMERA:
				for (u32 i = 0; i < 3; i++) {
					globalLightDirection[i] = LDir[i];
				}
				
				linear_mtxf_transpose_mul_vec3f(gCameraTransform, transformedLightDirection, globalLightDirection);
				
				sSceneLight->l->l.dir[0] = (s8)(transformedLightDirection[0]);
				//sSceneLight->l->l.dir[1] = (s8)(transformedLightDirection[0]);
				sSceneLight->l->l.dir[1] = LDir[1];
				sSceneLight->l->l.dir[2] = (s8)(transformedLightDirection[0]);
			break;
			case DIRECTIONAL_GLOBAL:
				sSceneLight->l->l.dir[0] = LDir[0];
				sSceneLight->l->l.dir[1] = LDir[1];
				sSceneLight->l->l.dir[2] = LDir[2];
			break;
			#if 0
			case POINT_CAMERA:
				for (u32 i = 0; i < 3; i++) {
					globalLightDirection[i] = LDir[i];
				}
				
				linear_mtxf_transpose_mul_vec3f(gCameraTransform, transformedLightDirection, globalLightDirection);
				
				sSceneLight->l->pl.pos[0] = (s8)(transformedLightDirection[0]) / gWorldScale;
				//sSceneLight->l->pl.pos[1] = (s8)(transformedLightDirection[0]) / gWorldScale;
				sSceneLight->l->pl.pos[1] = LDir[1] / gWorldScale;
				sSceneLight->l->pl.pos[2] = (s8)(transformedLightDirection[0]) / gWorldScale;
				//sSceneLight->l->pl.constant_attenuation = (constantFalloff == 0) ? 8 : constantFalloff;
				//sSceneLight->l->pl.linear_attenuation = linearFalloff;
				//sSceneLight->l->pl.quadratic_attenuation = quadraticFalloff;
				sSceneLight->l->pl.constant_attenuation = 4;
				sSceneLight->l->pl.linear_attenuation = 4;
				sSceneLight->l->pl.quadratic_attenuation = 4;
			break;
			case POINT_GLOBAL:
				//sSceneLight->l->pl.constant_attenuation = (constantFalloff == 0) ? 8 : constantFalloff;
				//sSceneLight->l->pl.linear_attenuation = linearFalloff;
				//sSceneLight->l->pl.quadratic_attenuation = quadraticFalloff;
				sSceneLight->l->pl.constant_attenuation = 4;
				sSceneLight->l->pl.linear_attenuation = 4;
				sSceneLight->l->pl.quadratic_attenuation = 4;
				sSceneLight->l->pl.pos[0] = LDir[0] / gWorldScale;
				sSceneLight->l->pl.pos[1] = LDir[1] / gWorldScale;
				sSceneLight->l->pl.pos[2] = LDir[2] / gWorldScale;
			break;
			#endif
		}
}

//not actually specular light, its a quirk with the way I have lighting set up
void setup_specular_light(Vec3c LDir, u8 LType) {
		sSpecularLight = (Lights1*)alloc_display_list(sizeof(Lights1));
		bcopy(&vanillaLight, sSpecularLight, sizeof(Lights1));
		Vec3f transformedLightDirection;
		Vec3f globalLightDirection;
		switch (LType) { //allow for it to have a type later?
			case FBPROBE_CAMERA:
			case DIRECTIONAL_CAMERA:
				for (u32 i = 0; i < 3; i++) {
					globalLightDirection[i] = LDir[i];
				}
				
				linear_mtxf_transpose_mul_vec3f(gCameraTransform, transformedLightDirection, globalLightDirection);
				
				sSpecularLight->l->l.dir[0] = (s8)(transformedLightDirection[0]);
				//sSpecularLight->l->l.dir[1] = (s8)(transformedLightDirection[0]);
				sSpecularLight->l->l.dir[1] = LDir[1];
				sSpecularLight->l->l.dir[2] = (s8)(transformedLightDirection[0]);
			break;
			case FBPROBE_GLOBAL:
			case DIRECTIONAL_GLOBAL:
				sSpecularLight->l->l.dir[0] = LDir[0];
				sSpecularLight->l->l.dir[1] = LDir[1];
				sSpecularLight->l->l.dir[2] = LDir[2];
			break;
			#if 0
			case POINT_CAMERA:
				for (u32 i = 0; i < 3; i++) {
					globalLightDirection[i] = LDir[i];
				}
				
				linear_mtxf_transpose_mul_vec3f(gCameraTransform, transformedLightDirection, globalLightDirection);
				
				sSceneLight->l->pl.pos[0] = (s8)(transformedLightDirection[0]) / gWorldScale;
				//sSceneLight->l->pl.pos[1] = (s8)(transformedLightDirection[0]) / gWorldScale;
				sSceneLight->l->pl.pos[1] = LDir[1] / gWorldScale;
				sSceneLight->l->pl.pos[2] = (s8)(transformedLightDirection[0]) / gWorldScale;
				//sSceneLight->l->pl.constant_attenuation = (constantFalloff == 0) ? 8 : constantFalloff;
				//sSceneLight->l->pl.linear_attenuation = linearFalloff;
				//sSceneLight->l->pl.quadratic_attenuation = quadraticFalloff;
				sSceneLight->l->pl.constant_attenuation = 4;
				sSceneLight->l->pl.linear_attenuation = 4;
				sSceneLight->l->pl.quadratic_attenuation = 4;
			break;
			case POINT_GLOBAL:
				//sSpecularLight->l->pl.constant_attenuation = (constantFalloff == 0) ? 8 : constantFalloff;
				//sSpecularLight->l->pl.linear_attenuation = linearFalloff;
				//sSpecularLight->l->pl.quadratic_attenuation = quadraticFalloff;
				sSpecularLight->l->pl.constant_attenuation = 4;
				sSpecularLight->l->pl.linear_attenuation = 4;
				sSpecularLight->l->pl.quadratic_attenuation = 4;
				sSpecularLight->l->pl.pos[0] = LDir[0] / gWorldScale;
				sSpecularLight->l->pl.pos[1] = LDir[1] / gWorldScale;
				sSpecularLight->l->pl.pos[2] = LDir[2] / gWorldScale;
			break;
			#endif
		}
}

//not actually an ambient light, once again just a unique lighting quirk
void setup_ambient_light(Vec3c LDir, u8 LType) {
		sAmbientLight = (Lights1*)alloc_display_list(sizeof(Lights1));
		bcopy(&vanillaLight, sAmbientLight, sizeof(Lights1));
		Vec3f transformedLightDirection;
		Vec3f globalLightDirection;
		switch (LType) { //allow for it to have a type later?
			case FBPROBE_CAMERA:
			case DIRECTIONAL_CAMERA:
				for (u32 i = 0; i < 3; i++) {
					globalLightDirection[i] = LDir[i];
				}
				
				linear_mtxf_transpose_mul_vec3f(gCameraTransform, transformedLightDirection, globalLightDirection);
				
				sAmbientLight->l->l.dir[0] = (s8)(transformedLightDirection[0]);
				//sAmbientLight->l->l.dir[1] = (s8)(transformedLightDirection[0]);
				sAmbientLight->l->l.dir[1] = LDir[1];
				sAmbientLight->l->l.dir[2] = (s8)(transformedLightDirection[0]);
			break;
			case FBPROBE_GLOBAL:
			case DIRECTIONAL_GLOBAL:
				sAmbientLight->l->l.dir[0] = LDir[0];
				sAmbientLight->l->l.dir[1] = LDir[1];
				sAmbientLight->l->l.dir[2] = LDir[2];
			break;
			#if 0
			case POINT_CAMERA:
				for (u32 i = 0; i < 3; i++) {
					globalLightDirection[i] = LDir[i];
				}
				
				linear_mtxf_transpose_mul_vec3f(gCameraTransform, transformedLightDirection, globalLightDirection);
				
				sSceneLight->l->pl.pos[0] = (s8)(transformedLightDirection[0]) / gWorldScale;
				//sSceneLight->l->pl.pos[1] = (s8)(transformedLightDirection[0]) / gWorldScale;
				sSceneLight->l->pl.pos[1] = LDir[1] / gWorldScale;
				sSceneLight->l->pl.pos[2] = (s8)(transformedLightDirection[0]) / gWorldScale;
				//sSceneLight->l->pl.constant_attenuation = (constantFalloff == 0) ? 8 : constantFalloff;
				//sSceneLight->l->pl.linear_attenuation = linearFalloff;
				//sSceneLight->l->pl.quadratic_attenuation = quadraticFalloff;
				sSceneLight->l->pl.constant_attenuation = 4;
				sSceneLight->l->pl.linear_attenuation = 4;
				sSceneLight->l->pl.quadratic_attenuation = 4;
			break;
			case POINT_GLOBAL:
				//sAmbientLight->l->pl.constant_attenuation = (constantFalloff == 0) ? 8 : constantFalloff;
				//sAmbientLight->l->pl.linear_attenuation = linearFalloff;
				//sAmbientLight->l->pl.quadratic_attenuation = quadraticFalloff;
				sAmbientLight->l->pl.constant_attenuation = 4;
				sAmbientLight->l->pl.linear_attenuation = 4;
				sAmbientLight->l->pl.quadratic_attenuation = 4;
				sAmbientLight->l->pl.pos[0] = LDir[0] / gWorldScale;
				sAmbientLight->l->pl.pos[1] = LDir[1] / gWorldScale;
				sAmbientLight->l->pl.pos[2] = LDir[2] / gWorldScale;
			break;
			#endif
		}
}

//extern u8 gHasFrameBuffer;

void setup_light_dynamic(Vec3c LDir, Vec3uc LCol, u8 LType) {
		sSceneLight = (Lights1*)alloc_display_list(sizeof(Lights1));
		if (gLightType < DIRECTIONAL_CAMERA && (gGlobalTimer & 15)) framebuffer_probe_color();
		Vec3f transformedLightDirection;
		Vec3f globalLightDirection;
		switch (LType) {
			case FBPROBE_CAMERA:
				//if (!gHasFrameBuffer) LType = DIRECTIONAL_CAMERA;
				for (u32 i = 0; i < 3; i++) {
					globalLightDirection[i] = LDir[i];
				}
				linear_mtxf_transpose_mul_vec3f(gCameraTransform, transformedLightDirection, globalLightDirection);
				
				sSceneLight->l->l.dir[0] = (s8)(transformedLightDirection[0]);
				//sSceneLight->l->l.dir[1] = (s8)(transformedLightDirection[1]);
				sSceneLight->l->l.dir[1] = LDir[1];
				sSceneLight->l->l.dir[2] = (s8)(transformedLightDirection[2]);
	
				sSceneLight->l->l.col[0] = approach_color_light(sSceneLight->l->l.col[0], pixColor[0]);
				sSceneLight->l->l.col[1] = approach_color_light(sSceneLight->l->l.col[1], pixColor[1]);
				sSceneLight->l->l.col[2] = approach_color_light(sSceneLight->l->l.col[2], pixColor[2]);
	
				sSceneLight->l->l.colc[0] = sSceneLight->l->l.col[0];
				sSceneLight->l->l.colc[1] = sSceneLight->l->l.col[1];
				sSceneLight->l->l.colc[2] = sSceneLight->l->l.col[2];
			break;
			case FBPROBE_GLOBAL:
				//if (!gHasFrameBuffer) LType = DIRECTIONAL_GLOBAL;
				sSceneLight->l->l.dir[0] = LDir[0];
				sSceneLight->l->l.dir[1] = LDir[1];
				sSceneLight->l->l.dir[2] = LDir[2];
				
				sSceneLight->l->l.col[0] = approach_color_light(sSceneLight->l->l.col[0], pixColor[0]);
				sSceneLight->l->l.col[1] = approach_color_light(sSceneLight->l->l.col[1], pixColor[1]);
				sSceneLight->l->l.col[2] = approach_color_light(sSceneLight->l->l.col[2], pixColor[2]);
	
				sSceneLight->l->l.colc[0] = sSceneLight->l->l.col[0];
				sSceneLight->l->l.colc[1] = sSceneLight->l->l.col[1];
				sSceneLight->l->l.colc[2] = sSceneLight->l->l.col[2];
			break;
			case DIRECTIONAL_CAMERA:
				//if (!gHasFrameBuffer) gIsDynamic = FALSE;
				for (u32 i = 0; i < 3; i++) {
					globalLightDirection[i] = LDir[i];
				}
				linear_mtxf_transpose_mul_vec3f(gCameraTransform, transformedLightDirection, globalLightDirection);
				
				sSceneLight->l->l.dir[0] = (s8)(transformedLightDirection[0]);
				//sSceneLight->l->l.dir[1] = (s8)(transformedLightDirection[1]);
				sSceneLight->l->l.dir[1] = LDir[1];
				sSceneLight->l->l.dir[2] = (s8)(transformedLightDirection[2]);
	
				sSceneLight->l->l.col[0] = LCol[0];
				sSceneLight->l->l.col[1] = LCol[1];
				sSceneLight->l->l.col[2] = LCol[2];
	
				sSceneLight->l->l.colc[0] = sSceneLight->l->l.col[0];
				sSceneLight->l->l.colc[1] = sSceneLight->l->l.col[1];
				sSceneLight->l->l.colc[2] = sSceneLight->l->l.col[2];
			break;
			case DIRECTIONAL_GLOBAL:
				//if (!gHasFrameBuffer) gIsDynamic = FALSE;
				sSceneLight->l->l.dir[0] = LDir[0];
				sSceneLight->l->l.dir[1] = LDir[1];
				sSceneLight->l->l.dir[2] = LDir[2];
	
				sSceneLight->l->l.col[0] = LCol[0];
				sSceneLight->l->l.col[1] = LCol[1];
				sSceneLight->l->l.col[2] = LCol[2];
	
				sSceneLight->l->l.colc[0] = sSceneLight->l->l.col[0];
				sSceneLight->l->l.colc[1] = sSceneLight->l->l.col[1];
				sSceneLight->l->l.colc[2] = sSceneLight->l->l.col[2];
			break;
			case OCCLUDE_CAMERA:
				for (u32 i = 0; i < 3; i++) {
					globalLightDirection[i] = LDir[i];
				}
				linear_mtxf_transpose_mul_vec3f(gCameraTransform, transformedLightDirection, globalLightDirection);
				
				sSceneLight->l->l.dir[0] = (s8)(transformedLightDirection[0]);
				//sSceneLight->l->l.dir[1] = (s8)(transformedLightDirection[1]);
				sSceneLight->l->l.dir[1] = LDir[1];
				sSceneLight->l->l.dir[2] = (s8)(transformedLightDirection[2]);
	
				sSceneLight->l->l.col[0] = approach_color_light(sSceneLight->l->l.col[0], LCol[0]);
				sSceneLight->l->l.col[1] = approach_color_light(sSceneLight->l->l.col[1], LCol[1]);
				sSceneLight->l->l.col[2] = approach_color_light(sSceneLight->l->l.col[2], LCol[2]);
	
				sSceneLight->l->l.colc[0] = sSceneLight->l->l.col[0];
				sSceneLight->l->l.colc[1] = sSceneLight->l->l.col[1];
				sSceneLight->l->l.colc[2] = sSceneLight->l->l.col[2];
			break;
			#if 0
			case OCCLUDE_GLOBAL:
				sSceneLight->l->l.dir[0] = LDir[0];
				sSceneLight->l->l.dir[1] = LDir[1];
				sSceneLight->l->l.dir[2] = LDir[2];
				
				sSceneLight->l->l.col[0] = approach_color_light(sSceneLight->l->l.col[0], LCol[0]);
				sSceneLight->l->l.col[1] = approach_color_light(sSceneLight->l->l.col[1], LCol[1]);
				sSceneLight->l->l.col[2] = approach_color_light(sSceneLight->l->l.col[2], LCol[2]);
	
				sSceneLight->l->l.colc[0] = sSceneLight->l->l.col[0];
				sSceneLight->l->l.colc[1] = sSceneLight->l->l.col[1];
				sSceneLight->l->l.colc[2] = sSceneLight->l->l.col[2];
			break;
			#endif
		}
}

void determine_dynamic_lighting(void) {
	switch (gDynamicLightPreset) {
		case LIGHT_RAINBOW:
			gCurrDynamicColor[0] = gGlobalFogColor[0];
			gCurrDynamicColor[1] = gGlobalFogColor[1];
			gCurrDynamicColor[2] = gGlobalFogColor[2];
			gGlobalFogColor[0] = (coss(gGlobalTimer * 200         ) + 1) * 127; //puppyprint rainbow code
			gGlobalFogColor[1] = (coss((gGlobalTimer * 200) + 21845) + 1) * 127;
			gGlobalFogColor[2] = (coss((gGlobalTimer * 200) - 21845) + 1) * 127;
		break;
		//case LIGHT_CASTLE_BASEMENT:
		//break;
	}
}

extern u8 gHasFrameBuffer;

//void process_lighting(struct GraphNodeCamera *node) {
void process_lighting(void) {
	Gfx *setLightsDL = alloc_display_list(sizeof(Gfx) * 3);
	Gfx *levelLightsDL = NULL;
	
	geo_append_display_list(setLightsDL, LAYER_OPAQUE);
	
	if (gOccludeLighting) {
		Vec3uc sOccludedLight;
		for (s32 i = 0; i < 3; i++) sOccludedLight[i] = 0;
		setup_light_dynamic(gCurrLightDirection, sOccludedLight, OCCLUDE_CAMERA);
	//} else if (gVanillaLighting || gFileSelect > 0) {
	} else if (gVanillaLighting) {
		setup_light(vanillaLightDirection, vanillaLight, DIRECTIONAL_CAMERA);
	} else {
		if (!gDisableLighting) {
			//acts like a faux specular light when light colors is set to 1 and light sources is set to 2
			//else cover the player in sSceneLight's colors, (on accurate video plugins)
			if (gNumLightSources > 1 || gNumLightColors > 1) setup_specular_light(gCurrLightDirection2, gLightType);
			
			//fallback to static bounce lights if framebuffer is not being emulated
			if (!gHasFrameBuffer) {
				if (gLightType == FBPROBE_CAMERA) {
					gLightType = DIRECTIONAL_CAMERA;
					gIsDynamic = FALSE;
				} else if (gLightType == FBPROBE_GLOBAL) {
					gLightType = DIRECTIONAL_GLOBAL;
					gIsDynamic = FALSE;
				}
			}
			
			if (!gIsDynamic && gLightType > FBPROBE_GLOBAL) {
				setup_light(gCurrLightDirection, gCurrStaticColor, gLightType);
			} else {
				if (gLightType > FBPROBE_GLOBAL) determine_dynamic_lighting();
				setup_light_dynamic(gCurrLightDirection, gCurrDynamicColor, gLightType);
			}
		
			//behaves like a black "ambient" light when light colors is set to 3 and light sources is set to 2
			//else cover the player in sSceneLight's colors (on accurate video plugins)
			if (gNumLightSources > 2 || gNumLightColors > 2) setup_ambient_light(gCurrLightDirection3, gLightType);
		}
	}
	
	determine_number_of_light_sources();
	
	//print_text_fmt_int (30, 120, "%d", gVanillaLighting);
	//print_text_fmt_int (30, 90, "%d", gNumLights);
	//print_text_fmt_int (30, 60, "%d", gCurrLightDirection[0]);
	//print_text_fmt_int (30, 40, "%d", gCurrLightDirection[1]);
	//print_text_fmt_int (30, 20, "%d", gCurrLightDirection[2]);
	
	
	//print_text_fmt_int (30, 60, "%d", rainbowLED[0]);
	//print_text_fmt_int (30, 40, "%d", rainbowLED[1]);
	//print_text_fmt_int (30, 20, "%d", rainbowLED[2]);
    
    // Set up the light display list
    // This has to be done after the area's GeoLayout is processed, as
    // some point lights may be defined there instead of by objects
	//Vec3f probePos;
    //if (gLightType > DIRECTIONAL_GLOBAL) {
        // Enable point lighting
        //gSPSetGeometryMode(setLightsDL++, G_POINT_LIGHTING);
		//if (gMarioObject) {
			//vec3f_copy(probePos, gMarioState->pos);
		//} else {
			//vec3f_copy(probePos, node->pos);
		//}
    //} else {
        // Disable point lighting (may not be required, but doesn't hurt)
        //gSPClearGeometryMode(setLightsDL++, G_POINT_LIGHTING);
    //}
	
	// Enable the lights
	
	//levelLightsDL = createLightDl(probePos, 300.0f);
	levelLightsDL = createLightDl();
	
    gSPDisplayList(setLightsDL++, levelLightsDL);

    // Terminate the lighting DL
    gSPEndDisplayList(setLightsDL++);
}


//this works as a function on inaccurate plugins
#define init_lighting() \
	static u8 init; \
	if (!init) { \
		gLightType = DIRECTIONAL_CAMERA;  \
		gDisableLighting = FALSE; \
		gIsDynamic = FALSE; \
		gVanillaLighting = FALSE; \
		gNumLightColors = 2; \
		gNumLightSources = 2; \
		init++; \
	}

/**
 * Process a camera node.
 */
void geo_process_camera(struct GraphNodeCamera *node) {
    Mtx *rollMtx = alloc_display_list(sizeof(*rollMtx));
    Mtx *viewMtx = alloc_display_list(sizeof(Mtx));

    if (node->fnNode.func != NULL) {
        node->fnNode.func(GEO_CONTEXT_RENDER, &node->fnNode.node, gMatStack[gMatStackIndex]);
    }
    mtxf_rotate_xy(rollMtx, node->rollScreen);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(rollMtx), G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);

    mtxf_lookat(gCameraTransform, node->pos, node->focus, node->roll);

    // Calculate the lookAt
#ifdef F3DEX_GBI_2
    // @bug This is where the LookAt values should be calculated but aren't.
    // As a result, environment mapping is broken on Fast3DEX2 without the
    // changes below.
    Mat4* cameraMatrix = &gCameraTransform;
    /**
    * HackerSM64 2.1: Now uses the correct "up" vector for the guLookAtReflect call in geo_process_master_list_sub.
    * It was originally sideways in vanilla, with vanilla's environment map textures sideways to accommodate, but those
    * textures are now rotated automatically on extraction to allow for this to be fixed.
    */
    gCurLookAt->l[0].l.dir[0] = (s8)(127.0f * (*cameraMatrix)[0][0]);
    gCurLookAt->l[0].l.dir[1] = (s8)(127.0f * (*cameraMatrix)[1][0]);
    gCurLookAt->l[0].l.dir[2] = (s8)(127.0f * (*cameraMatrix)[2][0]);
    gCurLookAt->l[1].l.dir[0] = (s8)(127.0f * -(*cameraMatrix)[0][1]);
    gCurLookAt->l[1].l.dir[1] = (s8)(127.0f * -(*cameraMatrix)[1][1]);
    gCurLookAt->l[1].l.dir[2] = (s8)(127.0f * -(*cameraMatrix)[2][1]);
#endif // F3DEX_GBI_2

#if WORLD_SCALE > 1
    // Make a copy of the view matrix and scale its translation based on WORLD_SCALE
    Mat4 scaledCamera;
    mtxf_copy(scaledCamera, gCameraTransform);
    for (int i = 0; i < 3; i++) {
        scaledCamera[3][i] /= WORLD_SCALE;
    }

    // Convert the scaled matrix to fixed-point and integrate it into the projection matrix stack
    guMtxF2L(scaledCamera, viewMtx);
#else
    guMtxF2L(gCameraTransform, viewMtx);
#endif
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(viewMtx), G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
    
	init_lighting();
	
	//if (gCurrLevelNum > 0) process_lighting(node); //crashes ares otherwise
	
	if (gCurrLevelNum > 0) process_lighting(); //crashes ares otherwise
    if (node->fnNode.node.children != 0) {
        gCurGraphNodeCamera = node;
        node->matrixPtr = &gCameraTransform;
        geo_process_node_and_siblings(node->fnNode.node.children);
        gCurGraphNodeCamera = NULL;
    }
}

/**
 * Process a translation / rotation node. A transformation matrix based
 * on the node's translation and rotation is created and pushed on both
 * the float and fixed point matrix stacks.
 * For the rest it acts as a normal display list node.
 */
void geo_process_translation_rotation(struct GraphNodeTranslationRotation *node) {
    Vec3f translation;

    vec3s_to_vec3f(translation, node->translation);
    mtxf_rotate_zxy_and_translate_and_mul(node->rotation, translation, gMatStack[gMatStackIndex + 1], gMatStack[gMatStackIndex]);

    inc_mat_stack();
    append_dl_and_return((struct GraphNodeDisplayList *)node);
}

/**
 * Process a translation node. A transformation matrix based on the node's
 * translation is created and pushed on both the float and fixed point matrix stacks.
 * For the rest it acts as a normal display list node.
 */
void geo_process_translation(struct GraphNodeTranslation *node) {
    Vec3f translation;

    vec3s_to_vec3f(translation, node->translation);
    mtxf_rotate_zxy_and_translate_and_mul(gVec3sZero, translation, gMatStack[gMatStackIndex + 1], gMatStack[gMatStackIndex]);

    inc_mat_stack();
    append_dl_and_return((struct GraphNodeDisplayList *)node);
}

/**
 * Process a rotation node. A transformation matrix based on the node's
 * rotation is created and pushed on both the float and fixed point matrix stacks.
 * For the rest it acts as a normal display list node.
 */
void geo_process_rotation(struct GraphNodeRotation *node) {
    mtxf_rotate_zxy_and_translate_and_mul(node->rotation, gVec3fZero, gMatStack[gMatStackIndex + 1], gMatStack[gMatStackIndex]);

    inc_mat_stack();
    append_dl_and_return(((struct GraphNodeDisplayList *)node));
}

/**
 * Process a scaling node. A transformation matrix based on the node's
 * scale is created and pushed on both the float and fixed point matrix stacks.
 * For the rest it acts as a normal display list node.
 */
void geo_process_scale(struct GraphNodeScale *node) {
    Vec3f scaleVec;

    vec3f_set(scaleVec, node->scale, node->scale, node->scale);
    mtxf_scale_vec3f(gMatStack[gMatStackIndex + 1], gMatStack[gMatStackIndex], scaleVec);

    inc_mat_stack();
    append_dl_and_return((struct GraphNodeDisplayList *)node);
}

/**
 * Process a billboard node. A transformation matrix is created that makes its
 * children face the camera, and it is pushed on the floating point and fixed
 * point matrix stacks.
 * For the rest it acts as a normal display list node.
 */
void geo_process_billboard(struct GraphNodeBillboard *node) {
    Vec3f translation;
    Vec3f scale = { 1.0f, 1.0f, 1.0f };

    vec3s_to_vec3f(translation, node->translation);

    if (gCurGraphNodeHeldObject != NULL) {
        vec3f_copy(scale, gCurGraphNodeHeldObject->objNode->header.gfx.scale);
    } else if (gCurGraphNodeObject != NULL) {
        vec3f_copy(scale, gCurGraphNodeObject->scale);
    }

    mtxf_billboard(gMatStack[gMatStackIndex + 1], gMatStack[gMatStackIndex], translation, scale, gCurGraphNodeCamera->roll);

    inc_mat_stack();
    append_dl_and_return((struct GraphNodeDisplayList *)node);
}

/**
 * Process a display list node. It draws a display list without first pushing
 * a transformation on the stack, so all transformations are inherited from the
 * parent node. It processes its children if it has them.
 */
void geo_process_display_list(struct GraphNodeDisplayList *node) {
    append_dl_and_return((struct GraphNodeDisplayList *)node);

    gMatStackIndex++;
}

/**
 * Process a generated list. Instead of storing a pointer to a display list,
 * the list is generated on the fly by a function.
 */
void geo_process_generated_list(struct GraphNodeGenerated *node) {
    if (node->fnNode.func != NULL) {
        Gfx *list = node->fnNode.func(GEO_CONTEXT_RENDER, &node->fnNode.node, (struct AllocOnlyPool *) gMatStack[gMatStackIndex]);

        if (list != NULL) {
            geo_append_display_list((void *) VIRTUAL_TO_PHYSICAL(list), GET_GRAPH_NODE_LAYER(node->fnNode.node.flags));
        }
    }
    if (node->fnNode.node.children != NULL) {
        geo_process_node_and_siblings(node->fnNode.node.children);
    }
}

/**
 * Process a background node. Tries to retrieve a background display list from
 * the function of the node. If that function is null or returns null, a black
 * rectangle is drawn instead.
 */
void geo_process_background(struct GraphNodeBackground *node) {
    Gfx *list = NULL;

    if (node->fnNode.func != NULL) {
        list = node->fnNode.func(GEO_CONTEXT_RENDER, &node->fnNode.node,
                                 (struct AllocOnlyPool *) gMatStack[gMatStackIndex]);
    }
    if (list != NULL) {
        geo_append_display_list((void *) VIRTUAL_TO_PHYSICAL(list), GET_GRAPH_NODE_LAYER(node->fnNode.node.flags));
    } else if (gCurGraphNodeMasterList != NULL) {
#ifndef F3DEX_GBI_2E
        Gfx *gfxStart = alloc_display_list(sizeof(Gfx) * 7);
#else
        Gfx *gfxStart = alloc_display_list(sizeof(Gfx) * 8);
#endif
        Gfx *gfx = gfxStart;

        gDPPipeSync(gfx++);
        gDPSetCycleType(gfx++, G_CYC_FILL);
        gDPSetFillColor(gfx++, node->background);
        gDPFillRectangle(gfx++, GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0), gBorderHeight,
        GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0) - 1, SCREEN_HEIGHT - gBorderHeight - 1);
        gDPPipeSync(gfx++);
        gDPSetCycleType(gfx++, G_CYC_1CYCLE);
        gSPEndDisplayList(gfx++);

        geo_append_display_list((void *) VIRTUAL_TO_PHYSICAL(gfxStart), LAYER_FORCE);
    }
    if (node->fnNode.node.children != NULL) {
        geo_process_node_and_siblings(node->fnNode.node.children);
    }
}

/**
 * Render an animated part. The current animation state is not part of the node
 * but set in global variables. If an animated part is skipped, everything afterwards desyncs.
 */
void geo_process_animated_part(struct GraphNodeAnimatedPart *node) {
    Vec3s rotation = { 0, 0, 0 };
    Vec3f translation = { node->translation[0], node->translation[1], node->translation[2] };

    if (gCurrAnimType == ANIM_TYPE_TRANSLATION) {
        translation[0] += gCurrAnimData[retrieve_animation_index(gCurrAnimFrame, &gCurrAnimAttribute)]
                          * gCurrAnimTranslationMultiplier;
        translation[1] += gCurrAnimData[retrieve_animation_index(gCurrAnimFrame, &gCurrAnimAttribute)]
                          * gCurrAnimTranslationMultiplier;
        translation[2] += gCurrAnimData[retrieve_animation_index(gCurrAnimFrame, &gCurrAnimAttribute)]
                          * gCurrAnimTranslationMultiplier;
        gCurrAnimType = ANIM_TYPE_ROTATION;
    } else {
        if (gCurrAnimType == ANIM_TYPE_LATERAL_TRANSLATION) {
            translation[0] +=
                gCurrAnimData[retrieve_animation_index(gCurrAnimFrame, &gCurrAnimAttribute)]
                * gCurrAnimTranslationMultiplier;
            gCurrAnimAttribute += 2;
            translation[2] +=
                gCurrAnimData[retrieve_animation_index(gCurrAnimFrame, &gCurrAnimAttribute)]
                * gCurrAnimTranslationMultiplier;
            gCurrAnimType = ANIM_TYPE_ROTATION;
        } else {
            if (gCurrAnimType == ANIM_TYPE_VERTICAL_TRANSLATION) {
                gCurrAnimAttribute += 2;
                translation[1] +=
                    gCurrAnimData[retrieve_animation_index(gCurrAnimFrame, &gCurrAnimAttribute)]
                    * gCurrAnimTranslationMultiplier;
                gCurrAnimAttribute += 2;
                gCurrAnimType = ANIM_TYPE_ROTATION;
            } else if (gCurrAnimType == ANIM_TYPE_NO_TRANSLATION) {
                gCurrAnimAttribute += 6;
                gCurrAnimType = ANIM_TYPE_ROTATION;
            }
        }
    }

    if (gCurrAnimType == ANIM_TYPE_ROTATION) {
        rotation[0] = gCurrAnimData[retrieve_animation_index(gCurrAnimFrame, &gCurrAnimAttribute)];
        rotation[1] = gCurrAnimData[retrieve_animation_index(gCurrAnimFrame, &gCurrAnimAttribute)];
        rotation[2] = gCurrAnimData[retrieve_animation_index(gCurrAnimFrame, &gCurrAnimAttribute)];
    }

    mtxf_rotate_xyz_and_translate_and_mul(rotation, translation, gMatStack[gMatStackIndex + 1], gMatStack[gMatStackIndex]);

    inc_mat_stack();
    append_dl_and_return(((struct GraphNodeDisplayList *)node));
}

/**
 * Initialize the animation-related global variables for the currently drawn
 * object's animation.
 */
void geo_set_animation_globals(struct AnimInfo *node, s32 hasAnimation) {
    struct Animation *anim = node->curAnim;

    if (hasAnimation) {
        node->animFrame = geo_update_animation_frame(node, &node->animFrameAccelAssist);
    }
    node->animTimer = gAreaUpdateCounter;
    if (anim->flags & ANIM_FLAG_HOR_TRANS) {
        gCurrAnimType = ANIM_TYPE_VERTICAL_TRANSLATION;
    } else if (anim->flags & ANIM_FLAG_VERT_TRANS) {
        gCurrAnimType = ANIM_TYPE_LATERAL_TRANSLATION;
    } else if (anim->flags & ANIM_FLAG_NO_TRANS) {
        gCurrAnimType = ANIM_TYPE_NO_TRANSLATION;
    } else {
        gCurrAnimType = ANIM_TYPE_TRANSLATION;
    }

    gCurrAnimFrame = node->animFrame;
    gCurrAnimEnabled = (anim->flags & ANIM_FLAG_DISABLED) == 0;
    gCurrAnimAttribute = segmented_to_virtual((void *) anim->index);
    gCurrAnimData = segmented_to_virtual((void *) anim->values);

    if (anim->animYTransDivisor == 0) {
        gCurrAnimTranslationMultiplier = 1.0f;
    } else {
        gCurrAnimTranslationMultiplier = (f32) node->animYTrans / (f32) anim->animYTransDivisor;
    }
}

/**
 * Process a shadow node. Renders a shadow under an object offset by the
 * translation of the first animated component and rotated according to
 * the floor below it.
 */
void geo_process_shadow(struct GraphNodeShadow *node) {
#ifndef DISABLE_SHADOWS
    if (gCurGraphNodeCamera != NULL && gCurGraphNodeObject != NULL) {
        Vec3f shadowPos;
        f32 shadowScale;

        if (gCurGraphNodeHeldObject != NULL) {
            vec3f_copy(shadowPos, gMatStack[gMatStackIndex][3]);
            shadowScale = node->shadowScale * gCurGraphNodeHeldObject->objNode->header.gfx.scale[0];
        } else {
            vec3f_copy(shadowPos, gCurGraphNodeObject->pos);
            shadowScale = node->shadowScale * gCurGraphNodeObject->scale[0];
        }

        s8 shifted = (gCurrAnimEnabled
                      && (gCurrAnimType == ANIM_TYPE_TRANSLATION
                       || gCurrAnimType == ANIM_TYPE_LATERAL_TRANSLATION)
        );

        if (shifted) {
            struct GraphNode *geo = node->node.children;
            f32 objScale = 1.0f;
            if (geo != NULL && geo->type == GRAPH_NODE_TYPE_SCALE) {
                objScale = ((struct GraphNodeScale *) geo)->scale;
            }

            f32 animScale = gCurrAnimTranslationMultiplier * objScale;
            Vec3f animOffset;
            animOffset[0] = gCurrAnimData[retrieve_animation_index(gCurrAnimFrame, &gCurrAnimAttribute)] * animScale;
            animOffset[1] = 0.0f;
            gCurrAnimAttribute += 2;
            animOffset[2] = gCurrAnimData[retrieve_animation_index(gCurrAnimFrame, &gCurrAnimAttribute)] * animScale;
            gCurrAnimAttribute -= 6;

            // simple matrix rotation so the shadow offset rotates along with the object
            f32 sinAng = sins(gCurGraphNodeObject->angle[1]);
            f32 cosAng = coss(gCurGraphNodeObject->angle[1]);

            shadowPos[0] += animOffset[0] * cosAng + animOffset[2] * sinAng;
            shadowPos[2] += -animOffset[0] * sinAng + animOffset[2] * cosAng;
        }

        Gfx *shadowList = create_shadow_below_xyz(shadowPos, shadowScale * 0.5f,
                                                  node->shadowSolidity, node->shadowType, shifted);

        if (shadowList != NULL) {
            mtxf_shadow(gMatStack[gMatStackIndex + 1],
                gCurrShadow.floorNormal, shadowPos, gCurrShadow.scale, gCurGraphNodeObject->angle[1]);

            inc_mat_stack();
            geo_append_display_list(
                (void *) VIRTUAL_TO_PHYSICAL(shadowList),
                gCurrShadow.isDecal ? LAYER_TRANSPARENT_DECAL : LAYER_TRANSPARENT
            );

            gMatStackIndex--;
        }
    }
#endif
    if (node->node.children != NULL) {
        geo_process_node_and_siblings(node->node.children);
    }
}

/**
 * Check whether an object is in view to determine whether if it should be drawn.
 * This is known as frustum culling.
 * It checks whether the object is far away, very close or behind the camera and 
 * vertically or horizontally out of view. 
 * The radius used is specified in DEFAULT_CULLING_RADIUS unless the object 
 * has a culling radius node that specifies another value.
 * 
 * The matrix parameter should be the top of the matrix stack, which is the
 * object's transformation matrix times the camera 'look-at' matrix. The math
 * is counter-intuitive, but it checks column 3 (translation vector) of this
 * matrix to determine where the origin (0,0,0) in object space will be once
 * transformed to camera space (x+ = right, y+ = up, z = 'coming out the screen').
 * 
 * In 3D graphics, you typically model the world as being moved in front of a
 * static camera instead of a moving camera through a static world, which in
 * this case simplifies calculations. Note that the perspective matrix is not
 * on the matrix stack, so there are still calculations with the fov to compute
 * the slope of the lines of the frustum, these are done once during geo_process_perspective.
 *
 *        z-
 *
 *  \     |     /
 *   \    |    /
 *    \   |   /
 *     \  |  /
 *      \ | /
 *       \|/
 *        C       x+
 *
 * Since (0,0,0) is unaffected by rotation, columns 0, 1 and 2 are ignored.
 */

#define NO_CULLING_EMULATOR_BLACKLIST (EMU_CONSOLE | EMU_WIIVC | EMU_ARES | EMU_SIMPLE64 | EMU_CEN64)

s32 obj_is_in_view(struct GraphNodeObject *node) {
    struct GraphNode *geo = node->sharedChild;

    s16 cullingRadius;

    if (geo != NULL && geo->type == GRAPH_NODE_TYPE_CULLING_RADIUS) {
        cullingRadius = ((struct GraphNodeCullingRadius *) geo)->cullingRadius;
    } else {
        cullingRadius = DEFAULT_CULLING_RADIUS;
    }

    // Check whether the object is not too far away or too close / behind the camera.
    // This makes the HOLP not update when the camera is far away, and it
    // makes PU travel safe when the camera is locked on the main map.
    // If Mario were rendered with a depth over 65536 it would cause overflow
    // when converting the transformation matrix to a fixed point matrix.
    f32 cameraToObjectDepth = node->cameraToObject[2];

    #define VALID_DEPTH_MIDDLE (-20100.f / 2.f)
    #define VALID_DEPTH_RANGE (19900 / 2.f)
    if (absf(cameraToObjectDepth - VALID_DEPTH_MIDDLE) >= VALID_DEPTH_RANGE + cullingRadius) {
        return FALSE;
    }

#ifndef CULLING_ON_EMULATOR
    // If an emulator is detected, skip any other culling.
    if(!(gEmulator & NO_CULLING_EMULATOR_BLACKLIST)){
        return TRUE;
    }
#endif

#ifdef VERTICAL_CULLING
    f32 vScreenEdge = -cameraToObjectDepth * gCurGraphNodeCamFrustum->halfFovVertical;

    // Unlike with horizontal culling, we only check if the object is bellow the screen
    // to prevent shadows from being culled.
    if (node->cameraToObject[1] < -vScreenEdge - cullingRadius) {
        return FALSE;
    }

#endif
    
    f32 hScreenEdge = -cameraToObjectDepth * gCurGraphNodeCamFrustum->halfFovHorizontal;

    if (absf(node->cameraToObject[0]) > hScreenEdge + cullingRadius) {
        return FALSE;
    }
    return TRUE;
}

#ifdef VISUAL_DEBUG
void visualise_object_hitbox(struct Object *node) {
    Vec3f bnds1, bnds2;
    // This will create a cylinder that visualises their hitbox.
    // If they do not have a hitbox, it will be a small white cube instead.
    if (node->oIntangibleTimer != -1) {
        vec3f_set(bnds1, node->oPosX, (node->oPosY - node->hitboxDownOffset), node->oPosZ);
        vec3f_set(bnds2, node->hitboxRadius, node->hitboxHeight-node->hitboxDownOffset, node->hitboxRadius);
        if (node->behavior == segmented_to_virtual(bhvWarp)
            || node->behavior == segmented_to_virtual(bhvDoorWarp)
            || node->behavior == segmented_to_virtual(bhvFadingWarp)) {
            debug_box_color(COLOR_RGBA32_DEBUG_WARP);
        } else {
            debug_box_color(COLOR_RGBA32_DEBUG_HITBOX);
        }

        debug_box(bnds1, bnds2, (DEBUG_SHAPE_CYLINDER));
        vec3f_set(bnds1, node->oPosX, (node->oPosY - node->hitboxDownOffset), node->oPosZ);
        vec3f_set(bnds2, node->hurtboxRadius, node->hurtboxHeight, node->hurtboxRadius);
        debug_box_color(COLOR_RGBA32_DEBUG_HURTBOX);
        debug_box(bnds1, bnds2, (DEBUG_SHAPE_CYLINDER));
    } else {
        vec3f_set(bnds1, node->oPosX, (node->oPosY - 15), node->oPosZ);
        vec3f_set(bnds2, 30, 30, 30);
        debug_box_color(COLOR_RGBA32_DEBUG_POSITION);
        debug_box(bnds1, bnds2, (DEBUG_SHAPE_BOX));
    }
}
#endif

/**
 * Process an object node.
 */
void geo_process_object(struct Object *node) {
    if (node->header.gfx.areaIndex == gCurGraphNodeRoot->areaIndex) {
        s32 isInvisible = (node->header.gfx.node.flags & GRAPH_RENDER_INVISIBLE);
        s32 noThrowMatrix = (node->header.gfx.throwMatrix == NULL);
        // Maintain throw matrix pointer if the game is paused as it won't be updated.
        Mat4 *oldThrowMatrix = (sCurrPlayMode == PLAY_MODE_PAUSED) ? node->header.gfx.throwMatrix : NULL;

        // If the throw matrix is null and the object is invisible, there is no need
        // to update billboarding, scale, rotation, etc. 
        // This still updates translation since it is needed for sound.
        if (isInvisible && noThrowMatrix) {
            mtxf_translate(gMatStack[gMatStackIndex + 1], node->header.gfx.pos);
        }
        else{
            if (!noThrowMatrix) {
                mtxf_scale_vec3f(gMatStack[gMatStackIndex + 1], *node->header.gfx.throwMatrix, node->header.gfx.scale);
            } else if (node->header.gfx.node.flags & GRAPH_RENDER_BILLBOARD) {
                mtxf_billboard(gMatStack[gMatStackIndex + 1], gMatStack[gMatStackIndex],
                            node->header.gfx.pos, node->header.gfx.scale, gCurGraphNodeCamera->roll);
            } else {
                mtxf_rotate_zxy_and_translate(gMatStack[gMatStackIndex + 1], node->header.gfx.pos, node->header.gfx.angle);
                mtxf_scale_vec3f(gMatStack[gMatStackIndex + 1], gMatStack[gMatStackIndex + 1], node->header.gfx.scale);
            }
        }

        node->header.gfx.throwMatrix = &gMatStack[++gMatStackIndex];
        linear_mtxf_mul_vec3f_and_translate(gCameraTransform, node->header.gfx.cameraToObject, (*node->header.gfx.throwMatrix)[3]);

        // FIXME: correct types
        if (node->header.gfx.animInfo.curAnim != NULL) {
            geo_set_animation_globals(&node->header.gfx.animInfo, (node->header.gfx.node.flags & GRAPH_RENDER_HAS_ANIMATION) != 0);
        }

        if (!isInvisible && obj_is_in_view(&node->header.gfx)) {
            gMatStackIndex--;
            inc_mat_stack();

            if (node->header.gfx.sharedChild != NULL) {
#ifdef VISUAL_DEBUG
                if (hitboxView) visualise_object_hitbox(node);
#endif
                gCurGraphNodeObject = (struct GraphNodeObject *) node;
                node->header.gfx.sharedChild->parent = &node->header.gfx.node;
                geo_process_node_and_siblings(node->header.gfx.sharedChild);
                node->header.gfx.sharedChild->parent = NULL;
                gCurGraphNodeObject = NULL;
            }
            if (node->header.gfx.node.children != NULL) {
                geo_process_node_and_siblings(node->header.gfx.node.children);
            }
        }

        gMatStackIndex--;
        gCurrAnimType = ANIM_TYPE_NONE;
        node->header.gfx.throwMatrix = oldThrowMatrix;
    }
}

/**
 * Process an object parent node. Temporarily assigns itself as the parent of
 * the subtree rooted at 'sharedChild' and processes the subtree, after which the
 * actual children are be processed. (in practice they are null though)
 */
void geo_process_object_parent(struct GraphNodeObjectParent *node) {
    if (node->sharedChild != NULL) {
        node->sharedChild->parent = (struct GraphNode *) node;
        geo_process_node_and_siblings(node->sharedChild);
        node->sharedChild->parent = NULL;
    }
    if (node->node.children != NULL) {
        geo_process_node_and_siblings(node->node.children);
    }
}

/**
 * Process a held object node.
 */
void geo_process_held_object(struct GraphNodeHeldObject *node) {
    Mat4 mat;
    Vec3f translation;
    Mat4 tempMtx;

#ifdef F3DEX_GBI_2
    gSPLookAt(gDisplayListHead++, gCurLookAt);
#endif

    if (node->fnNode.func != NULL) {
        node->fnNode.func(GEO_CONTEXT_RENDER, &node->fnNode.node, gMatStack[gMatStackIndex]);
    }
    if (node->objNode != NULL && node->objNode->header.gfx.sharedChild != NULL) {
        vec3_scale_dest(translation, node->translation, 0.25f);

        mtxf_translate(mat, translation);
        mtxf_copy(gMatStack[gMatStackIndex + 1], *gCurGraphNodeObject->throwMatrix);
        vec3f_copy(gMatStack[gMatStackIndex + 1][3], gMatStack[gMatStackIndex][3]);
        mtxf_copy(tempMtx, gMatStack[gMatStackIndex + 1]);
        mtxf_mul(gMatStack[gMatStackIndex + 1], mat, tempMtx);
        mtxf_scale_vec3f(gMatStack[gMatStackIndex + 1], gMatStack[gMatStackIndex + 1], node->objNode->header.gfx.scale);

        if (node->fnNode.func != NULL) {
            node->fnNode.func(GEO_CONTEXT_HELD_OBJ, &node->fnNode.node, (struct AllocOnlyPool *) gMatStack[gMatStackIndex + 1]);
        }

        inc_mat_stack();
        gGeoTempState.type = gCurrAnimType;
        gGeoTempState.enabled = gCurrAnimEnabled;
        gGeoTempState.frame = gCurrAnimFrame;
        gGeoTempState.translationMultiplier = gCurrAnimTranslationMultiplier;
        gGeoTempState.attribute = gCurrAnimAttribute;
        gGeoTempState.data = gCurrAnimData;
        gCurrAnimType = ANIM_TYPE_NONE;
        gCurGraphNodeHeldObject = (void *) node;
        if (node->objNode->header.gfx.animInfo.curAnim != NULL) {
            geo_set_animation_globals(&node->objNode->header.gfx.animInfo, (node->objNode->header.gfx.node.flags & GRAPH_RENDER_HAS_ANIMATION) != 0);
        }

        geo_process_node_and_siblings(node->objNode->header.gfx.sharedChild);
        gCurGraphNodeHeldObject = NULL;
        gCurrAnimType = gGeoTempState.type;
        gCurrAnimEnabled = gGeoTempState.enabled;
        gCurrAnimFrame = gGeoTempState.frame;
        gCurrAnimTranslationMultiplier = gGeoTempState.translationMultiplier;
        gCurrAnimAttribute = gGeoTempState.attribute;
        gCurrAnimData = gGeoTempState.data;
        gMatStackIndex--;
    }

    if (node->fnNode.node.children != NULL) {
        geo_process_node_and_siblings(node->fnNode.node.children);
    }
}

/**
 * Processes the children of the given GraphNode if it has any
 */
void geo_try_process_children(struct GraphNode *node) {
    if (node->children != NULL) {
        geo_process_node_and_siblings(node->children);
    }
}

typedef void (*GeoProcessFunc)();

// See enum 'GraphNodeTypes' in 'graph_node.h'.
static GeoProcessFunc GeoProcessJumpTable[] = {
    [GRAPH_NODE_TYPE_ORTHO_PROJECTION    ] = geo_process_ortho_projection,
    [GRAPH_NODE_TYPE_PERSPECTIVE         ] = geo_process_perspective,
    [GRAPH_NODE_TYPE_MASTER_LIST         ] = geo_process_master_list,
    [GRAPH_NODE_TYPE_LEVEL_OF_DETAIL     ] = geo_process_level_of_detail,
    [GRAPH_NODE_TYPE_SWITCH_CASE         ] = geo_process_switch,
    [GRAPH_NODE_TYPE_CAMERA              ] = geo_process_camera,
    [GRAPH_NODE_TYPE_TRANSLATION_ROTATION] = geo_process_translation_rotation,
    [GRAPH_NODE_TYPE_TRANSLATION         ] = geo_process_translation,
    [GRAPH_NODE_TYPE_ROTATION            ] = geo_process_rotation,
    [GRAPH_NODE_TYPE_OBJECT              ] = geo_process_object,
    [GRAPH_NODE_TYPE_ANIMATED_PART       ] = geo_process_animated_part,
    [GRAPH_NODE_TYPE_BILLBOARD           ] = geo_process_billboard,
    [GRAPH_NODE_TYPE_DISPLAY_LIST        ] = geo_process_display_list,
    [GRAPH_NODE_TYPE_SCALE               ] = geo_process_scale,
    [GRAPH_NODE_TYPE_SHADOW              ] = geo_process_shadow,
    [GRAPH_NODE_TYPE_OBJECT_PARENT       ] = geo_process_object_parent,
    [GRAPH_NODE_TYPE_GENERATED_LIST      ] = geo_process_generated_list,
    [GRAPH_NODE_TYPE_BACKGROUND          ] = geo_process_background,
    [GRAPH_NODE_TYPE_HELD_OBJ            ] = geo_process_held_object,
    [GRAPH_NODE_TYPE_CULLING_RADIUS      ] = geo_try_process_children,
    [GRAPH_NODE_TYPE_ROOT                ] = geo_try_process_children,
    [GRAPH_NODE_TYPE_START               ] = geo_try_process_children,
};

/**
 * Process a generic geo node and its siblings.
 * The first argument is the start node, and all its siblings will
 * be iterated over.
 */
void geo_process_node_and_siblings(struct GraphNode *firstNode) {
    s32 iterateChildren = TRUE;
    struct GraphNode *curGraphNode = firstNode;
    struct GraphNode *parent = curGraphNode->parent;

    // In the case of a switch node, exactly one of the children of the node is
    // processed instead of all children like usual
    if (parent != NULL) {
        iterateChildren = (parent->type != GRAPH_NODE_TYPE_SWITCH_CASE);
    }

    do {
        if (curGraphNode->flags & GRAPH_RENDER_ACTIVE) {
            if (curGraphNode->flags & GRAPH_RENDER_CHILDREN_FIRST) {
                geo_try_process_children(curGraphNode);
            } else {
                GeoProcessJumpTable[curGraphNode->type](curGraphNode);
            }
        } else {
            if (curGraphNode->type == GRAPH_NODE_TYPE_OBJECT) {
                ((struct GraphNodeObject *) curGraphNode)->throwMatrix = NULL;
            }
        }
    } while (iterateChildren && (curGraphNode = curGraphNode->next) != firstNode);
}

/**
 * Process a root node. This is the entry point for processing the scene graph.
 * The root node itself sets up the viewport, then all its children are processed
 * to set up the projection and draw display lists.
 */
f32 screenXMultiplyer;
f32 screenYMultiplyer;
f32 sizeXMultiplyer;
f32 sizeYMultiplyer;

void geo_process_root(struct GraphNodeRoot *node, Vp *b, Vp *c, s32 clearColor) {
    if (node->node.flags & GRAPH_RENDER_ACTIVE) {
        Mtx *initialMatrix;
        Vp *viewport = alloc_display_list(sizeof(*viewport));

        gDisplayListHeap = alloc_only_pool_init(main_pool_available() - sizeof(struct AllocOnlyPool), MEMORY_POOL_LEFT);
        initialMatrix = alloc_display_list(sizeof(*initialMatrix));
        gCurLookAt = (LookAt*)alloc_display_list(sizeof(LookAt));
        bzero(gCurLookAt, sizeof(LookAt));

        gMatStackIndex = 0;
        gCurrAnimType = ANIM_TYPE_NONE;
		
		screenYMultiplyer = 4;
		screenXMultiplyer = 4;
		
        if (gCurrLevelNum > 0) {
			vec3s_set(viewport->vp.vtrans, node->x * screenXMultiplyer, node->y * screenYMultiplyer, 511);
		} else {
			vec3s_set(viewport->vp.vtrans, node->x * 4, node->y * 4, 511);
		}
			//rainbowLED[0] = (coss(gGlobalTimer * 200         ) + 1) * 127; //puppyprint rainbow code
			//rainbowLED[1] = (coss((gGlobalTimer * 200) + 21845) + 1) * 127;
			//rainbowLED[2] = (coss((gGlobalTimer * 200) - 21845) + 1) * 127;
		if (gLakituState.curPos[1] < gMarioState->waterLevel) {
			sizeYMultiplyer = 4.9f + coss(gGlobalTimer * 250);
			sizeXMultiplyer = 4.9f + sins(gGlobalTimer * 125);
		} else {
			sizeYMultiplyer = 4;
			sizeXMultiplyer = 4;
		}
        vec3s_set(viewport->vp.vscale, node->width * sizeXMultiplyer, node->height * sizeYMultiplyer, 511);
		
		
		//vec3s_set(viewport->vp.vtrans, gScreenWidth * 2, gScreenHeight * 2, 511);
        //vec3s_set(viewport->vp.vscale, gScreenWidth * 2, gScreenHeight * 2, 511);

        if (b != NULL) {
            clear_framebuffer(clearColor);
            make_viewport_clip_rect(b);
            *viewport = *b;
        }

        else if (c != NULL) {
            clear_framebuffer(clearColor);
            make_viewport_clip_rect(c);
        }

        mtxf_identity(gMatStack[gMatStackIndex]);
        mtxf_to_mtx(initialMatrix, gMatStack[gMatStackIndex]);
        gMatStackFixed[gMatStackIndex] = initialMatrix;
        gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(viewport));
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(gMatStackFixed[gMatStackIndex]),
                  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gCurGraphNodeRoot = node;
        if (node->node.children != NULL) {
            geo_process_node_and_siblings(node->node.children);
        }
        gCurGraphNodeRoot = NULL;
#ifdef VANILLA_DEBUG
        if (gShowDebugText) {
            print_text_fmt_int(180, 36, "MEM %d", gDisplayListHeap->totalSpace - gDisplayListHeap->usedSpace);
        }
#endif
        main_pool_free(gDisplayListHeap);
    }
}
