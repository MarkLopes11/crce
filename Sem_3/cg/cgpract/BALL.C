#include<stdio.h>
#include<graphics.h>

void main()
{
    int gd = DETECT,gm;
    int i;
    initgraph(&gd,&gm,"");

    for(i=0;i<=getmaxy();i++)
    {
	circle(100+i,250,50);
	delay(10);
	cleardevice();
    }
    for(i=0;i<=getmaxy();i++)
    {
	circle(getmaxy()-i,250,50);
	delay(10);
	cleardevice();
    }
    getch();
    closegraph();

}
m