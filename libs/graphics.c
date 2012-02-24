#include "kernel.h"
#include "stdlib.h"
#include "math.h"
#include "graphics.h"

int quit = FALSE;
void graphics_init()
{
	screenPtr = 0xB8000;
	blank();
	asm volatile ("cli"); 	//disable interrupts
	irq_install_handler(1,graphics_keyboard_handler);
	timer_install();
	asm volatile ("sti"); //enable interrupts
}

void exit_graphics()
{
	cls();
	asm volatile ("cli"); 
	keyboard_install();
	timer_install();
	asm volatile ("sti"); 
}

void blank()
{
	int i,j;
	for(i = 0; i <80 ; i++)	//increment x
		for(j=0;j<25;j++)//increment y
		{
			screenPtr[(j*80*2)+i*2] = 0xDC;
			screenPtr[(j*80*2)+i*2+1] = 0;
		}
}

void putPixel(unsigned char colour, unsigned xPoint, unsigned yPnt)
{
	unsigned yPoint = yPnt/2;
	unsigned char attr; //the attributs of a cell
	//set the character	
	
	if(yPnt&1)	//set lower bits
	{
		attr = (colour)|(screenPtr[(yPoint*80*2)+xPoint*2+1] & 0xF0);
	}
	else
	{
		attr =(colour<<4)|(screenPtr[(yPoint*80*2)+xPoint*2+1] & 0x0F);
	}

	screenPtr[(yPoint*80*2)+xPoint*2+1] = attr;
}

void drawLine(unsigned x1,unsigned y1, unsigned x2,unsigned y2, unsigned char colour)
{
	int deltaX = x1-x2;
	int deltaY = y1-y2;

	double error = 0.0;
	double deltaError = (double)deltaY/(double)deltaX;
	int y = y1;
	
	int i;
	for(i=x1;i<x2;i++)
	{
		putPixel(colour,i,y);
		error += deltaError;

		if(abs(error) >= 0.5)
		{
			y = y+1;
			error = error - 1.0;
		}
	}
}

void drawRect(unsigned x, unsigned y, unsigned xlen, unsigned ylen, unsigned char colour)
{
	int i;
	for(i=0;i<=ylen;i++)
	{
		putPixel(colour,x,y+i);
		putPixel(colour,x+xlen,y+i);
	}

	for(i=0;i<=xlen;i++)
	{
		putPixel(colour,x+i,y);
		putPixel(colour,x+i,y+ylen);
	}
}

void hurr() {
	float *f=(float*)0xC0000;
	int g=1;
	int x,y,i,j;
	for(i = 0; i < 2*80*50; i++) {
		f[i]=0;
	}

	f[30*50+30] = 15.0;
	f[31*50+30] = 7.0;
	f[29*50+30] = 7.0;
	f[30*50+31] = 7.0;
	f[30*50+29] = 7.0;

	for(;;g++) {
		if(!quit)
		{
			for(y=1; y<50-1; y++) {
				for(x=1; x<80-1; x++) {
					putPixel(((u8)((
						f[((g)&1)*80*50+y*80+x]=((
						f[((1+g)&1)*80*50+(y-1)*80+x]+
						f[((1+g)&1)*80*50+(y+1)*80+x]+
						f[((1+g)&1)*80*50+y*80+x-1]+
						f[((1+g)&1)*80*50+y*80+x+1])/2-
						f[((g)&1)*80*50+y*80+x])*0.999999)*1.8)+0)&0xF,
						x,y);
				}
			}
		}
		else
		{
			blank();
			break;
		}
	}
}

void durr() {
	u8 *f = (u8*)0xC0000; //or whatever. 8k.
	int x,y,x2,y2,i,g,t;
	for(i=0;i<8000;i++){
		f[i]=0;
	}
	f[1620]=f[1621]=f[1622]=f[1624]=f[1700]=f[1783]=f[1784]=f[1861]=f[1862]=f[1864]=f[1940]=f[1942]=f[1944]=1;

	for(g=0;;g++) {
	f[1620]=f[1621]=f[1622]=f[1624]=f[1700]=f[1783]=f[1784]=f[1861]=f[1862]=f[1864]=f[1940]=f[1942]=f[1944]=1;
		for(x=1;x<80-1;x++){
			for(y=1;y<50-1;y++) {
				t=0;
				for(x2=-1;x2<2;x2++) {
					for(y2=-1;y2<2;y2++) {
						if(x2||y2) {
							t+=f[4000*((g+1)&1)+(y+y2)*80+x+x2];
						}
					}
				}
				putPixel(f[4000*(g&1)+y*80+x]=(t==3)||(t>1&&t<4&&f[4000*((g+1)&1)+y*80+x])*15,x,y);
			}
		}
	}
}

void graphics_keyboard_handler(struct regs *r)
{

	unsigned char scancode;

    /* Read from the keyboard's data buffer */
    	scancode = inportb(0x60);

    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
   	if (scancode & 0x80)
    	{
        /* You can use this one to see if the user released the
        *  shift, alt, or control keys... */
	//figure this out later
    	}
    	else
    	{
		if(scancode == 16)	//if you hit q
		{
			exit_graphics();
			printc('>');
		}
		else if(scancode == 57)	//if you hit spacebar
		{
			blank();
		}
		else if(scancode == 2)	//if you hit 1
		{
			drawLine(4,13,25,25,BLUE);
		}
		else if(scancode == 3)	//if you hit 2
		{
			int i;
			for(i=0;i<49;i++)
			{
				if(i&1)
				drawRect(i,i,79-i,49-1,BLUE);
				else
				drawRect(i,i,79-i,49-i,GREEN);
			}
		}
		else if(scancode == 4)	//if you hit 3
		{
			hurr();
		}
		else if(scancode == 5)//if you hit 4
		{
			durr();
		}
		else if(scancode == NEWLINE)
		{
			quit = TRUE;
		}
    	}
}
