Lights1 galoomba_f3dlite_material_053_lights = gdSPDefLights1(
	0x7F, 0x7F, 0x7F,
	0xFF, 0xFF, 0xFF, 0x28, 0x28, 0x28);

Lights1 galoomba_f3dlite_material_054_lights = gdSPDefLights1(
	0x7F, 0x7F, 0x7F,
	0xFF, 0xFF, 0xFF, 0x28, 0x28, 0x28);

Lights1 galoomba_f3dlite_material_002_lights = gdSPDefLights1(
	0x7F, 0x7F, 0x7F,
	0xFF, 0xFF, 0xFF, 0x28, 0x28, 0x28);

Lights1 galoomba_f3dlite_material_001_lights = gdSPDefLights1(
	0x7F, 0x7F, 0x7F,
	0xFF, 0xFF, 0xFF, 0x28, 0x28, 0x28);

Lights1 galoomba_f3dlite_material_055_lights = gdSPDefLights1(
	0x0, 0x7F, 0x0,
	0x0, 0xFF, 0x0, 0x28, 0x28, 0x28);

Gfx galoomba_goomba_body_rgba16_ci8_aligner[] = {gsSPEndDisplayList()};
u8 galoomba_goomba_body_rgba16_ci8[] = {
	#include "actors/galoomba/goomba_body.rgba16.ci8.inc.c"
};

Gfx galoomba_goomba_body_rgba16_pal_rgba16_aligner[] = {gsSPEndDisplayList()};
u8 galoomba_goomba_body_rgba16_pal_rgba16[] = {
	#include "actors/galoomba/goomba_body.rgba16.pal"
};

Gfx galoomba_goomba_face_rgba16_ci8_aligner[] = {gsSPEndDisplayList()};
u8 galoomba_goomba_face_rgba16_ci8[] = {
	#include "actors/galoomba/goomba_face.rgba16.ci8.inc.c"
};

Gfx galoomba_goomba_face_rgba16_pal_rgba16_aligner[] = {gsSPEndDisplayList()};
u8 galoomba_goomba_face_rgba16_pal_rgba16[] = {
	#include "actors/galoomba/goomba_face.rgba16.pal"
};

Gfx galoomba_galoomba_face_closed_rgba16_rgba16_aligner[] = {gsSPEndDisplayList()};
u8 galoomba_galoomba_face_closed_rgba16_rgba16[] = {
	#include "actors/galoomba/galoomba_face_closed.rgba16.inc.c"
};

Gfx galoomba_galoomba_face_dizzy_rgba16_rgba16_aligner[] = {gsSPEndDisplayList()};
u8 galoomba_galoomba_face_dizzy_rgba16_rgba16[] = {
	#include "actors/galoomba/galoomba_face_dizzy.rgba16.inc.c"
};

Vtx galoomba_000_displaylist_mesh_layer_1_vtx_0[4] = {
	{{ {8, 8, 0}, 0, {974, -16}, {0, 0, 127, 255} }},
	{{ {-8, 7, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-8, -9, 0}, 0, {-16, 974}, {0, 0, 127, 255} }},
	{{ {8, -9, 0}, 0, {974, 974}, {0, 0, 127, 255} }},
};

Gfx galoomba_000_displaylist_mesh_layer_1_tri_0[] = {
	gsSPVertex(galoomba_000_displaylist_mesh_layer_1_vtx_0 + 0, 4, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),
	gsSPEndDisplayList(),
};

Vtx galoomba_000_offset_002_mesh_layer_1_vtx_0[108] = {
	{{ {-24, 14, -59}, 0, {143, 1015}, {193, 41, 153, 255} }},
	{{ {1, -19, -67}, 0, {143, 1015}, {18, 0, 130, 255} }},
	{{ {-24, -50, -59}, 0, {143, 1015}, {192, 218, 153, 255} }},
	{{ {23, -19, -59}, 0, {143, 1015}, {43, 255, 136, 255} }},
	{{ {22, -52, -52}, 0, {143, 1015}, {35, 199, 148, 255} }},
	{{ {58, -19, -41}, 0, {143, 1015}, {101, 0, 179, 255} }},
	{{ {51, -54, -38}, 0, {143, 1015}, {85, 193, 186, 255} }},
	{{ {58, -55, 0}, 0, {143, 1015}, {106, 186, 0, 255} }},
	{{ {70, -19, 0}, 0, {143, 1015}, {127, 0, 0, 255} }},
	{{ {58, -19, 40}, 0, {143, 1015}, {101, 0, 77, 255} }},
	{{ {51, -54, 37}, 0, {143, 1015}, {86, 193, 69, 255} }},
	{{ {22, -52, 52}, 0, {143, 1015}, {35, 199, 108, 255} }},
	{{ {23, -18, 59}, 0, {143, 1015}, {43, 255, 120, 255} }},
	{{ {-24, -50, 58}, 0, {143, 1015}, {191, 218, 103, 255} }},
	{{ {1, -19, 67}, 0, {143, 1015}, {15, 0, 126, 255} }},
	{{ {-24, 14, 58}, 0, {143, 1015}, {192, 40, 102, 255} }},
	{{ {22, -52, 52}, 0, {143, 1015}, {35, 199, 108, 255} }},
	{{ {-24, -50, 58}, 0, {143, 1015}, {191, 218, 103, 255} }},
	{{ {-24, -82, 37}, 0, {143, 1015}, {199, 161, 63, 255} }},
	{{ {22, -75, 25}, 0, {143, 1015}, {43, 149, 54, 255} }},
	{{ {-13, -92, 0}, 0, {143, 1015}, {11, 129, 0, 255} }},
	{{ {23, -82, 0}, 0, {143, 1015}, {52, 140, 0, 255} }},
	{{ {22, -75, -25}, 0, {143, 1015}, {43, 149, 203, 255} }},
	{{ {-24, -82, -37}, 0, {143, 1015}, {199, 161, 193, 255} }},
	{{ {22, -52, -52}, 0, {143, 1015}, {35, 199, 148, 255} }},
	{{ {-24, -50, -59}, 0, {143, 1015}, {192, 218, 153, 255} }},
	{{ {51, -54, -38}, 0, {143, 1015}, {85, 193, 186, 255} }},
	{{ {58, -55, 0}, 0, {143, 1015}, {106, 186, 0, 255} }},
	{{ {51, -54, 37}, 0, {143, 1015}, {86, 193, 69, 255} }},
	{{ {23, -19, -59}, 0, {352, 892}, {43, 255, 136, 255} }},
	{{ {1, -19, -67}, 0, {259, 1155}, {18, 0, 130, 255} }},
	{{ {-24, 14, -59}, 0, {352, 1455}, {193, 41, 153, 255} }},
	{{ {23, -19, -59}, 0, {352, 892}, {43, 255, 136, 255} }},
	{{ {-24, 14, -59}, 0, {352, 1455}, {193, 41, 153, 255} }},
	{{ {22, 15, -52}, 0, {434, 904}, {31, 56, 146, 255} }},
	{{ {22, 38, -30}, 0, {670, 904}, {37, 106, 197, 255} }},
	{{ {-24, 45, -29}, 0, {677, 1455}, {198, 100, 203, 255} }},
	{{ {23, 45, 0}, 0, {1004, 892}, {46, 118, 0, 255} }},
	{{ {-13, 50, 0}, 0, {997, 1323}, {5, 127, 0, 255} }},
	{{ {-24, 49, 0}, 0, {998, 1455}, {201, 115, 0, 255} }},
	{{ {-31, 41, 0}, 0, {998, 1543}, {143, 58, 0, 255} }},
	{{ {-24, 45, 29}, 0, {1318, 1455}, {197, 100, 52, 255} }},
	{{ {22, 38, 30}, 0, {1337, 904}, {37, 106, 59, 255} }},
	{{ {-24, 14, 58}, 0, {1643, 1455}, {192, 40, 102, 255} }},
	{{ {22, 15, 52}, 0, {1572, 904}, {31, 56, 110, 255} }},
	{{ {23, -18, 59}, 0, {1654, 892}, {43, 255, 120, 255} }},
	{{ {51, 17, 37}, 0, {1410, 555}, {88, 62, 67, 255} }},
	{{ {58, -19, 40}, 0, {1440, 469}, {101, 0, 77, 255} }},
	{{ {58, 19, 0}, 0, {997, 469}, {106, 70, 0, 255} }},
	{{ {51, 17, 37}, 0, {1410, 555}, {88, 62, 67, 255} }},
	{{ {58, -19, 40}, 0, {1440, 469}, {101, 0, 77, 255} }},
	{{ {70, -19, 0}, 0, {997, 331}, {127, 0, 0, 255} }},
	{{ {58, -19, -41}, 0, {555, 469}, {101, 0, 179, 255} }},
	{{ {51, 17, -38}, 0, {585, 555}, {87, 62, 187, 255} }},
	{{ {23, -19, -59}, 0, {352, 892}, {43, 255, 136, 255} }},
	{{ {22, 15, -52}, 0, {434, 904}, {31, 56, 146, 255} }},
	{{ {22, 38, -30}, 0, {670, 904}, {37, 106, 197, 255} }},
	{{ {23, 45, 0}, 0, {1004, 892}, {46, 118, 0, 255} }},
	{{ {22, 38, 30}, 0, {1337, 904}, {37, 106, 59, 255} }},
	{{ {22, 15, 52}, 0, {1572, 904}, {31, 56, 110, 255} }},
	{{ {-24, 45, 29}, 0, {714, 1441}, {197, 100, 52, 255} }},
	{{ {-28, 20, 49}, 0, {485, 1574}, {137, 23, 39, 255} }},
	{{ {-24, 14, 58}, 0, {388, 1651}, {192, 40, 102, 255} }},
	{{ {-38, 7, 18}, 0, {828, 1574}, {132, 15, 21, 255} }},
	{{ {-31, 41, 0}, 0, {1036, 1403}, {143, 58, 0, 255} }},
	{{ {-38, 7, 18}, 0, {828, 1574}, {132, 15, 21, 255} }},
	{{ {-24, 45, 29}, 0, {714, 1441}, {197, 100, 52, 255} }},
	{{ {-38, 7, -1}, 0, {1038, 1575}, {130, 16, 0, 255} }},
	{{ {-38, 7, -20}, 0, {1248, 1575}, {132, 16, 234, 255} }},
	{{ {-24, 45, -29}, 0, {1356, 1445}, {198, 100, 203, 255} }},
	{{ {-28, 20, -50}, 0, {1585, 1580}, {137, 23, 217, 255} }},
	{{ {-24, 14, -59}, 0, {1682, 1658}, {193, 41, 153, 255} }},
	{{ {-29, -5, -48}, 0, {1562, 1742}, {137, 1, 211, 255} }},
	{{ {-24, -50, -59}, 0, {1682, 2085}, {192, 218, 153, 255} }},
	{{ {-30, -10, -44}, 0, {1517, 1755}, {136, 0, 214, 255} }},
	{{ {-38, -29, -18}, 0, {1231, 1814}, {130, 0, 237, 255} }},
	{{ {-38, -52, -19}, 0, {1245, 1968}, {132, 233, 239, 255} }},
	{{ {-24, -82, -37}, 0, {1444, 2296}, {199, 161, 193, 255} }},
	{{ {-24, -90, 0}, 0, {1035, 2352}, {181, 153, 0, 255} }},
	{{ {-13, -92, 0}, 0, {1035, 2460}, {11, 129, 0, 255} }},
	{{ {-24, -82, 37}, 0, {626, 2291}, {199, 161, 63, 255} }},
	{{ {-24, -90, 0}, 0, {1035, 2352}, {181, 153, 0, 255} }},
	{{ {-13, -92, 0}, 0, {1035, 2460}, {11, 129, 0, 255} }},
	{{ {-38, -52, 19}, 0, {824, 1967}, {132, 233, 17, 255} }},
	{{ {-24, -50, 58}, 0, {388, 2078}, {191, 218, 103, 255} }},
	{{ {-38, -29, 17}, 0, {841, 1812}, {130, 0, 19, 255} }},
	{{ {-30, -10, 43}, 0, {554, 1749}, {136, 0, 42, 255} }},
	{{ {-29, -5, 47}, 0, {509, 1736}, {137, 1, 45, 255} }},
	{{ {-28, 20, 49}, 0, {485, 1574}, {137, 23, 39, 255} }},
	{{ {-24, 14, 58}, 0, {388, 1651}, {192, 40, 102, 255} }},
	{{ {-38, 7, 18}, 0, {828, 1574}, {132, 15, 21, 255} }},
	{{ {-38, 7, -1}, 0, {1038, 1575}, {130, 16, 0, 255} }},
	{{ {-38, -32, 0}, 0, {1036, 1833}, {129, 0, 0, 255} }},
	{{ {-38, -29, -18}, 0, {1231, 1814}, {130, 0, 237, 255} }},
	{{ {-38, 7, -20}, 0, {1248, 1575}, {132, 16, 234, 255} }},
	{{ {-30, -10, -44}, 0, {1517, 1755}, {136, 0, 214, 255} }},
	{{ {-28, 20, -50}, 0, {1585, 1580}, {137, 23, 217, 255} }},
	{{ {-30, -10, -44}, 0, {1517, 1755}, {136, 0, 214, 255} }},
	{{ {-38, 7, -20}, 0, {1248, 1575}, {132, 16, 234, 255} }},
	{{ {-29, -5, -48}, 0, {1562, 1742}, {137, 1, 211, 255} }},
	{{ {-38, -32, 0}, 0, {1036, 1833}, {129, 0, 0, 255} }},
	{{ {-38, -29, -18}, 0, {1231, 1814}, {130, 0, 237, 255} }},
	{{ {-38, -52, -19}, 0, {1245, 1968}, {132, 233, 239, 255} }},
	{{ {-38, -52, 0}, 0, {1035, 1967}, {151, 185, 0, 255} }},
	{{ {-34, -53, 0}, 0, {1034, 2017}, {160, 173, 0, 255} }},
	{{ {-24, -90, 0}, 0, {1035, 2352}, {181, 153, 0, 255} }},
	{{ {-38, -52, 19}, 0, {824, 1967}, {132, 233, 17, 255} }},
	{{ {-38, -29, 17}, 0, {841, 1812}, {130, 0, 19, 255} }},
};

Gfx galoomba_000_offset_002_mesh_layer_1_tri_0[] = {
	gsSPVertex(galoomba_000_offset_002_mesh_layer_1_vtx_0 + 0, 16, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(2, 1, 3, 0),
	gsSP1Triangle(2, 3, 4, 0),
	gsSP1Triangle(5, 4, 3, 0),
	gsSP1Triangle(5, 6, 4, 0),
	gsSP1Triangle(5, 7, 6, 0),
	gsSP1Triangle(5, 8, 7, 0),
	gsSP1Triangle(9, 7, 8, 0),
	gsSP1Triangle(9, 10, 7, 0),
	gsSP1Triangle(9, 11, 10, 0),
	gsSP1Triangle(9, 12, 11, 0),
	gsSP1Triangle(13, 11, 12, 0),
	gsSP1Triangle(13, 12, 14, 0),
	gsSP1Triangle(12, 15, 14, 0),
	gsSP1Triangle(15, 13, 14, 0),
	gsSPVertex(galoomba_000_offset_002_mesh_layer_1_vtx_0 + 16, 16, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),
	gsSP1Triangle(4, 3, 2, 0),
	gsSP1Triangle(4, 5, 3, 0),
	gsSP1Triangle(4, 6, 5, 0),
	gsSP1Triangle(4, 7, 6, 0),
	gsSP1Triangle(8, 6, 7, 0),
	gsSP1Triangle(8, 7, 9, 0),
	gsSP1Triangle(10, 6, 8, 0),
	gsSP1Triangle(5, 6, 10, 0),
	gsSP1Triangle(5, 10, 11, 0),
	gsSP1Triangle(5, 11, 12, 0),
	gsSP1Triangle(5, 12, 3, 0),
	gsSP1Triangle(12, 0, 3, 0),
	gsSP1Triangle(13, 14, 15, 0),
	gsSPVertex(galoomba_000_offset_002_mesh_layer_1_vtx_0 + 32, 16, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(1, 3, 2, 0),
	gsSP1Triangle(1, 4, 3, 0),
	gsSP1Triangle(4, 5, 3, 0),
	gsSP1Triangle(4, 6, 5, 0),
	gsSP1Triangle(6, 4, 7, 0),
	gsSP1Triangle(8, 7, 4, 0),
	gsSP1Triangle(8, 9, 7, 0),
	gsSP1Triangle(6, 7, 9, 0),
	gsSP1Triangle(9, 5, 6, 0),
	gsSP1Triangle(9, 10, 5, 0),
	gsSP1Triangle(11, 10, 9, 0),
	gsSP1Triangle(11, 12, 10, 0),
	gsSP1Triangle(13, 12, 11, 0),
	gsSP1Triangle(13, 14, 12, 0),
	gsSP1Triangle(13, 15, 14, 0),
	gsSPVertex(galoomba_000_offset_002_mesh_layer_1_vtx_0 + 48, 16, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),
	gsSP1Triangle(0, 3, 4, 0),
	gsSP1Triangle(0, 4, 5, 0),
	gsSP1Triangle(6, 5, 4, 0),
	gsSP1Triangle(6, 7, 5, 0),
	gsSP1Triangle(7, 8, 5, 0),
	gsSP1Triangle(5, 8, 9, 0),
	gsSP1Triangle(5, 9, 0, 0),
	gsSP1Triangle(1, 0, 9, 0),
	gsSP1Triangle(1, 9, 10, 0),
	gsSP1Triangle(11, 1, 10, 0),
	gsSP1Triangle(12, 13, 14, 0),
	gsSP1Triangle(12, 15, 13, 0),
	gsSPVertex(galoomba_000_offset_002_mesh_layer_1_vtx_0 + 64, 16, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 3, 1, 0),
	gsSP1Triangle(0, 4, 3, 0),
	gsSP1Triangle(0, 5, 4, 0),
	gsSP1Triangle(5, 6, 4, 0),
	gsSP1Triangle(5, 7, 6, 0),
	gsSP1Triangle(8, 6, 7, 0),
	gsSP1Triangle(9, 8, 7, 0),
	gsSP1Triangle(9, 10, 8, 0),
	gsSP1Triangle(9, 11, 10, 0),
	gsSP1Triangle(9, 12, 11, 0),
	gsSP1Triangle(9, 13, 12, 0),
	gsSP1Triangle(12, 13, 14, 0),
	gsSP1Triangle(13, 15, 14, 0),
	gsSPVertex(galoomba_000_offset_002_mesh_layer_1_vtx_0 + 80, 16, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(3, 1, 0, 0),
	gsSP1Triangle(4, 3, 0, 0),
	gsSP1Triangle(4, 5, 3, 0),
	gsSP1Triangle(4, 6, 5, 0),
	gsSP1Triangle(7, 6, 4, 0),
	gsSP1Triangle(4, 8, 7, 0),
	gsSP1Triangle(4, 9, 8, 0),
	gsSP1Triangle(8, 6, 7, 0),
	gsSP1Triangle(8, 10, 6, 0),
	gsSP1Triangle(6, 10, 5, 0),
	gsSP1Triangle(11, 5, 10, 0),
	gsSP1Triangle(11, 12, 5, 0),
	gsSP1Triangle(11, 13, 12, 0),
	gsSP1Triangle(11, 14, 13, 0),
	gsSP1Triangle(15, 13, 14, 0),
	gsSPVertex(galoomba_000_offset_002_mesh_layer_1_vtx_0 + 96, 12, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 3, 1, 0),
	gsSP1Triangle(4, 5, 6, 0),
	gsSP1Triangle(4, 6, 7, 0),
	gsSP1Triangle(6, 8, 7, 0),
	gsSP1Triangle(6, 9, 8, 0),
	gsSP1Triangle(10, 8, 9, 0),
	gsSP1Triangle(10, 7, 8, 0),
	gsSP1Triangle(4, 7, 10, 0),
	gsSP1Triangle(4, 10, 11, 0),
	gsSPEndDisplayList(),
};

Vtx galoomba_000_offset_003_mesh_layer_1_vtx_0[29] = {
	{{ {27, 1, 28}, 0, {-16, -16}, {25, 69, 104, 255} }},
	{{ {48, 1, 12}, 0, {-16, -16}, {98, 66, 46, 255} }},
	{{ {48, 1, -19}, 0, {-16, -16}, {98, 67, 210, 255} }},
	{{ {27, -18, 23}, 0, {-16, -16}, {24, 172, 93, 255} }},
	{{ {-14, -13, 19}, 0, {-16, -16}, {213, 176, 89, 255} }},
	{{ {-15, -2, 23}, 0, {-16, -16}, {213, 65, 101, 255} }},
	{{ {27, 1, -34}, 0, {-16, -16}, {24, 70, 153, 255} }},
	{{ {44, -12, -19}, 0, {-16, -16}, {99, 192, 208, 255} }},
	{{ {44, -12, 12}, 0, {-16, -16}, {99, 193, 49, 255} }},
	{{ {31, -21, 12}, 0, {-16, -16}, {34, 136, 24, 255} }},
	{{ {-13, -18, 10}, 0, {-16, -16}, {226, 135, 23, 255} }},
	{{ {-33, -9, 5}, 0, {-16, -16}, {165, 175, 37, 255} }},
	{{ {-37, 1, 7}, 0, {-16, -16}, {159, 65, 50, 255} }},
	{{ {-37, 1, -13}, 0, {-16, -16}, {159, 66, 208, 255} }},
	{{ {-15, -2, -30}, 0, {-16, -16}, {213, 63, 154, 255} }},
	{{ {27, -18, -29}, 0, {-16, -16}, {24, 173, 163, 255} }},
	{{ {27, -18, -29}, 0, {-16, -16}, {24, 173, 163, 255} }},
	{{ {44, -12, -19}, 0, {-16, -16}, {99, 192, 208, 255} }},
	{{ {31, -21, -19}, 0, {-16, -16}, {34, 136, 231, 255} }},
	{{ {44, -12, 12}, 0, {-16, -16}, {99, 193, 49, 255} }},
	{{ {31, -21, 12}, 0, {-16, -16}, {34, 136, 24, 255} }},
	{{ {-13, -18, 10}, 0, {-16, -16}, {226, 135, 23, 255} }},
	{{ {-13, -18, -16}, 0, {-16, -16}, {225, 135, 232, 255} }},
	{{ {-33, -9, -12}, 0, {-16, -16}, {165, 176, 218, 255} }},
	{{ {-33, -9, 5}, 0, {-16, -16}, {165, 175, 37, 255} }},
	{{ {-37, 1, 7}, 0, {-16, -16}, {159, 65, 50, 255} }},
	{{ {-37, 1, -13}, 0, {-16, -16}, {159, 66, 208, 255} }},
	{{ {-14, -13, -26}, 0, {-16, -16}, {213, 174, 169, 255} }},
	{{ {-15, -2, -30}, 0, {-16, -16}, {213, 63, 154, 255} }},
};

Gfx galoomba_000_offset_003_mesh_layer_1_tri_0[] = {
	gsSPVertex(galoomba_000_offset_003_mesh_layer_1_vtx_0 + 0, 16, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(3, 1, 0, 0),
	gsSP1Triangle(4, 3, 0, 0),
	gsSP1Triangle(4, 0, 5, 0),
	gsSP1Triangle(5, 0, 6, 0),
	gsSP1Triangle(0, 2, 6, 0),
	gsSP1Triangle(6, 2, 7, 0),
	gsSP1Triangle(8, 7, 2, 0),
	gsSP1Triangle(8, 2, 1, 0),
	gsSP1Triangle(3, 8, 1, 0),
	gsSP1Triangle(9, 8, 3, 0),
	gsSP1Triangle(4, 9, 3, 0),
	gsSP1Triangle(4, 10, 9, 0),
	gsSP1Triangle(4, 11, 10, 0),
	gsSP1Triangle(5, 11, 4, 0),
	gsSP1Triangle(5, 12, 11, 0),
	gsSP1Triangle(5, 13, 12, 0),
	gsSP1Triangle(5, 14, 13, 0),
	gsSP1Triangle(5, 6, 14, 0),
	gsSP1Triangle(15, 14, 6, 0),
	gsSP1Triangle(6, 7, 15, 0),
	gsSPVertex(galoomba_000_offset_003_mesh_layer_1_vtx_0 + 16, 13, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(2, 1, 3, 0),
	gsSP1Triangle(2, 3, 4, 0),
	gsSP1Triangle(5, 2, 4, 0),
	gsSP1Triangle(5, 6, 2, 0),
	gsSP1Triangle(5, 7, 6, 0),
	gsSP1Triangle(5, 8, 7, 0),
	gsSP1Triangle(7, 8, 9, 0),
	gsSP1Triangle(7, 9, 10, 0),
	gsSP1Triangle(11, 7, 10, 0),
	gsSP1Triangle(11, 10, 12, 0),
	gsSP1Triangle(0, 11, 12, 0),
	gsSP1Triangle(0, 6, 11, 0),
	gsSP1Triangle(0, 2, 6, 0),
	gsSP1Triangle(6, 7, 11, 0),
	gsSPEndDisplayList(),
};

Vtx galoomba_000_offset_004_mesh_layer_1_vtx_0[29] = {
	{{ {27, 1, 33}, 0, {-16, -16}, {25, 69, 104, 255} }},
	{{ {48, 1, 17}, 0, {-16, -16}, {98, 66, 46, 255} }},
	{{ {48, 1, -13}, 0, {-16, -16}, {98, 67, 210, 255} }},
	{{ {27, -18, 28}, 0, {-16, -16}, {24, 172, 93, 255} }},
	{{ {-14, -13, 24}, 0, {-16, -16}, {213, 176, 89, 255} }},
	{{ {-15, -2, 28}, 0, {-16, -16}, {213, 65, 101, 255} }},
	{{ {27, 1, -29}, 0, {-16, -16}, {24, 70, 153, 255} }},
	{{ {44, -12, -13}, 0, {-16, -16}, {99, 192, 208, 255} }},
	{{ {44, -12, 17}, 0, {-16, -16}, {99, 193, 49, 255} }},
	{{ {31, -21, 17}, 0, {-16, -16}, {34, 136, 24, 255} }},
	{{ {-13, -18, 15}, 0, {-16, -16}, {226, 135, 23, 255} }},
	{{ {-33, -9, 10}, 0, {-16, -16}, {165, 175, 37, 255} }},
	{{ {-37, 1, 12}, 0, {-16, -16}, {159, 65, 50, 255} }},
	{{ {-37, 1, -8}, 0, {-16, -16}, {159, 66, 208, 255} }},
	{{ {-15, -2, -25}, 0, {-16, -16}, {213, 63, 154, 255} }},
	{{ {27, -18, -24}, 0, {-16, -16}, {24, 173, 163, 255} }},
	{{ {27, -18, -24}, 0, {-16, -16}, {24, 173, 163, 255} }},
	{{ {44, -12, -13}, 0, {-16, -16}, {99, 192, 208, 255} }},
	{{ {31, -21, -13}, 0, {-16, -16}, {34, 136, 231, 255} }},
	{{ {44, -12, 17}, 0, {-16, -16}, {99, 193, 49, 255} }},
	{{ {31, -21, 17}, 0, {-16, -16}, {34, 136, 24, 255} }},
	{{ {-13, -18, 15}, 0, {-16, -16}, {226, 135, 23, 255} }},
	{{ {-13, -18, -11}, 0, {-16, -16}, {225, 135, 232, 255} }},
	{{ {-33, -9, -6}, 0, {-16, -16}, {165, 176, 218, 255} }},
	{{ {-33, -9, 10}, 0, {-16, -16}, {165, 175, 37, 255} }},
	{{ {-37, 1, 12}, 0, {-16, -16}, {159, 65, 50, 255} }},
	{{ {-37, 1, -8}, 0, {-16, -16}, {159, 66, 208, 255} }},
	{{ {-14, -13, -21}, 0, {-16, -16}, {213, 174, 169, 255} }},
	{{ {-15, -2, -25}, 0, {-16, -16}, {213, 63, 154, 255} }},
};

Gfx galoomba_000_offset_004_mesh_layer_1_tri_0[] = {
	gsSPVertex(galoomba_000_offset_004_mesh_layer_1_vtx_0 + 0, 16, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(3, 1, 0, 0),
	gsSP1Triangle(4, 3, 0, 0),
	gsSP1Triangle(4, 0, 5, 0),
	gsSP1Triangle(5, 0, 6, 0),
	gsSP1Triangle(0, 2, 6, 0),
	gsSP1Triangle(6, 2, 7, 0),
	gsSP1Triangle(8, 7, 2, 0),
	gsSP1Triangle(8, 2, 1, 0),
	gsSP1Triangle(3, 8, 1, 0),
	gsSP1Triangle(9, 8, 3, 0),
	gsSP1Triangle(4, 9, 3, 0),
	gsSP1Triangle(4, 10, 9, 0),
	gsSP1Triangle(4, 11, 10, 0),
	gsSP1Triangle(5, 11, 4, 0),
	gsSP1Triangle(5, 12, 11, 0),
	gsSP1Triangle(5, 13, 12, 0),
	gsSP1Triangle(5, 14, 13, 0),
	gsSP1Triangle(5, 6, 14, 0),
	gsSP1Triangle(15, 14, 6, 0),
	gsSP1Triangle(6, 7, 15, 0),
	gsSPVertex(galoomba_000_offset_004_mesh_layer_1_vtx_0 + 16, 13, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(2, 1, 3, 0),
	gsSP1Triangle(2, 3, 4, 0),
	gsSP1Triangle(5, 2, 4, 0),
	gsSP1Triangle(5, 6, 2, 0),
	gsSP1Triangle(5, 7, 6, 0),
	gsSP1Triangle(5, 8, 7, 0),
	gsSP1Triangle(7, 8, 9, 0),
	gsSP1Triangle(7, 9, 10, 0),
	gsSP1Triangle(11, 7, 10, 0),
	gsSP1Triangle(11, 10, 12, 0),
	gsSP1Triangle(0, 11, 12, 0),
	gsSP1Triangle(0, 6, 11, 0),
	gsSP1Triangle(0, 2, 6, 0),
	gsSP1Triangle(6, 7, 11, 0),
	gsSPEndDisplayList(),
};


Gfx mat_galoomba_f3dlite_material_053[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT, TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT),
	gsDPSetTextureLUT(G_TT_RGBA16),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPSetLights1(galoomba_f3dlite_material_053_lights),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, galoomba_goomba_body_rgba16_pal_rgba16),
	gsDPSetTile(0, 0, 0, 256, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadTLUTCmd(5, 116),
	gsDPSetTextureImage(G_IM_FMT_CI, G_IM_SIZ_8b_LOAD_BLOCK, 1, galoomba_goomba_body_rgba16_ci8),
	gsDPSetTile(G_IM_FMT_CI, G_IM_SIZ_8b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 511, 512),
	gsDPSetTile(G_IM_FMT_CI, G_IM_SIZ_8b, 4, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPEndDisplayList(),
};

Gfx mat_revert_galoomba_f3dlite_material_053[] = {
	gsDPPipeSync(),
	gsDPSetTextureLUT(G_TT_NONE),
	gsSPEndDisplayList(),
};

Gfx mat_galoomba_f3dlite_material_054[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT, TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT),
	gsDPSetTextureLUT(G_TT_RGBA16),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPSetLights1(galoomba_f3dlite_material_054_lights),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, galoomba_goomba_face_rgba16_pal_rgba16),
	gsDPSetTile(0, 0, 0, 256, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadTLUTCmd(5, 152),
	gsDPSetTextureImage(G_IM_FMT_CI, G_IM_SIZ_8b_LOAD_BLOCK, 1, galoomba_goomba_face_rgba16_ci8),
	gsDPSetTile(G_IM_FMT_CI, G_IM_SIZ_8b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 512),
	gsDPSetTile(G_IM_FMT_CI, G_IM_SIZ_8b, 4, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 6, 0, G_TX_WRAP | G_TX_MIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 252),
	gsSPEndDisplayList(),
};

Gfx mat_revert_galoomba_f3dlite_material_054[] = {
	gsDPPipeSync(),
	gsDPSetTextureLUT(G_TT_NONE),
	gsSPEndDisplayList(),
};

Gfx mat_galoomba_f3dlite_material_002[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT, TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPSetLights1(galoomba_f3dlite_material_002_lights),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, galoomba_galoomba_face_closed_rgba16_rgba16),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 2047, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 7, 0, G_TX_WRAP | G_TX_MIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 256),
	gsSPEndDisplayList(),
};

Gfx mat_galoomba_f3dlite_material_001[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT, TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPSetLights1(galoomba_f3dlite_material_001_lights),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, galoomba_galoomba_face_dizzy_rgba16_rgba16),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 2047, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 7, 0, G_TX_WRAP | G_TX_MIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 256),
	gsSPEndDisplayList(),
};

Gfx mat_galoomba_f3dlite_material_055[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPSetLights1(galoomba_f3dlite_material_055_lights),
	gsSPEndDisplayList(),
};

Gfx galoomba_000_displaylist_mesh_layer_1[] = {
	gsSPDisplayList(mat_galoomba_f3dlite_material_053),
	gsSPDisplayList(galoomba_000_displaylist_mesh_layer_1_tri_0),
	gsSPDisplayList(mat_revert_galoomba_f3dlite_material_053),
	gsSPEndDisplayList(),
};

Gfx galoomba_000_offset_002_mesh_layer_1[] = {
	gsSPDisplayList(mat_galoomba_f3dlite_material_054),
	gsSPDisplayList(galoomba_000_offset_002_mesh_layer_1_tri_0),
	gsSPDisplayList(mat_revert_galoomba_f3dlite_material_054),
	gsSPEndDisplayList(),
};

Gfx galoomba_000_offset_002_mesh_layer_1_mat_override_f3dlite_material_002_0[] = {
	gsSPDisplayList(mat_galoomba_f3dlite_material_002),
	gsSPDisplayList(galoomba_000_offset_002_mesh_layer_1_tri_0),
	gsSPEndDisplayList(),
};

Gfx galoomba_000_offset_002_mesh_layer_1_mat_override_f3dlite_material_001_1[] = {
	gsSPDisplayList(mat_galoomba_f3dlite_material_001),
	gsSPDisplayList(galoomba_000_offset_002_mesh_layer_1_tri_0),
	gsSPEndDisplayList(),
};

Gfx galoomba_000_offset_003_mesh_layer_1[] = {
	gsSPDisplayList(mat_galoomba_f3dlite_material_055),
	gsSPDisplayList(galoomba_000_offset_003_mesh_layer_1_tri_0),
	gsSPEndDisplayList(),
};

Gfx galoomba_000_offset_004_mesh_layer_1[] = {
	gsSPDisplayList(mat_galoomba_f3dlite_material_055),
	gsSPDisplayList(galoomba_000_offset_004_mesh_layer_1_tri_0),
	gsSPEndDisplayList(),
};

Gfx galoomba_material_revert_render_settings[] = {
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsSPEndDisplayList(),
};

