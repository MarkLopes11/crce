#include <graphics.h>
#include <stdio.h>

void drawCircle(int xc, int yc, int x, int y) {
    putpixel(xc + x, yc + y, YELLOW);
    putpixel(xc - x, yc + y, YELLOW);
    putpixel(xc + x, yc - y, YELLOW);
    putpixel(xc - x, yc - y, YELLOW);
    putpixel(xc + y, yc + x, YELLOW);
    putpixel(xc - y, yc + x, YELLOW);
    putpixel(xc + y, yc - x, YELLOW);
    putpixel(xc - y, yc - x, YELLOW);
}

void bresenhamCircle(int xc, int yc, int r) {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (x <= y) {
        drawCircle(xc, yc, x, y);

        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, " ");

    int xc, yc, r;
    printf("Enter the center coordinates (xc yc): ");
    scanf("%d %d", &xc, &yc);
    printf("Enter the radius: ");
    scanf("%d", &r);

    bresenhamCircle(xc, yc, r);

    getch();
    closegraph();
    return 0;
}
