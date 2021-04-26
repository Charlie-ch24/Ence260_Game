/** @file   movement.h
    @author Euan Robinson & Charlie Hunter
    @date   20 October 2020
    @brief  Functions for serial communications, and updating playergrid
*/

#ifndef PLAYERGRID_H
#define PLAYERGRID_H
#include "system.h"
#include "ir_serial.h"
#include "visuals.h"
#include "Global.h"


/* Initialise player coord */
void reset_player_coord(Playergrid* playergrid);


/* Reset playergrid bitmap */
void reset_playergrid_bitmap(Playergrid* playergrid);


/* Update coordinate of playergrid */
void update_playergrid(uint8_t value, Playergrid* playergrid);


/* Sends signal to other player to update their grid with coordinates affected */
void update_opponent_playergrid(uint8_t value, Playergrid* playergrid);


/* Waits until it receives a signal of the opponent's move, and updates
 * own playergrid with coordinates affected
 */
void get_opponent_playergrid_update(Playergrid* playergrid);


#endif //PLAYERGRID_H
