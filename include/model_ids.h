#ifndef MODEL_IDS_H
#define MODEL_IDS_H

#define MODEL_NONE                             0x00

/* Global models that are loaded for every level */

#define MODEL_MARIO                            0x01        // mario_geo
#define MODEL_LUIGI                            0x02        // unused
#define MODEL_WARIO                            0x03        // unused
#define MODEL_WALUIGI                          0x04        // unused



//sm642 models include 
#define MODEL_KOOPA_X                          0x05        // blue_coin_switch_geo


#define MODEL_YOSHI                            0x55        // yoshi_geo
#define MODEL_BUBBLY_TREE                  0x17        // bubbly_tree_geo
#define MODEL_COURTYARD_SPIKY_TREE             0x18        // spiky_tree_geo
#define MODEL_SNOW_TREE                    0x19        // snow_tree_geo
#define MODEL_SSL_PALM_TREE                    0x1B        // palm_tree_geo
#define MODEL_CASTLE_CASTLE_DOOR               0x1C        // castle_door_geo - unused, original id
#define MODEL_CASTLE_WOODEN_DOOR_UNUSED        0x1D        // wooden_door_geo - unused, original id
#define MODEL_UNKNOWN_DOOR_1E                  0x1E        // referenced in special presets, undefined
#define MODEL_UNKNOWN_DOOR_21                  0x21        // referenced in special presets, undefined
#define MODEL_CASTLE_DOOR_0_STARS              0x22        // castle_door_0_star_geo
#define MODEL_CASTLE_DOOR_1_STAR               0x23        // castle_door_1_star_geo
#define MODEL_CASTLE_DOOR_3_STARS              0x24        // castle_door_3_stars_geo
#define MODEL_CASTLE_KEY_DOOR                  0x25        // key_door_geo
#define MODEL_CASTLE_WOODEN_DOOR               0x27        // wooden_door_geo
#define MODEL_COURTYARD_WOODEN_DOOR            0x27        // wooden_door_geo
#define MODEL_CCM_CABIN_DOOR                   0x27        // cabin_door_geo
#define MODEL_UNKNOWN_DOOR_28                  0x28        // referenced in special presets, undefined
#define MODEL_CASTLE_METAL_DOOR                0x29        // metal_door_geo
#define MODEL_CASTLE_GROUNDS_METAL_DOOR        0x29        // metal_door_geo

// Level model ID

// actor model IDs

// first set of actor bins (0x54-0x63)
// group 1
#define MODEL_BULLET_BILL                 0x54        // bullet_bill_geo
#define MODEL_YELLOW_SPHERE               0x55        // yellow_sphere_geo
#define MODEL_HOOT                        0x56        // hoot_geo
#define MODEL_YOSHI_EGG                   0x57        // yoshi_egg_geo
#define MODEL_THWOMP                      0x58        // thwomp_geo
#define MODEL_THWIMP                      0x8D        // thwomp_geo
#define MODEL_HEAVE_HO                    0x59        // heave_ho_geo

// group 2
#define MODEL_BLARGG                      0x54        // blargg_geo
#define MODEL_BULLY                       0x56        // bully_geo
#define MODEL_BULLY_BOSS                  0x57        // bully_boss_geo
#define MODEL_MOTOS                       0xE2        // bully_geo

// group 3
#define MODEL_WATER_BOMB                  0x54        // water_bomb_geo
#define MODEL_WATER_BOMB_SHADOW           0x55        // water_bomb_shadow_geo
#define MODEL_KING_BOBOMB                 0x56        // king_bobomb_geo

// group 4
#define MODEL_MANTA_RAY                   0x54        // manta_seg5_geo_05008D14
#define MODEL_UNAGI                       0x55        // unagi_geo
#define MODEL_SUSHI                       0x56        // sushi_geo
#define MODEL_DL_WHIRLPOOL                0x57        // whirlpool_seg5_dl_05013CB8
#define MODEL_CLAM_SHELL                  0x58        // clam_shell_geo

// group 5
#define MODEL_POKEY_HEAD                  0x54        // pokey_head_geo
#define MODEL_POKEY_BODY_PART             0x55        // pokey_body_part_geo
#define MODEL_TWEESTER                    0x56        // tweester_geo
#define MODEL_KLEPTO                      0x57        // klepto_geo
#define MODEL_EYEROK_LEFT_HAND            0x58        // eyerok_left_hand_geo
#define MODEL_EYEROK_RIGHT_HAND           0x59        // eyerok_right_hand_geo

// group 6
#define MODEL_DL_MONTY_MOLE_HOLE          0x54        // monty_mole_hole_seg5_dl_05000840
#define MODEL_MONTY_MOLE                  0x55        // monty_mole_geo
#define MODEL_UKIKI                       0x56        // ukiki_geo
#define MODEL_FWOOSH                      0x57        // fwoosh_geo

// group 7
#define MODEL_SPINDRIFT                   0x54        // spindrift_geo
#define MODEL_MR_BLIZZARD_HIDDEN          0x55        // mr_blizzard_hidden_geo
#define MODEL_MR_BLIZZARD                 0x56        // mr_blizzard_geo
#define MODEL_PENGUIN                     0x57        // penguin_geo

// group 8
#define MODEL_CAP_SWITCH_EXCLAMATION      0x54        // cap_switch_exclamation_seg5_dl_05002E00
#define MODEL_CAP_SWITCH                  0x55        // cap_switch_geo
#define MODEL_CAP_SWITCH_BASE             0x56        // cap_switch_base_seg5_dl_05003120

// group 9
#define MODEL_BOO                         0x54        // boo_geo
#define MODEL_BETA_BOO_KEY                0x55        // small_key_geo
#define MODEL_HAUNTED_CHAIR               0x56        // haunted_chair_geo
#define MODEL_MAD_PIANO                   0x57        // mad_piano_geo
#define MODEL_BOOKEND_PART                0x58        // bookend_part_geo
#define MODEL_BOOKEND                     0x59        // bookend_geo
#define MODEL_HAUNTED_CAGE                0x5A        // haunted_cage_geo

// group 10
#define MODEL_BIRDS                       0x54        // birds_geo

// group 11
#define MODEL_ENEMY_LAKITU                0x54        // enemy_lakitu_geo
#define MODEL_SPINY_BALL                  0x55        // spiny_ball_geo
#define MODEL_SPINY                       0x56        // spiny_geo
#define MODEL_WIGGLER_HEAD                0x57        // wiggler_head_geo
#define MODEL_WIGGLER_BODY                0x58        // wiggler_body_geo
#define MODEL_BUBBA                       0x59        // bubba_geo

// referenced in macro presets. Unknown usage.
#define MODEL_UNKNOWN_54                  0x54
#define MODEL_UNKNOWN_58                  0x58

// second set of actor bins, (0x64-0x73)
// group 12
#define MODEL_BOWSER                      0x64        // bowser_geo
#define MODEL_BOWSER_BOMB_CHILD_OBJ       0x65        // bowser_bomb_geo - Spawns as a chill object in bowser's behavior command, causing an explosion if it touches a bomb
#define MODEL_BOWSER_SMOKE                0x66        // bowser_impact_smoke_geo
#define MODEL_BOWSER_FLAMES               0x67        // bowser_flames_geo
#define MODEL_BOWSER_WAVE                 0x68        // invisible_bowser_accessory_geo
#define MODEL_BOWSER_NO_SHADOW            0x69        // bowser_geo_no_shadow

// group 13
#define MODEL_BUB                         0x64        // cheep_cheep_geo
#define MODEL_TREASURE_CHEST_BASE         0x65        // treasure_chest_base_geo
#define MODEL_TREASURE_CHEST_LID          0x66        // treasure_chest_lid_geo
#define MODEL_CYAN_FISH                   0x67        // cyan_fish_geo
#define MODEL_WATER_RING                  0x68        // water_ring_geo
#define MODEL_SKEETER                     0x69        // skeeter_geo

// group 14
#define MODEL_PIRANHA_PLANT               0x64        // piranha_plant_geo
#define MODEL_WHOMP                       0x67        // whomp_geo
#define MODEL_KOOPA_WITH_SHELL            0x68        // koopa_with_shell_geo
#define MODEL_RED_KOOPA_WITH_SHELL        0x7E        // koopa_with_shell_geo
#define MODEL_METALLIC_BALL               0x65        // metallic_ball_geo
#define MODEL_CHAIN_CHOMP                 0x66        // chain_chomp
#define MODEL_KOOPA_FLAG                  0x6A        // koopa_flag_geo
#define MODEL_WOODEN_POST                 0x6B        // wooden_post_geo

// group 15
#define MODEL_MIPS                        0x64        // mips_geo
#define MODEL_BOO_CASTLE                  0x65        // boo_castle_geo
#define MODEL_LAKITU                      0x66        // lakitu_geo

// group 16
#define MODEL_CHILL_BULLY                 0x64        // chilly_chief_geo
#define MODEL_BIG_CHILL_BULLY             0x65        // chilly_chief_big_geo
#define MODEL_MONEYBAG                    0x66        // moneybag_geo

// group 17
#define MODEL_SWOOP                       0x64        // swoop_geo
#define MODEL_SCUTTLEBUG                  0x65        // scuttlebug_geo
#define MODEL_MR_I_IRIS                   0x66        // mr_i_iris_geo
#define MODEL_MR_I_BODY                   0x67        // mr_i_geo
#define MODEL_MR_I MODEL_MR_I_BODY // backwards compatibility
#define MODEL_DORRIE                      0x68        // dorrie_geo

// other models
#define MODEL_YELLOW_COIN                 0x74        // yellow_coin_geo
#define MODEL_YELLOW_COIN_NO_SHADOW       0x75        // yellow_coin_no_shadow_geo
#define MODEL_BLUE_COIN                   0x76        // blue_coin_geo
#define MODEL_BLUE_COIN_NO_SHADOW         0x77        // blue_coin_no_shadow_geo
#define MODEL_HEART                       0x78        // heart_geo
#define MODEL_TRANSPARENT_STAR            0x79        // transparent_star_geo
#define MODEL_STAR                        0x7A        // star_geo
#define MODEL_TTM_SLIDE_EXIT_PODIUM       0x7B        // ttm_geo_000DF4
#define MODEL_WOODEN_SIGNPOST             0x7C        // wooden_signpost_geo
#define MODEL_UNKNOWN_7D                  0x7D        // referenced in macro presets. Unknown usage
#define MODEL_CANNON_BARREL               0x7F        // cannon_barrel_geo
#define MODEL_CANNON_BASE                 0x80        // cannon_base_geo
#define MODEL_BREAKABLE_BOX               0x81        // breakable_box_geo
#define MODEL_BREAKABLE_BOX_NO_SHADOW     0x82        // breakable_box_no_shadow_geo
#define MODEL_BREAKABLE_BOX_SMALL MODEL_BREAKABLE_BOX_NO_SHADOW // backwards compatibility
#define MODEL_EXCLAMATION_BOX_OUTLINE     0x83        // exclamation_box_outline_geo
#define MODEL_EXCLAMATION_POINT           0x84        // exclamation_point_seg8_dl_08025F08
#define MODEL_MARIOS_WINGED_METAL_CAP     0x85        // marios_winged_metal_cap_geo
#define MODEL_MARIOS_METAL_CAP            0x86        // marios_metal_cap_geo
#define MODEL_MARIOS_WING_CAP             0x87        // marios_wing_cap_geo
#define MODEL_MARIOS_CAP                  0x88        // marios_cap_geo
#define MODEL_EXCLAMATION_BOX             0x89        // exclamation_box_geo
#define MODEL_DIRT_ANIMATION              0x8A        // dirt_animation_geo
#define MODEL_CARTOON_STAR                0x8B        // cartoon_star_geo
#define MODEL_BLUE_COIN_SWITCH            0x8C        // blue_coin_switch_geo
// find me
#define MODEL_MIST                        0x8E        // mist_geo
#define MODEL_SPARKLES_ANIMATION          0x8F        // sparkles_animation_geo
#define MODEL_RED_FLAME                   0x90        // red_flame_geo
#define MODEL_BLUE_FLAME                  0x91        // blue_flame_geo
// find me
// find me
#define MODEL_BURN_SMOKE                  0x94        // burn_smoke_geo
#define MODEL_SPARKLES                    0x95        // sparkles_geo
#define MODEL_SMOKE                       0x96        // smoke_geo
#define MODEL_BLUE_KOOPA_WITH_SHELL       0x98        // koopa_shell_geo
#define MODEL_BLUE_KOOPA_WITHOUT_SHELL    0x99        // koopa_without_shell_geo
#define MODEL_YELLOW_KOOPA_WITH_SHELL     0X9A        // koopa_without_shell_geo
#define MODEL_YELLOW_KOOPA_WITHOUT_SHELL  0X9B        // koopa_without_shell_geo
#define MODEL_KOOPA_WITHOUT_SHELL         0x9C        // burn_smoke_geo - unused & duplicated
#define MODEL_KOOPA_SHELL                 0x9D        // koopa_without_shell_geo
#define MODEL_WHITE_PARTICLE_DL           0x9E        // white_particle_dl
#define MODEL_SAND_DUST                   0x9F        // sand_seg3_dl_particle
#define MODEL_WHITE_PARTICLE              0xA0        // white_particle_dl
#define MODEL_PEBBLE                      0xA1        // pebble_seg3_dl_0301CB00
#define MODEL_LEAVES                      0xA2        // leaves_geo
#define MODEL_WAVE_TRAIL                  0xA3        // wave_trail_geo
#define MODEL_WHITE_PARTICLE_SMALL        0xA4        // white_particle_small_dl
#define MODEL_SMALL_WATER_SPLASH          0xA5        // small_water_splash_geo
#define MODEL_IDLE_WATER_WAVE             0xA6        // idle_water_wave_geo
#define MODEL_WATER_SPLASH                0xA7        // water_splash_geo
#define MODEL_BUBBLE                      0xA8        // bubble_geo
#define MODEL_METAL_MARIO                 0xA9        // bubble_geo
#define MODEL_PURPLE_MARBLE               0xAA        // purple_marble_geo
// find me
#define MODEL_UNKNOWN_AC                  0xAC        // according to an special preset, it was the original id of the castle floor trap
// find me
// find me
// find me
// find me
// find me
// find me
#define MODEL_BOWSER_BOMB                 0xB3        // bowser_bomb_geo
#define MODEL_WATER_MINE                  0xB3        // water_mine_geo
#define MODEL_BOWLING_BALL                0xB4        // bowling_ball_geo
#define MODEL_TRAMPOLINE                  0xB5        // springboard_top_geo (unused)
#define MODEL_TRAMPOLINE_CENTER           0xB6        // springboard_spring_geo (unused)
#define MODEL_TRAMPOLINE_BASE             0xB7        // springboard_bottom_geo (unused)
#define MODEL_UNKNOWN_B8                  0xB8        // referenced in special presets as a static object. Unknown usage
#define MODEL_FISH                        0xB9        // fish_geo - fish without shadow, used
#define MODEL_FISH_SHADOW                 0xBA        // fish_shadow_geo - fish with shadow, unused
#define MODEL_BUTTERFLY                   0xBB        // butterfly_geo
#define MODEL_BLACK_BOBOMB                0xBC        // black_bobomb_geo
// find me
// find me
#define MODEL_RED_KOOPA_WITHOUT_SHELL     0xBF        // koopa_without_shell_geo
#define MODEL_GOOMBA                      0xC0        // goomba_geo
#define MODEL_SEAWEED                     0xC1        // seaweed_geo
#define MODEL_AMP                         0xC2        // dAmpGeo
#define MODEL_BOBOMB_BUDDY                0xC3        // bobomb_buddy_geo
#define MODEL_MIPSY                       0xC4        // silver_coin_geo
#define MODEL_TOADSWORTH                  0xC5        // silver_coin_no_shadow_geo
#define MODEL_MICRO_GOOMBA                0xC6        // micro_goomba_geo
#define MODEL_SSL_TOX_BOX                 0xC7        // ssl_geo_000630
#define MODEL_BOWSER_KEY_CUTSCENE         0xC8        // bowser_key_cutscene_geo
#define MODEL_DL_CANNON_LID               0xC9        // cannon_closed_seg8_dl_080048E0
#define MODEL_CHECKERBOARD_PLATFORM       0xCA        // checkerboard_platform_geo
#define MODEL_RED_FLAME_SHADOW            0xCB        // red_flame_shadow_geo
#define MODEL_BOWSER_KEY                  0xCC        // bowser_key_geo
#define MODEL_EXPLOSION                   0xCD        // explosion_geo
#define MODEL_SNUFIT                      0xCE        // snufit_geo
#define MODEL_PURPLE_SWITCH               0xCF        // purple_switch_geo
#define MODEL_CASTLE_STAR_DOOR_30_STARS   0xD0        // castle_geo_000F00
#define MODEL_CASTLE_STAR_DOOR_50_STARS   0xD1        // castle_geo_000F00
#define MODEL_CCM_SNOWMAN_BASE            0xD2        // ccm_geo_0003F0
#define MODEL_GUMMY                       0xD3
#define MODEL_1UP                         0xD4        // mushroom_1up_geo
#define MODEL_CASTLE_STAR_DOOR_8_STARS    0xD5        // castle_geo_000F00
#define MODEL_CASTLE_STAR_DOOR_70_STARS   0xD6        // castle_geo_000F00
#define MODEL_RED_COIN                    0xD7        // red_coin_geo
#define MODEL_RED_COIN_NO_SHADOW          0xD8        // red_coin_no_shadow_geo
#define MODEL_METAL_BOX                   0xD9        // metal_box_geo
#define MODEL_METAL_BOX_DL                0xDA        // metal_box_dl
#define MODEL_NUMBER                      0xDB        // number_geo
#define MODEL_FLYGUY                      0xDC        // shyguy_geo
#define MODEL_TOAD                        0xDD        // toad_geo
#define MODEL_PEACH                       0xDE        // peach_geo
#define MODEL_CHUCKYA                     0xDF        // chuckya_geo
#define MODEL_WHITE_PUFF                  0xE0        // white_puff_geo
#define MODEL_TRAJECTORY_MARKER_BALL      0xE1        // bowling_ball_track_geo - duplicate used in SSL Pyramid small sized and as a track ball





//semen is yummy

#define MODEL_LEVEL_GEOMETRY_03                0xffec
#define MODEL_LEVEL_GEOMETRY_04                0xffed
#define MODEL_LEVEL_GEOMETRY_05                0xffee
#define MODEL_LEVEL_GEOMETRY_06                0xffef
#define MODEL_LEVEL_GEOMETRY_07                0xfff0
#define MODEL_LEVEL_GEOMETRY_08                0xfff1
#define MODEL_LEVEL_GEOMETRY_09                0xfff2
#define MODEL_LEVEL_GEOMETRY_0A                0xfff3
#define MODEL_LEVEL_GEOMETRY_0B                0xfff4
#define MODEL_LEVEL_GEOMETRY_0C                0xfff5
#define MODEL_LEVEL_GEOMETRY_0D                0xfff6
#define MODEL_LEVEL_GEOMETRY_0E                0xfff7
#define MODEL_LEVEL_GEOMETRY_0F                0xfff8
#define MODEL_LEVEL_GEOMETRY_10                0xfff9
#define MODEL_LEVEL_GEOMETRY_11                0xfffa
#define MODEL_LEVEL_GEOMETRY_12                0xfffb
#define MODEL_LEVEL_GEOMETRY_13                0xfffc
#define MODEL_LEVEL_GEOMETRY_14                0xfffd
#define MODEL_LEVEL_GEOMETRY_15                0xfffe
#define MODEL_LEVEL_GEOMETRY_16                0xffff


// Menu Models (overwrites Level Geometry IDs)
#define MODEL_MAIN_MENU_MARIO_SAVE_BUTTON         MODEL_LEVEL_GEOMETRY_03   // main_menu_geo_0001D0
#define MODEL_MAIN_MENU_RED_ERASE_BUTTON          MODEL_LEVEL_GEOMETRY_04   // main_menu_geo_000290
#define MODEL_MAIN_MENU_BLUE_COPY_BUTTON          MODEL_LEVEL_GEOMETRY_05   // main_menu_geo_0002B8
#define MODEL_MAIN_MENU_YELLOW_FILE_BUTTON        MODEL_LEVEL_GEOMETRY_06   // main_menu_geo_0002E0
#define MODEL_MAIN_MENU_GREEN_SCORE_BUTTON        MODEL_LEVEL_GEOMETRY_07   // main_menu_geo_000308
#define MODEL_MAIN_MENU_MARIO_SAVE_BUTTON_FADE    MODEL_LEVEL_GEOMETRY_08   // main_menu_geo_000200
#define MODEL_MAIN_MENU_WARIO_SAVE_BUTTON_FADE    MODEL_LEVEL_GEOMETRY_0D   // main_menu_geo_000200
#define MODEL_MAIN_MENU_LUIGI_SAVE_BUTTON_FADE    MODEL_LEVEL_GEOMETRY_0E   // main_menu_geo_000200
#define MODEL_MAIN_MENU_WALUIGI_SAVE_BUTTON_FADE  MODEL_LEVEL_GEOMETRY_0F   // main_menu_geo_000200
#define MODEL_MAIN_MENU_MARIO_NEW_BUTTON          MODEL_LEVEL_GEOMETRY_09   // main_menu_geo_000230
#define MODEL_MAIN_MENU_MARIO_NEW_BUTTON_FADE     MODEL_LEVEL_GEOMETRY_0A   // main_menu_geo_000260
#define MODEL_MAIN_MENU_PURPLE_SOUND_BUTTON       MODEL_LEVEL_GEOMETRY_0B   // main_menu_geo_000330
#define MODEL_MAIN_MENU_GENERIC_BUTTON            MODEL_LEVEL_GEOMETRY_0C   // main_menu_geo_000358

// level model aliases to level geometry IDs. Possibly a relic from an older level
// format that used to rely on level geometry objects. (seen in WF, LLL, etc)
#define MODEL_BITDW_WARP_PIPE                     MODEL_LEVEL_GEOMETRY_12   // warp_pipe_geo
#define MODEL_THI_WARP_PIPE                       MODEL_LEVEL_GEOMETRY_16   // warp_pipe_geo
#define MODEL_VCUTM_WARP_PIPE                     MODEL_LEVEL_GEOMETRY_16   // warp_pipe_geo
#define MODEL_CASTLE_GROUNDS_WARP_PIPE            MODEL_LEVEL_GEOMETRY_16   // warp_pipe_geo

#endif // MODEL_IDS_H
