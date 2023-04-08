#ifndef UDP_H
#define UDP_H

//Init and cleanup of sockets
void UDP_init(bool *stopping);
void UDP_cleanup(void);

//Retrieves a command through UDP connection
void * UDP_getCommand(void*);

//Signals all threads to end their loops and join back to main
void UDP_stop(void);

#endif