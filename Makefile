IMAGE_FILE := bin/celeste.img

.PHONY: all clean setup run
setup:
	mkdir bin
	dd if=/dev/zero of=${IMAGE_FILE} bs=1k count=1440
	mformat -i ${IMAGE_FILE} -f 1440 ::
	mmd     -i ${IMAGE_FILE} ::/EFI
	mmd     -i ${IMAGE_FILE} ::/EFI/BOOT
	mcopy   -i ${IMAGE_FILE} res/zap-light16.psf ::

all:
	@echo "==================== BOOTLOADER ===================="
	cd bootloader && make all
	@echo "==================== KERNEL ===================="
	cd kernel     && make all

clean:
	rm -r bootloader/.temp/
	rm -r kernel/.temp/
run:
	qemu-system-x86_64 -pflash res/OVMF.fd -cdrom bin/celeste.img -s
