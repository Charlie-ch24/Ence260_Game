/** @file   visuals.h
    @author Euan Robinson & Charlie Hunter
    @date   20 October 2020
    @brief  Logic for lighting LED matrix and LED1
*/

#ifndef VISUALS_H
#define VISUALS_H
#include "system.h"
#include "pio.h"
#include "led.h"
#include "timer.h"
#include "ledmat.h"
#include "playergrid.h"
#include "tinygl.h"
#include "pacer.h"
#include "visuals.h"
#include "font5x7_1.h"
#include "Global.h"



/* Shows player position only (without bombs) */
void display_position(Playergrid* playergrid);


/* Shows playergrid with bombs and player position for one cycle */
void cycle_display(Playergrid* playergrid);


/* Makes LED blink for duration in seconds */
void blink(int seconds);


#endif //VIUSALS_H
