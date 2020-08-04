AR := ar
AS := gcc
CC := gcc
CXX := g++
LD := g++
RM := rm -fv

WORK_DIR :=`pwd`

ARFLAGS := rc

-include $(MKSCRIPTS)/flags.mk

DEPFILES := $(patsubst %.o,%.d, $(OBJS))

%.o: %.s
	$(AS) $(CFLAGS) -o $@ -c $(WORK_DIR)/$<

%.o: %.S
	$(AS) $(CFLAGS) -D__ASSEMBLY__ -MMD -o $@ -c $(WORK_DIR)/$<

%.o: %.c
	$(CC) $(CFLAGS) -MMD -o $@ -c $(WORK_DIR)/$<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -o $@ -c $(WORK_DIR)/$<

$(BUILD_DIR)/$(TARGET): $(OBJS)
ifeq ($(filter %.a,$(TARGET)),$(TARGET))
	$(AR) $(ARFLAGS) $@ $^
	ranlib $@
else ifeq ($(filter %.so,$(TARGET)),$(TARGET))
	$(LD) $(LDFLAGS) -shared -o $@ $(OBJS) $(LIBS)
else
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)
endif

include $(wildcard $(DEPFILES))

all: $(BUILD_DIR)/$(TARGET)

clean:
	$(RM) $(OBJS) $(DEPFILES) $(BUILD_DIR)/$(TARGET)

.PHONY: all clean $(BUILD_DIR)/$(TARGET)
