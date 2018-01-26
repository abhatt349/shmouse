#include <X11/Xlib.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
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
        XWarpPointer(display, None, None, 0, 0, 0, 0, 1, 1);
        sleep_ms(1);
        XCloseDisplay(display);
    }
   return 0;
}
