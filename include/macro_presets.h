#ifndef MACRO_PRESETS_H
#define MACRO_PRESETS_H

#include "macro_preset_names.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "object_constants.h"

struct MacroPreset {
    /*0x00*/ const BehaviorScript *behavior;
    /*0x04*/ ModelID16 model;
    /*0x06*/ s16 param;
};

struct MacroPreset MacroObjectPresets[] = {
};

#endif // MACRO_PRESETS_H
