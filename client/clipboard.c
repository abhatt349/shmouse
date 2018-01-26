#include "client.h"


/*
 * "main" method that does all clipboard processing
 * takes function pointers for a get function that returns current clipboard, and a set function that sets that clipboard to be 
 */
void clipboard_manage(Display * display, Window root, char* (*get)(), void (*set)(char *)) {
    Atom sel;
    Atom utf8;
    XEvent event;
    XSelectionRequestEvent * request;
    //Create a window to properly interact with x and take ownership of clipboard
    Window dummy = XCreateSimpleWindow(display, root, -10, -10, 1, 1, 0, 0, 0);
    //let it process selection related events
    XSelectInput(display, dummy, SelectionNotify | SelectionRequest | SelectionClear);
    //an atom is pretty much a predefined property that can't be changed.
    sel = XInternAtom(display, "CLIPBOARD", False);
    utf8 = XInternAtom(display, "UTF8_STRING", False);

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
                Window owner = XGetSelectionOwner(display, sel);
                if (owner == None) {
                    fprintf(stderr, "ERROR: clipboard has no owner");
                    exit(0);
                }
                //request utf8 into your target property
                XConvertSelection(display, sel, utf8, target, dummy, CurrentTime);
                int done = 0;
                while (!done) {
                    XSelectionEvent * selev;
                    XNextEvent(display, &event);
                    switch (event.type) {
                        case SelectionNotify:
                            selev = (XSelectionEvent*) &event.xselection;
                            if (selev->property == None) {
                                fprintf(stderr,"ERROR: Conversion could not be performed\n");
                            }
                            else {
                                char * clipboard = read_clipboard(display, dummy, target);
                                (*set)(clipboard);
                                done = 1;
                            }
                    }
                    //take back ownership
                }
                XSetSelectionOwner(display, sel, dummy, CurrentTime);
                break;


            case SelectionRequest:
                printf("[Clipboard] processing clipboard request\n");
                request = (XSelectionRequestEvent*) (&event.xselectionrequest);
                printf("Requestor: 0x%lx\n", request->requestor);
                if (request->target != utf8 || request->property == None) {
                    //printf("IF YOU SMELL WHAT THE ROCK IS COOKING\n\n\n");
                    reject_request(display, request);
                }
                else {
                    //printf("HULKAMANIA RUNS WILD, BROTHER\n\n\n");
                    send_utf8(display, request, utf8, (*get)());
                }
                break;
        }
    }
}

/*
 * pre: opened display
 * post: sends no property to request
 */
void reject_request(Display * display, XSelectionRequestEvent * req) {
    XSelectionEvent event;
    char * property = XGetAtomName(display, req->target);
    printf("[Clipboard] denying request for property %s\n", property);
    event.type = SelectionNotify;
    event.property = None; 

    event.target = req->target;
    event.time = req->time;
    event.requestor = req->requestor;
    event.selection = req->selection;

    XSendEvent(display, req->requestor, True, NoEventMask, (XEvent *) &event);
}
/*
 * pre: open display
 * post: proccesses request for clipboard selection if the client requests unicode. works for gtk applications such as chromium and gedit but not natively inside gnome-terminal or xterm
 */

void send_utf8(Display * display, XSelectionRequestEvent * req, Atom utf8, char * clipboard) {
    //printf("HULKAMANIA RUNS WILD BROTHER\n");
    XSelectionEvent event;
    //change property of requestor to include your text, because this is how selections in x work
    XChangeProperty(display, req->requestor, req->property, utf8, 8, PropModeReplace,(unsigned char *) clipboard, strlen(clipboard));

    printf("%s\n", clipboard);
    //contstruct your event
    event.type = SelectionNotify;
    event.requestor = req->requestor;
    event.target = req->target;
    event.time = req->time;
    event.selection = req->selection;
    event.property = req->property;

    XSendEvent(display, req->requestor, True, NoEventMask, (XEvent *) &event);

}
    
char * read_clipboard(Display * display, Window root, Atom target) {
    //when reading the clipboard, it is possible to work with what are called INCRs
    //int XGetWindowProperty(display, w, property, long_offset, long_length, delete, req_type, 
    //                        actual_type_return, actual_format_return, nitems_return, bytes_after_return, 
    //                                                prop_return)
    Atom actual_type_return;
    int actual_format_return;
    unsigned long size, num_items;
    unsigned char * str;
    //use this call to get the size, don't actually owrry about saving anything.Free the string afterwards since it saves 0 bits due to the size specification you gave of 0.
    XGetWindowProperty(display, root, target, 0, 0, False, AnyPropertyType,&actual_type_return, &actual_format_return, &num_items, &size, &str);
    //X mandates using Xfree() any time you're freeing memory used by x
    XFree(str);
    //we don't have time to support incr, so we have to exit if the clipboard is too big.
    Atom incr = XInternAtom(display, "INCR", False);
    if (actual_type_return == incr) {
        fprintf(stderr, "ERROR: clipboard too big to read in one go\n");
        exit(0);
    }
    Atom actual_actual_type_return;
    XGetWindowProperty(display, root, target, 0, size, False, AnyPropertyType, &actual_actual_type_return, &actual_format_return, &num_items, &num_items, &str);
    //we're done reading now, we can now delete the property we were using to get the data off
    //printf("Copied string: %s\n", str);
    XDeleteProperty(display, root, target);
    return str;
}

