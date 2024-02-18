#ifndef SEQ_IDS_H
#define SEQ_IDS_H

// Sometimes a sequence id is represented as one of the below ids (the base id),
// optionally OR'd with SEQ_VARIATION.
#define SEQ_BASE_ID 0x7f
#define SEQ_VARIATION 0x80
#define SEQUENCE_NONE 0xFF

#define SEQ_MENU_GAME_OVER (SEQ_WARIO | SEQ_VARIATION)

enum SeqId {
    SEQ_SOUND_PLAYER,  // 0x00
    SEQ_WARIO,  // 0x24
    SEQ_MOLD,  // 0x25
    SEQ_STARMAN,  // 0x26
    SEQ_LEVEL_INSIDE_CASTLE,  // 0x04
    SEQ_BASEMENT,  // 0x23
    SEQ_COUNT,
};

#endif // SEQ_IDS_H
