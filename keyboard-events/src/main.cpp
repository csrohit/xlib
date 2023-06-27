#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysymdef.h>
#include <algorithm>
#include <iostream>
#include <unistd.h>

int main()
{
    Display* dpy         = nullptr;
    Window root          = 0;
    Window w             = 0;
    int colorWhite       = 0;
    int colorBlack       = 0;
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
    XStoreName(dpy, w, "Rohit Nimkar: Key Press detection");
    XSelectInput(dpy, w, KeyPressMask | ExposureMask | StructureNotifyMask);

    GC gc = XCreateGC(dpy, w, 0, nullptr);
    XSetForeground(dpy, gc, colorWhite);
    GC ec = XCreateGC(dpy, w, 0, nullptr);
    XSetForeground(dpy, ec, colorBlack);
    wm_delete_window = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(dpy, w, &wm_delete_window, 1);
    XMapWindow(dpy, w);
    XFlush(dpy);

    while (!globalAbortFlag)
    {
        XEvent event;
        XNextEvent(dpy, &event);
        switch (event.type)
        {
        case Expose:
        {
            std::cout << "Notify event\n";
            XDrawLine(dpy, w, gc, 0, 0, 800, 600);
            break;
        }
        case DestroyNotify:
        {
            std::cout << "Window is destroyed\n";
            globalAbortFlag = true;
            break;
        }
        case MapNotify:
        {
            std::cout << "Window is mapped\n";
            XDrawLine(dpy, w, gc, 0, 0, 800, 600);
            break;
        }
        case ClientMessage:
        {
            if (event.xclient.data.l[0] == wm_delete_window) { globalAbortFlag = true; }
            break;
        }
        case KeyPress:
        {
            KeySym sym = XkbKeycodeToKeysym(dpy, event.xkey.keycode, 0, 0);
            std::cout << "Key pressed = " << XKeysymToString(sym) << std::endl;
            if (sym == XK_Escape) { globalAbortFlag = true; }
            break;
        }
        default:
        {
            std::cout << "received event: " << event.type << std::endl;
        }
        }
    }
    std::cout << "Exiting\n";
    XDestroyWindow(dpy, w);
    XCloseDisplay(dpy);

    return (0);
}
