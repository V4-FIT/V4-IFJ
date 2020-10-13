TARGET			= ic20int
SOURCE_DIR		= src
BUILD_DIR		= cmake-build-release

.PHONY: all $(TARGET) zip clean
all: $(TARGET)

$(TARGET): $(BUILD_DIR)
	$(MAKE) -C $<
	cp $</$@ ./

$(BUILD_DIR): CMakeLists.txt
	mkdir -p $@
	cd $@ && cmake -D CMAKE_BUILD_TYPE=Release ..

zip:
	cd $(SOURCE_DIR) && zip ../xgysel00.zip *.c *.h

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(TARGET)
