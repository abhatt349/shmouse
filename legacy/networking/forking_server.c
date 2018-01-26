#include "networking.h"

void process(char *s);
void subserver(int from_client);

int main() {

  int listen_socket1, listen_socket2;
  int f;
  listen_socket1 = server_setup();
  listen_socket2 = server_setup();
  while (1) {

    int client_socket1 = server_connect(listen_socket1);
    int client_socket2 = server_connect(listen_socket2);
    f = fork();
    if (f == 0) {
      subserver(client_socket1);
      subserver(client_socket2);
    }
    else {
      close(client_socket1);
      close(client_socket2);
    }
  }
}

void subserver(int client_socket) {
  char buffer[BUFFER_SIZE];

  while (read(client_socket, buffer, sizeof(buffer))) {

    printf("[subserver %d] received: [%s]\n", getpid(), buffer);
    process(buffer);
    write(client_socket, buffer, sizeof(buffer));
  }//end read loop
  close(client_socket);
  exit(0);
}

void process(char * s) {
  while (*s) {
    if (*s >= 'a' && *s <= 'z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    else  if (*s >= 'A' && *s <= 'Z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    s++;
  }
}
