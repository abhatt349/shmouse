#include "networking/networking.h"
#include "client/client.h"
int global_clip_socket = 0;

char* get() {
    //send message stating that u are requesting.
    write(global_clip_socket, REQUEST_MESSAGE, sizeof(REQUEST_MESSAGE));
    char *buffer = calloc(BUFFER_SIZE,sizeof(char));
    read(global_clip_socket, buffer, BUFFER_SIZE);
    return buffer;
}

void set(char *apples) {
    write(global_clip_socket, apples, strlen(apples));
    //write(global_clip_socket, "WATCHA GONNA DO BROTHER", sizeof("WATCHA GONNA DO BROTHER"));
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
        sprintf(buffer, "clip");
        printf("%s\n", buffer);
        write(server_socket1, "clip", sizeof("clip"));
        sprintf(buffer, "mouse");
        write(server_socket2, "mouse", sizeof("mouse"));
    }
    else {
        printf("Please specify the IP and number!\n");
        exit(0);
        //server_socket1 = client_setup( TEST_IP );
        //server_socket2 = client_setup( TEST_IP );
    }
    int f = fork();
    //if you're the parent, do mouse processing, else do clipboard processing
    if (f) {

        //if it's negative, then this sends data to the server as an input, if it's 0 it does nothing, otherwise it reads location data from the server
        int in_or_out = 0; 
        fd_set in_fd_set;
        Display * display = open_root_display();
        Window root = get_root_window(display);
        int x11_fd = ConnectionNumber(display);
        while (1) {
            read(server_socket2, buffer, BUFFER_SIZE);
            if (!strncmp(buffer, REQUEST_INPUT, strlen(buffer))) {
                in_or_out = -1;
            }
            if (!strncmp(buffer, REQUEST_OUTPUT, strlen(buffer))) {
                in_or_out = 1;
            }
            if (!strncmp(buffer, REQUEST_NONE, strlen(buffer))) {
                in_or_out = 0;
            }

            //if you're an output that reads data from the server, geterdone
            if (in_or_out > 0) {
                read(server_socket2, buffer, BUFFER_SIZE);
                int x, y;
                int mode, button;
                sscanf(buffer, "%d %d %d %d", &x,&y, &mode, &button);  
                display = open_root_display();
                root = get_active_window(display);
                set_pointer(display, root, x, y);
                fake_mouse_event(display, root, mode, button);
                XCloseDisplay(display);

            }
            //if you're an input do some other stui
            if (in_or_out < 0) {
                // final structure will be "x y Press/Release/None Button"
                FD_ZERO(&in_fd_set);
                FD_SET(x11_fd, &in_fd_set);
                struct timeval tv;
                XEvent event;
                tv.tv_usec = 1000;
                int x,y;
                if (!select(x11_fd+1, &in_fd_set, 0, 0, &tv)) {
                    //send upstream with no mouse event if there was a timeout
                    get_pointer(display, root, &x, &y);
                    sprintf(buffer, "%d %d %d %d", x,y, 0, 0);
                    write(server_socket2, buffer, strlen(buffer));

                } else {
                    get_pointer(display, root, &x, &y);
                }
                while (XPending(display)) {
                    XNextEvent(display, &event);
                    switch (event.type) {
                        case ButtonRelease:
                            switch(event.xbutton.button) {
                                default:
                                    //fill it up
                                    sprintf(buffer, "%d %d %d %d", x,y, MOUSE_RELEASE, event.xbutton.button);
                                    write(server_socket2, buffer, strlen(buffer));

                            }
                            break;
                        case ButtonPress:
                            switch(event.xbutton.button) {
                                default:
                                    //send upstream
                                    sprintf(buffer, "%d %d %d %d", x,y, MOUSE_PRESS, event.xbutton.button);
                                    write(server_socket2, buffer, strlen(buffer));
                            }
                            break;
                    }

                }

            }
        }
    }
    else {
        Display * display = open_root_display();
        Window root = get_root_window(display);
        clipboard_manage(display,root,&get,&set);

    }
}
