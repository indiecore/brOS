#ifndef SYSTEM_H
#define SYSTEM_H
unsigned char inportb (unsigned short);
void outportb (unsigned short , unsigned char);

typedef struct multiboot_memory_map {
	unsigned int size;
	unsigned int base_addr_low,base_addr_high;
// You can also use: unsigned long long int base_addr; if supported.
	unsigned int length_low,length_high;
// You can also use: unsigned long long int length; if supported.
	unsigned int type;
} multiboot_memory_map_t;

static __inline__ void io_wait(void)
{
   asm volatile("outb %%al, $0x80" : : "a"(0));
   // port 0x80 is used for 'checkpoints' during POST.
   // linux kernel seems to think it's free for use :-/
};
#endif
