#include "client.h"
// Simulate mouse click

int main() {
    sleep(2);
    Display * display = open_root_display();
    Window root = get_root_window(display);
    while (1) {
          fake_mouse_event(display, root, MOUSE_PRESS, Button5);
          fake_mouse_event(display, root, MOUSE_RELEASE, Button5);
          sleep(1);
    }
    printf("stay wok\n");
}

