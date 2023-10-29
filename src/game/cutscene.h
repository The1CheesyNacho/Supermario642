#ifndef Cutscene_H
#define Cutscene_H

#include "include/types.h"
#include "include/sm64.h"

typedef void* Cutscene[];
typedef struct Object* CutsceneActor;

typedef struct {
    u8 actor;
    u8 interpolation;
    s16 x;
    s16 y;
    s16 z;
    u32 frame;
} CutsceneKeyframe;

typedef struct {
    u8 type;
    u16 arg1;
    u16 arg2;
    u16 arg3;
    u16 arg4;
    u32 frame;
} CutsceneEvent;

#define CAMPOS 30
#define CAMROT 31

#define CUTEV_END (void*)0
#define CUTEV_BGM (void*)1
#define CUTEV_SFX (void*)2
#define CUTEV_STARTNEW (void*)3
#define CUTEV_SET_MODEL (void*)4
#define CUTEV_SET_ANIM (void*)5

#define CUTIP_LINEAR (void*)0
#define CUTIP_SMOOTH (void*)1
#define CUTIP_WAIT (void*)2
#define CUTIP_SLOW (void*)3
#define CUTIP_FAST (void*)4

#define CUTSCENE_SCRIPT_END() (void*)0, (void*)0, (void*)0, (void*)0, (void*)0, (void*)0
#define CUTSCENE_BEGINNING(delay) (void*)1, (void*)delay, (void*)0, (void*)0, (void*)0, (void*)0
#define CUTSCENE_AFTER(delay) (void*)2, (void*)delay, (void*)0, (void*)0, (void*)0, (void*)0
#define CUTSCENE_BACK() (void*)3, (void*)0, (void*)0, (void*)0, (void*)0, (void*)0
#define CUTSCENE_ACTOR(id, model, x, y, z) (void*)4, (void*)id, (void*)model, (void*)x, (void*)y, (void*)z
#define CUTSCENE_KEYFRAME(actor, x, y, z, interpolation) (void*)5, (void*)actor, (void*)x, (void*)y, (void*)z, interpolation
#define CUTSCENE_EVENT_ARG4(ev, arg1, arg2, arg3, arg4) (void*)6, (void*)ev, (void*)arg1, (void*)arg2, (void*)arg3, (void*)arg4
#define CUTSCENE_EVENT_ARG3(ev, arg1, arg2, arg3      ) (void*)6, (void*)ev, (void*)arg1, (void*)arg2, (void*)arg3, (void*)0
#define CUTSCENE_EVENT_ARG2(ev, arg1, arg2            ) (void*)6, (void*)ev, (void*)arg1, (void*)arg2, (void*)0   , (void*)0
#define CUTSCENE_EVENT_ARG1(ev, arg1                  ) (void*)6, (void*)ev, (void*)arg1, (void*)0   , (void*)0   , (void*)0
#define CUTSCENE_EVENT(     ev                        ) (void*)6, (void*)ev, (void*)0   , (void*)0   , (void*)0   , (void*)0

extern u8 cutscene_active;
extern CutsceneActor cutscene_actors[];
extern CutsceneKeyframe cutscene_keyframes[];
extern CutsceneEvent cutscene_events[];
extern u32 current_frame;
extern u32 cutscene_num_keyframes;
extern u32 cutscene_num_events;

#define cutscene_start(id) cutscene_start_func(*cutscenes[id])
#define cutscene_test 0

extern Cutscene* cutscenes[];

extern void cutscene_start_func(Cutscene cutscene);
extern void cutscene_stop();
extern void cutscene_step();

#endif