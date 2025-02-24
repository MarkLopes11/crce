#include<stdio.h>
#include<graphics.h>
#include<math.h>
void main()
{
 int gd = DETECT,gm;
 int x1=100,y1=200,x2=200,y2=300;
 int i,x,y,dx,dy,steps,Xinc,Yinc;
 initgraph(&gd,&gm," ");
 dx = x2-x1;
 dy = y2-y1;

 if(dx>dy)
 {
	steps = dx;
 }
 else
 {
	steps = dy;
 }
 Xinc = dx/steps;
 Yinc = dy/steps;

 putpixel(x1,y1,WHITE);

 x = x1;
 y = y1;

 for( i=0;i<steps;i++)
 {
	x = x+Xinc;
	y = y+Yinc;
	putpixel(x,y,WHITE);
 }
 getch();
 closegraph();









}