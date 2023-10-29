#include "game/cutscene.h"

#include "game/object_helpers.h"
#include "include/behavior_data.h"
#include "include/seq_ids.h"
#include "audio/external.h"
#include "game/camera.h"

u8 cutscene_active = 0;
CutsceneActor cutscene_actors[32];
CutsceneKeyframe cutscene_keyframes[256];
CutsceneEvent cutscene_events[256];
u32 current_frame = 0;
u32 cutscene_num_keyframes = 0;
u32 cutscene_num_events = 0;

Cutscene* cutscenes[] = {
    { // cutscene_test
        CUTSCENE_SCRIPT_END()
    }
};

void cutscene_start_func(Cutscene cutscene) {
    u32 prev_frame = 0;
    u32 frame = 0;
    u32 temp;
    u32 i;
    cutscene_stop();
    enable_time_stop_including_mario();
    cutscene_active = 1;
    current_frame = 0;
    cutscene_num_keyframes = 0;
    for (i = 0; i < 32; i++) {
        cutscene_actors[i] = spawn_object(NULL, MODEL_NONE, bhvCutsceneActor);
    }
    while (cutscene[0]) {
        switch ((u8)cutscene[0]) {
            case 1:
                prev_frame = frame;
                frame = cutscene[1];
                break;
            case 2:
                prev_frame = frame;
                frame = cutscene[1];
                frame += prev_frame;
                break;
            case 3:
                temp = prev_frame;
                prev_frame = frame;
                frame = temp;
                break;
            case 4:
                obj_set_model(cutscene_actors[(u32)cutscene[1]], cutscene[2]);
                cutscene_actors[(u32)cutscene[1]]->oPosX = (s32)cutscene[3];
                cutscene_actors[(u32)cutscene[1]]->oPosY = (s32)cutscene[4];
                cutscene_actors[(u32)cutscene[1]]->oPosZ = (s32)cutscene[5];
                break;
            case 5:
                cutscene_keyframes[cutscene_num_keyframes].actor = (u8)cutscene[1];
                cutscene_keyframes[cutscene_num_keyframes].x = (s16)cutscene[2];
                cutscene_keyframes[cutscene_num_keyframes].y = (s16)cutscene[3];
                cutscene_keyframes[cutscene_num_keyframes].z = (s16)cutscene[4];
                cutscene_keyframes[cutscene_num_keyframes].interpolation = (u8)cutscene[5];
                cutscene_keyframes[cutscene_num_keyframes].frame = frame;
                cutscene_num_keyframes++;
                break;
            case 6:
                cutscene_events[cutscene_num_events].type = (u8)cutscene[1];
                cutscene_events[cutscene_num_events].arg1 = (u16)cutscene[2];
                cutscene_events[cutscene_num_events].arg2 = (u16)cutscene[3];
                cutscene_events[cutscene_num_events].arg3 = (u16)cutscene[4];
                cutscene_events[cutscene_num_events].arg4 = (u16)cutscene[5];
                cutscene_events[cutscene_num_events].frame = frame;
                cutscene_num_events++;
                break;
        }
        cutscene += 6;
    }
}

void cutscene_stop() {
    u32 i;
    if (!cutscene_active) return;
    disable_time_stop_including_mario();
    for (i = 0; i < 32; i++) {
        obj_mark_for_deletion(cutscene_actors[i]);
    }
    cutscene_active = 0;
}

void cutscene_step() {
    CutsceneKeyframe* next_keyframes[32];
    CutsceneKeyframe* prev_keyframes[32];
    u8 actor;
    u8 i;
    f32 interpolation;
    f32 yaw, pitch;
    struct Object* obj;
    if (!cutscene_active) return;
    for (i = 0; i < cutscene_num_keyframes; i++) {
        actor = cutscene_keyframes[i].actor;
        if (i < cutscene_keyframes[i].frame) {
            if (next_keyframes[actor] == NULL) next_keyframes[actor] = &cutscene_keyframes[i];
            else if (next_keyframes[actor]->frame < cutscene_keyframes[i].frame) next_keyframes[actor] = &cutscene_keyframes[i];
        }
        else {
            if (prev_keyframes[actor] == NULL) prev_keyframes[actor] = &cutscene_keyframes[i];
            else if (prev_keyframes[actor]->frame > cutscene_keyframes[i].frame) prev_keyframes[actor] = &cutscene_keyframes[i];
        }
    }
    for (i = 0; i < 32; i++) {
        if (next_keyframes[i] == NULL && prev_keyframes[i] == NULL) continue; 
        if (next_keyframes[i] == NULL && prev_keyframes[i] != NULL) next_keyframes[i] = prev_keyframes[i];
        if (prev_keyframes[i] == NULL && next_keyframes[i] != NULL) prev_keyframes[i] = next_keyframes[i];
        interpolation = (f32)(current_frame - prev_keyframes[i]->frame) / (f32)(next_keyframes[i]->frame - prev_keyframes[i]->frame);
        switch (prev_keyframes[i]->interpolation) {
            case (u8)CUTIP_FAST: interpolation = 1 - (1 - interpolation) * (1 - interpolation); break;
            case (u8)CUTIP_SLOW: interpolation = interpolation * interpolation; break;
            case (u8)CUTIP_SMOOTH: interpolation = interpolation < 0.5 ? 2 * interpolation * interpolation : 1 - ((-2 * interpolation + 2) * (-2 * interpolation + 2)) / 2; break;
            case (u8)CUTIP_WAIT: interpolation = floor(interpolation); break;
        }
        cutscene_actors[i]->oPosX = (f32)((next_keyframes[i]->x - prev_keyframes[i]->x) * interpolation + prev_keyframes[i]->x);
        cutscene_actors[i]->oPosY = (f32)((next_keyframes[i]->y - prev_keyframes[i]->y) * interpolation + prev_keyframes[i]->y);
        cutscene_actors[i]->oPosZ = (f32)((next_keyframes[i]->z - prev_keyframes[i]->z) * interpolation + prev_keyframes[i]->z);
    }
    for (i = 0; i < cutscene_num_events; i++) {
        if (cutscene_events[i].frame == current_frame) {
            switch (cutscene_events[i].type) {
                case (u8)CUTEV_END:
                    cutscene_stop();
                    break;
                case (u8)CUTEV_BGM:
                    play_music(SEQ_PLAYER_LEVEL, SEQUENCE_ARGS(4, cutscene_events[i].arg1), 0);
                    break;
                case (u8)CUTEV_SFX:
                    play_sound(cutscene_events[i].arg1, gLakituState.pos);
                    break;
                case (u8)CUTEV_STARTNEW:
                    cutscene_start_func(*cutscenes[cutscene_events[i].arg1]);
                    break;
                case (u8)CUTEV_SET_MODEL:
                    obj_set_model(cutscene_actors[cutscene_events[i].arg1], cutscene_events[i].arg2);
                    break;
                case (u8)CUTEV_SET_ANIM:
                    obj = cutscene_actors[cutscene_events[i].arg1];
                    geo_obj_init_animation(&obj->header.gfx, &obj->oAnimations[cutscene_events[i].arg2]);
                    break;
            }
        }
    }
    vec3f_set(gCamera->pos, cutscene_actors[CAMPOS]->oPosX, cutscene_actors[CAMPOS]->oPosY, cutscene_actors[CAMPOS]->oPosZ);
    yaw = cutscene_actors[CAMROT]->oPosY;
    pitch = cutscene_actors[CAMROT]->oPosX;
    vec3f_set_dist_and_angle(gCamera->focus, gCamera->pos, 100, (s16)pitch, (s16)yaw);
    current_frame++;
}