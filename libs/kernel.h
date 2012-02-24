#ifndef KERNEL_H
#define KERNEL_H

#define FALSE 0
#define TRUE 1

#define HEAP 0xD0000
#define HEAPHW 0xD000000
#define HEAPMAGIC 0xBABEBEEF

void* currentHeapVal = HEAP;

//kernel method forward declarations
typedef int size_t;

/* This defines what the stack looks like after an ISR was running */
struct regs
{
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;    
};

typedef struct registers
{
    unsigned int ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
}registers_t;


typedef struct multiboot_memory_map {
	unsigned int size;
	unsigned int base_addr_low,base_addr_high;
// You can also use: unsigned long long int base_addr; if supported.
	unsigned int length_low,length_high;
// You can also use: unsigned long long int length; if supported.
	unsigned int type;
} multiboot_memory_map_t;

/* MAIN.C */
extern void *memcpy(void *dest, const void *src, size_t count);
extern void *memset(void *dest, char val, size_t count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
extern size_t strlen(const char *str);

/* CONSOLE.C */
extern void init_video(void);
extern void println(unsigned char *text);
extern void print(unsigned char *text);
extern void printc(unsigned char c);
extern void cls();

/* GDT.C */
extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
extern void gdt_install();

/* IDT.C */
extern void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
extern void idt_install();

/* ISRS.C */
extern void isr_install();

/* IRQ.C */
extern void irq_install_handler(int irq, void (*handler)(struct regs *r));
extern void irq_uninstall_handler(int irq);
extern void irq_install();

/* TIMER.C */
extern void wait(int ticks);
extern void timer_install();

/* KEYBOARD.C */
extern void keyboard_install();

/*Memory Managment*/
extern void* malloc(unsigned size);
extern void free(void*);

int printGreeting();

#endif
