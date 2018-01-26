#include "client.h"

char clip[4096] = "foobar";
char * clipboard = clip;

char * get() {
    return clipboard;
}
void set(char* new) {
    clipboard = new;
}
int main() {
    Display * display = open_root_display();
    Window root = get_root_window(display);
    clipboard_manage(display, root, &get, &set);
}
