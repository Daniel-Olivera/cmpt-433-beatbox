#ifndef BEAT_GENERATOR_H
#define BEAT_GENERATOR_H

#include "../periodTimer/periodTimer.h"

#define BEATGEN_MAX_TEMPO 300
#define BEATGEN_MIN_TEMPO 40

//Different sounds; can be expanded later
enum SOUND {
    BASS,
    HIHAT,
    SNARE,
    NUM_SOUNDS
};

//Different beats for readability
enum BEAT_MODE {
    NO_BEAT,
    ROCK_BEAT,
    ROCK_BEAT_TWO,
    NUM_BEATS
};

//Initizializes the beat generator by setting up the rock beats,
//initializing the audioMixer and creating the thread used for
//continuously generating sounds
void beatGen_init(void);

//Cleans up the above
void beatGen_cleanup(void);

//Returns the current bpm/tempo
int beatGen_getBpm(void);

//Changes the currently playing beat to the next one
void beatGen_cycleBeat(void);

//Returns the current beat
int beatGen_getBeat(void);

//Sets the current beat to a specific one
void beatGen_setBeat(int num);

//Returns the current volume (from the audioMixer)
int beatGen_getVolume(void);

//Increases the volume by 5 (up to 100)
void beatGen_increaseVolume(void);

//Decreases the volume by 5 (down to 0)
void beatGen_decreaseVolume(void);

//Increases the bpm/tempo by 5 (up to 300)
void beatGen_increaseTempo(void);

//Decreases the bpm/tempo by 5 (down to 40)
void beatGen_decreaseTempo(void);

//Enqueues the corresponding sound in the audioMixer
void beatGen_playBassSound(void);
void beatGen_playHihatSound(void);
void beatGen_playSnareSound(void);

#endif