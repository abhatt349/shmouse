#include "networking.h"

int main(int argc, char **argv) {

  int server_socket1, server_socket2;
  char buffer[BUFFER_SIZE];

  if (argc == 2) {
    server_socket1 = client_setup( argv[1]);
    server_socket2 = client_setup( argv[1]);
  }
  else {
    server_socket1 = client_setup( TEST_IP );
    server_socket2 = client_setup( TEST_IP );
  }

  while (1) {
    printf("enter data: ");
    fgets(buffer, sizeof(buffer), stdin);
    *strchr(buffer, '\n') = 0;
    write(server_socket1, buffer, sizeof(buffer));
    read(server_socket1, buffer, sizeof(buffer));
    printf("received: [%s]\n", buffer);
  }
}
