#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void sleep_ms(int ms) {
    //this sleeps in seconds+nanoseconds, so lets convert our ms
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000*1000;
    nanosleep(&ts, NULL);
}
int main() {
    while (1) {
        Display * display = XOpenDisplay(NULL); 
        Window dest_w = DefaultRootWindow(display);
        XWarpPointer(display, None, dest_w, 0, 0, 0, 0, 100, 100);
        sleep_ms(1);
        XCloseDisplay(display);
    }
    return 0;
}
