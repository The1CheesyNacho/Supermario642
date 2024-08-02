#ifndef MENU_HEADER_H
#define MENU_HEADER_H

#include "types.h"

// geo
extern const GeoLayout geo_menu_mario_save_button[];
extern const GeoLayout geo_menu_mario_save_button_fade[];
extern const GeoLayout geo_menu_wario_save_button_fade[];
extern const GeoLayout geo_menu_luigi_save_button_fade[];
extern const GeoLayout geo_menu_waluigi_save_button_fade[];
extern const GeoLayout geo_menu_mario_new_button[];
extern const GeoLayout geo_menu_mario_new_button_fade[];
extern const GeoLayout geo_menu_erase_button[];
extern const GeoLayout geo_menu_copy_button[];
extern const GeoLayout geo_menu_file_button[];
extern const GeoLayout geo_menu_score_button[];
extern const GeoLayout geo_menu_sound_button[];
extern const GeoLayout geo_menu_generic_button[];
extern const GeoLayout geo_menu_file_select_strings_and_menu_cursor[];
extern const GeoLayout geo_menu_act_selector_strings[];

// leveldata
<<<<<<< HEAD
extern const Gfx dl_menu_mario_save_button_base[];
extern const Gfx dl_menu_wario_save_button_base[];
extern const Gfx dl_menu_luigi_save_button_base[];
extern const Gfx dl_menu_waluigi_save_button_base[];
extern const Gfx dl_menu_mario_new_button_base[];
extern const Gfx dl_menu_save_button_back[];
extern const Gfx dl_menu_save_button_fade_back[];
extern const Gfx dl_menu_erase_button[];
extern const Gfx dl_menu_copy_button[];
extern const Gfx dl_menu_file_button[];
extern const Gfx dl_menu_score_button[];
extern const Gfx dl_menu_sound_button[];
extern const Gfx dl_menu_generic_button[];
extern const Gfx dl_menu_idle_hand[];
extern const Gfx dl_menu_grabbing_hand[];
extern const Texture *const menu_hud_lut[];
extern const Texture *const menu_font_lut[];
extern const Gfx dl_menu_ia8_text_begin[];
extern const Gfx dl_menu_ia8_text_end[];
extern const Gfx dl_menu_rgba16_wood_course[];
extern const Gfx dl_menu_texture_course_upper[];
extern const Gfx dl_menu_texture_niveau_upper[];
extern const Gfx dl_menu_texture_kurs_upper[];
extern const Collision main_menu_seg7_collision[];
#ifdef VERSION_EU
extern const u8 eu_course_strings_en_table[];
extern const u8 eu_course_strings_fr_table[];
extern const u8 eu_course_strings_de_table[];
=======
extern Gfx dl_menu_mario_save_button_base[];
extern Gfx dl_menu_mario_new_button_base[];
extern Gfx dl_menu_save_button_back[];
extern Gfx dl_menu_save_button_fade_back[];
extern Gfx dl_menu_erase_button[];
extern Gfx dl_menu_copy_button[];
extern Gfx dl_menu_file_button[];
extern Gfx dl_menu_score_button[];
extern Gfx dl_menu_sound_button[];
extern Gfx dl_menu_generic_button[];
extern Gfx dl_menu_idle_hand[];
extern Gfx dl_menu_grabbing_hand[];
extern struct DiacriticLUTEntry menu_font_diacritic_lut[];
extern struct AsciiCharLUTEntry menu_font_lut[];
extern struct Utf8LUT menu_font_utf8_lut;
extern Texture texture_menu_font_char_umlaut[];
extern Gfx dl_menu_ia8_text_begin[];
extern Gfx dl_menu_ia8_text_end[];
extern Gfx dl_menu_rgba16_wood_course[];
extern Collision main_menu_seg7_collision[];

#ifdef MULTILANG
extern const char *(*course_strings_language_table[])[];
extern Gfx dl_menu_rgba16_wood_course_end[];
extern Gfx dl_menu_texture_course_upper[];
extern const char *course_strings_en_table[];
#ifdef ENABLE_FRENCH
extern Gfx dl_menu_texture_niveau_upper[];
extern const char *course_strings_fr_table[];
#endif
#ifdef ENABLE_GERMAN
extern Gfx dl_menu_texture_kurs_upper[];
extern const char *course_strings_de_table[];
#endif
#ifdef ENABLE_JAPANESE
extern const char *course_strings_jp_table[];
#endif
#ifdef ENABLE_SPANISH
extern const char *course_strings_es_table[];
extern Gfx dl_menu_texture_nivel_upper[];
#endif
>>>>>>> 3423721b4b0ee5c68ecacc0608a8b07a297165e3
#endif

// script
extern const LevelScript level_main_menu_entry_file_select[];
extern const LevelScript level_main_menu_entry_act_select[];

#endif
