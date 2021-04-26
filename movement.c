/** @file   movement.h
    @author Euan Robinson & Charlie Hunter
    @date   20 October 2020
    @brief  Allows for player to control movement, bombing, and defusing
*/

#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ledmat.h"
#include "playergrid.h"
#include "Global.h"
#include <stdbool.h>

#define GAME_PACER_RATE 2700


/* Moves position north */
static void move_north(Playergrid* playergrid)
{
    playergrid->posY -= 1;
    if (playergrid->posY == -1) {
        playergrid->posY = GRID_HEIGHT - 1;
    }
}


/* Moves position south */
static void move_south(Playergrid* playergrid)
{
    playergrid->posY += 1;
    if (playergrid->posY == GRID_HEIGHT) {
        playergrid->posY = 0;
    }
}


/* Moves position east */
static void move_east(Playergrid* playergrid)
{
    playergrid->posX += 1;
    if (playergrid->posX == GRID_WIDTH) {
        playergrid->posX = 0;
    }
}


/* Moves position west */
static void move_west(Playergrid* playergrid)
{
    playergrid->posX -= 1;
    if (playergrid->posX == -1) {
        playergrid->posX = GRID_WIDTH - 1;
    }
}


/* A function for the player to control their movement across the grid,
 * and to 'bomb' or 'defuse' depending on the phase.
 * Phase is 0 for bombing, and 1 for defusing.
 */
void control(int phase, Playergrid* playergrid)
{
    // Default starting position, middle of the grid
    reset_player_coord(playergrid);
    ledmat_init();
    pacer_init(GAME_PACER_RATE);    // The pacer rate is faster for the game than the ticker

    // Loops until player bombs or defuses
    bool is_moving = true;
    while (is_moving)
    {
        pacer_wait ();
        navswitch_update ();

        // Players can only see their own bombs in the bombing phase (0)
        if (phase == 0) {
            cycle_display(playergrid);
        } else {
            display_position(playergrid);
        }

        // Polls navswitch for cardinal movement
        if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
            move_north(playergrid);
        }
        if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
            move_south(playergrid);
        }
        if (navswitch_push_event_p (NAVSWITCH_WEST)) {
            move_west(playergrid);
        }
        if (navswitch_push_event_p (NAVSWITCH_EAST)) {
            move_east(playergrid);
        }

        // Polls navswitch for action movement
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            // If in bombing phase, add a bomb to site on playergrid
            if (phase == 0) {
                if (!((playergrid->bitmap[playergrid->posX] >> playergrid->posY) & 1)) {
                    update_playergrid(0, playergrid); // Adds bomb
                    is_moving = false;   // Loop terminates
                }
            // If in defusing phase, attempt to defuse a bomb if it exists, and update opponent on attempt
            } else {
                update_playergrid(1, playergrid); // Defuses bomb
                update_opponent_playergrid(1, playergrid);
                is_moving = false;   // Loop terminates
            }
        }
    }
}
