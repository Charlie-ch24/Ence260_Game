/** @file   scoring.h
    @author Euan Robinson & Charlie Hunter
    @date   20 October 2020
    @brief  Scoring logic and GameOver screen
*/



#ifndef SCORING_H
#define SCORING_H
#include "system.h"
#include "visuals.h"
#include "navswitch.h"
#include "pacer.h"
#include "tinygl.h"
#include "Global.h"
#include <string.h>
#include <stdio.h>


/*  Loops through the playergrid to calculate number of bombs left, i.e the score  */
uint8_t calculate_score(Playergrid* playergrid);


/*  Displays the GameOver screen and calulated score */
void display_score_and_game_over(uint8_t score);


#endif //SCORING_H
