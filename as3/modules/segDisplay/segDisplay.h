#ifndef SEG_DISPLAY_H
#define SEG_DISPLAY_H

//Used to display different digits on the 14-Segment display
//using I2C

//Cleans up and resets the displays
void SegDisplay_cleanup(void);

//Initializes the display by exporting the pins, etc.
void SegDisplay_Init(void);

//Displays a number on the 14-segment display
void * SegDisplay_showNum(void*);

//Changes the number to be displayed
void SegDisplay_setNum(int input);

#endif