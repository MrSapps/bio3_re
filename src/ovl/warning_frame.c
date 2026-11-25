#include "ovl/warning.h"

/* red frame with rounded corners behind the message */
void Warn_frame_draw(void)
{
    WARN_POLY Poly;
    LONG Row;
    LONG Quad;
    LONG k;
    LONG Col;
    LONG r;

    Poly.Tag = 0x05000000;
    Poly.Rgb_code = 0x280000FF;
    for (Row = 0; Row < 16; Row++) {
        for (Quad = 0; Quad < 4; Quad++) {
            signed char Sign[4][2] = { { 1, 1 }, { -1, 1 }, { 1, -1 }, { -1, -1 } };

            for (k = 0; k < 4; k++) {
                Col = (k + 2) >> 2;
                r = Row + (k & 1);
                Poly.Xy[k] = ((Sign[Quad][1] * Warn_circle[16 - r][Col] + 0x78) << 16)
                           | (Sign[Quad][0] * Warn_circle[r][Col] + 0xA0);
            }
            DrawPrim(&Poly);
        }
    }
    Poly.Xy[0] = 0x500070;
    Poly.Xy[1] = 0x480078;
    Poly.Xy[2] = 0xA800C8;
    Poly.Xy[3] = 0xA000D0;
    DrawPrim(&Poly);
}
