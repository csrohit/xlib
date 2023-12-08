/**
 * @file            main.cpp
 * @description     Create a blank window
 * @author          Rohit Nimkar
 * @version         1.3
 * @date            2023-12-9
 *
 * @copyright Copyright (c) 2023
 *
 * @attention
 *
 *  Use of this source code is governed by a BSD-style
 *  license that can be found in the LICENSE file or at
 *  opensource.org/licenses/BSD-3-Clause
 *
 */

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

#define WIN_WIDTH  800U
#define WIN_HEIGHT 600U

/* Windowing related variables */
Display             *dpy              = NULL;  // connection to server
Window               root             = 0UL;   // handle of root window [Desktop]
Window               w                = 0UL;   // handle of current window
int                  iScreen          = 0;     // handle to DefaultScreen
XVisualInfo          vi               = {0};   // Pointer to current visual info
XSetWindowAttributes xsarr            = {};    // structure for windows attributes
bool                 gbAbortFlag      = false; // Global abort flag
bool                 gbFullscreen     = false; // global state variable for fullscreen
static Atom          wm_delete_window = 0;     // atomic variable to detect close button click
XRectangle           rect             = {0};   // window dimentions rectangle
int                  defaultDepth     = 0;     // default depth of screen
int                  ret              = -1;

int main()
{
    /* Open connection to x-server */
    dpy = XOpenDisplay(NULL);
    if (NULL == dpy)
    {
        printf("Failed to open connection to display");
        return -1;
    }

    iScreen = DefaultScreen(dpy);
    printf("Default screen no. %d\n", iScreen);

    defaultDepth = XDefaultDepth(dpy, iScreen);
    printf("Default depth of screen %d\n", defaultDepth);

    root = XDefaultRootWindow(dpy);

    ret = XMatchVisualInfo(dpy, iScreen, defaultDepth, TrueColor, &vi);
    if (0 == ret)
    {
        printf("Failed to get a visual\n");
        XCloseDisplay(dpy);
        return -1;
    }

    xsarr.event_mask = ExposureMask | StructureNotifyMask | VisibilityChangeMask;

    /* create window */
    w = XCreateWindow(dpy,          // connection to x-server
                      root,         // root window
                      0,            // x-coordinate of top-left corner
                      0,            // y co-ordinate of top-left corner
                      WIN_WIDTH,    // window width
                      WIN_HEIGHT,   // window height
                      0U,           // boder width
                      defaultDepth, // depth of the window
                      InputOutput,  // class of window
                      vi.visual,    // pointer to visual structure
                      CWEventMask,  // mask indicating which values are
                                    // passed in attributes
                      &xsarr);      // pointer to attributes structure

    if (0 == w)
    {
        printf("failed to create window\n");
        XCloseDisplay(dpy);
        return -1;
    }

    /* Set Window title */
    XStoreName(dpy, w, "Rohit Nimkar: Blank Window");

    /* Listen for window close event */
    Atom wmDelete = XInternAtom(dpy, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(dpy, w, &wmDelete, 1);

    /* Make window visible on screen */
    XMapWindow(dpy, w);

    /* send all commands to x-server to ensure window is displayed */
    XFlush(dpy);

    while (false == gbAbortFlag)
    {
        XEvent event;

        /*blocking call: wait for next event*/
        XNextEvent(dpy, &event);

        switch (event.type)
        {
            case Expose:
            {
                /* the event is generated when a portion of a window that was previously obscured becomes visible again */
                printf("Window is exposed\n");
                break;
            }
            case MapNotify:
            {
                /* the event is generated when window is visible on display */
                printf("Window is visible on screen\n");
                break;
            }
            case ClientMessage:
            {
                if (event.xclient.data.l[0] == wmDelete)
                {
                    gbAbortFlag = true;
                }

                break;
            }
            default:
            {
                printf("event: %d\n", event.type);
                break;
            }
        }
    }

    /* Destroy the window */
    XDestroyWindow(dpy, w);

    /* Release connection to x-server */
    XCloseDisplay(dpy);

    return (0);
}

