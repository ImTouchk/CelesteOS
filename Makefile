IMAGE_FILE := bin/celeste.img

.PHONY: all clean setup
setup:
	dd if=/dev/zero of=${IMAGE_FILE} bs=1k count=1440
	mformat -i ${IMAGE_FILE} -f 1440 ::
	mmd     -i ${IMAGE_FILE} ::/EFI
	mmd     -i ${IMAGE_FILE} ::/EFI/BOOT
	mcopy   -i ${IMAGE_FILE} res/zap-light16.psf ::

all:
	cd bootloader && make all
	cd kernel     && make all
	mcopy -o -i ${IMAGE_FILE} bin/BOOTX64.EFI ::/EFI/BOOT
	mcopy -o -i ${IMAGE_FILE} bin/kernel.elf  ::

clean:
	rm -r bootloader/.temp/
	rm -r kernel/.temp/