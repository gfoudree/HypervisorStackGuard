all:
	cd llvm_pass/build; rm -rf *; cmake ..; make; cd -

	i686-elf-as boot.s -o boot.o
	i686-elf-gcc -c kernel.c -o kernel_plain.o -std=gnu99 -ffreestanding -O0 -Wall -Wextra
	clang -Xclang -load -Xclang llvm_pass/build/bb_insert_callback/libBBInsertCallback.so -c kernel.c -o kernel.o -m32 -ffreestanding
	i686-elf-gcc -T linker.ld -o kernel.bin -ffreestanding -O0 -nostdlib boot.o kernel.o -lgcc
	i686-elf-gcc -T linker.ld -o kernel_plain.bin -ffreestanding -O0 -nostdlib boot.o kernel_plain.o -lgcc

clean:
	rm -f *.o
	rm -f *.bin

