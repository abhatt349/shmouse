#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#define BUFFER_SIZE 256
#define PORT "11877"
#define DEFAULT_IP "localhost"

void error_check(int i, char *s);
int server_setup();
int client_setup(char * nickname);
int server_connect;
