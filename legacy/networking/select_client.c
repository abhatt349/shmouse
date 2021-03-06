#include "networking.h"

int global_clip_socket = 0;

char* get() {
  char *buffer = malloc(BUFFER_SIZE);
  read(global_clip_socket, buffer, sizeof(buffer));
  return buffer;
}

void set(char *apples) {
  write(global_clip_socket, apples, sizeof(apples));
  //write(global_clip_socket, "hello", sizeof("hello"));
}


int main(int argc, char **argv) {

  int server_socket1, server_socket2;
  char buffer[BUFFER_SIZE];

  fd_set read_fds;

  if (argc == 3) {
    server_socket1 = client_setup( argv[1]);
    global_clip_socket = server_socket1;
    server_socket2 = client_setup( argv[1]);
    write(server_socket1, argv[2], sizeof(argv[2]));
    write(server_socket2, argv[2], sizeof(argv[2]));
    read(server_socket1, buffer, sizeof(buffer));
    printf("%s", buffer);
    read(server_socket2, buffer, sizeof(buffer));
    printf("%s", buffer);
//    sprintf(buffer, "clip");
//    printf("%s\n", buffer);
    write(server_socket1, "clip", sizeof("clip"));
//    sprintf(buffer, "mouse");
    write(server_socket2, "mouse", sizeof("mouse"));
  }
  else {
    server_socket1 = client_setup( TEST_IP );
    server_socket2 = client_setup( TEST_IP );
  }

  Display * display = open_root_display();
  Window root = get_root_window(display);
  clipboard_manage(display,root,&get,&set);
/*
  while (1) {

    printf("enter data: ");
    //the above printf does not have \n
    //flush the buffer to immediately print
    fflush(stdout);

    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
    FD_SET(server_socket1, &read_fds); //add socket1 to fd set
    FD_SET(server_socket2, &read_fds); //add socket2 to fd set

    //select will block until either fd is ready
    select(FD_SETSIZE, &read_fds, NULL, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      fgets(buffer, sizeof(buffer), stdin);
      *strchr(buffer, '\n') = 0;
      write(server_socket1, buffer, sizeof(buffer));
      read(server_socket1, buffer, sizeof(buffer));
      printf("received: [%s]\n", buffer);
    }//end stdin select

    //currently the server is not set up to
    //send messages to all the clients, but
    //this would allow for broadcast messages
    if (FD_ISSET(server_socket1, &read_fds)) {
      read(server_socket1, buffer, sizeof(buffer));
      printf("[SERVER BROADCAST] [%s]\n", buffer);
      printf("enter data: ");
      //the above printf does not have \n
      //flush the buffer to immediately print
      fflush(stdout);
    }//end socket select

    if (FD_ISSET(server_socket2, &read_fds)) {
      read(server_socket2, buffer, sizeof(buffer));
      printf("[SERVER BROADCAST] [%s]\n", buffer);
      printf("enter data: ");
      //the above printf does not have \n
      //flush the buffer to immediately print
      fflush(stdout);

    }//end socket select
  }//end loop
*/
}

