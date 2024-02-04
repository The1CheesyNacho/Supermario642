#include <PR/ultratypes.h>

#include "sm64.h"
#include "area.h"
#include "audio/external.h"
#include "audio/load.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "camera.h"
#include "engine/graph_node.h"
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "game_init.h"
#include "interaction.h"
#include "level_table.h"
#include "level_update.h"
#include "main.h"
#include "mario.h"
#include "mario_actions_airborne.h"
#include "mario_actions_automatic.h"
#include "mario_actions_cutscene.h"
#include "mario_actions_moving.h"
#include "mario_actions_object.h"
#include "mario_actions_stationary.h"
#include "mario_actions_submerged.h"
#include "mario_misc.h"
#include "mario_step.h"
#include "memory.h"
#include "object_fields.h"
#include "object_helpers.h"
#include "object_list_processor.h"
#include "print.h"
#include "save_file.h"
#include "sound_init.h"
#include "rumble_init.h"
void init_luigi(void) {
    gMarioStates[1].actionTimer = 0;
    gMarioStates[1].framesSinceA = 0xFF;
    gMarioStates[1].framesSinceB = 0xFF;

    gMarioStates[1].invincTimer = 0;

    gMarioStates[1].flags = (MARIO_NORMAL_CAP | MARIO_CAP_ON_HEAD);
    gMarioStates[1].forwardVel = 0.0f;
    gMarioStates[1].squishTimer = 0;

    gMarioStates[1].hurtCounter = 0;
    gMarioStates[1].healCounter = 0;

    gMarioStates[1].capTimer = 0;
    gMarioStates[1].quicksandDepth = 0.0f;

    gMarioStates[1].heldObj = NULL;
    gMarioStates[1].riddenObj = NULL;
    gMarioStates[1].usedObj = NULL;

    gMarioStates[1].waterLevel =
        find_water_level(gPlayerSpawnInfos[1].startPos[0], gPlayerSpawnInfos[1].startPos[2]);

    gMarioStates[1].area = gCurrentArea;
    gMarioStates[1].marioObj = gLuigiObject;
    gMarioStates[1].marioObj->header.gfx.animInfo.animID = -1;
    vec3s_copy(gMarioStates[1].faceAngle, gPlayerSpawnInfos[1].startAngle);
    vec3s_set(gMarioStates[1].angleVel, 0, 0, 0);
    vec3s_to_vec3f(gMarioStates[1].pos, gPlayerSpawnInfos[1].startPos);
    vec3f_set(gMarioStates[1].vel, 0, 0, 0);
    gMarioStates[1].floorHeight =
        find_floor(gMarioStates[1].pos[0], gMarioStates[1].pos[1], gMarioStates[1].pos[2], &gMarioStates[1].floor);

    if (gMarioStates[1].pos[1] < gMarioStates[1].floorHeight) {
        gMarioStates[1].pos[1] = gMarioStates[1].floorHeight;
    }

    gMarioStates[1].marioObj->header.gfx.pos[1] = gMarioStates[1].pos[1];

    gMarioStates[1].action =
        (gMarioStates[1].pos[1] <= (gMarioStates[1].waterLevel - 100)) ? ACT_WATER_IDLE : ACT_IDLE;

    mario_reset_bodystate(&gMarioStates[0]);
    update_mario_info_for_cam(&gMarioStates[0]);
    gMarioStates[1].marioBodyState->punchState = 0;

    gMarioStates[1].marioObj->oPosX = gMarioStates[1].pos[0];
    gMarioStates[1].marioObj->oPosY = gMarioStates[1].pos[1];
    gMarioStates[1].marioObj->oPosZ = gMarioStates[1].pos[2];

    gMarioStates[1].marioObj->oMoveAnglePitch = gMarioStates[1].faceAngle[0];
    gMarioStates[1].marioObj->oMoveAngleYaw = gMarioStates[1].faceAngle[1];
    gMarioStates[1].marioObj->oMoveAngleRoll = gMarioStates[1].faceAngle[2];



    vec3f_copy(gMarioStates[1].marioObj->header.gfx.pos, gMarioStates[1].pos);
    vec3s_set(gMarioStates[1].marioObj->header.gfx.angle, 0, gMarioStates[1].faceAngle[1], 0);
}


void init_luigi_from_save_file(void) {
    gLuigiState->playerID = 1;
    gLuigiState->flags = 0;
    gLuigiState->action = 0;
    gLuigiState->spawnInfo = &gPlayerSpawnInfos[1];
    gLuigiState->statusForCamera = &gPlayerCameraState[1];
    gLuigiState->marioBodyState = &gBodyStates[1];
    gLuigiState->controller = &gControllers[1];
    gLuigiState->numCoins = 0;
    gLuigiState->numStars =
        save_file_get_total_star_count(gCurrSaveFileNum - 1, COURSE_MIN - 1, COURSE_MAX - 1);
    gLuigiState->numKeys = 0;

    gLuigiState->numLives = 4;
    gLuigiState->health = 0x880;

    gLuigiState->prevNumStarsForDialog = gLuigiState->numStars;
    gLuigiState->animYTrans = 0xBD;

    gHudDisplay.coins = 0;
    gHudDisplay.wedges = 8;
}