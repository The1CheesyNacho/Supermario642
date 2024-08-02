#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "moving_texture_macros.h"
#include "textures.h"

#include "make_const_nonconst.h"
#include "levels/rr/texture.inc.c"
#include "levels/rr/areas/1/1/model.inc.c"
#include "levels/rr/areas/1/2/model.inc.c"
#include "levels/rr/areas/1/3/model.inc.c"
#include "levels/rr/areas/1/4/model.inc.c"
#include "levels/rr/areas/1/5/model.inc.c"
#include "levels/rr/areas/1/6/model.inc.c"
#include "levels/rr/areas/1/7/model.inc.c"
#include "levels/rr/areas/1/8/model.inc.c"
#include "levels/rr/areas/1/9/model.inc.c"
#include "levels/rr/areas/1/10/model.inc.c"
#include "levels/rr/areas/1/11/1.inc.c"
#include "levels/rr/areas/1/11/2.inc.c"
#include "levels/rr/areas/1/12/model.inc.c"
#include "levels/rr/areas/1/13/model.inc.c"
#include "levels/rr/areas/1/14/model.inc.c"
#include "levels/rr/areas/1/15/model.inc.c"
#include "levels/rr/areas/1/16/model.inc.c"
#include "levels/rr/areas/1/17/model.inc.c"
#include "levels/rr/areas/1/18/model.inc.c"
#include "levels/rr/areas/1/19/model.inc.c"
#include "levels/rr/areas/1/20/model.inc.c"
#include "levels/rr/areas/1/21/model.inc.c"
#include "levels/rr/flying_carpet/model.inc.c"
#include "levels/rr/swinging_platform/model.inc.c"
#include "levels/rr/rotating_bridge_platform/model.inc.c"
#include "levels/rr/cruiser_wing/model.inc.c"
#include "levels/rr/octagonal_platform/model.inc.c"
#include "levels/rr/sliding_platform/model.inc.c"
#include "levels/rr/pyramid_platform/model.inc.c" // Unused
#include "levels/rr/elevator_platform/model.inc.c"
#include "levels/rr/seesaw_platform/model.inc.c"
#include "levels/rr/donut_block/1.inc.c"
#include "levels/rr/donut_block/2.inc.c"
#include "levels/rr/l_platform/model.inc.c" // also unused
#include "levels/rr/tricky_triangles_1/model.inc.c"
#include "levels/rr/tricky_triangles_2/model.inc.c"
#include "levels/rr/tricky_triangles_3/model.inc.c"
#include "levels/rr/tricky_triangles_4/model.inc.c"
#include "levels/rr/tricky_triangles_5/model.inc.c"
#include "levels/rr/flying_carpet/collision.inc.c"
#include "levels/rr/swinging_platform/collision.inc.c"
#include "levels/rr/rotating_bridge_platform/collision.inc.c"
#include "levels/rr/octagonal_platform/collision.inc.c"
#include "levels/rr/sliding_platform/collision.inc.c"
#include "levels/rr/pyramid_platform/collision.inc.c"
#include "levels/rr/elevator_platform/collision.inc.c"
#include "levels/rr/seesaw_platform/collision.inc.c"
#include "levels/rr/donut_block/collision.inc.c"
#include "levels/rr/l_platform/collision.inc.c"
#include "levels/rr/tricky_triangles_1/collision.inc.c"
#include "levels/rr/tricky_triangles_2/collision.inc.c"
#include "levels/rr/tricky_triangles_3/collision.inc.c"
#include "levels/rr/tricky_triangles_4/collision.inc.c"
#include "levels/rr/tricky_triangles_5/collision.inc.c"
#include "levels/rr/areas/1/collision.inc.c"
#include "levels/rr/areas/1/trajectory.inc.c"
