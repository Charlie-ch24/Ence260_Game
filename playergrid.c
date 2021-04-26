/** @file   movement.h
    @author Euan Robinson & Charlie Hunter
    @date   20 October 2020
    @brief  Functions for serial communications, and updating playergrid
*/

#include "system.h"
#include "ir_serial.h"
#include "visuals.h"
#include "Global.h"
#include <stdbool.h>


/* Encodes coordinates and value into a bitcode. Calculation assigns
 * each of the 35 possible coordinates in the grid a specific number.
 * Bitcode is between 0 and 34 inclusive if value is 0. Bitcode is
 * between 35 and 69 inclusive if value is 1.
 */
static uint8_t encode(uint8_t value, Playergrid* playergrid)
{
    uint8_t bitcode = 0x00;
    bitcode = 5 * playergrid->posY + playergrid->posX;
    if (value == 1) {
        bitcode += NUMBER_OF_POSITIONS; // If value 1, interval is [35, 69]
    }

    return bitcode;
}


/* Decodes a bitcode into its coordinates and value. Reverse of encode() */
static uint8_t decode(uint8_t bitcode, Playergrid* playergrid)
{
    uint8_t value = 0;

    // Check value first
    if (bitcode > NUMBER_OF_POSITIONS) {
        value = 1;
        bitcode -= NUMBER_OF_POSITIONS;
    }

    // Retrieve coordinates
    playergrid->posX = bitcode % 5;
    playergrid->posY = bitcode / 5;

    return value;
}


/* Initialise player coordinates */
void reset_player_coord(Playergrid* playergrid)
{
    // Places player in centre of grid
    playergrid->posX = 2;
    playergrid->posY = 3;
}


/* Reset playergrid bitmap */
void reset_playergrid_bitmap(Playergrid* playergrid)
{
    for (uint8_t i = 0; i < GRID_WIDTH; i++) {
        playergrid->bitmap[i] = 0x00;
    }
}


/* Update coordinate of playergrid */
void update_playergrid(uint8_t value, Playergrid* playergrid)
{
    if (value == 0) {
        playergrid->bitmap[playergrid->posX] |= (1 << playergrid->posY);   // Places a bomb (0) at position (regardless of what's there)
    } else if (value == 1) {

        // If bomb is defused, blink blue LED
        if ((playergrid->bitmap[playergrid->posX] >> playergrid->posY) & 1) {
            blink(1);
        }
        playergrid->bitmap[playergrid->posX] &= ~(1 << playergrid->posY); // Removes bomb (regardless of what's there)
    }
}


/* Sends signal to other player to update their grid with coordinates affected */
void update_opponent_playergrid(uint8_t value, Playergrid* playergrid)
{
    ir_serial_init();
    uint8_t bitcode = 0x00;
    bitcode = encode(value, playergrid);
    ir_serial_transmit(bitcode);
}


/* Waits until it receives a signal of the opponent's move, and updates
 * own playergrid with coordinates affected
 */
void get_opponent_playergrid_update(Playergrid* playergrid)
{
    uint8_t is_updated = 0;
    uint8_t bitcode = 0xFF;
    uint8_t value = 0;
    while (is_updated != 1) {
        ir_serial_ret_t ret;
        ret = ir_serial_receive(&bitcode);
        if (ret == IR_SERIAL_OK) {
            is_updated = 1;
        }
        pacer_wait();
    }
    value = decode(bitcode, playergrid);
    update_playergrid(value, playergrid);   // Update own playergrid
}
