SOURCE_DIR		= src
BUILD_DIR		= build

# pack
PACK_DIR		= pack
ZIPFILE			= xgysel00.zip

.PHONY: all Release Debug test testd zip clean
all: Release

Release: $(BUILD_DIR)
	cmake --build $(BUILD_DIR) --config $@

Debug: $(BUILD_DIR)
	cmake --build $(BUILD_DIR) --config $@

$(BUILD_DIR): CMakeLists.txt
	cmake -B $@

test:
	cd $(BUILD_DIR) && ctest -C Release --output-on-failure

testd:
	cd $(BUILD_DIR) && ctest -C Debug --output-on-failure

zip: $(PACK_DIR) pack/Makefile
	cd $< && zip ../$(ZIPFILE) *.c *.h Makefile

pack/Makefile: Makefile.template $(PACK_DIR)
	cp $< $@
	sed -i "s/TMPLT_SRCS/$(shell find $(PACK_DIR) -type f -name '*.c' -exec basename {} \;)/g" $@
	sed -i "s/TMPLT_INCL/$(shell find $(PACK_DIR) -type f -name '*.h' -exec basename {} \;)/g" $@

$(PACK_DIR):
	mkdir -p $@
	cp $(shell find $(SOURCE_DIR)/ -type f -name '*.c') $@
	cp $(shell find $(SOURCE_DIR)/ -type f -name '*.h') $@

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(PACK_DIR)
	rm -rf Testing
