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
.SHELLFLAGS := -o pipefail -c 'ulimit -v 1048576; eval "$$1"' --

ASFLAGS  := -EL -march=r3000 -mtune=r3000 -msoft-float -no-pad-sections -G0 -Iinclude
LDFLAGS  := -EL --no-check-sections -nostdlib

# ASPSX pads every object's .rdata to a word; the ld script packs objects with SUBALIGN(2), so
# append that padding (RODATA_PAD). Jump tables stay 8-aligned relative to their own object only.
RODATA_PAD := printf '\t.section .rodata\n\t.align 2\n'
# C pipeline: cpp | cc1 (PsyQ 4.4 cc1psx = GCC 2.8.1 SN32, ASPSX 2.77) | maspsx | GNU as
COMPILER ?= gcc-2.8.1-psx
CC1      := tools/compilers/$(COMPILER)/cc1
PSYQ_INC ?= /home/snake/dev/psyq_sdk/psyq_4.4/include
CPPFLAGS := -P -undef -D__GNUC__=2 -Dmips -D__psx__ -D_LANGUAGE_C -Iinclude -Isrc -I$(PSYQ_INC)
CFLAGS   := -quiet -O2 -G0 -mgas -msoft-float -fno-builtin -funsigned-char
MASPSX   := $(PYTHON) tools/maspsx/maspsx.py --aspsx-version=2.77 -G0

S_FILES  := $(wildcard asm/*.s asm/data/*.s)
C_FILES  := $(shell find src -name '*.c' 2>/dev/null)
O_FILES  := $(patsubst %.s,$(BUILD)/%.s.o,$(S_FILES)) $(patsubst %.c,$(BUILD)/%.c.o,$(C_FILES))

.DELETE_ON_ERROR:

.PHONY: all split check clean distclean progress

all: check

split:
	$(SPLAT) split $(BASENAME).yaml

$(BUILD)/%.s.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD)/%.c.o: %.c
	@mkdir -p $(dir $@)
	cpp $(CPPFLAGS) -MMD -MF $@.d -MT $@ $< | $(CC1) $(CFLAGS) -o - | $(MASPSX) | { cat; $(RODATA_PAD); } | $(AS) $(ASFLAGS) -o $@ -

-include $(shell find $(BUILD) -name '*.d' 2>/dev/null)

# names introduced while matching (syms/*.txt); PROVIDE so C definitions win
SYMS_LD  := $(BUILD)/syms.ld
$(SYMS_LD): $(wildcard syms/*.txt)
	@mkdir -p $(dir $@)
	sed -E 's://.*$$::; s/^\s*([A-Za-z_][A-Za-z0-9_.]*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;/PROVIDE(\1 = \2);/' $^ > $@

$(BUILD)/$(BASENAME).elf: $(O_FILES) $(BASENAME).ld undefined_funcs_auto.txt undefined_syms_auto.txt $(SYMS_LD)
	$(LD) $(LDFLAGS) -T $(BASENAME).ld -T undefined_funcs_auto.txt -T undefined_syms_auto.txt -T $(SYMS_LD) \
		-Map $(BUILD)/$(BASENAME).map -o $@

$(BUILD)/$(BASENAME): $(BUILD)/$(BASENAME).elf
	$(OBJCOPY) -O binary $< $@

progress:
	@$(PYTHON) tools/progress.py

check: $(BUILD)/$(BASENAME)
	@sha1sum -c $(BASENAME).sha1

clean:
	rm -rf $(BUILD)

distclean: clean
	rm -rf asm assets $(BASENAME).ld undefined_*_auto.txt
