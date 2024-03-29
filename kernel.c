#include "libs/kernel.h"
#include "libs/stdlib.h"
#include "libs/io.h"
#include "libs/math.h"
//int cursor = 0;	//this is the current screen position of the cursor ie: where stuff gets written
//unsigned char *videoram;	//global vram :D

void kmain(void* mbd, unsigned int magic )
{
   if ( magic != 0x2BADB002 )
   {
      /* Something went not according to specs. Print an error */
      /* message and halt, but do *not* rely on the multiboot */
      /* data structure. */

	println("Error on load");
   }
 
   /* You could either use multiboot.h */
   /* (http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh) */
   /* or do your offsets yourself. The following is merely an example. */ 
	//asm volatile ("cli"); 
	gdt_install();
	idt_install();

	char * boot_loader_name =(char*) ((long*)mbd)[16];
	init_video();

	isr_install();
	irq_install();

	asm volatile ("sti"); 
	timer_install();
	keyboard_install();

	println(boot_loader_name);
	printGreeting();

	//printc(22/0);
	//asm volatile("int $0x3");
	for(;;)
		;	//empty loop
}

void* malloc(unsigned size)
{
	void* retVal = currentHeapVal;
	currentHeapVal+=size;
	return retVal;
}




int printGreeting()
{
	println("Welcome to brOS");	//print a string
	printc('>');			//a prompt :)
	return 0;
}
