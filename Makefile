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
C_FILES  := $(shell find src/game -name '*.c' 2>/dev/null)
O_FILES  := $(patsubst %.s,$(BUILD)/%.s.o,$(S_FILES)) $(patsubst %.c,$(BUILD)/%.c.o,$(C_FILES))

.DELETE_ON_ERROR:

.PHONY: all split split-ovl check check-ovl clean distclean progress

all: check check-ovl

split:
	$(SPLAT) split $(BASENAME).yaml

$(BUILD)/%.s.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

# WARNING.BIN: the mod-chip check (warning.c, frame.c; not Capcom code) was built with gcc 2.7.2 and plain GNU as, not
# ASPSX: macro lw/sw/lbu sym($x) via $at or the destination reg, delay slots filled by gas, sections padded
# to 16 bytes, move = addu. Capcom's task (task.c) uses the normal pipeline. Keep in sync with tools/fdiff.py.
# The cc1 is decompals' gcc-2.7.2-cdk (cygnus-2.7.2-970404) with two later cygnus changes switched off (NOTES.md):
# reload_cse_simplify_set returns 0 (no `move $a2,$a1` for a second 0), assign_parms adds no REG_EQUIV for
# register parameters (their live length is not doubled). -fpic keeps `lw $3,sym` unsplit (no -msplit-addresses).
WARNING_CC1 := tools/compilers/gcc-2.7.2-cdk-warning/cc1
$(WARNING_CC1): tools/compilers/gcc-2.7.2-cdk/cc1
	echo "4986a1686556aff4081ad082205b1df930e0b0f6  $<" | sha1sum -c --quiet
	@mkdir -p $(dir $@)
	cp $< $@.tmp
	printf '\061\300\303' | dd of=$@.tmp bs=1 seek=$$((0x1389d2)) conv=notrunc status=none
	printf '\220\351' | dd of=$@.tmp bs=1 seek=$$((0x4e77b)) conv=notrunc status=none
	echo "215c4f4cb79647896307ddcb087d6e42cbe33bb3  $@.tmp" | sha1sum -c --quiet
	mv $@.tmp $@

WARNING_GAS_OBJS := $(BUILD)/src/ovl/warning/warning.c.o $(BUILD)/src/ovl/warning/frame.c.o
$(WARNING_GAS_OBJS): $(BUILD)/%.c.o: %.c $(WARNING_CC1)
	@mkdir -p $(dir $@)
	cpp $(CPPFLAGS) -MMD -MF $@.d -MT $@ $< | $(WARNING_CC1) $(CFLAGS) -fpic -o - \
		| sed -E 's/^(\s*)move\t([^,]+),(.*)$$/\1addu\t\2,\3,$$0/' \
		| $(AS) $(filter-out -no-pad-sections,$(ASFLAGS)) -o $@ -

$(BUILD)/%.c.o: %.c
	@mkdir -p $(dir $@)
	cpp $(CPPFLAGS) -MMD -MF $@.d -MT $@ $< | $(CC1) $(CFLAGS) -o -| $(MASPSX) | { cat; $(RODATA_PAD); } | $(AS) $(ASFLAGS) -o $@ -

-include $(shell find $(BUILD) -name '*.d' 2>/dev/null)

# names introduced while matching (syms/*.txt); PROVIDE so C definitions win. The overlay links use it too,
# so exe names that are only in syms/*.txt (not yet in symbol_addrs.txt / the splits) resolve in overlay C.
SYMS_LD  := $(BUILD)/syms.ld
$(SYMS_LD): $(wildcard syms/*.txt)
	@mkdir -p $(dir $@)
	sed -E 's://.*$$::; s/^\s*([A-Za-z_][A-Za-z0-9_.]*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;/PROVIDE(\1 = \2);/' $^ > $@

$(BUILD)/$(BASENAME).elf: $(O_FILES) $(BASENAME).ld undefined_funcs_auto.txt undefined_syms_auto.txt $(SYMS_LD)
	$(LD) $(LDFLAGS) -T $(BASENAME).ld -T undefined_funcs_auto.txt -T undefined_syms_auto.txt -T $(SYMS_LD) \
		-Map $(BUILD)/$(BASENAME).map -o $@

$(BUILD)/$(BASENAME): $(BUILD)/$(BASENAME).elf
	$(OBJCOPY) -O binary $< $@

# Code overlays (CD_DATA/BIN/*.BIN), one splat config each: ovl/<name>.yaml (tools/mkovl.py)
OVLS     := $(basename $(notdir $(wildcard ovl/*.yaml)))
OVL_BINS := $(foreach o,$(OVLS),$(BUILD)/ovl/$(o).bin)

$(BUILD)/assets/%.bin.o: assets/%.bin
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary -O elf32-tradlittlemips --rename-section .data=.data,alloc,load,data,contents $< $@

define OVL_RULES
$(BUILD)/ovl/$(1).elf: $$(patsubst %.s,$(BUILD)/%.s.o,$$(wildcard asm/data/ovl/$(1)/*.s)) \
		$$(patsubst %.bin,$(BUILD)/%.bin.o,$$(wildcard assets/ovl/$(1)/*.bin)) \
		$$(patsubst %.c,$(BUILD)/%.c.o,$$(wildcard src/ovl/$(1)/*.c)) ovl/$(1).ld $(SYMS_LD)
	@mkdir -p $(BUILD)/ovl
	$$(LD) $$(LDFLAGS) -T ovl/$(1).ld -T ovl/$(1).undefined_funcs_auto.txt -T ovl/$(1).undefined_syms_auto.txt \
		$$(addprefix -T ,$$(wildcard ovl/$(1).bss.ld)) -T $(SYMS_LD) \
		-Map $(BUILD)/ovl/$(1).map -o $$@
endef
$(foreach o,$(OVLS),$(eval $(call OVL_RULES,$(o))))

# Enemy modules are linked for two load slots: split for slot 1 (0x80100000), relinked for slot 2.
EMS      := $(filter em%,$(OVLS))
OVL_BINS += $(foreach o,$(EMS),$(BUILD)/ovl/$(o)_2.bin)
define EM_SLOT2
$(BUILD)/ovl/$(1)_2.ld: ovl/$(1).ld
	@mkdir -p $(BUILD)/ovl
	sed 's/0x80100000/0x8010D000/; s|build/asm/data/ovl/$(1)/header.rodata.s.o|$(BUILD)/ovl/$(1)_2.header.s.o|' $$< > $$@
$(BUILD)/ovl/$(1)_2.header.s.o: ovl/$(1)_2.header.s
	$$(AS) $$(ASFLAGS) -o $$@ $$<
$(BUILD)/ovl/$(1)_2.bin: $(BUILD)/ovl/$(1)_2.elf $$(wildcard ovl/$(1)_2.fixup)
	$$(OBJCOPY) -O binary $$< $$@
	$$(if $$(wildcard ovl/$(1)_2.fixup),$$(PYTHON) tools/fixup.py $$@ ovl/$(1)_2.fixup)
$(BUILD)/ovl/$(1)_2.elf: $(BUILD)/ovl/$(1).elf $(BUILD)/ovl/$(1)_2.ld $(BUILD)/ovl/$(1)_2.header.s.o $(SYMS_LD)
	$$(LD) $$(LDFLAGS) -T $(BUILD)/ovl/$(1)_2.ld -T ovl/$(1).undefined_funcs_auto.txt \
		-T ovl/$(1).undefined_syms_auto.txt -T ovl/$(1).bss.ld -T $(SYMS_LD) -Map $(BUILD)/ovl/$(1)_2.map -o $$@
endef
$(foreach o,$(EMS),$(eval $(call EM_SLOT2,$(o))))

$(BUILD)/ovl/%.bin: $(BUILD)/ovl/%.elf
	$(OBJCOPY) -O binary $< $@

$(BUILD)/asm/data/ovl/%.s.o: asm/data/ovl/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

split-ovl:
	for y in ovl/*.yaml; do $(SPLAT) split $$y > /dev/null || exit 1; done
	for o in $(EMS); do $(PYTHON) tools/mkovl.py --bss $$o || exit 1; done

check-ovl: $(OVL_BINS)
	@cd $(BUILD)/ovl && sha1sum -c ../../ovl/overlays.sha1

progress:
	@$(PYTHON) tools/progress.py

check: $(BUILD)/$(BASENAME)
	@sha1sum -c $(BASENAME).sha1

clean:
	rm -rf $(BUILD)

distclean: clean
	rm -rf asm assets $(BASENAME).ld undefined_*_auto.txt
