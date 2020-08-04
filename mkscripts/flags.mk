INCSDIR += \
	include \
	. \

DEFINES += \
	DEBUG_LEVEL=DEBUG_LEVEL_TRACE \
	DEBUG_SUBLIME_PREFIX \

GCC_COMMON_FLAGS += \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-O2 \
	-g \

ifeq ($(filter %.so,$(TARGET)),$(TARGET))
GCC_COMMON_FLAGS += -fPIC
endif

ASMFLAGS += \
	$(addprefix -I,$(INCSDIR)) \
	$(addprefix -D,$(DEFINES)) \
	$(GCC_COMMON_FLAGS) \

CFLAGS += \
	$(addprefix -I,$(INCSDIR)) \
	$(addprefix -D,$(DEFINES)) \
	$(GCC_COMMON_FLAGS) \
	-std=gnu99 \

CXXFLAGS += \
	$(addprefix -I,$(INCSDIR)) \
	$(addprefix -D,$(DEFINES)) \
	$(GCC_COMMON_FLAGS) \
	-std=gnu++11 \

LDFLAGS += \
	-rdynamic \
	-L$(BUILD_DIR) \
	-O2 \
