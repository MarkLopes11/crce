#include<stdio.h>
#include<graphics.h>

typedef struct
{
    int x,y;
}Point;

typedef struct
{
    int x_min,x_max,y_min,y_max;
}Window;

int isInsideWindow(Point point, Window window)
{
    return(point.x<=window.x_max && point.x>=window.x_min && point.y<=window.y_max && point.y>=window.y_min);
}

int main()
{

    Point p;
    Window w;
    int gd = DETECT,gm;
    initgraph(&gd,&gm,"");
    clrscr();

    printf("Enter the location of the point(x,y): ");
    scanf("%d %d",&p.x,&p.y);

    printf("Enter the co-ordinates of the window(xmin,ymin,xmax,ymax): ");
    scanf("%d %d %d %d",&w.x_min,&w.y_min,&w.x_max,&w.y_max);

    rectangle(w.x_min,w.y_min,w.x_max,w.y_max);

   if( isInsideWindow(p, w ))
   {
   putpixel(p.x,p.y,GREEN);
    printf("The point is inside the window.\n");
   }
   else{
   putpixel(p.x,p.y,RED);
    printf("The point outside the window.");
   }
  getch();
  closegraph();

}

