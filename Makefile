LIBS= libs/
INTS= interrupts/
 
imgs/floppy.img: kernel.bin	#pads out the binary with bootable sections
	cat  grub/stage1 grub/stage2 grub/pad kernel.bin > imgs/floppy.img

kernel.bin: kernel.o asm/loader.o	#links all the files into the kernel
	 ld -T linker.ld -o kernel.bin asm/loader.o kernel.o $(LIBS)system.o $(LIBS)math.o $(LIBS)io.o $(LIBS)string.o $(LIBS)stdlib.o gdt.o idt.o isr.o irq.o $(LIBS)graphics.o -zmuldefs

kernel.o: io.o string.o system.o math.o stdlib.o gdt.o idt.o isr.o irq.o graphics.o	#compiles the main kernel
	gcc -o kernel.o -c kernel.c -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-builtin -s -Os -O3

#compiles the library functions
graphics.o: irq.o
	gcc -o $(LIBS)graphics.o -c $(LIBS)graphics.c -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-builtin
irq.o: isr.o
	gcc -o irq.o -c irq.c -Wall -Wextra -nostdlib -nodefaultlibs -fno-builtin

isr.o: idt.o
	gcc -o isr.o -c isr.c -Wall -Wextra -nostdlib -nodefaultlibs -fno-builtin
idt.o: gdt.o
	gcc -o idt.o -c idt.c -Wall -Wextra -nostdlib -nodefaultlibs -fno-builtin
gdt.o: stdlib.o
	gcc -o gdt.o -c gdt.c -Wall -Wextra -nostdlib -nodefaultlibs -fno-builtin
stdlib.o: string.o
	gcc -o $(LIBS)stdlib.o -c $(LIBS)stdlib.c -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-builtin
string.o: io.o
	gcc -o $(LIBS)string.o -c $(LIBS)string.c -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-builtin
io.o:	math.o	
	gcc -o $(LIBS)io.o -c $(LIBS)io.c -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-builtin
math.o: system.o
	gcc -o $(LIBS)math.o -c $(LIBS)math.c -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-builtin
system.o: 
	gcc -o $(LIBS)system.o -c $(LIBS)system.c -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-builtin

asm/loader.o:  asm/loader.s
	nasm -f aout -o asm/loader.o asm/loader.s


clean:
	rm -f kernel.o kernel.bin asm/*.o libs/*.o imgs/*.img
