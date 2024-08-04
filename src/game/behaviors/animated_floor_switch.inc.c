// animated_floor_switch.inc.c

struct FloorSwitchTriggeredAnimationFrame {
    const Collision *collision;
    ModelID16 model;
};

struct FloorSwitchTriggeredAnimationFrame sFloorSwitchTriggeredAnimationFrames[][5] = {
};

s16 sAnimatesOnFloorSwitchPressTimers[] = { 250, 200, 200 };

void bhv_animates_on_floor_switch_press_init(void) {
    o->parentObj = cur_obj_nearest_object_with_behavior(bhvFloorSwitchAnimatesObject);
}

void bhv_animates_on_floor_switch_press_loop(void) {
    if (o->oFloorSwitchPressAnimationSwitchNotTicking != 0) {
        if (o->parentObj->oAction != 2) {
            o->oFloorSwitchPressAnimationSwitchNotTicking = 0;
        }

        if (o->oFloorSwitchPressAnimationDoResetTime != 0) {
            o->oFloorSwitchPressAnimationTickTimer = sAnimatesOnFloorSwitchPressTimers[o->oBehParams2ndByte];
        } else {
            o->oFloorSwitchPressAnimationTickTimer = 0;
        }
    } else if (o->parentObj->oAction == 2) {
        o->oFloorSwitchPressAnimationDoResetTime ^= 1;
        o->oFloorSwitchPressAnimationSwitchNotTicking = 1;
    }

    if (o->oFloorSwitchPressAnimationTickTimer != 0) {
        if (o->oFloorSwitchPressAnimationTickTimer < 60) {
            cur_obj_play_sound_1(SOUND_GENERAL2_SWITCH_TICK_SLOW);
        } else {
            cur_obj_play_sound_1(SOUND_GENERAL2_SWITCH_TICK_FAST);
        }

        if (--o->oFloorSwitchPressAnimationTickTimer == 0) {
            o->oFloorSwitchPressAnimationDoResetTime = 0;
        }

        if (o->oFloorSwitchPressAnimationDoubleFrame < 9) {
            o->oFloorSwitchPressAnimationDoubleFrame++;
        }
    } else if ((o->oFloorSwitchPressAnimationDoubleFrame -= 2) < 0) {
        o->oFloorSwitchPressAnimationDoubleFrame = 0;
        o->oFloorSwitchPressAnimationDoResetTime = 1;
    }

    o->collisionData = segmented_to_virtual(
        sFloorSwitchTriggeredAnimationFrames[o->oBehParams2ndByte][o->oFloorSwitchPressAnimationDoubleFrame / 2].collision);

    cur_obj_set_model(sFloorSwitchTriggeredAnimationFrames[o->oBehParams2ndByte][o->oFloorSwitchPressAnimationDoubleFrame / 2].model);
}
