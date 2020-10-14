SOURCE_DIR		= src
BUILD_DIR		= build

.PHONY: all Release Debug zip clean
all: Release

Release: config
	cmake --build $(BUILD_DIR) --config $@
	
Debug: config
	cmake --build $(BUILD_DIR) --config $@

config: CMakeLists.txt
	cmake -B $(BUILD_DIR)

zip:
	cd $(SOURCE_DIR) && zip ../xgysel00.zip *.c *.h

clean:
	rm -rf $(BUILD_DIR)
