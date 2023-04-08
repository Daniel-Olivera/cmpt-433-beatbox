#include "beatGenerator.h"
#include "../audioMixer/audioMixer.h"
#include "../tools/tools.h"
#include "../segDisplay/segDisplay.h"
#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>

#define BASS_FILE "beatbox-wave-files/100051__menegass__gui-drum-bd-hard.wav"
#define HIHAT_FILE "beatbox-wave-files/100053__menegass__gui-drum-cc.wav"
#define SNARE_FILE "beatbox-wave-files/100059__menegass__gui-drum-snare-soft.wav"
#define MEASURES 8

wavedata_t* beats[NUM_BEATS][MEASURES][NUM_SOUNDS];

static wavedata_t bassFile;
static wavedata_t hihatFile;
static wavedata_t snareFile;

static wavedata_t* bassHandle = &bassFile;
static wavedata_t* hihatHandle = &hihatFile;
static wavedata_t* snareHandle = &snareFile;

static int currentBeat = ROCK_BEAT;
static float bpm = 120; //default 120

static bool stopping = false;
static pthread_t beatGenThreadId;

static void setupRockBeat(void);
static void setupOtherBeat(void);
static void* beatGenThread(void* arg);

void beatGen_init(void)
{
    setupRockBeat();
    setupOtherBeat();
    SegDisplay_Init();
    SegDisplay_setNum(currentBeat);
    AudioMixer_init();
    AudioMixer_readWaveFileIntoMemory(BASS_FILE, bassHandle);
    AudioMixer_readWaveFileIntoMemory(HIHAT_FILE, hihatHandle);
    AudioMixer_readWaveFileIntoMemory(SNARE_FILE, snareHandle);
    pthread_create(&beatGenThreadId, NULL, beatGenThread, NULL);
}

void beatGen_cleanup(void)
{
    stopping = true;
	pthread_join(beatGenThreadId, NULL);
    AudioMixer_freeWaveFileData(&bassFile);
    AudioMixer_freeWaveFileData(&hihatFile);
    AudioMixer_freeWaveFileData(&snareFile);
    AudioMixer_cleanup();
    SegDisplay_cleanup();
}

int beatGen_getBpm(void)
{
    return bpm;
}

void beatGen_cycleBeat(void)
{
    currentBeat++;
    if(currentBeat == NUM_BEATS){
        currentBeat = NO_BEAT;
    }
    SegDisplay_setNum(currentBeat);
}

int beatGen_getBeat(void)
{
    return currentBeat;
}

void beatGen_setBeat(int num)
{
    currentBeat = num;
    SegDisplay_setNum(currentBeat);
}


int beatGen_getVolume(void)
{
    return AudioMixer_getVolume();
}

void beatGen_increaseVolume(void)
{
    int currentVol = AudioMixer_getVolume();
    AudioMixer_setVolume(currentVol + 5);
}

void beatGen_decreaseVolume(void)
{
    int currentVol = AudioMixer_getVolume();
    AudioMixer_setVolume(currentVol - 5);
}

void beatGen_increaseTempo(void)
{
    if(bpm < BEATGEN_MAX_TEMPO)
        bpm += 5;
}

void beatGen_decreaseTempo(void)
{
    if(bpm > BEATGEN_MIN_TEMPO)
        bpm -= 5;
}

void beatGen_playBassSound(void)
{
    AudioMixer_queueSound(bassHandle);
}

void beatGen_playHihatSound(void)
{
    AudioMixer_queueSound(hihatHandle);
}

void beatGen_playSnareSound(void)
{
    AudioMixer_queueSound(snareHandle);
}

static int getBPMDelay(void)
{
    return (60/bpm/2) * 1000;
}


static void setupRockBeat(void)
{
    for(int i = 0; i < MEASURES; i++){
        beats[ROCK_BEAT][i][0] = hihatHandle;
        if(i == (MEASURES / 2) || i == 0){
            beats[ROCK_BEAT][i][1] = bassHandle;
        }
        if(i == 2 || i == 6){
            beats[ROCK_BEAT][i][1] = snareHandle;
        }
    }
}

static void setupOtherBeat(void)
{
    for(int i = 0; i < MEASURES; i++){
        beats[ROCK_BEAT_TWO][i][0] = hihatHandle;
        if(i == 0 || i == 1 || i == 5){
            beats[ROCK_BEAT_TWO][i][1] = bassHandle;
        }
        if(i == 2 || i == 6){
            beats[ROCK_BEAT_TWO][i][1] = snareHandle;
        }
    }
}

static void* beatGenThread(void* arg)
{
    sleepForMs(350);
    while(!stopping){

        for(int i = 0; i < MEASURES; i++){
            for(int j = 0; j < NUM_SOUNDS; j++){
                if(beats[currentBeat][i][j] != NULL){
                    AudioMixer_queueSound(beats[currentBeat][i][j]);
                }
            }
            sleepForMs(getBPMDelay());
        }

    }
    return NULL;
}