#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysymdef.h>
#include <iostream>
#include "line.h"
int main()
{
    Display* dpy                 = nullptr;
    Window root                  = 0UL;
    Window w                     = 0UL;
    int colorWhite               = 0;
    int colorBlack               = 0;
    bool globalAbortFlag         = false;
    static Atom wm_delete_window = 0UL;
    XColor systemColor           = {};
    XColor exactColor            = {};
    int scr                      = 0;
    Colormap defaultColormap     = {};
    Visual* defaultVisual        = {};
    GC gc  = nullptr;
    GC ec  = nullptr;
    Status rc = 0;

    dpy = XOpenDisplay(":0");
    if (nullptr == dpy)
    {
        std::cerr << "Failed to connect to x-server\n";
        return -1;
    }

    root            = XDefaultRootWindow(dpy);
    scr             = DefaultScreen(dpy);
    defaultColormap = DefaultColormap(dpy, scr);
    defaultVisual   = DefaultVisual(dpy, scr);
    colorBlack      = BlackPixel(dpy, scr);

    w = XCreateSimpleWindow(dpy, root, 0, 0, 800, 600, 0, colorBlack, colorBlack);
    XStoreName(dpy, w, "Rohit Nimkar: Red colored lines");

    /* register for eventsx */
    XSelectInput(dpy, w, ExposureMask | KeyPress | StructureNotifyMask | ButtonMotionMask | ButtonPressMask | ButtonReleaseMask);

    /* register for window manager delete event */
    wm_delete_window = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(dpy, w, &wm_delete_window, 1);

    rc = XAllocNamedColor(dpy, defaultColormap, "green", &exactColor, &systemColor);
    if (0 == rc) { std::cerr << "Failed to allocate entry\n"; }
    std::cout << "Entry for color green allocated as (" << exactColor.red << ", " << exactColor.green << ", " << exactColor.blue << ")\n";

    gc = XCreateGC(dpy, w, 0, nullptr);
    XSetForeground(dpy, gc, exactColor.pixel);

    ec = XCreateGC(dpy, w, 0, nullptr);
    XSetForeground(dpy, ec, colorBlack);

    /* make window visible */
    XMapWindow(dpy, w);
    XFlush(dpy);

    line l1, l2;
    while (!globalAbortFlag)
    {
        XEvent event;
        XNextEvent(dpy, &event);
        switch (event.type)
        {
        case Expose:
        {
            std::cout << "Notify event\n";
            // XDrawLine(dpy, w, gc, 0, 0, 800, 600);
            break;
        }
        case ButtonPress:
        {
            l1.setStart(point(event.xbutton.x, event.xbutton.y));
            break;
        }
        case KeyPress:
        {
            l1.setEnd(point(event.xbutton.x, event.xbutton.y));
            l1.draw(dpy, w, gc);
            break;
        }
        case MotionNotify:
        {
            l1.draw(dpy, w, ec);
            l1.setEnd(point(event.xbutton.x, event.xbutton.y));
            l1.draw(dpy, w, gc);
            break;
        }
        case ClientMessage:
        {
            if (event.xclient.data.l[0] == wm_delete_window) { globalAbortFlag = true; }
            break;
        }
        case KeyRelease:
        {
            KeySym sym = XkbKeycodeToKeysym(dpy, event.xkey.keycode, 0, 0);
            std::cout << "Key pressed = " << XKeysymToString(sym) << std::endl;
            if (sym == XK_Escape) { globalAbortFlag = true; }
            break;
        }
        }
    }

    std::cout << "Exiting\n";
    XDestroyWindow(dpy, w);
    XCloseDisplay(dpy);

    return (0);
}
