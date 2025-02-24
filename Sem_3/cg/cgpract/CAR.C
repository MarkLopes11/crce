#include <graphics.h>
#include <conio.h>

void drawCar() {
    // Draw the body of the car
    setcolor(YELLOW);
    rectangle(100, 200, 300, 250);
    floodfill(200, 220, YELLOW);

    // Draw the roof
    setcolor(YELLOW);
    rectangle(150, 150, 250, 200);
    floodfill(200, 175, YELLOW);

    // Draw the wheels
    setcolor(WHITE);
    
    circle(250, 250,25);
    circle(150, 250,25);
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\Turboc3\\BGI");

    drawCar();

    getch();
    closegraph();

    return 0;
}
