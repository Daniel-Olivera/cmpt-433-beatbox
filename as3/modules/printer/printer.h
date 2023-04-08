#ifndef PRINTER_H
#define PRINTER_H

#include "../periodTimer/periodTimer.h"

/*
    Initializes and starts a thread for printing
    different info to the screen every 1000ms

    Info comes from the beatGenerator using the
    provided periodTimer to get avgs and other information
*/


void printer_init(void);
void printer_cleanup(void);

#endif
