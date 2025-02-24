#include<graphics.h>
#include<stdio.h>
#include<conio.h>

void main()
{
	int gd =DETECT,gm;
	int i;
	initgraph(&gd,&gm,"");

	for(i=0;i<=getmaxy();i++)
	{
		circle(100+i,50,20);
		line(100+i,70,50+i,100);
		line(100+i,70,150+i,100);
		line(100+i,70,100+i,140);
		line(100+i,140,70+i,200);
		line(100+i,140,130+i,200);
		delay(10);
		cleardevice();


	}
	getch();
	closegraph();
}