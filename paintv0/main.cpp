#include <X11/X.h>
#include <X11/Xlib.h>
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include "line.h"
int main()
{
    Display* dpy;
    Window root;
    Window w;
    int colorWhite;
    int colorBlack;

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
    XStoreName(dpy, w, "Rohit Nimkar: Paint v0");
    XSelectInput(dpy, w, KeyPressMask | KeyReleaseMask | ExposureMask | StructureNotifyMask | ButtonMotionMask | Button1MotionMask | ButtonPressMask | ButtonReleaseMask);

    GC gc = XCreateGC(dpy, w, 0, nullptr);
    XSetForeground(dpy, gc, colorWhite);
    GC ec = XCreateGC(dpy, w, 0, nullptr);
    XSetForeground(dpy, ec, colorBlack);
    XMapWindow(dpy, w);
    XFlush(dpy);

    line l1, l2;
    while (1)
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

        case KeyPress:
        {
            KeySym sym = XKeycodeToKeysym(dpy, event.xkey.keycode, 0);
            std::cout << "Key pressed = " << sym << std::endl;
            break;
        }
        case ButtonPress:
        {
            std::cout << "Mouse button pressed at (" << event.xbutton.x << ", " << event.xbutton.y << ")" << std::endl;
            l1.setStart(point(event.xbutton.x, event.xbutton.y));
            break;
        }
        case ButtonRelease:
        {
            std::cout << "Button released at (" << event.xbutton.x << ", " << event.xbutton.y << ")\n";
            l1.setEnd(point(event.xbutton.x, event.xbutton.y));
            l1.draw(dpy, w, gc);
            break;
        }
        case MotionNotify:
            l1.draw(dpy, w, ec);
            std::cout << "Mouse is at (" << event.xbutton.y << ", " << event.xbutton.y << ")\n";
            l1.setEnd(point(event.xbutton.x, event.xbutton.y));
            l1.draw(dpy, w, gc);
        }
    }

    sleep(5);
    XDestroyWindow(dpy, w);
    XCloseDisplay(dpy);

    return (0);
}
