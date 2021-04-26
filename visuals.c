/** @file   visuals.h
    @author Euan Robinson & Charlie Hunter
    @date   20 October 2020
    @brief  Logic for lighting LED matrix and LED1
*/

#include "system.h"
#include "pio.h"
#include "led.h"
#include "timer.h"
#include "ledmat.h"
#include "playergrid.h"
#include "tinygl.h"
#include "pacer.h"
#include "font5x7_1.h"
#include "Global.h"


/* Define PIO pins driving LED matrix rows. */
static const pio_t ledmat_rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO,
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/* Define PIO pins driving LED matrix columns. */
static const pio_t ledmat_cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};


/* Shows player position only (without bombs) */
void display_position(Playergrid* playergrid)
{
    // Turn all pixels off
    for (uint8_t i = 0; i < LEDMAT_ROWS_NUM; i++) {
        pio_output_high(ledmat_rows[i]);
    }
    for (uint8_t i = 0; i < LEDMAT_COLS_NUM; i++) {
        pio_output_high(ledmat_cols[i]);
    }

    // Turn pixel of coordinate position on
    pio_output_low(ledmat_cols[playergrid->posX]);
    pio_output_low(ledmat_rows[playergrid->posY]);


    // Turn pixels off again
    pio_output_high(ledmat_cols[playergrid->posX]);
    pio_output_high(ledmat_rows[playergrid->posY]);
}


/* Shows playergrid with bombs and player position for one cycle */
void cycle_display(Playergrid* playergrid)
{
    uint8_t current_column = 0;
    // Display all five columns
    while (current_column < LEDMAT_COLS_NUM) {
        ledmat_display_column(playergrid->bitmap[current_column], current_column);
        current_column++;
    }
    current_column = 0;

    // Display position
    display_position(playergrid);
}


/* Makes LED blink for duration in seconds */
void blink(int seconds)
{
    timer_tick_t now;
    timer_init();
    led_init();
    now = timer_get();
    led_set(LED1, 1);
    now = timer_wait_until (now + (timer_tick_t)(TIMER_RATE * seconds)); // Blinks for number of seconds
    led_set(LED1, 0);
}
