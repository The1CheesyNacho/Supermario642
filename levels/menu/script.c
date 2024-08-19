#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "game/area.h"
#include "game/level_update.h"
#include "menu/file_select.h"
#include "game/game_init.h"
#include "menu/star_select.h"

#include "levels/scripts.h"

#include "actors/common1.h"

#include "make_const_nonconst.h"
#include "levels/menu/header.h"

const LevelScript level_main_menu_entry_file_select[] = {
    SET_REG(TEST_LEVEL),
    CALL(     /*arg*/ 0, /*func*/ lvl_start_at_correct_location),
    EXECUTE(/*seg*/ SEGMENT_GLOBAL_LEVEL_SCRIPT, /*script*/ _scriptsSegmentRomStart, /*scriptEnd*/ _scriptsSegmentRomEnd, /*entry*/ level_main_scripts_entry),
    EXIT(),
};

const LevelScript level_main_menu_entry_act_select_exit[] = {
    EXIT(),
};

const LevelScript level_main_menu_entry_act_select[] = {
    CALL(/*arg*/ 0, /*func*/ lvl_set_current_level),
    JUMP_IF(/*op*/ OP_EQ, /*arg*/ FALSE, (level_main_menu_entry_act_select_exit)),
    INIT_LEVEL(),
    LOAD_GODDARD(),
    LOAD_LEVEL_DATA(menu),
    ALLOC_LEVEL_POOL(),

    AREA(/*index*/ 2, geo_menu_act_selector_strings),
        OBJECT(/*model*/ MODEL_NONE, /*pos*/ 0, -100, 0, /*angle*/ 0, 0, 0, /*behParam*/ BP(0x04, 0x00, 0x00, 0x00), /*beh*/ bhvActSelector),
        TERRAIN(/*terrainData*/ main_menu_seg7_collision),
    END_AREA(),

    FREE_LEVEL_POOL(),
    LOAD_AREA(/*area*/ 2),
#ifdef NO_SEGMENTED_MEMORY
        // sVisibleStars is set to 0 during FIXED_LOAD above on N64, but not when NO_SEGMENTED_MEMORY is used.
        // lvl_init_act_selector_values_and_stars must be called here otherwise the previous
        // value is retained and causes incorrect drawing during the 16 transition frames.
        CALL(/*arg*/ 0, /*func*/ lvl_init_act_selector_values_and_stars),
#endif
    TRANSITION(/*transType*/ WARP_TRANSITION_FADE_FROM_COLOR, /*time*/ 16, /*color*/ 0xFF, 0xFF, 0xFF),
    SLEEP(/*frames*/ 16),
    SET_MENU_MUSIC(/*seq*/ 0x000D),
#ifndef NO_SEGMENTED_MEMORY
    CALL(     /*arg*/ 0, /*func*/ lvl_init_act_selector_values_and_stars),
#endif
    CALL_LOOP(/*arg*/ 0, /*func*/ lvl_update_obj_and_load_act_button_actions),
    GET_OR_SET(/*op*/ OP_SET, /*var*/ VAR_CURR_ACT_NUM),
    STOP_MUSIC(/*fadeOutTime*/ 0x00BE),
    TRANSITION(/*transType*/ WARP_TRANSITION_FADE_INTO_COLOR, /*time*/ 16, /*color*/ 0xFF, 0xFF, 0xFF),
    SLEEP(/*frames*/ 16),
    CLEAR_LEVEL(),
    SLEEP_BEFORE_EXIT(/*frames*/ 1),

    EXIT(),
};
