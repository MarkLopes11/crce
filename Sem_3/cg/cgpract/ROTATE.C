#include <graphics.h>
#include <stdio.h>
#include <math.h>

int main() {
    int gd = DETECT, gm;


    int x1 = 100, y1 = 100; // Initial coordinates of the line
    int x2 = 200, y2 = 200;
    int centerX = 150, centerY = 150; // Center of rotation

    int angle = 45; // Rotation angle in degrees
    int newX1,newX2,newY1,newY2;
    // Convert the angle to radians
    double radians = angle * (M_PI / 180.0);
    initgraph(&gd,&gm,"");
    // Calculate the new coordinates after rotation
    newX1 = centerX + (x1 - centerX) * cos(radians) - (y1 - centerY) * sin(radians);
    newY1 = centerY + (x1 - centerX) * sin(radians) + (y1 - centerY) * cos(radians);
    newX2 = centerX + (x2 - centerX) * cos(radians) - (y2 - centerY) * sin(radians);
    newY2 = centerY + (x2 - centerX) * sin(radians) + (y2 - centerY) * cos(radians);

    // Draw the original line
    line(x1, y1, x2, y2);

    // Draw the rotated line
    line(newX1, newY1, newX2, newY2);

    getch();
    closegraph();

    return 0;
}
