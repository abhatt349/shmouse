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
}

/* hard sets pointer location relative to ROOT window
 *
 *
 */
int set_pointer(int x, int y) {
    int src_x = 0;
    int src_y = 0;
    
    XWarpPointer(display, src_w, dest_w, src_x, src_y, src_width, src_height, dest_x,
                dest_y)

}


