ROOT_DIR := `pwd`
BUILD_DIR := $(ROOT_DIR)/build
MKSCRIPTS := $(ROOT_DIR)/mkscripts
DATE := $(shell date +%y.%m.%d\ %H:%M:%S)
MKFLAGS := -j8 -s ROOT_DIR=$(ROOT_DIR) BUILD_DIR=$(BUILD_DIR) MKSCRIPTS=$(MKSCRIPTS)

.PHONY: run all clean

run: all
	@mkdir -p "log/$(DATE)"
	@git diff > "log/$(DATE)/git.diff"
	@git status > "log/$(DATE)/git.status"
	@cp --archive --link $(BUILD_DIR) "log/$(DATE)/build"
	@script -q -c "LD_LIBRARY_PATH=$(BUILD_DIR) catchsegv $(BUILD_DIR)/open_visual_studio.elf" "log/$(DATE)/run.log"

all:
	@mkdir -p $(BUILD_DIR)
	$(MAKE) $(MKFLAGS) -C debug         all
	$(MAKE) $(MKFLAGS) -C main          all

clean:
	$(MAKE) $(MKFLAGS) -C debug         clean
	$(MAKE) $(MKFLAGS) -C main          clean
	@rm -rfv $(BUILD_DIR)
