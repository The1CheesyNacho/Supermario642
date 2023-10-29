#include "cutscene.h"

#include "object_helpers.h"
#include "behavior_data.h"

u8 cutscene_active = 0;
CutsceneActor cutscene_actors[32];
CutsceneKeyframe cutscene_keyframes[256];
CutsceneEvent cutscene_events[256];
u32 current_frame = 0;
u32 cutscene_num_keyframes = 0;
u32 cutscene_num_events = 0;

void start_cutscene(Cutscene cutscene) {
    stop_cutscene();
    cutscene_active = 1;
    current_frame = 0;
    cutscene_num_keyframes = 0;
    for (u8 i = 0; i < 32; i++) {
        cutscene_actors[i] = spawn_object(NULL, MODEL_NONE, bhvCutsceneActor);
    }
    u32 prev_frame = 0;
    u32 frame = 0;
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
                u32 temp = prev_frame;
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
                cutscene_num_events++;
                break;
        }
        cutscene += 6;
    }
}

void stop_cutscene() {
    if (!cutscene_active) return;
    for (u8 i = 0; i < 32; i++) {
        obj_mark_for_deletion(cutscene_actors[i]);
    }
    cutscene_active = 0;
}

void cutscene_step() {
    if (!cutscene_active) return;
    CutsceneKeyframe* next_keyframes[32];
    CutsceneKeyframe* prev_keyframes[32];
    for (u8 i = 0; i < cutscene_num_keyframes; i++) {
        u8 actor = cutscene_keyframes[i].actor;
        if (i < cutscene_keyframes[i].frame) {
            if (next_keyframes[actor] == NULL) next_keyframes[actor] = &cutscene_keyframes[i];
            else if (next_keyframes[actor]->frame < cutscene_keyframes[i].frame) next_keyframes[actor] = &cutscene_keyframes[i];
        }
        else {
            if (prev_keyframes[actor] == NULL) prev_keyframes[actor] = &cutscene_keyframes[i];
            else if (prev_keyframes[actor]->frame > cutscene_keyframes[i].frame) prev_keyframes[actor] = &cutscene_keyframes[i];
        }
    }
    for (u8 i = 0; i < 32; i++) {
        if (next_keyframes[i] == NULL && prev_keyframes[i] != NULL) next_keyframes[i] = prev_keyframes[i];
        if (prev_keyframes[i] == NULL && next_keyframes[i] != NULL) prev_keyframes[i] = next_keyframes[i];
    }
    current_frame++;
}