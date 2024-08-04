void bhv_gum_update(void) {
	f32 Yspd = 64.0f;
	u32 CarmackMeme;
	if (cur_obj_is_mario_on_platform()) {
		//this is awful
		if (gMarioStates[0].controller->buttonPressed & A_BUTTON) {
			Yspd = 200;
		}
		set_mario_action(gMarioState, ACT_DOUBLE_JUMP, 0);
		gMarioStates[0].vel[1] = Yspd;
		o->oAction = 1;
	}
	if (o->oAction == 1) {
		if (o->oTimer == 4) {
			o->oAction = 0;
			o->oPosY = o->oHomeY;
		} else {
			if (o->oTimer > 2) {
				o->oPosY = o->oHomeY + (f32)(o->oTimer % 3) * 6;
			} else {
				o->oPosY = o->oHomeY - (f32)(o->oTimer) * 6;
			}
		}
	}

            o->oForwardVel = -10.0f;
            if (o->oMoveFlags & OBJ_MOVE_LANDED) {
                o->oSubAction++;
                o->oVelY = 0.0f;
                o->oScuttlebugTimer = 0;
                o->oFlags |= OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
                o->oInteractStatus = INT_STATUS_NONE;
            }
}