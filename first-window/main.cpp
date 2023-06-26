/**
 * @file main.cpp
 * @description Create a blank window
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
#include <X11/Xlib.h>
#include <assert.h>
#include <unistd.h>

int main()
{
    // get connection to xserver
    Display *display = XOpenDisplay(nullptr);
    assert(display);

    // create window
    Window w = XCreateWindow(
        display,
        DefaultRootWindow(display),
        0, 0, 
        800, 600, 0, 
        CopyFromParent, CopyFromParent, CopyFromParent,
        0, 0
    );

    // map created window to display
    XMapWindow(display, w);
    
    // send requests to xserver
    XFlush(display);

    // close window after 10 seconds
    sleep(10);
    return (0);
}

