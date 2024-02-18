/* Define lists for list of course for macros. Each of the following fields are described:
 * Argument 1: Course macro for define.
 * Argument 2: Star collection dance cutscenes
 * Each hex digit determines what dance cutscene to play for the stars in the course. The last digit is
 * unused. See determine_dance_cutscene() in camera.c for details.
 * Cutscene Digits:
 *      0: Lakitu flies away after the dance
 *      1: The camera rotates around mario
 *      2: The camera goes to a closeup of mario
 *      3: Bowser keys and the grand star
 *      4: Default, used for 100 coin stars, 8 red coin stars in bowser levels, and secret stars
 */
DEFINE_COURSE(COURSE_NONE, 0x44444440) // (0)  Course Hub (Castle Grounds)
DEFINE_COURSE(COURSE_SR, 0x44444440) 
DEFINE_COURSE(COURSE_TSNS, 0x44444440) 
DEFINE_COURSES_END()
DEFINE_BONUS_COURSE(COURSE_BITDW, 0x34444440) // (16) Bowser in the Dark World
