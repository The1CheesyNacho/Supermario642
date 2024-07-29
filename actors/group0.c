#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

// Note: This bin does not use bin IDs, unlike the other segmented bins.
#include "mario/model.inc.c"

#include "bubble/model.inc.c"

#include "walk_smoke/model.inc.c"

#include "burn_smoke/model.inc.c"

#include "small_water_splash/model.inc.c"

#include "water_wave/model.inc.c"

#include "sparkle/model.inc.c"

#include "water_splash/model.inc.c"

#include "white_particle_small/model.inc.c"

#include "sparkle_animation/model.inc.c"

#ifdef S2DEX_TEXT_ENGINE
#include "src/s2d_engine/s2d_config.h"
#include FONT_C_FILE
#endif

#include "luigi/model.inc.c"
#include "wario/model.inc.c"
<<<<<<< HEAD
#include "waluigi/model.inc.c"
=======
#include "waluigi/model.inc.c"
#include "metal_mario/model.inc.c"
#include "toad_player/model.inc.c"
>>>>>>> 84eac60a11a495b05f5bbc6f08a29db4f270dc0d
