#ifndef SPECIAL_PRESETS_H
#define SPECIAL_PRESETS_H

#include "special_preset_names.h"
#include "behavior_data.h"
#include "model_ids.h"

// Special Preset types
#define SPTYPE_NO_YROT_OR_PARAMS  0 // object is 8-bytes long, no y-rotation or any behavior params
#define SPTYPE_YROT_NO_PARAMS     1 // object is 10-bytes long, has y-rotation but no params
#define SPTYPE_PARAMS_AND_YROT    2 // object is 12-bytes long, has y-rotation and params
#define SPTYPE_UNKNOWN            3 // object is 14-bytes long, has 3 extra shorts that get converted to floats.
#define SPTYPE_DEF_PARAM_AND_YROT 4 // object is 10-bytes long, has y-rotation and uses the default param

struct SpecialPreset {
    /*00*/ u8  preset_id;
    /*01*/ u8  type;      // Determines whether object is 8, 10, 12 or 14 bytes long.
    /*02*/ u8  defParam;  // Default parameter, only used when type is SPTYPE_DEF_PARAM_AND_YROT
    /*03*/ ModelID16  model;
    /*04*/ const BehaviorScript *behavior;
};

// Some Models ID's are missing their names because they are probably unused

static struct SpecialPreset SpecialObjectPresets[] = {
};

#endif // SPECIAL_PRESETS_H
