#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysymdef.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <unistd.h>

int main()
{
    Display* dpy                 = nullptr;
    Window root                  = 0;
    Window w                     = 0;
    int colorWhite               = 0;
    int colorBlack               = 0;
    bool globalAbortFlag         = false;
    static Atom wm_delete_window = 0;
    char const* fontName         = "-bitstream-courier 10 pitch-bold-i-normal--0-0-0-0-m-0-adobe-standard";
    XFontStruct* fontInfo        = nullptr;
    int screenWidth              = 0;
    int screenHeight             = 0;
    int screen                   = 0;

    dpy = XOpenDisplay(":0");
    if (nullptr == dpy)
    {
        std::cerr << "Failed to connect to x-server\n";
        return -1;
    }

    int len;
    char** fonts = XListFonts(dpy, "*", 1000, &len);
    std::cout << "returned " << len << " fonts" << std::endl;
    for (uint32_t idx = 0; idx < len; ++idx) { std::cout << fonts[idx] << std::endl; }
    XFreeFontNames(fonts);
    screen = DefaultScreen(dpy);

    fontInfo = XLoadQueryFont(dpy, fontName);
    if (nullptr == fontInfo)
    {
        std::cerr << "failed to load font\n";
        return -1;
    }

    root         = XDefaultRootWindow(dpy);
    colorWhite   = WhitePixel(dpy, DefaultScreen(dpy));
    colorBlack   = BlackPixel(dpy, DefaultScreen(dpy));
    screenWidth  = XDisplayWidth(dpy, screen);
    screenHeight = XDisplayHeight(dpy, screen);

    std::cout << "Display dimensions " << screenWidth << "x" << screenHeight << std::endl;

    w = XCreateSimpleWindow(dpy, root, 0, 0, 800, 600, 0, colorBlack, colorBlack);
    XStoreName(dpy, w, "Rohit Nimkar: font rendering");
    XSelectInput(dpy, w, KeyPressMask | ExposureMask | StructureNotifyMask);

    GC gc = XCreateGC(dpy, w, 0, nullptr);
    XSetForeground(dpy, gc, colorWhite);
    XSetFont(dpy, gc, fontInfo->fid);

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
                    char const* name             = "Font rendering in xlib by Rohit Nimkar";
                    const char* middleString     = "This string is in middle";
                    XWindowAttributes attributes = {};
                    XGetWindowAttributes(dpy, w, &attributes);
                    int stringWidth  = XTextWidth(fontInfo, middleString, strlen(middleString));
                    int stringHeight = fontInfo->ascent + fontInfo->descent;
                    int x            = (attributes.width - stringWidth) / 2;
                    int y            = (attributes.height - stringHeight) / 2;

                    XDrawString(dpy, w, gc, 10, 20, name, strlen(name));
                    XDrawString(dpy, w, gc, x, y, middleString, strlen(middleString));
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
                    XFlush(dpy);
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
            case ConfigureNotify:
                {
                    /* changes in window size, position, stacking order */
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
