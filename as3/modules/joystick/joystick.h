#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdio.h>
#include <stdlib.h>

enum JSDIRECTIONS {JSDIRECTIONS_UP,
                    JSDIRECTIONS_DOWN,
                    JSDIRECTIONS_LEFT,
                    JSDIRECTIONS_RIGHT,
                    JSDIRECTIONS_IN,
                    JSDIRECTIONS_NUM_DIRECTIONS,
                    JSDIRECTIONS_NONE};

//Allows usage of the joystick on the zen cape

//Exports and configs pins before starting a thread
//to listen to joystick inputs
void JSCONTROLS_init(void);

//Cleans up the above
void JSCONTROLS_cleanup(void);

//Returns the currently pressed direction
int JSCONTROLS_getStickDirection(void);

#endif