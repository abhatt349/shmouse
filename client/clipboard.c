#include "client.h"


/*
 * "main" method that does all clipboard processing
 * takes function pointers for a get function that returns current clipboard, and a set function that sets that clipboard to be 
 */
void clipboard_manage(Display * display, Window root, char (*get)(), void (*set)(char *)) {
    Atom sel;
    Atom utf8;
    XEvent event;
    XSelectionRequestEvent * request;
    //Create a window to properly interact with x and take ownership of clipboard
    Window dummy = XCreateSimpleWindow(display, root, -10, -10, 1, 1, 0, 0, 0);
    //let it process selection related events
    XSelectInput(display, dummy, SelectionRequest | SelectionClear);
    //an atom is pretty much a predefined property that can't be changed.
    sel = XInternAtom(display, "CLIPBOARD", False);
    utf8 = XInternAtom(display, "UTF8_String", False);

    //take whats yours
    XSetSelectionOwner(display, sel, dummy, CurrentTime);
    while (1) {
        //get the next event (this is a blocking call)
        XNextEvent(display, &event);
        switch (event.type) {
            case SelectionClear:
                printf("[Clipboard] Selection ownership lost.. resetting clipboard\n");
                // save the new clipboard in memory and prepare to send it up, then reclaim ownership
                Atom target = XInternAtom(display, "TEMP", False); // target that we will hold everything in temporarily
                char * clipboard = read_clipboard(display, root, target)
                (*set)(clipboard);
                //take back ownership
                XSetSelectionOwner(display, sel, dummy, CurrentTime);
                break;
            case SelectionRequest:
                printf("[Clipboard] processing clipboard request\n");
                request = (XSelectionRequestEvent*) (&event.xselectionrequest);
                if (request->target != utf8) {
                    reject_request(display, request);
                }
                else {
                    send_utf8(display, request, utf8, get());
                }
                break;
        }
    }
}


void reject_request(Display * display, XSelectionRequestEvent * req) {
}
/*
 * pre: open display
 * post: proccesses request for clipboard selection if the client requests unicode. works for gtk applications such as chromium and gedit but not natively inside gnome-terminal or xterm
 */

void send_utf8(Display * display, XSelectionRequestEvent * req, Atom utf8, char * clipboard) {
    XSelectionEvent event;
    //change property of requestor to include your text, because this is how selections in x work
    XChangeProperty(display, req->requestor, req->property, utf8, 8, PropModeReplace,(unsigned char *) clipboard, strlen(clipboard));

    //contstruct your event
    event.type = SelectionNotify;
    event.target = req->target;
    event.time = req->time;
    event.selection = req->selection;
    event.property = req->property;
    XSendEvent(display, req->requestor, True, NoEventMask, (XEvent *) &event);

}
    

