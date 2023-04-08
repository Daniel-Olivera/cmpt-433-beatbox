#include <stdio.h>
#include <stdbool.h>
#include "modules/tools/tools.h"
#include "modules/joystick/joystick.h"
#include "modules/beatGenerator/beatGenerator.h"
#include "modules/accelerometer/accel.h"
#include "modules/segDisplay/segDisplay.h"
#include "modules/printer/printer.h"
#include "modules/periodTimer/periodTimer.h"
#include "modules/UDP/udp.h"

#define BPM (float)120

#define BASS "beatbox-wave-files/100052__menegass__gui-drum-bd-soft.wav";
#define HIHAT "beatbox-wave-files/100053__menegass__gui-drum-cc.wav"
#define SNARE "beatbox-wave-files/100059__menegass__gui-drum-snare-soft.wav"

static bool stopping = false;

int main(void)
{
    Period_init();
    JSCONTROLS_init();
    beatGen_init();
    Accel_init();
    printer_init();
    UDP_init(&stopping);

    sleepForMs(300);
    while(!stopping){
        sleepForMs(1);
        //do nothing :)
    }

    printf("\nshutting down.\n");
    printer_cleanup();
    printf("printer shut down.\n");
    JSCONTROLS_cleanup();
    printf("joystick shut down.\n");
    beatGen_cleanup();
    printf("beatgen shut down.\n");
    Accel_cleanup();
    printf("accel shut down.\n");
    UDP_cleanup();
    printf("udp shut down.\n");

    Period_cleanup();

    return 0;
}