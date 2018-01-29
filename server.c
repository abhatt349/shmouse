#include "networking/networking.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
void clip_process(char *s);
void subserver(int from_client);
int client_socket;
int* clients;
char* shared_clip;
void mouse_process(int client_socket, int client_index);
char ** get_args(char * cmd);

//mouse subspecific:
int up_fd;
int down_fd;

int main() {
  int listen_socket;
  int f;
  int subserver_count = 0;
  char buffer[BUFFER_SIZE];

  //set of file descriptors to read from
  fd_set read_fds;

  listen_socket = server_setup();
  int clip_mem = shmget(CLIP_KEY, 10000 * sizeof(char), 0777 | IPC_CREAT);
  int clip_sem = semget(CLIP_KEY, 1, IPC_CREAT);
  int mouse = shmget(MOUSE_KEY, 64 * sizeof(int), 0777 | IPC_CREAT);
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
        shared_clip = shmat(clip_mem, 0, 0);
        clients = shmat(mouse, 0, 0);
        sprintf(shared_clip, "shmeeeeeeemeeee");
        if (!fork()) {
            subserver(client_socket);
        }
        else {
            close(client_socket);
        }
    }//end listen_socket select

    //if stdin triggered select
    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
        //if you don't read from stdin, it will continue to trigger select()
        fgets(buffer, sizeof(buffer), stdin);
        char ** args = get_args(buffer);
        int controller;
        int controlled;
        if (args[1] != NULL) {
            controller = atoi(args[1]);
        }
        if (args[2] != NULL) {
            controlled = atoi(args[2]);
        }
        printf("Controlling #%d with #%d\n", controlled, controller);
        //mouse_control(controller, controlled);


    }//end stdin select
  }
}

/* subserver: forks off subserver for the given port, whether it be a mouse or clip server will be determined by whatever it gets a connection request for.
 *
 * post: returns the index that the client specified
 */
void subserver(int client_socket) {
    char buffer[BUFFER_SIZE];

    read(client_socket, buffer, sizeof(buffer));
    int client_index = atoi(buffer);
    printf("%d\n", client_index);
    //for testing client select statement
    write(client_socket, "hello client\n", sizeof("hello client\n"));
    read(client_socket, buffer, sizeof(buffer));
    //printf("%s\n", buffer);    //diagnostic
    if (strcmp(buffer, "clip")) {
        printf("Mouse!!\n");
        mouse_process(client_socket,client_index);

    }
    else {
        printf("Clipboard!!\n");
        char *buf = (char *) calloc(BUFFER_SIZE, sizeof(char));
        while (read(client_socket, buf, BUFFER_SIZE)) {

            printf("[subserver %d] received: [%s]\n", getpid(), buf);
            clip_process(buf);

        }
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

void mouse_process(int client_socket, int client_index) {
    char *buf = (char *) calloc(BUFFER_SIZE, sizeof(char));
    clients[client_index] = getpid();
    char *fifo_name;
    sprintf(fifo_name, "%dUP", getpid());
    mkfifo(fifo_name, 0777);
    up_fd = open(fifo_name, O_RDONLY);
    sprintf(fifo_name, "%dDOWN", getpid());
    mkfifo(fifo_name, 0777);
    down_fd = open(fifo_name, O_WRONLY);
    //select call on server instructions vs continuing current task
    //  while (read(up_fifo_fd, buf, BUFFER_SIZE)) {
    //  }
    //get array containing pids of clients. clients[index] returns pid of subserver that's managing
}



char ** get_args(char * cmd) {
    size_t args_sz = 64;
    char ** args = calloc(args_sz, sizeof(char));
    int i = 0;
    char * arg;
    while ((arg = strsep(&cmd, " ")) != NULL) {
        //if the arg isnt \0, add it to the args list.
        if (*arg != '\0') {
            if ( i+1 >= args_sz) {
                args_sz *= 2;
                void * tmp = realloc(args, args_sz * sizeof(char));
                if (tmp != NULL) {
                    args = (char **) tmp;
                }
                else {
                    free(args);
                    return NULL;
                }
            }
            args[i++] = arg;
        }
    }
    args[i] = 0;
    return args;

}

