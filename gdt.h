#ifndef GDT_H
#define GDT_H
void gdt_set_gate(int, unsigned long, unsigned long, unsigned char, unsigned char);
void gdt_install();
#endif
