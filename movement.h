/** @file   movement.h
    @author Euan Robinson & Charlie Hunter
    @date   20 October 2020
    @brief  Allows for player to control movement, bombing, and defusing
*/




#ifndef MOVEMENT_H
#define MOVEMENT_H
#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ledmat.h"
#include "playergrid.h"
#include "Global.h"

/* A function for the player to control their movement across the grid,
 * and to 'bomb' or 'defuse' depending on the phase.
 * Phase is 0 for bombing, and 1 for defusing.
 */
void control(int action, Playergrid* playergrid);


#endif //MOVEMENT_H
