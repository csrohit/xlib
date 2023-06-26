/**
 * @file        main.cpp
 * @description print information related to display
 * @version     1.0
 * @date        2023-06-26
 * @copyright Copyright 2023 Rohit Nimkar
 *
 * @attention
 *  Use of this source code is governed by a BSD-style
 *  license that can be found in the LICENSE file or at
 *  opensource.org/licenses/BSD-3-Clause
 */
#include <X11/X.h>
#include <iostream>
#include <X11/Xlib.h>
#include <ostream>

int main()
{
    int screenWidth;
    int screenHeight;

    Window root;

    unsigned long colorWhite;
    unsigned long colorBlack;

    Display* dpy;
    int defaultScreen;

    dpy = XOpenDisplay(":0");
    if (nullptr == dpy)
    {
        std::cout << "Failed to connect to xserver\n";
        return -1;
    }

    defaultScreen = XDefaultScreen(dpy);
    root          = RootWindow(dpy, defaultScreen);

    screenWidth  = XDisplayWidth(dpy, defaultScreen);
    screenHeight = XDisplayHeight(dpy, defaultScreen);
    std::cout << "ID of root window is " << root << std::endl;
    std::cout << "ID of default display is " << defaultScreen << std::endl;
    std::cout << "Dimentions of display " << defaultScreen << " are " << screenWidth << "x" << screenHeight << std::endl;
    colorWhite = WhitePixel(dpy, defaultScreen);
    colorBlack = BlackPixel(dpy, defaultScreen);
    std::cout << "Value of white pixel is " << colorWhite << std::endl;
    std::cout << "Value of black pixel us " << colorBlack << std::endl;
    return (0);
}
