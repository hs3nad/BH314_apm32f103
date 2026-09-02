.PHONY: build flash flash-stlink erase erase-stlink clean debug

build:
	./scripts/build.sh

flash:
	./scripts/flash.sh

flash-stlink:
	./scripts/flash-stlink.sh

erase:
	./scripts/erase.sh

erase-stlink:
	./scripts/erase-stlink.sh

clean:
	./scripts/clean.sh

debug:
	./scripts/debug-openocd.sh
