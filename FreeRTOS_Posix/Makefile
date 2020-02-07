OUTDIR := build
SRCDIRS := AsyncIO Common_Demo/Full Common_Demo/Minimal FileIO ParTest portable/GCC/Posix FreeRTOS-Kernel
INCDIRS := . $(wildcard */include) portable/GCC/Posix
EXE := $(OUTDIR)/runsim
SRCS := main.c $(wildcard $(SRCDIRS:%=%/*.c)) FreeRTOS-Kernel/portable/MemMang/heap_3.c

CC := gcc
LDFLAGS := -pthread -lrt
CFLAGS := -Wall -DUSE_STDIO=1 -D__GCC_POSIX__=1
ifeq (,$(filter release, $(MAKECMDGOALS)))
  # default to debugging and no optimization
  CFLAGS += -O0 -ggdb
else
  # for the release target, use optimization
  CFLAGS += -O2
endif
DEPFLAGS = -MM -MT $@ -MP
CPPFLAGS := $(INCDIRS:%=-I%)

OBJS := $(SRCS:%.c=$(OUTDIR)/%.o)
DEPS := $(OBJS:.o=.d)

# use recursive calls to ensure all dependencies are known for the build
all:
	$(MAKE) depend
	$(MAKE) build

# read dependency files if they exist
ifeq (,$(filter distclean, $(MAKECMDGOALS)))
  include $(wildcard $(DEPS))
endif

depend: $(DEPS)

build: $(EXE)

clean:
	-find $(OUTDIR) -name \*.o -delete
	rm -f $(EXE)

distclean:
	rm -fr $(OUTDIR) $(EXE)

release: clean build

.PHONY: all build clean depend distclean release

$(DEPS): $(OUTDIR)/%.d : %.c
	mkdir -p $(dir $@)
	$(CC)  $(CPPFLAGS) $(DEPFLAGS) $(OUTPUT_OPTION) $<

$(OBJS): $(OUTDIR)/%.o : %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(OUTPUT_OPTION) $<

$(EXE): $(OBJS)
	$(LINK.c) $^ -o $@
