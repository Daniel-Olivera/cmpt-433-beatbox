/*
    Initializes the joystick as well as provides all of the
    functionalities needed to get joystick inputs from the user
*/

#include "joystick.h"
#include "../tools/tools.h"
#include "../beatGenerator/beatGenerator.h"
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

#define JSUP_VAL "/sys/class/gpio/gpio26/value"
#define JSRT_VAL "/sys/class/gpio/gpio47/value"
#define JSDN_VAL "/sys/class/gpio/gpio46/value"
#define JSLFT_VAL "/sys/class/gpio/gpio65/value"
#define JSIN_VAL "/sys/class/gpio/gpio27/value"

#define JSUP_D "/sys/class/gpio/gpio26/direction"
#define JSRT_D "/sys/class/gpio/gpio47/direction"
#define JSDN_D "/sys/class/gpio/gpio46/direction"
#define JSLFT_D "/sys/class/gpio/gpio65/direction"
#define JSIN_D "/sys/class/gpio/gpio27/direction"

static FILE* jsDirectionFiles[JSDIRECTIONS_NUM_DIRECTIONS];
static FILE* jsValueFiles[JSDIRECTIONS_NUM_DIRECTIONS];

int currentDirection = JSDIRECTIONS_NONE;

static bool stopping = false;
static pthread_t joystickThreadId;

//prototypes
static void JSCONTROLS_open_direction_files(char*);
static void JSCONTROLS_close_direction_files(void);
static void JSCONTROLS_open_value_files(char*);
static void JSCONTROLS_close_value_files(void);
static void JSCONTROLS_readStickDirection(void);
static void* joystickThread(void* arg);

void JSCONTROLS_init(void)
{
    //export the pins for the iff not already exported
    exportGpioPin("14");
    exportGpioPin("15");
    exportGpioPin("16");
    exportGpioPin("17");
    exportGpioPin("18");

    //configure the pins for gpio mode
    runCommand("config-pin p8.14 gpio"); //joystick up pin
    runCommand("config-pin p8.15 gpio"); //joystick right pin
    runCommand("config-pin p8.16 gpio"); //joystick down pin
    runCommand("config-pin p8.18 gpio"); //joystick left pin
    runCommand("config-pin p8.17 gpio"); //joystick in pin


    JSCONTROLS_open_direction_files("w");

    for(int i = 0; i < JSDIRECTIONS_NUM_DIRECTIONS; i++){
        int charWritten = fprintf(jsDirectionFiles[i], "%s", "in");
        if(charWritten <= 0){
            printf("ERROR SETTING INPUT DIRECTION");
        }
    }
    
    JSCONTROLS_close_direction_files();

	pthread_create(&joystickThreadId, NULL, joystickThread, NULL);
}

void JSCONTROLS_cleanup(void)
{
    stopping = true;
	pthread_join(joystickThreadId, NULL);
}

static void JSCONTROLS_open_direction_files(char* mode)
{
    jsDirectionFiles[0] = fopen(JSUP_D, mode);
    jsDirectionFiles[1] = fopen(JSRT_D, mode);
    jsDirectionFiles[2] = fopen(JSDN_D, mode);
    jsDirectionFiles[3] = fopen(JSLFT_D, mode);
    jsDirectionFiles[4] = fopen(JSIN_D, mode);

    for(int i = 0; i < JSDIRECTIONS_NUM_DIRECTIONS; i++){
        if (jsDirectionFiles[i] == NULL){
            printf("ERROR: Unable to open direction file: %d", i);
        }
    }
}

static void JSCONTROLS_close_direction_files(void)
{
    for(int i = 0; i < JSDIRECTIONS_NUM_DIRECTIONS; i++){
        fclose(jsDirectionFiles[i]);
    }
}

static void JSCONTROLS_open_value_files(char* mode)
{
    jsValueFiles[0] = fopen(JSUP_VAL, mode);
    jsValueFiles[1] = fopen(JSRT_VAL, mode);
    jsValueFiles[2] = fopen(JSDN_VAL, mode);
    jsValueFiles[3] = fopen(JSLFT_VAL, mode);
    jsValueFiles[4] = fopen(JSIN_VAL, mode);

    for(int i = 0; i < JSDIRECTIONS_NUM_DIRECTIONS; i++){
        if (jsValueFiles[i] == NULL){
            printf("ERROR: Unable to open direction file: %d", i);
        }
    }
}

static void JSCONTROLS_close_value_files(void)
{
    for(int i = 0; i < JSDIRECTIONS_NUM_DIRECTIONS; i++){
        fclose(jsValueFiles[i]);
    }
}

int JSCONTROLS_getStickDirection(void)
{
    JSCONTROLS_readStickDirection();
    return currentDirection;
}

static void JSCONTROLS_readStickDirection(void)
{
    JSCONTROLS_open_value_files("r");

    const int MAX_LENGTH = 1024;
    char JSBuff_UP[MAX_LENGTH];
    char JSBuff_RT[MAX_LENGTH];
    char JSBuff_DN[MAX_LENGTH];
    char JSBuff_LFT[MAX_LENGTH];
    char JSBuff_IN[MAX_LENGTH];

    fgets(JSBuff_UP,    MAX_LENGTH, jsValueFiles[0]);
    fgets(JSBuff_RT,    MAX_LENGTH, jsValueFiles[1]);
    fgets(JSBuff_DN,    MAX_LENGTH, jsValueFiles[2]);
    fgets(JSBuff_LFT,   MAX_LENGTH, jsValueFiles[3]);
    fgets(JSBuff_IN,    MAX_LENGTH, jsValueFiles[4]);

    JSCONTROLS_close_value_files();

    if(strcmp(JSBuff_UP, "0\n") == 0){
        currentDirection = JSDIRECTIONS_UP;
    }
    else if(strcmp(JSBuff_RT, "0\n") == 0){
        currentDirection = JSDIRECTIONS_RIGHT;
    }
    else if(strcmp(JSBuff_DN, "0\n") == 0){
        currentDirection = JSDIRECTIONS_DOWN;
    }
    else if(strcmp(JSBuff_LFT, "0\n") == 0){
        currentDirection = JSDIRECTIONS_LEFT;
    }
    else if(strcmp(JSBuff_IN, "0\n") == 0){
        currentDirection = JSDIRECTIONS_IN;
    }
    else {
        currentDirection = JSDIRECTIONS_NONE;
    }
}

void* joystickThread(void* arg){
    while(!stopping){
        if (JSCONTROLS_getStickDirection() == JSDIRECTIONS_UP){
            beatGen_increaseVolume();
            sleepForMs(300);
        }

        if (JSCONTROLS_getStickDirection() == JSDIRECTIONS_DOWN){
            beatGen_decreaseVolume();
            sleepForMs(300);
        }
        if (JSCONTROLS_getStickDirection() == JSDIRECTIONS_LEFT){
            beatGen_decreaseTempo();
            sleepForMs(300);
        }

        if (JSCONTROLS_getStickDirection() == JSDIRECTIONS_RIGHT){
            beatGen_increaseTempo();
            sleepForMs(300);
        }

        if (JSCONTROLS_getStickDirection() == JSDIRECTIONS_IN){
            beatGen_cycleBeat();
            sleepForMs(300);
        }
        sleepForMs(10);
    }
    return NULL;
}