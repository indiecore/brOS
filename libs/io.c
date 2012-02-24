#include "kernel.h"
#include "io.h"
#include "stdlib.h"
#include "string.h"
#include "graphics.h"

#define attrib  0x02

int shift = FALSE;
int reading = FALSE;

/* Scrolls the screen */
void scroll(void)
{
    unsigned blank, temp;

    /* A blank is defined as a space... we need to give it
    *  backcolor too */
    blank = 0x20 | (attrib << 8);

    /* Row 25 is the end, this means we need to scroll up */
    if(csr_y > 25)
    {
        /* Move the current text chunk that makes up the screen
        *  back in the buffer by a line */
        //temp = csr_y - 25 + 1;
        //memcpy (textmemptr, textmemptr + temp, (25 - temp) * 80 * 2);

        /* Finally, we set the chunk of memory that occupies
        *  the last line of text to our 'blank' character */
        //memsetw (textmemptr + (25 - temp) * 80, blank, 80);
       // csr_y = 25 - 1;
	cls();
    }
}

/* Updates the hardware cursor: the little blinking line
*  on the screen under the last character pressed! */
void mv_csr(void)
{
    unsigned temp;

    /* The equation for finding the index in a linear
    *  chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    temp = csr_y * 80 + csr_x/2;

    /* This sends a command to indicies 14 and 15 in the
    *  CRT Control Register of the VGA controller. These
    *  are the high and low bytes of the index that show
    *  where the hardware cursor is to be 'blinking'.*/
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

/* Clears the screen */
void cls()
{
    int i,j;

	csr_x =0;
	csr_y=0;
	for(i = 0; i <80 ; i++)
		for(j=0;j<25;j++)
			printc(' ');

    /* Update out virtual cursor, and then move the
    *  hardware cursor */
    csr_x = 0;
    csr_y = 0;
    mv_csr();
}

void putPixel(unsigned char colour, unsigned xPoint, unsigned yPnt)
{

//	unsigned short pixel = (backColour<<4)|(foreColour & 0x0F);
	unsigned yPoint = yPnt/2;
	unsigned char attr; //the attributs of a cell
	textmemptr[(yPoint*80*2)+xPoint*2] = 0xDC;	//set the character	
	
	if(yPnt&1)	//set lower bits
	{
		attr = (colour)|(textmemptr[(yPoint*80*2)+xPoint*2+1] & 0xF0);
	}
	else
	{
		attr =(colour<<4)|(textmemptr[(yPoint*80*2)+xPoint*2+1] & 0x0F);
	}

	textmemptr[(yPoint*80*2)+xPoint*2+1] = attr;
}

/* Puts a single character on the screen */
void printc(unsigned char c)
{
	if(c=='\n')
	{
		csr_x=0;
		csr_y++;
	}
	else if(c== 0x08)
	{
		if(csr_x !=0)
		{
			csr_x-=2;
			textmemptr[(csr_x)+(80*2)*csr_y]=' ';	
			textmemptr[(csr_x+1)+(80*2)*csr_y]=attrib;
		}
	}
	else if(c>=' ')
	{
		textmemptr[(csr_x++)+(80*2)*csr_y]=c;	
		textmemptr[(csr_x++)+(80*2)*csr_y]=attrib;
	}

    /* If the cursor has reached the edge of the screen's width, we
    *  insert a new line in there */
    if(csr_x >= 80*2)
    {
        csr_x = 0;
        csr_y++;
    }

    /* Scroll the screen if needed, and finally move the cursor */
    scroll();
    mv_csr();
}

void print(unsigned char* text)
{
	int i = 0;
	while(text[i]!='\0')
		printc(text[i++]);	
}

/* Uses the above routine to output a string... */
void println(unsigned char *text)
{
	int i = 0;
	while(text[i]!='\0')
		printc(text[i++]);
	printc('\n');
}	

/* Sets the forecolor and backcolor that we will use */
void setTextColour(unsigned char forecolor, unsigned char backcolor)
{
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    //attrib = (backcolor << 4) | (forecolor & 0x0F);
}

/* Sets our text-mode VGA pointer, then clears the screen for us */
void init_video(void)
{
    textmemptr = (unsigned char *)0xB8000;
    cls();
}


//------------------------------
// Start of keyboard stuff
//------------------------------

//input read(char* input)

void read(unsigned char* read)
{
	reading = TRUE;
	readPtr = 0;
	while(input[readPtr-1]!='\n')
		{;}
	
	memset(read,' ',readPtr);

	int i = 0;
	while(i<readPtr)
	{
		read[i] = input[i];
		i++;
	}

	read[readPtr-1]='\0';
	readPtr =0;
	reading = FALSE;
}

//input print to screen

unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',		/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,					/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

unsigned char kbdusShift[128] =
{
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
  '(', ')', '_', '+', '\b',	/* Backspace */
  '\t',			/* Tab */
  'Q', 'W', 'E', 'R',	/* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',		/* Enter key */
    0,			/* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',	/* 39 */
 '\"', '~',   0,		/* Left shift */
 '|', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */
  'M', '<', '>', '?',   0,					/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

void parse_instruction(unsigned char* pnt)
{
	if(strcmp(pnt,"clear")==0)
	{
		cls();
		printc('>');
	}
	else if(strcmpl(pnt,"echo", 4)==0)
	{
		println(pnt+5);
		printc('>');
	}
	else if(strcmp(pnt,"graphics")==0)
	{
		graphics_init();
	}
	else if(strcmp(pnt,"stime")==0)
	{
		char time[33];
		itoa(stime(),time,10,0);
		println(time);
		printc('>');
	}
	else if(strcmpl(pnt,"dump",4)==0)
	{
		unsigned char* kernel = atoi(pnt+5);
		int i;
		unsigned char nums[33];
		for(i = 0; i<7*80;i++)
		{
			itoa(kernel[i],nums,16,0);
			print(nums);
			printc(' ');
		}
		printc('\n');
		printc('>');
	}
	else
	{
		println("bad command");
	}

	readPtr=0;
}

void keyboard_handler(struct regs *r)
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
        /* Here, a key was just pressed. Please note that if you
        *  hold a key down, you will get repeated key press
        *  interrupts. */

        /* Just to show you how this works, we simply translate
        *  the keyboard scancode into an ASCII value, and then
        *  display it to the screen. You can get creative and
        *  use some flags to see if a shift is pressed and use a
        *  different layout, or you can add another 128 entries
        *  to the above layout to correspond to 'shift' being
        *  held. If shift is held using the larger lookup table,
        *  you would add 128 to the scancode when you look for it */
	if(scancode == SHIFTL)
	{
		shift = !shift;
	}
	if(!shift)
        	printc(kbdus[scancode]);
	else
		printc(kbdusShift[scancode]);

	if(reading)
	{
		input[readPtr++] = kbdus[scancode];
	}
	else
	{
		
		if(scancode == NEWLINE)
		{
			input[readPtr++] = '\0';
			parse_instruction(input);	
		}
		else
		{
			input[readPtr++] = kbdus[scancode];
		}
	}
    }
}

void keyboard_install()
{
	irq_install_handler(1,keyboard_handler);
}
