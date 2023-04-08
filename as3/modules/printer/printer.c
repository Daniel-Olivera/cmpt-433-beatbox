#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "printer.h"
#include "../tools/tools.h"
#include "../beatGenerator/beatGenerator.h"

static bool stopping = false;
static pthread_t printerThreadId;
Period_statistics_t periodTimer;

static void* printData(void* arg);

void printer_init(void)
{
    sleepForMs(1000);
    pthread_create(&printerThreadId, NULL, printData, NULL);
}

void printer_cleanup(void)
{
    stopping = true;
    pthread_join(printerThreadId, NULL);
}

static void* printData(void* arg)
{
    while(!stopping){
        //Doesn't print the rest without printing something like this first ¯\_(ツ)_/¯
        printf("\n");
        printf("M%d\t", beatGen_getBeat());
        printf("%dbpm\t", beatGen_getBpm());
        printf("vol:%d\t", beatGen_getVolume());

        Period_getStatisticsAndClear(PERIOD_EVENT_BUFFER_FILLED, &periodTimer);
        printf("Audio[%.3f, %.3f] avg %.3f/%d\t", periodTimer.minPeriodInMs, periodTimer.maxPeriodInMs,
                                                         periodTimer.avgPeriodInMs, periodTimer.numSamples);

        Period_getStatisticsAndClear(PERIOD_EVENT_ACCEL_READ, &periodTimer);
        printf("Accel[%.3f, %.3f] avg %.3f/%d", periodTimer.minPeriodInMs, periodTimer.maxPeriodInMs,
                                                         periodTimer.avgPeriodInMs, periodTimer.numSamples);
        sleepForMs(1000);

    }
    
    return NULL;
}