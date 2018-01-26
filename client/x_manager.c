#include "client.h"

/*
 * BEGIN X11 get abstractions
*/

Display * open_root_display() {
    Display * display = XOpenDisplay(NULL); 
    if (display == NULL) {
        fprintf(stderr, "ERROR: Unable to open display");
        exit(1);
    }
    return display;
}


/*
 * pre: opened display
 * post: returns root window (encompasses entire screen)
 */
Window get_root_window(Display * display) {
    Window dest_w = DefaultRootWindow(display);
    //possible error checking for the future
    return dest_w;
}

/*
 * pre: opened display
 * post: returns currently selected window
 */
Window get_active_window(Display * display) {
    Window active;
    int revert_to;
    XGetInputFocus(display, & active, &revert_to);
    return active;
}

/*
 * pre: opened display
 * post: gets resolution of default screen, important for scaling. most likely cleaner solutions out there but this should do the trick
 */
void get_resolution(Display * display, int *width, int * height) {
    Screen * screen = DefaultScreenOfDisplay(display);
    *width = screen-> width;
    *height = screen->height;
}

/*
 * pre: opened display, most likely window = root window
 * post: x and y are updated to have the current coords 
 */
int get_pointer(Display * display, Window window, int * x, int * y) {
    *x = 0;
    *y = 0;
    int win_x;
    int win_y;
    unsigned int mask;
    Window returned;
    XQueryPointer(display, window, &returned, &returned, x, y, &win_x, &win_y, &mask);
    return 1;
}


/*
 * BEGIN SET METHODS
 */

Cursor make_blank_cursor(Display * display, Window window) {
    static char colors[] = {0};
    Cursor cursor;
    Pixmap blank;
    XColor color;
    blank = XCreateBitmapFromData(display, window, colors, 1, 1);
    cursor = XCreatePixmapCursor(display, blank, blank, &color, &color, 1, 1);
    //grab pointer to stop it from being used, make it use the specified cursor
    unsigned int event_mask = ButtonReleaseMask | ButtonPressMask;
    XGrabPointer(display, window, False, event_mask, GrabModeAsync, GrabModeAsync, None, cursor, CurrentTime);
    return cursor;
}


/*
 * pre: opened root window and display
 * pos:hard sets pointer location relative to ROOT window
 */
int set_pointer(Display * display, Window window,int dest_x, int dest_y) {
    int src_x = 0, src_y = 0;
    unsigned int src_width=0, src_height=0;    
    XWarpPointer(display, None, window, src_x, src_y, src_width, src_height, dest_x,
            dest_y);
    return 1;

}
/*
 * pre: opened root window
 * post: moves pointer by this delta
 */
int move_pointer(Display * display, int dest_x, int dest_y) {
    return set_pointer(display, None, dest_x, dest_y);
}

int fake_mouse_event(Display * display, Window root, int type, int button) {
    XEvent event;
    memset(&event, 0, sizeof(event));
    event.xbutton.same_screen= True; //same screen as root window
    event.xbutton.subwindow = root; 
    //get the rest from querying the pointer
    //this is the one time sub windows and stuff actually matter, so keep running this in a loop until there are no more subwindows to find the most encapsulated code
    while ((event.xbutton.subwindow ) != 0) {
        event.xbutton.window = event.xbutton.subwindow;
        XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    }
    if (type == MOUSE_PRESS) {
        event.type = ButtonPress;
        if (!XSendEvent(display, PointerWindow, True, ButtonPressMask, &event)) fprintf(stderr, "ERROR: Could not send button press event");
        return MOUSE_PRESS;
    } 
    if (type == MOUSE_RELEASE) {
        event.type = ButtonRelease;
        if (!XSendEvent(display, PointerWindow, True, ButtonReleaseMask, &event)) fprintf(stderr, "ERROR: Could not send button release event");
        return MOUSE_RELEASE;
    }
    else {
        return 0;
    }


}
