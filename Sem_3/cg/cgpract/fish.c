#include <graphics.h>
#include <conio.h>

void drawFish(int x, int y) {
    // Body of the fish (ellipse)
    setcolor(YELLOW);
    ellipse(x, y, 0, 360, 30, 15);
    floodfill(x, y, YELLOW);

    // Tail of the fish (triangle)
    setcolor(YELLOW);
    line(x - 30, y, x - 45, y - 15);
    line(x - 45, y - 15, x - 45, y + 15);
    line(x - 45, y + 15, x - 30, y);
    floodfill(x - 37, y, YELLOW);

    // Eye of the fish (small ellipse)
    setcolor(BLACK);
    ellipse(x + 15, y - 5, 0, 360, 3, 5);
    floodfill(x + 15, y - 5, BLACK);
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\Turboc3\\BGI");

    // Draw a single fish
    drawFish(200, 200);

    getch();
    closegraph();
    return 0;
}
