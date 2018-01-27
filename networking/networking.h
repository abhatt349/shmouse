#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <signal.h>

#ifndef NETWORKING_H
#define NETWORKING_H

#define BUFFER_SIZE 2000
#define PORT "1234"
#define TEST_IP "127.0.0.1"

#define CLIP_KEY 17934
#define ARR_KEY 12344
#define REQUEST_MESSAGE "&&&&&&"
void error_check(int i, char *s);
int server_setup();
int server_connect(int sd);
int client_setup(char * server);

char* make_copy(char* str);
size_t replace_spaces_with_nulls(char* str);
char** parse_args(char* line);


#endif
