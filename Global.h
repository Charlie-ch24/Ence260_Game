/** @file   Global.h
    @author Euan Robinson & Charlie Hunter
    @date   16 October 2020
    @brief  Holds the playergrid global struct
*/

#define GRID_HEIGHT 7
#define GRID_WIDTH 5
#define NUMBER_OF_POSITIONS GRID_HEIGHT * GRID_WIDTH

#ifndef GLOBAL_H
#define GLOBAL_H


/* Global declaration of playergrid type that will be defined in game.c */
typedef struct playergrid_t {
    int8_t posX;        // Holds current x position (columns 0 to 4)
    int8_t posY;        // Holds current y position (rows 0 to 6)
    uint8_t bitmap[5];  // Holds bitmap of where bombs are
} Playergrid;


#endif //GLOBAL_H
