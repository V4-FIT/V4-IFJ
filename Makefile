SOURCE_DIR		= src
BUILD_DIR		= build

.PHONY: all Release Debug test zip clean
all: Release

Release: $(BUILD_DIR)
	cmake --build $(BUILD_DIR) --config $@
	
Debug: $(BUILD_DIR)
	cmake --build $(BUILD_DIR) --config $@

$(BUILD_DIR): CMakeLists.txt
	cmake -B $@
	
test:
	cd $(BUILD_DIR) && ctest -C Release --output-on-failure

zip:
	cd $(SOURCE_DIR) && zip ../xgysel00.zip *.c *.h

clean:
	rm -rf $(BUILD_DIR)
	rm -rf Testing
