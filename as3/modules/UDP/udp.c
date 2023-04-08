#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../tools/tools.h"
#include "../beatGenerator/beatGenerator.h"

#define MAX_LEN 1024
#define PORT 12345

#define TOKEN_BUFSIZE 64
#define TOK_DELIMITER " "

static pthread_t UDPThread;
static int sock_desc;
static struct sockaddr_in sock_in;
static struct sockaddr_in sinRemote;
static unsigned int sin_len = sizeof(sinRemote);
static char messageRx[MAX_LEN];
bool *stopping_p;

//Prototypes
static char* parseInput(char* input);

void UDP_stop(void)
{
    *stopping_p = true;
}

void * UDP_getCommand(void* nothing)
{
    while(1){

        int bytesRx = recvfrom(sock_desc, messageRx, MAX_LEN - 1, 0, 
                                (struct sockaddr*) &sinRemote, &sin_len);

        messageRx[bytesRx] = 0;
        //Hard stop if the msg is "stop"
        if(strcmp(messageRx, "shutdown") == 0){
            UDP_stop();
            break;
        }

        //parse and run the command from the message
        char* output;
        output = parseInput(messageRx);

        //send response
        char messageTx[MAX_LEN];
        snprintf(messageTx, MAX_LEN, "%s\n", output);
        sin_len = sizeof(sinRemote);
        sendto(sock_desc, messageTx, strlen(messageTx), 0, (struct sockaddr*) &sinRemote, sin_len);
    }

    close(sock_desc);
    return NULL;
}

void UDP_init(bool *stopping)
{
    stopping_p = stopping;
    memset(&sock_in, 0, sizeof(sock_in));
    sock_in.sin_family = AF_INET;
    sock_in.sin_port = htons(PORT);
    sock_in.sin_addr.s_addr = htonl(INADDR_ANY);

    sock_desc = socket(PF_INET, SOCK_DGRAM, 0);
    bind(sock_desc, (struct sockaddr*) &sock_in, sizeof(sock_in));

    pthread_create(&UDPThread, NULL, UDP_getCommand, NULL);
}

void UDP_cleanup(void)
{
    pthread_join(UDPThread, NULL);
}

static char* parseInput(char* input)
{
    char* result = " ";

    if(strcmp(input, "none") == 0){
        result = "None";
        beatGen_setBeat(NO_BEAT);
    }
    else if(strcmp(input, "rock1") == 0){
        result = "Rock 1";
        beatGen_setBeat(ROCK_BEAT);
    }
    else if(strcmp(input, "rock2") == 0){
        result = "Rock 2";
        beatGen_setBeat(ROCK_BEAT_TWO);
    }
    else if(strcmp(input, "decreaseVolume") == 0){
        beatGen_decreaseVolume();
        int currentVol = beatGen_getVolume();
        char buffer[MAX_LEN];
        sprintf(buffer,"%d", currentVol);
        result = buffer;
    }
    else if(strcmp(input, "increaseVolume") == 0){
        beatGen_increaseVolume();
        int currentVol = beatGen_getVolume();
        char buffer[MAX_LEN];
        sprintf(buffer,"%d", currentVol);
        result = buffer;
    }
    else if(strcmp(input, "decreaseTempo") == 0){
        beatGen_decreaseTempo();
        int currentBPM = beatGen_getBpm();
        char buffer[MAX_LEN];
        sprintf(buffer,"%d", currentBPM);
        result = buffer;
    }
    else if(strcmp(input, "increaseTempo") == 0){
        beatGen_increaseTempo();
        int currentBPM = beatGen_getBpm();
        char buffer[MAX_LEN];
        sprintf(buffer,"%d", currentBPM);
        result = buffer;
    }
    else if(strcmp(input, "hihat") == 0){
        beatGen_playHihatSound();
        result = "hihat";
    }
    else if(strcmp(input, "snare") == 0){
        beatGen_playSnareSound();
        result = "snare";
    }
    else if(strcmp(input, "bass") == 0){
        beatGen_playBassSound();
        result = "bass";
    }
    else if(strcmp(input, "getTempo") == 0){
        int currentBPM = beatGen_getBpm();
        char buffer[MAX_LEN];
        sprintf(buffer,"%d", currentBPM);
        result = buffer;
    }
    else if(strcmp(input, "getVolume") == 0){
        int currentVol = beatGen_getVolume();
        char buffer[MAX_LEN];
        sprintf(buffer,"%d", currentVol);
        result = buffer;
    }
    else if(strcmp(input, "getBeat") == 0){
        int currentBeat = beatGen_getBeat();
        if(currentBeat == NO_BEAT)
            result = "None";
        if(currentBeat == ROCK_BEAT)
            result = "Rock 1";
        if(currentBeat == ROCK_BEAT_TWO)
            result = "Rock 2";
    }
    else{
        result = "Unknown Command.\n";
    }
    
    return result;
}
