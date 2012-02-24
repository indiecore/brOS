#ifndef __IOH
#define __IOH
#define NEWLINE 28
#define SHIFTL 42


unsigned char* textmemptr;
unsigned char* input;
unsigned char* readIn;
unsigned int readPtr = 0;


int csr_x = 0, csr_y = 0;	//virtual cursor positions

//printf();	//undefined

void read(unsigned char*);
void parse_instruction(unsigned char*);
void putPixel(unsigned char, unsigned, unsigned);

#endif
