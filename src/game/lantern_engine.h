#ifndef LANTERN_ENGINE_64_H
#define LANTERN_ENGINE_64_H

/////////////////////////////////////////////LIGHT DIRECTION/////////////////////////////////////////////
extern Vec3c vanillaLightDirection;
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////LIGHT COLOR/////////////////////////////////////////////////
extern Lights1 vanillaLight;
extern Lights1 indoorLight;
extern Lights1 bobLight;
extern Lights1 riLight;
extern Lights1 lllLight;
extern Lights1 sslLight;
extern Lights1 waterLight;
extern Lights1 bitsLight;
extern Lights1 bitdwLight;
extern Lights1 wfLight;
extern Lights1 snowLight;
extern Lights1 thiLight;
extern Lights1 hmcLight;
extern Lights1 jrbLight;
extern Lights1 sw2Light;
extern Lights1 ggZLight;
//extern Lights1 blackLight; //you can deinitialize lighting
//extern Lights1 whiteLight; // you can just disable lighting or use vanilla
/////////////////////////////////////////////////////////////////////////////////////////////////////////

enum LightPreset {
	VANILLA_COLORS, // 0
	INDOOR_COLORS, // 1
	BOB_COLORS, // 2
	RI_COLORS, // 3
	LLL_COLORS, // 4
	SSL_COLORS, // 5
	WATER_COLORS, // 6
	BITS_COLORS, // 7
	BITDW_COLORS, // 8
	WF_COLORS, // 9
	SNOW_COLORS, // 10
	THI_COLORS, // 11
	HMC_COLORS, // 12
	JRB_COLORS, // 13
	SW2_COLORS, // 14
	GGZ_COLORS, // 15
};

enum DynamicLight {
	LIGHT_RAINBOW,
};


//FBPROBE stands for FrameBuffer Probe

enum LightType {
	FBPROBE_CAMERA, // 0
	FBPROBE_GLOBAL, // 1
	DIRECTIONAL_CAMERA, // 2
	DIRECTIONAL_GLOBAL, // 3
	OCCLUDE_CAMERA, // 4
	OCCLUDE_GLOBAL, // 5
	//POINT_CAMERA, // 6
	//POINT_GLOBAL, // 7
};

#define MAX_LIGHTS 3

struct LevelLights {
	u8 numLightSrcs : MAX_LIGHTS; //separate from light colors as they aren't mutually exclusive
	u8 numLightCols : (MAX_LIGHTS - 1); //1 light at a time doesn't work properly in this engine
	u8 lightType : 5; //directional, point, or framebuffer light
	u8 isDynamic : 1; //is color/direction static or dynamic?
	u8 isVanilla : 1;
	u8 isOccluded : 1;
	u8 disableLighting : 1; //for pure white lighting (lightning bolts)
	Vec3f worldSpacePos;
	//Vec3f worldSpacePosSpec;
	//Vec3f worldSpacePosAmb;
	Vec3c lightDir;
	Vec3c lightDirSpec;
	Vec3c lightDirAmb;
	u8 dynamicPreset;
	//Lights1 nextLightCol;
	Lights1 staticLightCol;
	//Lights1 prevLightCol;
	Vec3uc dynamicLightCol;
};

#if 0
struct LightField {
};
#endif

/* Light Param Guide 

Param 1 = Number of Light Colors
Param 2 = Light Preset (Static or Dynamic)
Param 3 = Light Type (Directional, Point, or FrameBuffer)
Param 4 = Number of Light Sources

*/

extern struct LevelLights gLevelLights[];

#define gCurrLevelLight gLevelLights[0]

#define gNumLightSources gCurrLevelLight.numLightSrcs
#define gNumLightColors gCurrLevelLight.numLightCols
#define gLightType gCurrLevelLight.lightType
#define gDisableLighting gCurrLevelLight.disableLighting
#define gOccludeLighting gCurrLevelLight.isOccluded
#define gVanillaLighting gCurrLevelLight.isVanilla
#define gIsDynamic gCurrLevelLight.isDynamic
#define gCurrStaticColor gCurrLevelLight.staticLightCol
#define gCurrDynamicColor gCurrLevelLight.dynamicLightCol
#define gCurrLightDirection gCurrLevelLight.lightDir
#define gCurrLightPos gCurrLevelLight.worldSpacePos
#define gStaticLightPreset gCurrLevelLight.staticPreset
#define gDynamicLightPreset gCurrLevelLight.dynamicPreset

#define gCurrLightDirection2 gCurrLevelLight.lightDirSpec
//#define gCurrLightPos2 gCurrLevelLight.worldSpacePosSpec

#define gCurrLightDirection3 gCurrLevelLight.lightDirAmb
//#define gCurrLightPos3 gCurrLevelLight.worldSpacePosAmb


#endif // LANTERN_ENGINE_64_H
