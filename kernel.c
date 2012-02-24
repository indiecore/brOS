#include "kernel.h"

int clearScreen(unsigned char*);	//todo: write a .h
int printGreeting(unsigned char*);
int printc(unsigned char*, char);
int println(char*);

//int cursor = 0;	//this is the current screen position of the cursor ie: where stuff gets written
//unsigned char *videoram;	//global vram :D

void kmain(void* mbd, unsigned int magic )
{
   if ( magic != 0x2BADB002 )
   {
      /* Something went not according to specs. Print an error */
      /* message and halt, but do *not* rely on the multiboot */
      /* data structure. */
   }
 
   /* You could either use multiboot.h */
   /* (http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh) */
   /* or do your offsets yourself. The following is merely an example. */ 
   char * boot_loader_name =(char*) ((long*)mbd)[16];

/*grab some vram so we can write to the screen*/
 
   	vram = (unsigned char *) 0xb8000;

  	clearScreen();
	printGreeting();
	
/*
	int i;	
	for(i=0;i<192;i++)
	{
		videoram[i*2]=i;
		videoram[i*2+1]=0x07;
	}*/

  /* videoram[0] = 65;  character 'A' */
  /* videoram[1] = 0x07;  forground, background color. */
 
   /* Write your kernel here.*/
}

int clearScreen()
{
	int i;
	
	for(i=0;i<5000;i++)
		vram[i]=0;
	return 0;
}

int printGreeting()
{
	println("Welcome to brOS");	//print a null terminated string
	return 0;
}

int printc(char c)		//a simple function for writing a single character to the screen
{
	vram[cursor++]=c;	//prints the character, increments the cursor
	vram[cursor++]=0x02;	//set up the colour for the printed character green on black
	
	return 0;
}

int println(char* line)
{
	int i = 0;
	while(line[i]!='\0')
	{
		printc(videoram,line[i++]);
	}
	
/*80 lines on a line. If it is exactly at the start of a new line it will be cursor-1%80=0*/
	//cursor-=cursor%80*2;
	int temp = cursor;
	cursor+=80*2;
	cursor-=temp;	
		printc(videoram,'>');
	return 0;
}
