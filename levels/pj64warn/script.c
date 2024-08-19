#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "game/level_update.h"

#include "levels/scripts.h"

#include "actors/common1.h"
#include "make_const_nonconst.h"
#include "levels/pj64warn/header.h"

const LevelScript level_pj64warn_entry[] = {
    INIT_LEVEL(),
    FIXED_LOAD(/*loadAddr*/ _goddardSegmentStart, /*romStart*/ _goddardSegmentRomStart, /*romEnd*/ _goddardSegmentRomEnd),
    LOAD_MIO0(/*seg*/ 0x07, _menu_segment_7SegmentRomStart, _menu_segment_7SegmentRomEnd),
    ALLOC_LEVEL_POOL(),
    CALL(     /*arg*/ 0, /*func*/ geo_trhing),
    EXIT(),
};
