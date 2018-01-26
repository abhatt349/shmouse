#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <X11/Xlib.h>
#include <string.h>

#define MOUSE_RELEASE -1
#define MOUSE_PRESS 1
Display * open_root_display();
Window get_root_window(Display * display);
Window get_active_window(Display * display);
void get_resolution(Display * display, int * width, int * height);
int get_pointer(Display * display, Window window, int * x, int * y);
int set_pointer(Display * display, Window window, int dest_x, int dest_y);
int move_pointer(Display * display, int dest_x, int dest_y);
Cursor make_blank_cursor(Display * display, Window window);
int fake_mouse_event(Display * display, Window window, int type, int button);

void clipboard_manage(Display * display, Window root, char* (*get)(), void (*set)(char *));
char * read_clipboard(Display * display, Window root, Atom target); 
void reject_request(Display * dispay, XSelectionRequestEvent * req);
void send_utf8(Display * display, XSelectionRequestEvent * req, Atom utf8, char * clipboard);

