BASENAME := slus_009.23
TARGET   := iso/SLUS_009.23
BUILD    := build

# Cross binutils unpacked locally (apt-get download binutils-mipsel-linux-gnu; dpkg-deb -x ...)
BINUTILS := $(CURDIR)/tools/binutils/usr
export LD_LIBRARY_PATH := $(BINUTILS)/lib/x86_64-linux-gnu
CROSS    := $(BINUTILS)/bin/mipsel-linux-gnu-
AS       := $(CROSS)as
LD       := $(CROSS)ld
OBJCOPY  := $(CROSS)objcopy

PYTHON   := .venv/bin/python
SPLAT    := .venv/bin/splat

# Cap memory of every recipe at 1 GiB
SHELL    := /bin/bash
.SHELLFLAGS := -c 'ulimit -v 1048576; eval "$$1"' --

ASFLAGS  := -EL -march=r3000 -mtune=r3000 -msoft-float -no-pad-sections -G0 -Iinclude
LDFLAGS  := -EL --no-check-sections -nostdlib

S_FILES  := $(shell find asm -name '*.s' 2>/dev/null)
O_FILES  := $(patsubst %.s,$(BUILD)/%.o,$(S_FILES))

.PHONY: all split check clean distclean

all: check

split:
	$(SPLAT) split $(BASENAME).yaml

$(BUILD)/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD)/$(BASENAME).elf: $(O_FILES) $(BASENAME).ld undefined_funcs_auto.txt undefined_syms_auto.txt
	$(LD) $(LDFLAGS) -T $(BASENAME).ld -T undefined_funcs_auto.txt -T undefined_syms_auto.txt \
		-Map $(BUILD)/$(BASENAME).map -o $@

$(BUILD)/$(BASENAME): $(BUILD)/$(BASENAME).elf
	$(OBJCOPY) -O binary $< $@

check: $(BUILD)/$(BASENAME)
	@sha1sum -c $(BASENAME).sha1

clean:
	rm -rf $(BUILD)

distclean: clean
	rm -rf asm assets $(BASENAME).ld undefined_*_auto.txt
