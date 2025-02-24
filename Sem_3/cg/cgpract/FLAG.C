#include <stdio.h>
#include <conio.h>
#include <graphics.h>

void boundaryFill(int x, int y, int fillColor, int borderColor) {
    if (getpixel(x, y) != borderColor && getpixel(x, y) != fillColor) {
        putpixel(x, y, fillColor);
        boundaryFill(x + 1, y, fillColor, borderColor);
        boundaryFill(x - 1, y, fillColor, borderColor);
        boundaryFill(x, y + 1, fillColor, borderColor);
        boundaryFill(x, y - 1, fillColor, borderColor);
    }
}

void main() {
    int x, r, y;
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\Turboc3\\BGI");

    // Draw the flag background (rectangle)
    rectangle(50, 100, 600, 250);
    setfillstyle(SOLID_FILL, GREEN);
    floodfill(325, 200, WHITE);

    // Draw the orange stripe
    rectangle(50, 100, 600, 150);
    setfillstyle(SOLID_FILL, RED);
    floodfill(325, 125, WHITE);

    // Draw the white stripe
    rectangle(50, 150, 600, 200);
    setfillstyle(SOLID_FILL, WHITE);
    floodfill(325, 175, WHITE);

    // Draw the Ashoka Chakra (Blue Circle)
    x = 325;
    y = 175;
    r = 35;

    setcolor(BLUE);
    circle(x, y, r);
    setfillstyle(SOLID_FILL, BLUE);
    floodfill(x, y, BLUE);

    // Fill the Ashoka Chakra using the boundary fill algorithm
    boundaryFill(x, y, BLACK, BLUE);

    getch();
    closegraph();
}
