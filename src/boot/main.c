#include <ultra64.h>
#include <PR/os_internal_reg.h>
#include <PR/os_system.h>
#include <PR/os_vi.h>
#include <stdio.h>

#include "sm64.h"
#include "audio/external.h"
#include "game/game_init.h"
#include "game/mario.h"
#include "game/debug.h"
#include "game/memory.h"
#include "game/sound_init.h"
#include "buffers/buffers.h"
#include "segments.h"
#include "game/main.h"
#include "game/rumble_init.h"
#include "game/version.h"
#ifdef UNF
#include "usb/usb.h"
#include "usb/debug.h"
#endif
#include "game/puppyprint.h"
#include "game/puppylights.h"
#include "game/profiling.h"
#include "game/emutest.h"


// New system to verify mario's moves. Inspired by UE5's MoveUpdatedComponent function.
// Advantage:
// 1. Can no longer clip ceilings and steep floors
// 2. No more high speed clips
// 3. Consistently lands on steep floors
// 4. SM64 has an error up to 25% for moving mario. This has an error up to 1.56%.
// 5. Runs 4 collision calls per tick instead of 16 (95% of the time)
// 6. Consistent between swimming, aerial and ground step
// 7. Gets rid of quarterstep oddities
 
// Movedata lets us pass by struct to reduce arg passing overhead
struct MoveData {
    struct Surface *HitSurface; // Raycast hit result
    struct Surface *Wall;
    struct Surface *Floor;
    struct Surface *Ceil;
    f32 IntendedPos[3]; // Position we believe to be a good enough approximation for where mario can go
    f32 GoalPos[3];     // Position we originally wanted to move towards
    f32 FloorHeight;
    f32 CeilHeight;
    f32 MarioHeight;
    s32 StepArgs;
    f32 BiggestValidMove; // How much we managed to move
};
// Snap to the first collision in direction
ALWAYS_INLINE void CheckMoveEndPosition(struct MarioState *m, struct MoveData *MoveResult) {
    MoveResult->HitSurface = 0;
    Vec3f MoveVector;
    MoveVector[0] = MoveResult->IntendedPos[0] - m->pos[0];
    MoveVector[1] = MoveResult->IntendedPos[1] - m->pos[1];
    MoveVector[2] = MoveResult->IntendedPos[2] - m->pos[2];
    f32 MoveSize = vec3f_length(MoveVector);
    if (MoveSize > 0.0f) {
        // Scale up move size to account for mario's size
        f32 ScaledMoveSize = ((MoveSize + MARIOWIDENESS) / MoveSize);
        // Seperate clipvector saves us some multiplications down the line!
        Vec3f ClipVector;
        ClipVector[0] = MoveVector[0] * ScaledMoveSize;
        ClipVector[1] = MoveVector[1] * ScaledMoveSize;
        ClipVector[2] = MoveVector[2] * ScaledMoveSize;
 
        // Use the middle of Mario's to most represent his hitbox (idealls this would be a capsule cast)
        m->pos[1] += MARIOHEIGHT / 2;
        Vec3f HitPos;
        find_surface_on_ray(m->pos, ClipVector, &MoveResult->HitSurface, HitPos, 7);
        m->pos[1] -= MARIOHEIGHT / 2;
 
        // Clip if collision was found
        if (MoveResult->HitSurface != NULL) {
            const f32 DistanceMoved = sqrtf(sqr(HitPos[0] - MoveResult->IntendedPos[0])
                                            + sqr(HitPos[1] - MoveResult->IntendedPos[1])
                                            + sqr(HitPos[2] - MoveResult->IntendedPos[2]));
            // move back either by as wide as mario is or the whole distance, whatever is less.
            const f32 MoveBackScale = (MIN(DistanceMoved, MARIOWIDENESS) / MoveSize);
            if (absf((MoveResult->HitSurface)->normal.y) <= 23) {
                MoveResult->IntendedPos[0] = HitPos[0] - MoveVector[0] * MoveBackScale;
                MoveResult->IntendedPos[1] =
                    HitPos[1] - MoveVector[1] * MoveBackScale - MARIOHEIGHT / 2;
                MoveResult->IntendedPos[2] = HitPos[2] - MoveVector[2] * MoveBackScale;
            } else if ((MoveResult->HitSurface)->normal.y < 0.f) {
                // let the binary search find a good position towards mario's direction
                MoveResult->IntendedPos[0] = HitPos[0] + MoveResult->HitSurface->normal.x;
                MoveResult->IntendedPos[1] =
                    HitPos[1] + MoveResult->HitSurface->normal.y - MARIOHEIGHT / 2;
                MoveResult->IntendedPos[2] = HitPos[2] + MoveResult->HitSurface->normal.z;
            } else {
                MoveResult->IntendedPos[0] = HitPos[0];
                // Snap far enough down to guarantee find_floor will find a bigger value.
                MoveResult->IntendedPos[1] = HitPos[1] - ((f32) FLOOR_SNAP_OFFSET) / 2.f;
                MoveResult->IntendedPos[2] = HitPos[2];
            }
        }
    }
}
 
// Checks if the new position is valid.
s32 CheckMoveValid(struct MarioState *m, struct MoveData *MoveResult) {
    // Wall collisino happens first since walls will never prevent a move.
    MoveResult->Wall =
        resolve_and_return_wall_collisions(MoveResult->IntendedPos, (60.0f), MARIOWIDENESS);
    MoveResult->FloorHeight =
        find_floor_marioair(MoveResult->IntendedPos[0], MoveResult->IntendedPos[1],
                            MoveResult->IntendedPos[2], &MoveResult->Floor, m->vel[1]);
    // oob is invalid
    if (!MoveResult->Floor)
        return 0;
    // snap up early to make sure ceiling test happens from the right spot
    if ((MoveResult->StepArgs & STEP_SNAP_TO_FLOOR)
        && MoveResult->IntendedPos[1] < MoveResult->FloorHeight + FLOOR_SNAP_OFFSET) {
        MoveResult->IntendedPos[1] = MoveResult->FloorHeight;
    } else if (MoveResult->IntendedPos[1] < MoveResult->FloorHeight) {
        MoveResult->IntendedPos[1] = MoveResult->FloorHeight;
    }
    MoveResult->CeilHeight = vec3f_find_ceil(MoveResult->IntendedPos, &MoveResult->Ceil);
    // Mario does not fit here!
    if (MoveResult->FloorHeight + MoveResult->MarioHeight >= MoveResult->CeilHeight)
        return 0;
 
    return 1;
}
 
// Set Mario's data and determine the StepResult from the MoveResult.
s32 FinishMove(struct MarioState *m, struct MoveData *MoveResult) {
    m->floor = MoveResult->Floor;
    m->ceil = MoveResult->Ceil;
    m->wall = MoveResult->Wall;
    m->floorHeight = MoveResult->FloorHeight;
    m->ceilHeight = MoveResult->CeilHeight;
    vec3f_copy(m->pos, MoveResult->IntendedPos);
 
    if (m->ceilHeight < m->pos[1] + MoveResult->MarioHeight) {
        m->pos[1] = m->ceilHeight - MoveResult->MarioHeight;
        if ((MoveResult->StepArgs & STEP_CHECK_HANG) && m->ceil != NULL
            && ((m->ceil->type & (SURFACE_HANGABLE << 8)))) {
            m->vel[1] = 0.0f;
            return STEP_GRAB_CEILING;
        }
    }
    // if we are not set to snap to the floor but landed despite that, on ground takes priority!
    if (!(MoveResult->StepArgs & STEP_SNAP_TO_FLOOR) && (m->pos[1] <= m->floorHeight))
        return STEP_ON_GROUND;
 
    if (m->wall) {
        if (m->wall->type & (SURFACE_BURNING << 8)) {
            return STEP_HIT_LAVA;
        }
        if (MoveResult->StepArgs & STEP_CHECK_LEDGE_GRAB) {
            if (check_ledge_grab(m, m->wall, MoveResult->GoalPos, MoveResult->IntendedPos)) {
                return STEP_GRAB_LEDGE;
            }
        }
        u16 WallAngleMaxDiff = MoveResult->StepArgs & STEP_SNAP_TO_FLOOR
                                   ? 0x8000 - 23
                                   : 0x8000 - 23;
        if (absi((s16) (atan2s(m->wall->normal.z, m->wall->normal.x) - m->faceAngle[1]))
            >= WallAngleMaxDiff) {
            return STEP_HIT_WALL;
        }
    }
 
    // If we haven't moved, we hit either oob or a ceiling.
#define ZERO_POINT_FIVE_TO_THE_POWER_OF_MINUS_NUM_SEARCHES 0.015625f
    if (MoveResult->BiggestValidMove < ZERO_POINT_FIVE_TO_THE_POWER_OF_MINUS_NUM_SEARCHES) {
        return STEP_HIT_WALL;
    }
 
    return m->pos[1] <= m->floorHeight ? STEP_ON_GROUND : STEP_IN_AIR;
}
// Scales the move. The Y is assumed to always be valid (if not, we are ceiling bonking anyway)
s32 ScaleMove(struct MarioState *m, struct MoveData *MoveResult, f32 Scale) {
    MoveResult->IntendedPos[0] = (MoveResult->GoalPos[0] - m->pos[0]) * Scale + m->pos[0];
    MoveResult->IntendedPos[1] = MoveResult->GoalPos[1];
    MoveResult->IntendedPos[2] = (MoveResult->GoalPos[2] - m->pos[2]) * Scale + m->pos[2];
}
// Performs a generic step and returns the step result
// [StepArgs] checks for special interactions like ceilings, ledges and floor snapping
s32 PerformStep(struct MarioState *m, Vec3f GoalPos, const s32 StepArgs) {
    struct MoveData MoveResult;
    MoveResult.MarioHeight = (m->action & ACT_FLAG_SHORT_HITBOX) ? MARIOHEIGHT / 2.f : MARIOHEIGHT;
    MoveResult.StepArgs = StepArgs;
    vec3f_copy(MoveResult.IntendedPos, GoalPos);
    s32 IterationsRemaining = 2;
DoItAgain:
    CheckMoveEndPosition(m, &MoveResult);
    vec3f_copy(MoveResult.GoalPos, MoveResult.IntendedPos);
 
    // If the move is outright valid (VAST MAJORITY OF MOVES), just exit instantly.
    if (CheckMoveValid(m, &MoveResult)) {
        if (MoveResult.HitSurface) {
            struct Surface *HitSurface;
            Vec3f HitPos;
            Vec3f ClipVector;
            ClipVector[0] = MoveResult.GoalPos[0] - m->pos[0];
            // move back up because floors in HitSurface move mario down (ensures snapping)
            ClipVector[1] =
                MoveResult.GoalPos[1] - m->pos[1]
                + (MoveResult.HitSurface->normal.y > 23 ? FLOOR_SNAP_OFFSET / 2.f + 4.f
                                                                   : 0.f);
            ClipVector[2] = MoveResult.GoalPos[2] - m->pos[2];
            find_surface_on_ray(m->pos, ClipVector, &HitSurface, HitPos, 7);
            // Ensure nothing moved mario's feet through a surface.
            // (Ledgegrabs may teleport mario, but they happen in FinishMove)
            if (HitSurface) {
                // Give it another try, we do want to move as much as possible.
                vec3f_copy(MoveResult.GoalPos, HitPos);
                IterationsRemaining--;
                if (IterationsRemaining)
                    goto DoItAgain;
                // No valid moves managed to be made. Emergency exit!
                return STEP_HIT_WALL;
            }
        }
        // Full move happened
        MoveResult.BiggestValidMove = 1.f;
        return FinishMove(m, &MoveResult);
    }
    // Move was unsuccessful. Scale it down to a precision of 2^-NUM_SEARCHES
    f32 CurrentMoveSize = 0.5f;
    MoveResult.BiggestValidMove = 0.f;
#define NUM_SEARCHES 6
    for (s32 BinarySplitsReamining = NUM_SEARCHES; BinarySplitsReamining > 0; BinarySplitsReamining--) {
        ScaleMove(m, &MoveResult, MoveResult.BiggestValidMove + CurrentMoveSize);
        if (CheckMoveValid(m, &MoveResult)) {
            MoveResult.BiggestValidMove += CurrentMoveSize;
        }
        CurrentMoveSize *= 0.5f;
    }
    ScaleMove(m, &MoveResult, MoveResult.BiggestValidMove);
    // No valid move can be made. We are stuck OOB.
    // This should only happen if a platform OOB teleported away.
    // Mario should die here.
    if (!CheckMoveValid(m, &MoveResult)) {
        return STEP_HIT_WALL;
    }
    // We've moved, but not the full distance.
    return FinishMove(m, &MoveResult);
}

// Message IDs
enum MessageIDs {
    MESG_SP_COMPLETE = 100,
    MESG_DP_COMPLETE,
    MESG_VI_VBLANK,
    MESG_START_GFX_SPTASK,
    MESG_NMI_REQUEST,
    MESG_RCP_HUNG,
};


// OSThread gUnkThread; // unused?
OSThread gIdleThread;
OSThread gMainThread;
OSThread gGameLoopThread;
OSThread gSoundThread;

OSIoMesg gDmaIoMesg;
OSMesg gMainReceivedMesg;

OSMesgQueue gDmaMesgQueue;
OSMesgQueue gSIEventMesgQueue;
OSMesgQueue gPIMesgQueue;
OSMesgQueue gIntrMesgQueue;
OSMesgQueue gSPTaskMesgQueue;

OSMesg gDmaMesgBuf[1];
OSMesg gPIMesgBuf[32];
OSMesg gSIEventMesgBuf[1];
OSMesg gIntrMesgBuf[16];
OSMesg gUnknownMesgBuf[16];

OSViMode VI;

struct Config gConfig;

struct VblankHandler *gVblankHandler1       = NULL;
struct VblankHandler *gVblankHandler2       = NULL;
struct VblankHandler *gVblankHandler3       = NULL;
struct SPTask        *gActiveSPTask         = NULL;
struct SPTask        *sCurrentAudioSPTask   = NULL;
struct SPTask        *sCurrentDisplaySPTask = NULL;
struct SPTask        *sNextAudioSPTask      = NULL;
struct SPTask        *sNextDisplaySPTask    = NULL;
s8  gAudioEnabled      = TRUE;
u32 gNumVblanks        = 0;
s8  gResetTimer        = 0;
s8  gNmiResetBarsTimer = 0;
s8  gDebugLevelSelect  = FALSE;

#ifdef VANILLA_DEBUG
s8 gShowDebugText = FALSE;

// unused
UNUSED static u16 sDebugTextKeySequence[] = {
    D_JPAD, D_JPAD, U_JPAD, U_JPAD, L_JPAD, R_JPAD, L_JPAD, R_JPAD
};
static s16 sDebugTextKey = 0;
UNUSED void handle_debug_key_sequences(void) {
    if (gPlayer1Controller->buttonPressed != 0) {
        if (sDebugTextKeySequence[sDebugTextKey++] == gPlayer1Controller->buttonPressed) {
            if (sDebugTextKey == ARRAY_COUNT(sDebugTextKeySequence)) {
                sDebugTextKey = 0;
                gShowDebugText ^= 1;
            }
        } else {
            sDebugTextKey = 0;
        }
    }
}
#endif

void setup_mesg_queues(void) {
    osCreateMesgQueue(&gDmaMesgQueue, gDmaMesgBuf, ARRAY_COUNT(gDmaMesgBuf));
    osCreateMesgQueue(&gSIEventMesgQueue, gSIEventMesgBuf, ARRAY_COUNT(gSIEventMesgBuf));
    osSetEventMesg(OS_EVENT_SI, &gSIEventMesgQueue, NULL);

    osCreateMesgQueue(&gSPTaskMesgQueue, gUnknownMesgBuf, ARRAY_COUNT(gUnknownMesgBuf));
    osCreateMesgQueue(&gIntrMesgQueue, gIntrMesgBuf, ARRAY_COUNT(gIntrMesgBuf));
    osViSetEvent(&gIntrMesgQueue, (OSMesg) MESG_VI_VBLANK, 1);

    osSetEventMesg(OS_EVENT_SP, &gIntrMesgQueue, (OSMesg) MESG_SP_COMPLETE);
    osSetEventMesg(OS_EVENT_DP, &gIntrMesgQueue, (OSMesg) MESG_DP_COMPLETE);
    osSetEventMesg(OS_EVENT_PRENMI, &gIntrMesgQueue, (OSMesg) MESG_NMI_REQUEST);
}

void alloc_pool(void) {
    void *start = (void *) SEG_POOL_START;
    void *end = (void *) (SEG_POOL_START + POOL_SIZE);

    main_pool_init(start, end);
    gEffectsMemoryPool = mem_pool_init(EFFECTS_MEMORY_POOL, MEMORY_POOL_LEFT);
#ifdef PUPPYLIGHTS
    gLightsPool = mem_pool_init(PUPPYLIGHTS_POOL, MEMORY_POOL_LEFT);
#endif
}

void create_thread(OSThread *thread, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri pri) {
    thread->next = NULL;
    thread->queue = NULL;
    osCreateThread(thread, id, entry, arg, sp, pri);
}

#ifdef VERSION_SH
extern void func_sh_802f69cc(void);
#endif

void handle_nmi_request(void) {
    gResetTimer = 1;
    gNmiResetBarsTimer = 0;
    stop_sounds_in_continuous_banks();
    sound_banks_disable(SEQ_PLAYER_SFX, SOUND_BANKS_BACKGROUND);
    fadeout_music(90);
#ifdef VERSION_SH
    func_sh_802f69cc();
#endif
}

void receive_new_tasks(void) {
    struct SPTask *spTask;

    while (osRecvMesg(&gSPTaskMesgQueue, (OSMesg *) &spTask, OS_MESG_NOBLOCK) != -1) {
        spTask->state = SPTASK_STATE_NOT_STARTED;
        switch (spTask->task.t.type) {
            case 2:
                sNextAudioSPTask = spTask;
                break;
            case 1:
                sNextDisplaySPTask = spTask;
                break;
        }
    }

    if (sCurrentAudioSPTask == NULL && sNextAudioSPTask != NULL) {
        sCurrentAudioSPTask = sNextAudioSPTask;
        sNextAudioSPTask = NULL;
    }

    if (sCurrentDisplaySPTask == NULL && sNextDisplaySPTask != NULL) {
        sCurrentDisplaySPTask = sNextDisplaySPTask;
        sNextDisplaySPTask = NULL;
    }
}

void start_sptask(s32 taskType) {
    if (taskType == M_AUDTASK) {
        gActiveSPTask = sCurrentAudioSPTask;
    } else {
        gActiveSPTask = sCurrentDisplaySPTask;
    }

    osSpTaskLoad(&gActiveSPTask->task);
    osSpTaskStartGo(&gActiveSPTask->task);
    gActiveSPTask->state = SPTASK_STATE_RUNNING;
}

void interrupt_gfx_sptask(void) {
    if (gActiveSPTask->task.t.type == M_GFXTASK) {
        gActiveSPTask->state = SPTASK_STATE_INTERRUPTED;
        osSpTaskYield();
    }
}

void start_gfx_sptask(void) {
    if (gActiveSPTask == NULL
     && sCurrentDisplaySPTask != NULL
     && sCurrentDisplaySPTask->state == SPTASK_STATE_NOT_STARTED) {
        start_sptask(M_GFXTASK);
        profiler_rsp_started(PROFILER_RSP_GFX);
    }
}

void pretend_audio_sptask_done(void) {
    gActiveSPTask = sCurrentAudioSPTask;
    gActiveSPTask->state = SPTASK_STATE_RUNNING;
    osSendMesg(&gIntrMesgQueue, (OSMesg) MESG_SP_COMPLETE, OS_MESG_NOBLOCK);
}

void handle_vblank(void) {
    gNumVblanks++;
    if (gResetTimer > 0 && gResetTimer < 100) {
        gResetTimer++;
    }

    receive_new_tasks();

    // First try to kick off an audio task. If the gfx task is currently
    // running, we need to asynchronously interrupt it -- handle_sp_complete
    // will pick up on what we're doing and start the audio task for us.
    // If there is already an audio task running, there is nothing to do.
    // If there is no audio task available, try a gfx task instead.
    if (sCurrentAudioSPTask != NULL) {
        if (gActiveSPTask != NULL) {
            interrupt_gfx_sptask();
        } else {
            if (gAudioEnabled) {
                start_sptask(M_AUDTASK);
            } else {
                pretend_audio_sptask_done();
            }
            profiler_rsp_started(PROFILER_RSP_AUDIO);
        }
    } else {
        if (gActiveSPTask == NULL
         && sCurrentDisplaySPTask != NULL
         && sCurrentDisplaySPTask->state != SPTASK_STATE_FINISHED) {
            start_sptask(M_GFXTASK);
            profiler_rsp_started(PROFILER_RSP_GFX);
        }
    }
#if ENABLE_RUMBLE
    rumble_thread_update_vi();
#endif

    // Notify the game loop about the vblank.
    if (gVblankHandler1 != NULL) osSendMesg(gVblankHandler1->queue, gVblankHandler1->msg, OS_MESG_NOBLOCK);
    if (gVblankHandler2 != NULL) osSendMesg(gVblankHandler2->queue, gVblankHandler2->msg, OS_MESG_NOBLOCK);
    if (gVblankHandler3 != NULL) osSendMesg(gVblankHandler3->queue, gVblankHandler3->msg, OS_MESG_NOBLOCK);
}

void handle_sp_complete(void) {
    struct SPTask *curSPTask = gActiveSPTask;

    gActiveSPTask = NULL;

    if (curSPTask->state == SPTASK_STATE_INTERRUPTED) {
        // handle_vblank tried to start an audio task while there was already a gfx task
        // running, so it had to interrupt the gfx task. That interruption just finished.
        if (osSpTaskYielded(&curSPTask->task) == 0) {
            // The gfx task completed before we had time to interrupt it.
            // Mark it finished, just like below.
            curSPTask->state = SPTASK_STATE_FINISHED;
            profiler_rsp_completed(PROFILER_RSP_GFX);
        } else {
            profiler_rsp_yielded();
        }

        // Start the audio task, as expected by handle_vblank.
        if (gAudioEnabled) {
            start_sptask(M_AUDTASK);
        } else {
            pretend_audio_sptask_done();
        }
        profiler_rsp_started(PROFILER_RSP_AUDIO);
    } else {
        curSPTask->state = SPTASK_STATE_FINISHED;
        if (curSPTask->task.t.type == M_AUDTASK) {
            profiler_rsp_completed(PROFILER_RSP_AUDIO);
            // After audio tasks come gfx tasks.
            if ((sCurrentDisplaySPTask != NULL)
             && (sCurrentDisplaySPTask->state != SPTASK_STATE_FINISHED)) {
                if (sCurrentDisplaySPTask->state == SPTASK_STATE_INTERRUPTED) {
                    profiler_rsp_resumed();
                } else {
                    profiler_rsp_started(PROFILER_RSP_GFX);
                }
                start_sptask(M_GFXTASK);
            }
            sCurrentAudioSPTask = NULL;
            if (curSPTask->msgqueue != NULL) {
                osSendMesg(curSPTask->msgqueue, curSPTask->msg, OS_MESG_NOBLOCK);
            }
        } else {
            // The SP process is done, but there is still a Display Processor notification
            // that needs to arrive before we can consider the task completely finished and
            // null out sCurrentDisplaySPTask. That happens in handle_dp_complete.
            profiler_rsp_completed(PROFILER_RSP_GFX);
        }
    }
}

void handle_dp_complete(void) {
    // Gfx SP task is completely done.
    if (sCurrentDisplaySPTask->msgqueue != NULL) {
        osSendMesg(sCurrentDisplaySPTask->msgqueue, sCurrentDisplaySPTask->msg, OS_MESG_NOBLOCK);
    }
    sCurrentDisplaySPTask->state = SPTASK_STATE_FINISHED_DP;
    sCurrentDisplaySPTask = NULL;
}

OSTimerEx RCPHangTimer;
void start_rcp_hang_timer(void) {
    if (RCPHangTimer.started == FALSE) {
        osSetTimer(&RCPHangTimer.timer, OS_USEC_TO_CYCLES(3000000), (OSTime) 0, &gIntrMesgQueue, (OSMesg) MESG_RCP_HUNG);
        RCPHangTimer.started = TRUE;
    }
}

void stop_rcp_hang_timer(void) {
    osStopTimer(&RCPHangTimer.timer);
    RCPHangTimer.started = FALSE;
}

void alert_rcp_hung_up(void) {
    error("RCP is HUNG UP!! Oh! MY GOD!!");
}

/**
 * Increment the first and last values of the stack.
 * If they're different, that means an error has occured, so trigger a crash.
*/
#ifdef DEBUG
void check_stack_validity(void) {
    gIdleThreadStack[0]++;
    gIdleThreadStack[THREAD1_STACK - 1]++;
    assert(gIdleThreadStack[0] == gIdleThreadStack[THREAD1_STACK - 1], "Thread 1 stack overflow.")
    gThread3Stack[0]++;
    gThread3Stack[THREAD3_STACK - 1]++;
    assert(gThread3Stack[0] == gThread3Stack[THREAD3_STACK - 1], "Thread 3 stack overflow.")
    gThread4Stack[0]++;
    gThread4Stack[THREAD4_STACK - 1]++;
    assert(gThread4Stack[0] == gThread4Stack[THREAD4_STACK - 1], "Thread 4 stack overflow.")
    gThread5Stack[0]++;
    gThread5Stack[THREAD5_STACK - 1]++;
    assert(gThread5Stack[0] == gThread5Stack[THREAD5_STACK - 1], "Thread 5 stack overflow.")
#if ENABLE_RUMBLE
    gThread6Stack[0]++;
    gThread6Stack[THREAD6_STACK - 1]++;
    assert(gThread6Stack[0] == gThread6Stack[THREAD6_STACK - 1], "Thread 6 stack overflow.")
#endif
}
#endif


extern void crash_screen_init(void);
extern OSViMode VI;
void thread3_main(UNUSED void *arg) {
    setup_mesg_queues();
    alloc_pool();
    load_engine_code_segment();
    detect_emulator();
#ifndef UNF
    crash_screen_init();
#endif

#ifdef UNF
    debug_initialize();
#endif

#ifdef DEBUG
    osSyncPrintf("Super Mario 64\n");
#if 0 // if your PC username isn't your real name feel free to uncomment
    osSyncPrintf("Built by: %s\n", __username__);
#endif
    osSyncPrintf("Compiler: %s\n", __compiler__);
    osSyncPrintf("Linker  : %s\n", __linker__);
#endif

    if (!(gEmulator & EMU_CONSOLE)) {
        gBorderHeight = BORDER_HEIGHT_EMULATOR;
#ifdef RCVI_HACK
        VI.comRegs.vSync = 525*20;   
        change_vi(&VI, SCREEN_WIDTH, SCREEN_HEIGHT);
        osViSetMode(&VI);
        osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON);
        osViSetSpecialFeatures(OS_VI_GAMMA_OFF);
#endif
    } else {
        gBorderHeight = BORDER_HEIGHT_CONSOLE;
    }
#ifdef DEBUG
    gIdleThreadStack[0] = 0;
    gIdleThreadStack[THREAD1_STACK - 1] = 0;
    gThread3Stack[0] = 0;
    gThread3Stack[THREAD3_STACK - 1] = 0;
    gThread4Stack[0] = 0;
    gThread4Stack[THREAD4_STACK - 1] = 0;
    gThread5Stack[0] = 0;
    gThread5Stack[THREAD5_STACK - 1] = 0;
#if ENABLE_RUMBLE
    gThread6Stack[0] = 0;
    gThread6Stack[THREAD6_STACK - 1] = 0;
#endif
#endif

    create_thread(&gSoundThread, THREAD_4_SOUND, thread4_sound, NULL, gThread4Stack + THREAD4_STACK, 20);
    osStartThread(&gSoundThread);

    create_thread(&gGameLoopThread, THREAD_5_GAME_LOOP, thread5_game_loop, NULL, gThread5Stack + THREAD5_STACK, 10);
    osStartThread(&gGameLoopThread);

    while (TRUE) {
        OSMesg msg;
        osRecvMesg(&gIntrMesgQueue, &msg, OS_MESG_BLOCK);
#ifdef DEBUG
        check_stack_validity();
#endif
        switch ((uintptr_t) msg) {
            case MESG_VI_VBLANK:
                handle_vblank();
                break;
            case MESG_SP_COMPLETE:
                handle_sp_complete();
                break;
            case MESG_DP_COMPLETE:
                stop_rcp_hang_timer();
                handle_dp_complete();
                break;
            case MESG_START_GFX_SPTASK:
                start_rcp_hang_timer();
                start_gfx_sptask();
                break;
            case MESG_NMI_REQUEST:
                handle_nmi_request();
                break;
            case MESG_RCP_HUNG:
                alert_rcp_hung_up();
                break;
        }
    }
}

void set_vblank_handler(s32 index, struct VblankHandler *handler, OSMesgQueue *queue, OSMesg *msg) {
    handler->queue = queue;
    handler->msg = msg;

    switch (index) {
        case 1:
            gVblankHandler1 = handler;
            break;
        case 2:
            gVblankHandler2 = handler;
            break;
        case 3:
            gVblankHandler3 = handler;
            break;
    }
}

void send_sp_task_message(OSMesg *msg) {
    osWritebackDCacheAll();
    osSendMesg(&gSPTaskMesgQueue, msg, OS_MESG_NOBLOCK);
}

void dispatch_audio_sptask(struct SPTask *spTask) {
    if (gAudioEnabled && spTask != NULL) {
        osWritebackDCacheAll();
        osSendMesg(&gSPTaskMesgQueue, spTask, OS_MESG_NOBLOCK);
    }
}

void exec_display_list(struct SPTask *spTask) {
    if (spTask != NULL) {
        osWritebackDCacheAll();
        spTask->state = SPTASK_STATE_NOT_STARTED;
        if (sCurrentDisplaySPTask == NULL) {
            sCurrentDisplaySPTask = spTask;
            sNextDisplaySPTask = NULL;
            osSendMesg(&gIntrMesgQueue, (OSMesg) MESG_START_GFX_SPTASK, OS_MESG_NOBLOCK);
        } else {
            sNextDisplaySPTask = spTask;
        }
    }
}

void turn_on_audio(void) {
    gAudioEnabled = TRUE;
}

void turn_off_audio(void) {
    gAudioEnabled = FALSE;
    while (sCurrentAudioSPTask != NULL) {
        ;
    }
}

void change_vi(OSViMode *mode, int width, int height) {
    mode->comRegs.width  = width;
    mode->comRegs.xScale = ((width * 512) / 320);
    if (height > 240) {
        mode->comRegs.ctrl     |= 0x40;
        mode->fldRegs[0].origin = (width * 2);
        mode->fldRegs[1].origin = (width * 4);
        mode->fldRegs[0].yScale = (0x2000000 | ((height * 1024) / 240));
        mode->fldRegs[1].yScale = (0x2000000 | ((height * 1024) / 240));
        mode->fldRegs[0].vStart = (mode->fldRegs[1].vStart - 0x20002);
    } else {
        mode->fldRegs[0].origin = (width * 2);
        mode->fldRegs[1].origin = (width * 4);
        mode->fldRegs[0].yScale = ((height * 1024) / 240);
        mode->fldRegs[1].yScale = ((height * 1024) / 240);
    }
}

void get_audio_frequency(void) {
    switch (gConfig.tvType) {
#if defined(VERSION_JP) || defined(VERSION_US)
    case MODE_NTSC: gConfig.audioFrequency = 1.0f;    break;
    case MODE_MPAL: gConfig.audioFrequency = 0.9915f; break;
    case MODE_PAL:  gConfig.audioFrequency = 0.9876f; break;
#else
    case MODE_NTSC: gConfig.audioFrequency = 1.0126f; break;
    case MODE_MPAL: gConfig.audioFrequency = 1.0086f; break;
    case MODE_PAL:  gConfig.audioFrequency = 1.0f;    break;
#endif
    }
}

/**
 * Initialize hardware, start main thread, then idle.
 */
void thread1_idle(UNUSED void *arg) {
    osCreateViManager(OS_PRIORITY_VIMGR);
    switch (osTvType) {
        case OS_TV_NTSC:
            //osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);
            VI = osViModeTable[OS_VI_NTSC_LAN1];
            gConfig.tvType = MODE_NTSC;
            break;
        case OS_TV_MPAL:
            //osViSetMode(&osViModeTable[OS_VI_MPAL_LAN1]);
            VI = osViModeTable[OS_VI_NTSC_LAN1];
            gConfig.tvType = MODE_MPAL;
            break;
        case OS_TV_PAL:
            //osViSetMode(&osViModeTable[OS_VI_PAL_LAN1]);
            VI = osViModeTable[OS_VI_NTSC_LAN1];
            gConfig.tvType = MODE_PAL;
            break;
    }
    get_audio_frequency();
    change_vi(&VI, SCREEN_WIDTH, SCREEN_HEIGHT);
    osViSetMode(&VI);
    osViBlack(TRUE);
    osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON);
    osViSetSpecialFeatures(OS_VI_GAMMA_OFF);
    osCreatePiManager(OS_PRIORITY_PIMGR, &gPIMesgQueue, gPIMesgBuf, ARRAY_COUNT(gPIMesgBuf));
    create_thread(&gMainThread, THREAD_3_MAIN, thread3_main, NULL, gThread3Stack + THREAD3_STACK, 100);
    osStartThread(&gMainThread);

    osSetThreadPri(NULL, 0);

    // halt
    while (TRUE) {
        ;
    }
}

// Clear RAM on boot
void ClearRAM(void) {
    bzero(_mainSegmentEnd, (size_t)osMemSize - (size_t)OS_K0_TO_PHYSICAL(_mainSegmentEnd));
}

#ifdef ISVPRINT
extern u32 gISVDbgPrnAdrs;
extern u32 gISVFlag;

void osInitialize_fakeisv() {
    /* global flag to skip `__checkHardware_isv` from being called. */
    gISVFlag = 0x49533634;  // 'IS64'

    /* printf writes go to this address, cen64(1) has this hardcoded. */
    gISVDbgPrnAdrs = 0x13FF0000;

    /* `__printfunc`, used by `osSyncPrintf` will be set. */
    __osInitialize_isv();
}
#endif

void main_func(void) {
    ClearRAM();
    __osInitialize_common();
#ifdef ISVPRINT
    osInitialize_fakeisv();
#endif
    create_thread(&gIdleThread, THREAD_1_IDLE, thread1_idle, NULL, gIdleThreadStack + THREAD1_STACK, 100);
    osStartThread(&gIdleThread);
}
