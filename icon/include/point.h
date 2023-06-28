#ifndef POINT_H
#define POINT_H

class point
{
    private:
    int x;
    int y;

    public:
    point(int x_loc, int y_loc) : x(x_loc), y(y_loc){};
    void update(int x_loc, int y_loc)
    {
        this->x = x_loc;
        this->y = y_loc;
    }

    friend class line;
};

#endif // !POINT_H
