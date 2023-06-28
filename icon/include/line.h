#ifndef LINE_H
#define LINE_H

#include "point.h"
#include <X11/X.h>
#include <X11/Xlib.h>

class line
{
    private:
    point start;
    point end;

    public:
    line() : start(0, 0), end(0, 0) {}
    line(point s, point e) : start(s), end(e){};
    inline void draw(Display* dpy, Window w, GC gc) { XDrawLine(dpy, w, gc, start.x, start.y, end.x, end.y); };
    void setStart(point p)
    {
        start.x = p.x;
        start.y = p.y;
    }
    void setEnd(point p)
    {
        end.x = p.x;
        end.y = p.y;
    }
};

#endif // !LINE_H
