// rotating_octagonal_plat.inc.c

static const Collision *sOctagonalPlatformCollision[] = {
};

static s16 sOctagonalPlatformAngularVelocities[] = { 300, -300, 600, -600 };

void bhv_rotating_octagonal_plat_init(void) {
    o->collisionData = segmented_to_virtual(sOctagonalPlatformCollision[GET_BPARAM2(o->oBehParams)]);
    o->oAngleVelYaw = sOctagonalPlatformAngularVelocities[GET_BPARAM1(o->oBehParams)];
}

void bhv_rotating_octagonal_plat_loop(void) {
    o->oFaceAngleYaw += o->oAngleVelYaw;
}
