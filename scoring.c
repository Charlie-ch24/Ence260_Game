/** @file   scoring.c
    @author Euan Robinson & Charlie Hunter
    @date   20 October 2020
    @brief  Scoring logic and GameOver screen
*/

#include "system.h"
#include "visuals.h"
#include "navswitch.h"
#include "pacer.h"
#include "tinygl.h"
#include "Global.h"
#include "font5x7_1.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


/* Loops through column and returns number of bombs in that column */
static uint8_t score_column(uint8_t row_pattern)
{
    uint8_t column_score = 0;

    // Count 1s in the column of the playergrid
    for (uint8_t current_bit = 0; current_bit < LEDMAT_ROWS_NUM; current_bit++) {
        if ((row_pattern >> current_bit) & 1) {
            column_score++;
        }
    }
    return column_score;
}


/*  Loops through the playergrid to calculate number of bombs left, i.e the score  */
uint8_t calculate_score(Playergrid* playergrid)
{
    uint8_t score = 0;
    uint8_t current_column = 0;
    while (current_column < LEDMAT_COLS_NUM) {
        score += score_column(playergrid->bitmap[current_column]); // Takes sum of all columns
        current_column++;
    }
    return score;
}


/*  Displays the GameOver screen and calulated score   */
void display_score_and_game_over(uint8_t score)
{
    // Initialise display tick
    bool is_game_end = false;

    // Loop display score until navswitch pressed
    char buffer[16];
    tinygl_text("GAME OVER! PRESS FOR SCORE...");
    tinygl_update();
    bool is_display_score = false;
    while(!is_display_score) {
        navswitch_update();

        // Updates tinygl ticker message
        tinygl_update();

        // Breaks GameOver screen to display score if navswitch is pressed down
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            is_display_score = true;
        }
        pacer_wait();
    }

    // Loops score display until navswitch pressed
    sprintf(buffer, "%d", score); // Score is converting from integer to string

    tinygl_text(buffer);
    tinygl_update();

    while(!is_game_end) {
        navswitch_update();
        tinygl_text(buffer);
        tinygl_update();

        // Ends the game if navswitch is pressed down
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            is_game_end = true;
        }

        pacer_wait();
    }

    // Returns to start of game
}
