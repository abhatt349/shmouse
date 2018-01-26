#include "client.h"
int main() {
    Display * display = open_root_display();
    Window root = get_root_window(display);
    int width, height;
    get_resolution(display, &width, &height);
    printf("Screen resolution: %dx%d\n", width, height);
    make_blank_cursor(display, root);
    fd_set in_fds;
    int x11_fd = ConnectionNumber(display);
    XEvent event;
    while (1) {
        //use a file description set
        FD_ZERO(&in_fds);
        FD_SET(x11_fd, &in_fds);
        struct timeval tv;
        XEvent event;
        tv.tv_sec = 1;
        tv.tv_usec=1000;
        if (select(x11_fd+1, &in_fds, 0, 0, &tv)) {
        }
        int x, y;
        get_pointer(display, root, &x, &y);
        printf("Cursor at %d, %d\n",x, y);
        while (XPending(display)) {
            XNextEvent(display, &event);
            switch(event.type) {
                case ButtonRelease: 
                    switch(event.xbutton.button) {
                        default:
                            printf("Button %d released\n", event.xbutton.button);
                            break;

                    }
                    break;
                case ButtonPress:
                    switch(event.xbutton.button) {
                        default:
                            printf("Button %d pressed\n", event.xbutton.button);
                            break;

                    }
                    break;
            }
        }
        
    }
}
