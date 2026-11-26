/*
 * MEM_CARD.BIN: libcard CARD module (_card_clear), linked after the BIOS stubs at the end of the overlay.
 * Built with gcc 2.7.2 (Makefile): `addiu sp` before `jr ra` with a nop in the delay slot; gcc 2.8 puts the
 * sp restore in the slot (as PsyQ 4.3-4.5's own CARD.OBJ does, which is also padded to 0x40: not this copy).
 */
#include <sys/types.h>
#include <libapi.h>

long _card_clear(long chan)
{
    _new_card();
    return _card_write(chan, 0x3F, NULL);
}
