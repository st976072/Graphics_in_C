#include<graphics.h>
#include<conio.h>
#include<dos.h>
#include<alloc.h>

union REGS i,o;
int midx,midy;
char far *p1,far *p2,far *p3,far *p4;
void save(int,int,int,int);
void restore(int,int);
void restrictmouseptr(int,int,int,int);
void getmousepos(int*,int*,int*);
void showmouseptr();
void hidemouseptr();
void main()
{
	int gd = DETECT,gm,sy,button,x,y,sx,tx,ty,x1,y1,x2,y2;

	if(initmouse() == 0)
	{
		printf("\nMouse driver not loaded....");
		exit(1);
	}

	printf("%d",getbkcolor());
	initgraph(&gd,&gm,"C:\\TC\\BGI");

	gotoxy(1,1);
		printf("Draw box......");
	gotoxy(60,1);
		printf("Right button to exit");
		do
		{
			showmouseptr();
			getmousepos(&button,&tx,&ty);

			if(button&1 == 1)
			{
				hidemouseptr();
				sx = x = x1 = x2 = tx;
				sy = y = y1 = y2 = ty;
				setcolor(BLUE);
				save(x1,y1,x2,y2);
				rectangle(x1,y1,x2,y2);
				getmousepos(&button,&tx,&ty);

				while((button&1)==1)
				{
					if(x != tx || y!= ty)
					{
						setcolor(YELLOW);
						rectangle(x1,y1,x2,y2);
						restore(x1,y1);
						x = tx;
						y = ty;

						if(x<sx)
						{
							x1 = x;
							x2 = sx;
						}
						else
						{
							x1 = sx;
							x2 = x;
						}
						if(y<sy)
						{
							y1 = y;
							y2 = sy;
						}
						else
						{
							y1 = sy;
							y2 = y;
						}
						setcolor(WHITE);
						save(x1,y1,x2,y2);
						rectangle(x1,y1,x2,y2);
					}

					getmousepos(&button,&tx,&ty);

				}
				restore(x1,y1);
				showmouseptr();
			}
		}while((button&2) != 2);

		gotoxy(1,1);
		printf("Press any key to exit");
		gotoxy(60,1);
		printf(" ");
		getch();
		closegraph();
}
void save(int x1,int y1,int x2,int y2)
{
	unsigned area1,area2,area3,area4;

	midx = x1 + (x2-x1)/2;
	midy = y1 + (y2-y1)/2;

	area1 = imagesize(x1,y1,midx,midy);
	p1 = farmalloc(area1);
	area2 = imagesize(midx + 1,y1,x2,midy);
	p2 = farmalloc(area2);
	area3 = imagesize(x1,midy + 1,midx,y2);
	p3 = farmalloc(area3);
	area4 = imagesize(midx + 1,midy + 1,x2,y2);
	p4 = farmalloc(area4);

	if(p1 == NULL||p2 == NULL || p3 == NULL || p4 == NULL)
	{
		closegraph();
		printf("Memory allocation errope!");
		exit(2);
	}
	getimage(x1,y1,midx,midy,p1);
	getimage(midx + 1,y1,x2,midy,p2);
	getimage(x1,midy + 1,midx,y2,p3);
	getimage(midx + 1,midy + 1,x2,y2,p4);
}
void restore(int x1,int y1)
{
	putimage(x1,y1,p1,OR_PUT);
	putimage(midx + 1,y1,p2,OR_PUT);
	putimage(x1,midy + 1, p3, OR_PUT);
	putimage(midx+1,midy + 1,p4,OR_PUT);

	farfree(p1);
	farfree(p2);
	farfree(p3);
	farfree(p4);
}
int initmouse()
{
	i.x.ax = 0;
	int86(0x33,&i,&o);
	return(o.x.ax);
}
void showmouseptr()
{
	i.x.ax = 1;
	int86(0x33,&i,&o);
}
void hidemouseptr()
{
	i.x.ax = 2;
	int86(0x33,&i,&o);
}
void restrictmouseptr(int x1,int y1,int x2,int y2)
{
	i.x.ax = 7;
	i.x.cx = x1;
	i.x.dx = x2;
	int86(0x33,&i,&o);
	i.x.ax = 8;
	i.x.cx = y1;
	i.x.dx = y2;
	int86(0x33,&i,&o);
}
void getmousepos(int*button,int*x,int*y)
{
	i.x.ax = 3;
	int86(0x33,&i,&o);
	*button = o.x.bx;
	*x = o.x.cx;
	*y = o.x.dx;
}