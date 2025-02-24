#include <stdio.h>
#include <conio.h>
#include <graphics.h>
void main()
{
    int gd = DETECT, gm;
    int x = 100, y = 100, x1 = 100, y1 = 200, x2 = 200, y2 = 100, shear;
    initgraph(&gd, &gm, "");
    printf("ENTER THE SHAERING FACTOR: ");
    scanf("%d", &shear);
    cleardevice();
    line(x, y, x1, y1);
    line(x1, y1, x2, y2);
    line(x2, y2, x, y);
    setcolor(YELLOW);
    x = x + y * shear;
    x1 = x1 + y1 * shear;
    x2 = x2 + y2 * shear;
    line(x, y, x1, y1);
    line(x1, y1, x2, y2);
    line(x2, y2, x, y);
    printf("ALONG Y-AXIS");
    y = y + x * shear;
    y1 = y1 + x1 * shear;
    y2 = y2 + x2 * shear;
    line(x, y, x1, y1);
    line(x1, y1, x2, y2);
    line(x2, y2, x, y);
    getch();
    closegraph();
}
