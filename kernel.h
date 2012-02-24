#ifndef KERNEL_H
#define KERNEL_H

int clearScreen();
int printGreeting();
int printc(char);
int println(char*);

int cursor;
unsigned char* vram;

#endif
