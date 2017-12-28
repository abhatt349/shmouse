# Shmouse
## By Aryan Bhatt, Eric Chen, Gilvir Gill

Looking to increase productivity when you're trying to do your Systems project, fifty college apps, and ten different projects for the various humanities classes you unknowingly signed up for? Multiple monitors not cutting it anymore? Shmouse is a C program that uses Xlib to allow you to use the same keyboard and mouse between completely different computers! It even lets you transfer our clipboard between multiple devices! 
### Required libraries:
* Xlib (wayland is way too outlandish)
### UI
To get started, run the server program, `./server`. This will manage the active keyboard, mouse, and clipboard. Next, you can begin launching clients with `./client`, which will automatically assume the new computer to be the rightmost. In order to specify a certain left right order, you can specify a right-left priority, with the smallest number being the leftmost. For example, running `./server -n 10` on computer 1 and `./server -n 1` on computer 2, computer 2 will show up as the leftmost. Once multiple computers are established, you will be able to use any mouse on any computer. For instane, mouse 1 is connected to computer 1. If you move mouse 1 all the way to the left of computer 1's screen, it will then move into the rightmost point of computer 2. However, if you now use mouse 2, the cursor will be wherever it was the last time you used mouse 2. 


For more technical details, please view ** DESIGN** in the main directory of this repo.
