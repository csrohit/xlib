/**
 * @file main.cpp
 * @description Create a blank window and draw a white diagonal
 * @author Rohit Nimkar
 * @version 1.0
 * @date 2023-06-26
 * @copyright Copyright 2023 Rohit Nimkar
 *
 * @attention
 *  Use of this source code is governed by a BSD-style
 *  license that can be found in the LICENSE file or at
 *  opensource.org/licenses/BSD-3-Clause
 */ 
#include <X11/X.h>
#include <X11/Xlib.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>

int main()
{
    // get connection to xserver
    Display *dpy = XOpenDisplay(nullptr);
    assert(dpy);

    int blackColor = BlackPixel(dpy, DefaultScreen(dpy));
    int whiteColor = WhitePixel(dpy, DefaultScreen(dpy));
    // create window
    Window w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 800, 600, 0, blackColor, blackColor);
    XStoreName(dpy, w, "Rohit Radheshyam Nimkar: First xlib window");
    XSelectInput(dpy, w, StructureNotifyMask);

    // map created window to display
    XMapWindow(dpy, w);


    GC gc = XCreateGC(dpy, w, 0, nullptr);
    XSetForeground(dpy, gc, whiteColor);

    while(1)
    {
        XEvent evt;
        XNextEvent(dpy, &evt);
        if(evt.type == MapNotify)
            break;
    }
    std::cout << "Drawing white line\n";
    XDrawLine(dpy, w, gc, 0, 0, 800, 600);
    
    // send requests to xserver
    XFlush(dpy);

    // close window after 10 seconds
    sleep(10);
    return (0);
}

