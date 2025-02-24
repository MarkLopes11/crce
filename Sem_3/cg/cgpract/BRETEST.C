
#include <graphics.h>
#include <stdio.h>
#include <math.h>

void main() {
    int gd = DETECT, gm;
    int i, x1, x2, y1, y2, dx, dy, pk, steps, x, y;

    initgraph(&gd, &gm, " ");
    printf("Enter the first point: ");
    scanf("%d %d", &x1, &y1);
    printf("Enter the end point: ");
    scanf("%d %d", &x2, &y2);

    dx = abs(x2 - x1);
    dy = abs(y2 - y1);

    x = x1;
    y = y1;

    if (dx >= dy) {
        steps = dx;
    } else {
        steps = dy;
    }

    pk = 2 * dy - dx;

    for (i = 0; i < steps; i++) {
        putpixel(x, y, YELLOW);

        if (pk > 0) {
            pk = pk + 2 * (dy - dx);
            x = x + (x2 > x1 ? 1 : -1);
            y = y + (y2 > y1 ? 1 : -1);
        } else {
            pk = pk + 2 * dy;
            x = x + (x2 > x1 ? 1 : -1);
        }
    }

    getch();
    closegraph();
}
