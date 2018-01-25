#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//make a blank cursor to "disable" the actual one
Cursor make_blank_cursor(Display *  display, Window window) {
    static char colors[] = {00};
    Cursor cursor;
    Pixmap blank;
    XColor color;
    blank = XCreateBitmapFromData(display, window, colors, 1, 1);
    cursor = XCreatePixmapCursor(display, blank, blank, &color, &color, 1, 1);

    //grab the pointer to stop it from being used.
    XGrabPointer(display, window, False, (KeyPressMask|KeyReleaseMask)&0, GrabModeAsync, GrabModeAsync, None, cursor, CurrentTime);
}

int main() {
    Display *  display = XOpenDisplay(NULL);
    Window window = XDefaultRootWindow(display);
    make_blank_cursor(display, window);
    while (1) {
        int root_x, root_y, win_x, win_y;
        unsigned int mask;
        Window returned;
        XQueryPointer(display, window, &returned, &returned, &root_x, &root_y, &win_x, &win_y, &mask);
        printf("%d,%d\n", root_x, root_y);
    }
    
}
