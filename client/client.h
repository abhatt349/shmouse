#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <X11/Xlib.h>


Display * open_root_display();
Window get_root_window(Display * display);
Window get_active_window(Display * display);
void get_resolution(Display * display, int * width, int * height);
int get_pointer(Display * display, Window window, int * x, int * y);
int set_pointer(Display * display, Window window, int dest_x, int dest_y);
int move_pointer(Display * display, int dest_x, int dest_y);



