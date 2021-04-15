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
	mdel  -i ${IMAGE_FILE} ::/EFI/BOOT/BOOTX64.EFI
	mcopy -i ${IMAGE_FILE} bin/BOOTX64.EFI ::/EFI/BOOT

clean:
	rm bootloader/.temp/
	rm kernel/.temp/