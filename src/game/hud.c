#include <PR/ultratypes.h>

#include "gex.h"
#include "sm64.h"
#include "actors/common1.h"
#include "audio/external.h"
#include "audio/load.h"
#include "gfx_dimensions.h"
#include "game_init.h"
#include "level_update.h"
#include "camera.h"
#include "print.h"
#include "ingame_menu.h"
#include "hud.h"
#include "segment2.h"
#include "area.h"
#include "save_file.h"
#include "print.h"
#include "engine/surface_load.h"
#include "engine/math_util.h"
#include "puppycam2.h"
#include "puppyprint.h"

#include "config.h"

/* @file hud.c
 * This file implements HUD rendering and power meter animations.
 * That includes stars, lives, coins, camera status, power meter, timer
 * cannon reticle, and the unused keys.
 **/

#ifdef BREATH_METER
#define HUD_BREATH_METER_X         40
#define HUD_BREATH_METER_Y         32
#define HUD_BREATH_METER_HIDDEN_Y -20
#endif

// ------------- FPS COUNTER ---------------
// To use it, call print_fps(x,y); every frame.
#define FRAMETIME_COUNT 30

OSTime frameTimes[FRAMETIME_COUNT];
u8 curFrameTimeIndex = 0;

#include "PR/os_convert.h"

#ifdef USE_PROFILER
float profiler_get_fps();
#else
// Call once per frame
f32 calculate_and_update_fps() {
    OSTime newTime = osGetTime();
    OSTime oldTime = frameTimes[curFrameTimeIndex];
    frameTimes[curFrameTimeIndex] = newTime;

    curFrameTimeIndex++;
    if (curFrameTimeIndex >= FRAMETIME_COUNT) {
        curFrameTimeIndex = 0;
    }
    return ((f32)FRAMETIME_COUNT * 1000000.0f) / (s32)OS_CYCLES_TO_USEC(newTime - oldTime);
}
#endif

void print_fps(s32 x, s32 y) {
#ifdef USE_PROFILER
    f32 fps = profiler_get_fps();
#else
    f32 fps = calculate_and_update_fps();
#endif
    char text[14];

    sprintf(text, "FPS %2.2f", fps);
#ifdef PUPPYPRINT
    print_small_text(x, y, text, PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_OUTLINE);
#else
    print_text(x, y, text);
#endif
}

// ------------ END OF FPS COUNER -----------------

struct PowerMeterHUD {
    s8 animation;
    s16 x;
    s16 y;
};

struct CameraHUD {
    s16 status;
};

// Stores health segmented value defined by numHealthWedges
// When the HUD is rendered this value is 8, full health.
static s16 sPowerMeterStoredHealth;

static struct PowerMeterHUD sPowerMeterHUD = {
    POWER_METER_HIDDEN,
    HUD_POWER_METER_X,
    HUD_POWER_METER_HIDDEN_Y,
};

// Power Meter timer that keeps counting when it's visible.
// Gets reset when the health is filled and stops counting
// when the power meter is hidden.
s32 sPowerMeterVisibleTimer = 0;

u8 gHudIdle = 0;
s32 gHudIdleTimer = 0;
s32 gHudForceIdle = 0;
s32 gHudRedCoinTimer = 0;
f32 gHudOffset = 0;
f32 gHudRedCoinOffset = 0;

#ifdef BREATH_METER
static s16 sBreathMeterStoredValue;
static struct PowerMeterHUD sBreathMeterHUD = {
    BREATH_METER_HIDDEN,
    HUD_BREATH_METER_X,
    HUD_BREATH_METER_HIDDEN_Y,
};
s32 sBreathMeterVisibleTimer = 0;
#endif

static struct CameraHUD sCameraHUD = { CAM_STATUS_NONE };

/**
 * Renders a rgba16 16x16 glyph texture from a table list.
 */
void render_hud_tex_lut(s32 x, s32 y, Texture *texture) {
    gDPPipeSync(gDisplayListHead++);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, texture);
    gSPDisplayList(gDisplayListHead++, &dl_hud_img_load_tex_block);
    gSPTextureRectangle(gDisplayListHead++, x << 2, y << 2, (x + 15) << 2, (y + 15) << 2,
                        G_TX_RENDERTILE, 0, 0, 4 << 10, 1 << 10);
}

/**
 * Renders a rgba16 16x16 glyph texture from a table list.
 */
void render_hud_tex_lut_big(s32 x, s32 y, Texture *texture) {
    gDPPipeSync(gDisplayListHead++);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, texture);
    gSPDisplayList(gDisplayListHead++, &dl_hud_img_load_tex_block_32);
    gSPTextureRectangle(gDisplayListHead++, x << 2, y << 2, (x + 31) << 2, (y + 31) << 2,
                        G_TX_RENDERTILE, 0, 0, 4 << 10, 1 << 10);
}

/**
 * Renders a rgba16 8x8 glyph texture from a table list.
 */
void render_hud_small_tex_lut(s32 x, s32 y, Texture *texture) {
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0,
                G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD);
    gDPTileSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 2, 0, G_TX_RENDERTILE, 0,
                G_TX_CLAMP, 3, G_TX_NOLOD, G_TX_CLAMP, 3, G_TX_NOLOD);
    gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, (8 - 1) << G_TEXTURE_IMAGE_FRAC, (8 - 1) << G_TEXTURE_IMAGE_FRAC);
    gDPPipeSync(gDisplayListHead++);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, texture);
    gDPLoadSync(gDisplayListHead++);
    gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 8 * 8 - 1, CALC_DXT(8, G_IM_SIZ_16b_BYTES));
    gSPTextureRectangle(gDisplayListHead++, x << 2, y << 2, (x + 7) << 2, (y + 7) << 2, G_TX_RENDERTILE,
                        0, 0, 4 << 10, 1 << 10);
}

/**
 * Renders power meter health segment texture using a table list.
 */
void render_power_meter_health_segment(s16 numHealthWedges) {
    u8 *(*healthLUT)[];

    healthLUT = segmented_to_virtual(power_meter_health_segments_lut);

    gDPPipeSync(gDisplayListHead++);
    g_Tani_LoadTextureImage2(gDisplayListHead++, (*healthLUT)[numHealthWedges * 2], G_IM_FMT_RGBA,
                             G_IM_SIZ_16b, 32, 64, 0, 7) gSP1Triangle(gDisplayListHead++, 0, 1, 2, 0);
    gSP1Triangle(gDisplayListHead++, 0, 2, 3, 0);
    g_Tani_LoadTextureImage2(gDisplayListHead++, (*healthLUT)[(numHealthWedges * 2) + 1], G_IM_FMT_RGBA,
                             G_IM_SIZ_16b, 32, 64, 0, 7) gSP1Triangle(gDisplayListHead++, 4, 5, 6, 0);
    gSP1Triangle(gDisplayListHead++, 4, 6, 7, 0);
}

/**
 * Renders power meter display lists.
 * That includes the "POWER" base and the colored health segment textures.
 */
void render_dl_power_meter(s16 numHealthWedges) {
    Mtx *translateMtx;
    Mtx *scaleMtx;

    translateMtx = alloc_display_list(sizeof(Mtx));
    scaleMtx = alloc_display_list(sizeof(Mtx));

    if (translateMtx == NULL || scaleMtx == NULL) {
        return;
    }

    guTranslate(translateMtx, (f32) sPowerMeterHUD.x, (f32) sPowerMeterHUD.y, 0);
    guScale(scaleMtx, 0.75f, 0.75f, 1.0f);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(translateMtx++),
              G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(scaleMtx++),
              G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

    gSPDisplayList(gDisplayListHead++, &dl_power_meter_base);

    render_power_meter_health_segment(numHealthWedges);

    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

/**
 * Power meter animation called when there's less than 8 health segments
 * Checks its timer to later change into deemphasizing mode.
 */
void animate_power_meter_emphasized(void) {
    s16 hudDisplayFlags = gHudDisplay.flags;

    if (!(hudDisplayFlags & HUD_DISPLAY_FLAG_EMPHASIZE_POWER)) {
        if (sPowerMeterVisibleTimer == 45.0f) {
            sPowerMeterHUD.animation = POWER_METER_DEEMPHASIZING;
        }
    } else {
        sPowerMeterVisibleTimer = 0;
    }
}

/**
 * Power meter animation called after emphasized mode.
 * Moves power meter y pos speed until it's at 200 to be visible.
 */
static void animate_power_meter_deemphasizing(void) {
    s16 speed = 5;

    if (sPowerMeterHUD.y > HUD_POWER_METER_Y - 20) speed = 3;
    if (sPowerMeterHUD.y > HUD_POWER_METER_Y - 10) speed = 2;
    if (sPowerMeterHUD.y > HUD_POWER_METER_Y -  5) speed = 1;

    sPowerMeterHUD.y += speed;

    if (sPowerMeterHUD.y > HUD_POWER_METER_Y) {
        sPowerMeterHUD.y = HUD_POWER_METER_Y;
        sPowerMeterHUD.animation = POWER_METER_VISIBLE;
    }
}

/**
 * Power meter animation called when there's 8 health segments.
 * Moves power meter y pos quickly until it's at 301 to be hidden.
 */
static void animate_power_meter_hiding(void) {
    sPowerMeterHUD.y += 20;
    if (sPowerMeterHUD.y > HUD_POWER_METER_HIDDEN_Y) {
        sPowerMeterHUD.animation = POWER_METER_HIDDEN;
        sPowerMeterVisibleTimer = 0;
    }
}

/**
 * Handles power meter actions depending of the health segments values.
 */
void handle_power_meter_actions(s16 numHealthWedges) {
    // Show power meter if health is not full, less than 8
    if (numHealthWedges < 8 && sPowerMeterStoredHealth == 8
        && sPowerMeterHUD.animation == POWER_METER_HIDDEN) {
        sPowerMeterHUD.animation = POWER_METER_EMPHASIZED;
        sPowerMeterHUD.y = HUD_POWER_METER_EMPHASIZED_Y;
    }

    // Show power meter if health is full, has 8
    if (numHealthWedges == 8 && sPowerMeterStoredHealth == 7) {
        sPowerMeterVisibleTimer = 0;
    }

    // After health is full, hide power meter
    if (numHealthWedges == 8 && sPowerMeterVisibleTimer > 45.0f) {
        sPowerMeterHUD.animation = POWER_METER_HIDING;
    }

    // Update to match health value
    sPowerMeterStoredHealth = numHealthWedges;

#ifndef BREATH_METER
    // If Mario is swimming, keep power meter visible
    if (gPlayerCameraState->action & ACT_FLAG_SWIMMING) {
        if (sPowerMeterHUD.animation == POWER_METER_HIDDEN
            || sPowerMeterHUD.animation == POWER_METER_EMPHASIZED) {
            sPowerMeterHUD.animation = POWER_METER_DEEMPHASIZING;
            sPowerMeterHUD.y = HUD_POWER_METER_EMPHASIZED_Y;
        }
        sPowerMeterVisibleTimer = 0;
    }
#endif
}

/**
 * Renders the power meter that shows when Mario is in underwater
 * or has taken damage and has less than 8 health segments.
 * And calls a power meter animation function depending of the value defined.
 */
f32 prevHurt = 0;
f32 hurt = 0;
f32 hurtTimer = 0;

f32 map_value(f32 srcMin, f32 srcMax, f32 dstMin, f32 dstMax, f32 x) {
    return (x - srcMin) / (srcMax - srcMin) * (dstMax - dstMin) + dstMin;
}

void render_hud_power_meter(void) {
    s16 shownHealthWedges = gHudDisplay.wedges;
    f32 powerMeterX, powerMeterY;
    f32 srcX, srcY, dstX, dstY;
    f32 interpolation, shakiness, range;
    prevHurt = hurt;
    hurt = gMarioStates[0].hurtCounter;
    if (hurtTimer != 0) {
        hurtTimer--;
        if (hurtTimer < 16 && gMarioStates[0].health < 0x0300) hurtTimer = 16;
    }
    if (hurt != 0 && prevHurt == 0) hurtTimer = 90;
    powerMeterX = SCREEN_CENTER_X;
    powerMeterY = SCREEN_HEIGHT - 32 - 10;
    if (hurtTimer > 15) {
        powerMeterX = SCREEN_WIDTH / 2.f - 28;
        powerMeterY = SCREEN_HEIGHT / 2.f + 32;
    }
    else if (hurtTimer <= 15 && hurtTimer != 0) {
        srcX = SCREEN_WIDTH / 2.f - 28;
        srcY = SCREEN_HEIGHT / 2.f + 32;
        dstX = powerMeterX;
        dstY = powerMeterY;
        interpolation = 1 - sqr(1 - map_value(15, 1, 0, 1, hurtTimer));
        powerMeterX = map_value(0, 1, srcX, dstX, interpolation);
        powerMeterY = map_value(0, 1, srcY, dstY, interpolation);
    }
    shakiness = gMarioStates[0].hurtCounter;
    range = (s32)(shakiness * 3);
    if (range > 0) {
        powerMeterX = powerMeterX + (random_float() * (range + range) - range);
        powerMeterY = powerMeterY + (random_float() * (range + range) - range);
    }
    sPowerMeterHUD.x = powerMeterX - 12;
    sPowerMeterHUD.y = powerMeterY;
    render_dl_power_meter(shownHealthWedges);
}



/**
 * Renders power meter health segment texture using a table list.
 */
void render_luigi_meter_health_segment(s16 numHealthWedges) {
    u8 *(*healthLUT)[];

    healthLUT = segmented_to_virtual(luigi_meter_health_segments_lut);

    gDPPipeSync(gDisplayListHead++);
    g_Tani_LoadTextureImage2(gDisplayListHead++, (*healthLUT)[numHealthWedges * 2], G_IM_FMT_RGBA,
                             G_IM_SIZ_16b, 32, 64, 0, 7) gSP1Triangle(gDisplayListHead++, 0, 1, 2, 0);
    gSP1Triangle(gDisplayListHead++, 0, 2, 3, 0);
    g_Tani_LoadTextureImage2(gDisplayListHead++, (*healthLUT)[(numHealthWedges * 2) + 1], G_IM_FMT_RGBA,
                             G_IM_SIZ_16b, 32, 64, 0, 7) gSP1Triangle(gDisplayListHead++, 4, 5, 6, 0);
    gSP1Triangle(gDisplayListHead++, 4, 6, 7, 0);
}



/**
 * Renders breath meter display lists.
 * That includes the base and the colored segment textures.
 */
void render_dl_breath_meter(s16 numBreathWedges) {
    Mtx *translateMtx;
    Mtx *scaleMtx;

    translateMtx = alloc_display_list(sizeof(Mtx));
    scaleMtx = alloc_display_list(sizeof(Mtx));

    if (translateMtx == NULL) {
        return;
    }

    guTranslate(translateMtx, (f32) sBreathMeterHUD.x, (f32) sBreathMeterHUD.y, 0);
    guScale(scaleMtx, 0.75f, 0.75f, 1.0f);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(translateMtx++),
              G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(scaleMtx++),
              G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

    gSPDisplayList(gDisplayListHead++, &dl_power_meter_base);

    render_luigi_meter_health_segment(numBreathWedges);

    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}



void render_hud_power_meter_luigi(void) {
    s16 shownHealthWedges = gHudDisplay.wedgesluigi;
    f32 powerMeterX, powerMeterY;
    f32 srcX, srcY, dstX, dstY;
    f32 interpolation, shakiness, range;
    prevHurt = hurt;
    hurt = gMarioStates[1].hurtCounter;
    if (hurtTimer != 0) {
        hurtTimer--;
        if (hurtTimer < 16 && gMarioStates[1].lhealth < 0x0300) hurtTimer = 16;
    }
    if (hurt != 0 && prevHurt == 0) hurtTimer = 90;
    powerMeterX = SCREEN_CENTER_X;
    powerMeterY = SCREEN_HEIGHT - 32 - 10;
    if (hurtTimer > 15) {
        powerMeterX = SCREEN_WIDTH / 2.f - 28;
        powerMeterY = SCREEN_HEIGHT / 2.f + 32;
    }
    else if (hurtTimer <= 15 && hurtTimer != 0) {
        srcX = SCREEN_WIDTH / 2.f - 28;
        srcY = SCREEN_HEIGHT / 2.f + 32;
        dstX = powerMeterX;
        dstY = powerMeterY;
        interpolation = 1 - sqr(1 - map_value(15, 1, 0, 1, hurtTimer));
        powerMeterX = map_value(0, 1, srcX, dstX, interpolation);
        powerMeterY = map_value(0, 1, srcY, dstY, interpolation);
    }
    shakiness = gMarioStates[1].hurtCounter;
    range = (s32)(shakiness * 3);
    if (range > 0) {
        powerMeterX = powerMeterX + (random_float() * (range + range) - range);
        powerMeterY = powerMeterY + (random_float() * (range + range) - range);
    }
    sPowerMeterHUD.x = powerMeterX + 12;
    sPowerMeterHUD.y = powerMeterY;
    render_dl_breath_meter(shownHealthWedges);
}


/**
 * Renders the amount of lives Mario has.
 */
void render_hud_mario_lives(void) {
if(gSoundMode == SOUND_MODE_MONO){
    print_text(SCREEN_CENTER_X - 15 - 7, HUD_TOP_Y - 60, ","); // 'Mario Head' glyph
    print_text(SCREEN_CENTER_X - 7, HUD_TOP_Y - 60, "*"); // 'X' glyph
    print_text_fmt_int(SCREEN_CENTER_X + 15 - 7, HUD_TOP_Y - 60, "%d", gHudDisplay.lives);
}
if(gSoundMode == SOUND_MODE_STEREO){
    print_text(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(20) - (1 - gHudOffset) * 120, HUD_TOP_Y, ","); // 'Mario Head' glyph
    print_text(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(36) - (1 - gHudOffset) * 120, HUD_TOP_Y, "*"); // 'X' glyph
    print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(51) - (1 - gHudOffset) * 120, HUD_TOP_Y, "%d", gHudDisplay.lives);
    print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(71) + (1 - gHudOffset) * 120, HUD_TOP_Y, "+"); // 'Luigi Head' glyph
    print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(56) + (1 - gHudOffset) * 120, HUD_TOP_Y, "*"); // 'X' glyph
    print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(40) + (1 - gHudOffset) * 120, HUD_TOP_Y, "%d", gHudDisplay.lives);
}
}

#ifdef VANILLA_STYLE_CUSTOM_DEBUG
void render_debug_mode(void) {
    print_text(180, 40, "DEBUG MODE");
    print_text_fmt_int(5, 20, "Z %d", gMarioState->pos[2]);
    print_text_fmt_int(5, 40, "Y %d", gMarioState->pos[1]);
    print_text_fmt_int(5, 60, "X %d", gMarioState->pos[0]);
    print_text_fmt_int(10, 100, "SPD %d", (s32) gMarioState->forwardVel);
    print_text_fmt_int(10, 120, "ANG 0*%04x", (u16) gMarioState->faceAngle[1]);
    print_fps(10,80);
}
#endif

/**
 * Renders the amount of coins collected.
 */
void render_hud_coins(void) {
    print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(132) - 4 , HUD_TOP_Y, "$"); // 'Coin' glyph
    print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(119) - 2, HUD_TOP_Y, "*"); // 'X' glyph
    print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(103) -4, HUD_TOP_Y, "%d", gHudDisplay.coins);
}

/**
 * Renders the amount of coins collected.
 */
void render_hud_meme(void) {
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
    print_generic_string(0, 0, "THIS ROM HACK DOES NOT SUPPORT PJ64"); // 'Coin' glyph
    print_generic_string(0, 0, "IF U USE PJ64 U GOT HELL BEFOFRE U DIE"); // 'Coin' glyph
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
}

/**
 * Renders the amount of stars collected.
 * Disables "X" glyph when Mario has 100 stars or more.
 */
void render_hud_stars(void) {
    if (gHudFlash == HUD_FLASH_STARS && gGlobalTimer & 0x8) return;
    print_text(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(100) - 10, HUD_TOP_Y, "^"); // 'Star' glyph
    print_text(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(116) - 10, HUD_TOP_Y, "*"); // 'X' glyph
    print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(129) - 10,HUD_TOP_Y, "%d", gHudDisplay.stars);
}

/**
 * Unused function that renders the amount of keys collected.
 * Leftover function from the beta version of the game.
 */
void render_hud_keys(void) {
    char* keys[] = { ".", "/" };
    for (u8 i = 0; i < 3; i++) {
        print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(71) + (1 - gHudOffset) * 120 + i * 15, HUD_TOP_Y - 60, keys[i < gMarioState->numKeys]);
    }
}

/**
 * Renders the timer when Mario start sliding in PSS.
 */
void render_hud_timer(void) {
    Texture *(*hudLUT)[58] = segmented_to_virtual(&main_hud_lut);
    u16 timerValFrames = gHudDisplay.timer;
    u16 timerMins = timerValFrames / (30 * 60);
    u16 timerSecs = (timerValFrames - (timerMins * 1800)) / 30;
    u16 timerFracSecs = ((timerValFrames - (timerMins * 1800) - (timerSecs * 30)) & 0xFFFF) / 3;

#if MULTILANG
    switch (eu_get_language()) {
        case LANGUAGE_ENGLISH: print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(150), 185,  "TIME"); break;
        case LANGUAGE_FRENCH:  print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(155), 185, "TEMPS"); break;
        case LANGUAGE_GERMAN:  print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(150), 185,  "ZEIT"); break;
    }
#else
    print_text(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(150), 185, "TIME");
#endif

    print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(91), 185, "%0d", timerMins);
    print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(71), 185, "%02d", timerSecs);
    print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(37), 185, "%d", timerFracSecs);

    gSPDisplayList(gDisplayListHead++, dl_hud_img_begin);
    render_hud_tex_lut(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(81), 32, (*hudLUT)[GLYPH_APOSTROPHE]);
    render_hud_tex_lut(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(46), 32, (*hudLUT)[GLYPH_DOUBLE_QUOTE]);
    gSPDisplayList(gDisplayListHead++, dl_hud_img_end);
}

/**
 * Sets HUD status camera value depending of the actions
 * defined in update_camera_status.
 */
void set_hud_camera_status(s16 status) {
    sCameraHUD.status = status;
}

/**
 * Renders camera HUD glyphs using a table list, depending of
 * the camera status called, a defined glyph is rendered.
 */
void render_hud_camera_status(void) {
    Texture *(*cameraLUT)[6] = segmented_to_virtual(&main_hud_camera_lut);
    s32 x = GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(HUD_CAMERA_X);
    s32 y = 205;

    if (sCameraHUD.status == CAM_STATUS_NONE) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, dl_hud_img_begin);
    render_hud_tex_lut(x, y, (*cameraLUT)[GLYPH_CAM_CAMERA]);

    switch (sCameraHUD.status & CAM_STATUS_MODE_GROUP) {
        case CAM_STATUS_MARIO:
            render_hud_tex_lut(x + 16, y, (*cameraLUT)[GLYPH_CAM_MARIO_HEAD]);
            break;
        case CAM_STATUS_LAKITU:
            render_hud_tex_lut(x + 16, y, (*cameraLUT)[GLYPH_CAM_LAKITU_HEAD]);
            break;
        case CAM_STATUS_FIXED:
            render_hud_tex_lut(x + 16, y, (*cameraLUT)[GLYPH_CAM_FIXED]);
            break;
    }

    switch (sCameraHUD.status & CAM_STATUS_C_MODE_GROUP) {
        case CAM_STATUS_C_DOWN:
            render_hud_small_tex_lut(x + 4, y + 16, (*cameraLUT)[GLYPH_CAM_ARROW_DOWN]);
            break;
        case CAM_STATUS_C_UP:
            render_hud_small_tex_lut(x + 4, y - 8, (*cameraLUT)[GLYPH_CAM_ARROW_UP]);
            break;
    }

    gSPDisplayList(gDisplayListHead++, dl_hud_img_end);
}

void render_hud_red_coins() {
    print_text(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(8), 16 - (1 - gHudRedCoinOffset) * 120, "@"); // 'Coin' glyph
    print_text(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(23), 16 - (1 - gHudRedCoinOffset) * 120, "*"); // 'X' glyph
    print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(35), 16 - (1 - gHudRedCoinOffset) * 120, "%d", gRedCoinsCollected);
}

void hud_set_idle(u8 idle) {
    gHudIdle = idle;
}

void hud_force_idle() {
    gHudIdleTimer = MAX(15, gHudIdleTimer);
    gHudForceIdle = 150;
}

/**
 * Render HUD strings using hudDisplayFlags with it's render functions,
 * excluding the cannon reticle which detects a camera preset for it.
 */
void render_hud(void) {
    if (gHudIdle || gHudForceIdle > 0) {
        gHudIdleTimer++;
        if (gHudIdleTimer > 60) gHudIdleTimer = 60;
    }
    else {
        gHudIdleTimer--;
        if (gHudIdleTimer < 0) gHudIdleTimer = 0;
    }
    if (gHudForceIdle > 0) gHudForceIdle--;
    gHudOffset = (1 - (1 - gHudIdleTimer / 60.f) * (1 - gHudIdleTimer / 60.f));
    if (gRedCoinsCollected > 0) {
        gHudRedCoinTimer++;
        if (gHudRedCoinTimer > 60) gHudRedCoinTimer = 60;
        gHudRedCoinOffset = (1 - (1 - gHudRedCoinTimer / 60.f) * (1 - gHudRedCoinTimer / 60.f));
    }
    s16 hudDisplayFlags = gHudDisplay.flags;

    if (hudDisplayFlags == HUD_DISPLAY_NONE) {
        sPowerMeterHUD.animation = POWER_METER_HIDDEN;
        sPowerMeterStoredHealth = 8;
        sPowerMeterVisibleTimer = 0;
#ifdef BREATH_METER
        sBreathMeterHUD.animation = BREATH_METER_HIDDEN;
        sBreathMeterStoredValue = 8;
        sBreathMeterVisibleTimer = 0;
#endif
    } else {
#ifdef VERSION_EU
        // basically create_dl_ortho_matrix but guOrtho screen width is different
        Mtx *mtx = alloc_display_list(sizeof(*mtx));

        if (mtx == NULL) {
            return;
        }

        create_dl_identity_matrix();
        guOrtho(mtx, -16.0f, SCREEN_WIDTH + 16, 0, SCREEN_HEIGHT, -10.0f, 10.0f, 1.0f);
        gSPPerspNormalize(gDisplayListHead++, 0xFFFF);
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(mtx),
                  G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
#else
        create_dl_ortho_matrix();
#endif

        if (gCurrentArea != NULL && gCurrentArea->camera->mode == CAMERA_MODE_INSIDE_CANNON) {
            render_hud_cannon_reticle();
        }

#ifdef ENABLE_LIVES
        if (hudDisplayFlags & HUD_DISPLAY_FLAG_LIVES) {
            render_hud_mario_lives();
        }
#endif

        if (hudDisplayFlags & HUD_DISPLAY_FLAG_COIN_COUNT) {
            render_hud_coins();
            render_hud_red_coins();
        }

        if (hudDisplayFlags & HUD_DISPLAY_FLAG_STAR_COUNT) {
            render_hud_stars();
        }


        if (hudDisplayFlags & HUD_DISPLAY_FLAG_CAMERA_AND_POWER) render_hud_power_meter_luigi();


        if (hudDisplayFlags & HUD_DISPLAY_FLAG_CAMERA_AND_POWER) {
            render_hud_power_meter();
#ifdef PUPPYCAM
            if (!gPuppyCam.enabled) {
#endif
            render_hud_camera_status();
#ifdef PUPPYCAM
            }
#endif
        }

        if (hudDisplayFlags & HUD_DISPLAY_FLAG_TIMER) {
            render_hud_timer();
        }

#ifdef VANILLA_STYLE_CUSTOM_DEBUG
        if (gCustomDebugMode) {
            render_debug_mode();
        }
#endif
    }
}