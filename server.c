#include "networking/networking.h"

void clip_process(char *s);
void clip_subserver(int from_client);
int client_index = 0;
int isClip = 0;
int client_socket;
char* shared_clip;

int main() {
  int sub_pids[64];
  int listen_socket;
  int f;
  int subserver_count = 0;
  char buffer[BUFFER_SIZE];

  //set of file descriptors to read from
  fd_set read_fds;

  listen_socket = server_setup();
  int clip_mem = shmget(CLIP_KEY, 10000 * sizeof(char), 0777 | IPC_CREAT);
  int clip_sem = semget(CLIP_KEY, 1, IPC_CREAT);

  while (1) {

    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds); //0 out fd set
    FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
    FD_SET(listen_socket, &read_fds); //add socket to fd set

    //select will block until either fd is ready
    select(FD_SETSIZE, &read_fds, NULL, NULL, NULL);

    //if listen_socket triggered select
    if (FD_ISSET(listen_socket, &read_fds)) {
     client_socket = server_connect(listen_socket);
//     int fds[2];
//     pipe(fds);
     f = fork();
     shared_clip = shmat(clip_mem, 0, 0);
     sprintf(shared_clip, "shmeeeeeeemeeee");
     if(!f) { //if child
       clip_subserver(client_socket);
//       close(fds[0]); // for child to write to the parent
//       char s[10];
//       sprintf(s, "%d", client_index*2 + isClip);
//       printf("%s", s);
//       sleep(2); // parent will wait for the child to run, even without using wait
//       write(fds[1], s, sizeof(s));
    }
     else {
       subserver_count++;
       close(client_socket);
//       close(fds[1]);
//       char s[10];
//       read(fds[0], s, sizeof(s));
//       sub_pids[atoi(s)] = f;
     }
    }//end listen_socket select

    //if stdin triggered select
    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      //if you don't read from stdin, it will continue to trigger select()
      fgets(buffer, sizeof(buffer), stdin);
      //char** command_arr = parse_args(buffer);


    }//end stdin select
  }
}

void clip_subserver(int client_socket) {
  char buffer[BUFFER_SIZE];

  read(client_socket, buffer, sizeof(buffer));
  client_index = atoi(buffer);
  printf("%d\n", client_index);
  //for testing client select statement
  write(client_socket, "hello client\n", sizeof("hello client\n"));
  read(client_socket, buffer, sizeof(buffer));
  //printf("%s\n", buffer);    //diagnostic
  if (strcmp(buffer, "clip")) {
    isClip = 0;
    printf("Mouse!!\n");
  }
  else {
    isClip = 1;
    printf("Clipboard!!\n");
  }
  char *buf = (char *) calloc(BUFFER_SIZE, sizeof(char));
  while (read(client_socket, buf, BUFFER_SIZE)) {

    printf("[subserver %d] received: [%s]\n", getpid(), buf);
    clip_process(buf);

  }//end read loop
  close(client_socket);
  exit(0);
}

void clip_process(char * s) {
  if(strncmp(s, REQUEST_MESSAGE, strlen(REQUEST_MESSAGE)-1)) {
    printf("wrote to mem\n");
    sprintf(shared_clip, "%s", s);
  }
  else {
    write(client_socket, shared_clip, strlen(shared_clip));
  }
}
