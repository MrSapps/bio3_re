#include "common.h"
#include "ovl/warning.h"

INCLUDE_ASM("asm/nonmatchings/ovl/warning/warning", func_801840F8);

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

INCLUDE_ASM("asm/nonmatchings/ovl/warning/warning", func_80185288);

INCLUDE_ASM("asm/nonmatchings/ovl/warning/warning", func_80185418);
