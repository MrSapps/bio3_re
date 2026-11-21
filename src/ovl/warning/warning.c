/* built with plain GNU as (see Makefile): include asm at top level, without maspsx's markers */
#define INCLUDE_ASM(FOLDER, NAME) \
    __asm__(".text\n.set noat\n.set noreorder\n.include \"" FOLDER "/" #NAME ".s\"\n.set reorder\n.set at\n")
#include "common.h"
#include "ovl/warning.h"

/* pick the check and the message language from the BIOS region */
void func_801840F8(void)
{
    switch (Bios_region) {
    case 'A':
        Warn_state = 1;
        Warn_lang_no = 1;
        break;
    case 'E':
        Warn_state = 0;
        Warn_lang_no = 2;
        break;
    case 'I':
    default:
        Warn_state = 1;
        Warn_lang_no = 0;
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl/warning/warning", func_80184164);

INCLUDE_ASM("asm/nonmatchings/ovl/warning/warning", func_80184A9C);

INCLUDE_ASM("asm/nonmatchings/ovl/warning/warning", func_80184C70);

INCLUDE_ASM("asm/nonmatchings/ovl/warning/warning", func_80184D60);

INCLUDE_ASM("asm/nonmatchings/ovl/warning/warning", func_80184ECC);

INCLUDE_ASM("asm/nonmatchings/ovl/warning/warning", func_80185040);

/* horizontal extent of a 8x15 BIOS font glyph: (leftmost bit << 16) | rightmost bit */
ULONG func_801851F4(UCHAR *pFont)
{
    USHORT Left;
    USHORT Right;
    LONG First;
    LONG Bit;
    LONG Row;

    Left = 0;
    Right = 7;
    for (Row = 14; Row >= 0; Row--) {
        First = 1;
        for (Bit = 7; Bit >= 0; Bit--) {
            if ((*pFont >> Bit) & 1) {
                if (First) {
                    if (Left < Bit)
                        Left = Bit;
                    if (Bit < Right)
                        Right = Bit;
                    First = 0;
                } else if (Bit < Right)
                    Right = Bit;
            }
        }
        pFont++;
    }
    return (Right << 16) | Left;
}

INCLUDE_RODATA("asm/nonmatchings/ovl/warning/warning", D_80184050);
