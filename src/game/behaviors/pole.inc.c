
/**
 * Behaviors for bhvPoleGrabbing and bhvGiantPole.
 *
 * bhvPoleGrabbing is a generic behavior used almost anywhere the player can
 * grab onto and climb up another object.
 *
 * bhvGiantPole controls the large pole in Whomp's Fortress and differs from
 * other poles because it spawns a yellow ball that rests at its top.
 */

/**
 * Initialize the hitbox height field for bhvPoleGrabbing.
 */
void bhv_pole_init(void) {
    /**
     * This is equivalent to using `o->oBehParams2ndByte` according to
     * `spawn_objects_from_info`.
     */
    // This treats bparam1 and bparam2 as a single value
    o->hitboxHeight = GET_BPARAM12(o->oBehParams) * 10;
}

/**
 * Main loop for bhvGiantPole. It is equivalent to bhv_pole_base_loop() except
 * for the fact that it spawns a yellow sphere at the top of the pole on the
 * first frame.
 */
void bhv_giant_pole_loop(void) {
    if (o->oDistanceToMario < 3000.0f) {
        cur_obj_spawn_star_at_y_offset(o->oPosX + 23, o->oPosY + 23, o->oPosZ, 0.0f);
    }
    bhv_pole_base_loop();
}
