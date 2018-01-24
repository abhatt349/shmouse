#include <X11/Xlib.h>

int initialize_server(); //opens shmemory, does initial connectio
int subserver(int client_socket);
int close_server();
int control(int input_pc, int output_pc);

