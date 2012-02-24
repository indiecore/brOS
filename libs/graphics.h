#ifndef __GRAPHICSH
#define __GRAPHICSH

#define NEWLINE 28
#define SHIFTL 42

//colours
#define BLACK 0x00
#define BLUE 0x01
#define GREEN 0x02
#define CYAN 0x03
#define RED 0x04
#define MAGENTA 0x05
#define BROWN 0x06
#define LGREY 0x07
#define GREY 0x08
#define LBLUE 0x09
#define LGREEN 0x0A
#define LCYAN 0x0B
#define LRED 0x0C
#define LMAGENTA 0x0D
#define YELLOW 0x0E
#define WHITE 0x0F

typedef unsigned char u8;
#define height 50
#define width 80

unsigned char* screenPtr;

void graphics_init();
void exit_graphics();

void blank();
void putPixel(unsigned char, unsigned, unsigned);
void drawLine(unsigned, unsigned, unsigned,unsigned, unsigned char);
void drawRect(unsigned, unsigned, unsigned, unsigned, unsigned char);
void timeThing();
void hurr();
void durr();
void graphics_keyboard_handler();


#endif
