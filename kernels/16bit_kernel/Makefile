all: kernel.asm
	nasm kernel.asm -f bin -o kernel.bin

run:
	qemu-system-i386 -enable-kvm -fda kernel.bin -nographic

clean:
	rm -v kernel.bin
