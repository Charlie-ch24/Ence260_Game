/** @file   game.c
    @author Euan Robinson & Charlie Hunter
    @date   20 October 2020
    @brief  Main body of game, and setup helper function
*/

#include "system.h"
#include "tinygl.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_serial.h"
#include "playergrid.h"
#include "ledmat.h"
#include "movement.h"
#include "scoring.h"
#include "visuals.h"
#include "../fonts/font5x7_1.h"
#include "Global.h"
#include "ir_uart.h"
#include <stdbool.h>

#define TEXT_PACER_RATE 500
#define LOOP_RATE 300
#define SCORE_DISPLAY_TIME 10
#define MESSAGE_RATE 25

#define NUMBER_OF_BOMBS 9
#define NUMBER_OF_TURNS 12


/* A function to run the bombing phase of the game */
static void bombing_phase(Playergrid* playergrid)
{
    uint8_t bombs_placed = 0;
    while (bombs_placed < NUMBER_OF_BOMBS) {
        control(0, playergrid);
        bombs_placed++;
    }
    blink(1);   // Blue LED flashes once to show player is ready
}


/* A function to run the defusing phase of the game */
static void defusing_phase(uint8_t player_number, Playergrid* playergrid)
{
    bool is_turn = false;    // True if currently player's turn
    if (player_number == 1) {
        is_turn = true; // Player 1 is starting player
    }

    uint8_t turn = 0;
    while (turn < NUMBER_OF_TURNS) {
        if (is_turn) {  // Take turn if player's turn
            control(1, playergrid);
            is_turn = false;
            turn++;
        } else {        // Wait for other player to send update
            get_opponent_playergrid_update(playergrid);
            is_turn = true;
        }
    }
}


/* A function to designate player 1, and return player number */
uint8_t setup(void)
{
    // Shows starting display
    tinygl_text("BOMBER!  ");
    tinygl_update();

    // Initialise player number and signals
    uint8_t player_number = 0;
    uint8_t signal = 0xFF;      // This signal means "I am player 1"
    uint8_t data = 0x00;        // Data (signal from other player)

    // Loops until a player presses their navswitch to call themself player 1
    while(1) {
        tinygl_update();
        navswitch_update();

        // Designates as player 1 if navswitch pressed
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            ir_serial_transmit(signal);
            player_number = 1;
            return player_number;
        }

        // Designates as player 2 if signal received
        ir_serial_ret_t ret;
        ret = ir_serial_receive(&data);
        if (ret == IR_SERIAL_OK) {  // Ensures signal was legitimate
            if (data == 0xFF) {
                player_number = 2;
                return player_number;
            }
        }
        pacer_wait();
    }
}


/* Main function. Loops the game BOMBER until the FunKit is turned off. */
int main(void)
{
    // Initialise drivers and utilities
    system_init();
    navswitch_init ();
    ir_serial_init ();
    pacer_init(TEXT_PACER_RATE);
    tinygl_init (TEXT_PACER_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);

    // Initialise playergrid
    Playergrid playergrid = {2,3, {0x00, 0x00, 0x00, 0x00, 0x00}};

    // Loops game until FunKit is turned off
    while (1)
    {
        // Setup phase
        uint8_t player_number = setup(); // Designate player number
        reset_player_coord(&playergrid);
        reset_playergrid_bitmap(&playergrid);

        // Bombing phase
        bombing_phase(&playergrid);

        // Defusing phase; players take turns trying to locate opponent bombs
        defusing_phase(player_number, &playergrid);

        // Scoring phase
        pacer_init(TEXT_PACER_RATE);    // Change pacer rate back from GAME_PACER_RATE
        uint8_t score = calculate_score(&playergrid);
        display_score_and_game_over(score);

        blink(3); // Long LED flash to indicate game restarting

        // Game restarts
        reset_playergrid_bitmap(&playergrid);
    }

    return 0;
}



