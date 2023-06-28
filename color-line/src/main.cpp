#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysymdef.h>
#include <algorithm>
#include <iostream>
#include <ratio>
#include <unistd.h>
#include "line.h"
int main()
{
    Display* dpy;
    Window root;
    Window w;
    int colorWhite;
    int colorBlack;
    bool globalAbortFlag = false;
    static Atom wm_delete_window;

    dpy = XOpenDisplay(":0");
    if (nullptr == dpy)
    {
        std::cerr << "Failed to connect to x-server\n";
        return -1;
    }

    root       = XDefaultRootWindow(dpy);
    colorWhite = WhitePixel(dpy, DefaultScreen(dpy));
    colorBlack = BlackPixel(dpy, DefaultScreen(dpy));

    w = XCreateSimpleWindow(dpy, root, 0, 0, 800, 600, 0, colorBlack, colorBlack);
    XStoreName(dpy, w, "Rohit Nimkar: Red colored lines");
    XSelectInput(dpy, w, ExposureMask | KeyPress | StructureNotifyMask | ButtonMotionMask | ButtonPressMask | ButtonReleaseMask);
    wm_delete_window = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(dpy, w, &wm_delete_window, 1);

    XColor systemColor, exactColor;
    exactColor.red = 0xffff;
    exactColor.blue = 0xf0f0;
    exactColor.green = 0x0000;

    Colormap myColorMap = DefaultColormap(dpy, DefaultScreen(dpy));
    Visual* defaultVisual = DefaultVisual(dpy, DefaultScreen(dpy));
    Colormap rn_colormap = XCreateColormap(dpy, w, defaultVisual, AllocNone);

    Status rc;
    rc = XAllocNamedColor(dpy, myColorMap, "green", &exactColor, &systemColor);
    if(0 == rc)
    {
        std::cerr << "Failed to allocate entry\n";
    }
    std::cout<< "Entry for color green allocated as (" << exactColor.red << ", " << exactColor.green << ", " << exactColor.blue << ")\n";

    GC gc = XCreateGC(dpy, w, 0, nullptr);
    XSetForeground(dpy, gc, exactColor.pixel);

    GC ec = XCreateGC(dpy, w, 0, nullptr);
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
