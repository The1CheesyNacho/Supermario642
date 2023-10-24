// thwomp.inc.c

#include "src/engine/math_util.h"
#include "src/game/level_update.h"

void grindel_thwomp_act_on_ground(void) {
    if (o->oTimer == 0) {
        o->oThwompRandomTimer = random_float() * 10.0f + 20.0f;
    }
    if (o->oTimer > o->oThwompRandomTimer) {
        o->oAction = GRINDEL_THWOMP_ACT_RISING;
    }
}

void grindel_thwomp_act_falling(void) {
    o->oVelY += -4.0f;
    o->oPosY += o->oVelY;
    if (o->oPosY < o->oHomeY) {
        o->oPosY = o->oHomeY;
        o->oVelY = 0.0f;
        o->oAction = GRINDEL_THWOMP_ACT_LAND;
    }
}

void grindel_thwomp_act_land(void) {
    if (o->oTimer == 0 && o->oDistanceToMario < 1500.0f) {
        cur_obj_shake_screen(SHAKE_POS_SMALL);
        cur_obj_play_sound_2(SOUND_OBJ_THWOMP);
    }
    if (o->oTimer >= 10) {
        o->oAction = GRINDEL_THWOMP_ACT_ON_GROUND;
    }
}

void grindel_thwomp_act_floating(void) {
    if (o->oTimer == 0) {
        o->oThwompRandomTimer = random_float() * 30.0f + 10.0f;
    }
    if (o->oTimer > o->oThwompRandomTimer) {
        o->oAction = GRINDEL_THWOMP_ACT_FALLING;
    }
}

void grindel_thwomp_act_rising(void) {
    if (o->oBehParams2ndByte + 40 < o->oTimer) {
        o->oAction = GRINDEL_THWOMP_ACT_FLOATING;
        o->oPosY += 5.0f;
    } else {
        o->oPosY += 10.0f;
    }
}

ObjActionFunc sGrindelThwompActions[] = {
    grindel_thwomp_act_rising,
    grindel_thwomp_act_floating,
    grindel_thwomp_act_falling,
    grindel_thwomp_act_land,
    grindel_thwomp_act_on_ground
};

void bhv_grindel_thwomp_loop(void) {
    cur_obj_call_action_function(sGrindelThwompActions);
}

void bhv_thwimp_init(void) {
    o->oThwimpState = 0;
    o->oThwimpTimer = 30;
    o->oGravity = 2;
}

void bhv_thwimp_loop(void) {
    switch (o->oThwimpState) {
        case 0: // waiting
            if (o->oDistanceToMario < 4000) o->oThwimpTimer--;
            if (o->oThwimpTimer == 0) o->oThwimpState = 1;
            break;
        case 1: // rotating
            int diff = signum(o->oAngleToMario - o->oFaceAngleYaw);
            o->oFaceAngleYaw = approach_angle(o->oFaceAngleYaw, o->oAngleToMario, 1024);
            if (diff != signum(o->oAngleToMario - o->oFaceAngleYaw)) {
                o->oMoveAngleYaw = o->oFaceAngleYaw;
                o->oThwimpState = 2;
                o->oForwardVel = 5;
                o->oVelY = 50;
            }
            break;
        case 2: // jumping
            o->oVelY -= o->oGravity;
            cur_obj_move_using_vel();
            cur_obj_move_xz_using_fvel_and_yaw();
            cur_obj_if_hit_wall_bounce_away();
            cur_obj_update_floor_and_walls();
            s16 collisionFlags = object_step();
            if (collisionFlags & OBJ_COL_FLAG_GROUNDED) {
                o->oThwimpState = 0;
                o->oThwimpTimer = 30;
                o->oForwardVel = 0;
                o->oVelY = 0;
            }
            break;
    }
}