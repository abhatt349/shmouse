# Shmouse
## By Aryan Bhatt, Eric Chen, Gilvir Gill

Looking to increase productivity when you're trying to do your Systems project, fifty college apps, and ten different projects for the various humanities classes you unknowingly signed up for? Multiple monitors not cutting it anymore? Shmouse is a C program that uses Xlib to allow you to use the same keyboard and mouse between completely different computers! It even lets you transfer our clipboard between multiple devices! 
### Required libraries:
* Xlib (wayland is way too outlandish) aswell as Xtst `libxtst-dev` in aptitude default repos
### UI
In it's current iteration, Shmouse provides a shared clipboard feature aswell as a shared mouse feature. All of this depends on a supported X11 environment. Since X is an anchient piece of software, ensuring that everything on the client end works as intended is key.

To test client side features, try `make test`. You will see three executables: `x_out_test`, which on run will scroll your mouse wheel down every second, `x_in_test`, which records mouse activity including click events in terminal, and `clipboard_test`, which behaves as a full fledged clipboard manager for the X environment. Since X stores selections (i.e. the clipboard) in window properties, closing a window usually means losing the clipboard. The function contained inside
clipboard_test acts as a clipboard manager, it essentially hijacks the clipboard and makes itself the owner. When a new window attempts to take ownership, it lets it, then copies over the contents of the new clipboard into memory, and then forces itself back into having ownership. 

Only if these three functions work will you have a hope of having a good time. At this point, you are ready to start the server and clients. You can start up the server simply with `./run_server`, and then run the client with `./run_client {ip} {number}`. This number serves as an identifier on the server side and is also important for the future seamless mode that will allow the mouse to appear to travel over different displays, indicating the left-right order from 0->n. 
At this point, you should notice that the clipboard is shared between all clients connected to the server.

### Known Bugs

* Clipboard sometimes isn't copied properly, or starts releasing random characters due to lack of precautions in terms of sending data over sockets.
* Clipboard selections don't function at all when used in terminal emulators (known problem with gnome-terminal and xterm, not tested on others)
* Mouse sharing doesn't work yet across the socket, despite all necessary functions being in place clientside.* Clipboard selections don't function at all when used in terminal emulators (known problem with gnome-terminal and xterm, not tested on others)

For more technical details, please view ** DESIGN** in the main directory of this repo.
