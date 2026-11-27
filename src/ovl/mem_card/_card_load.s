/* PsyQ libcard BIOS A0 jump stub (hand-written in the library) */

glabel _card_load
    addiu      $t2, $zero, 0xA0
    jr         $t2
     addiu     $t1, $zero, 0xAC
endlabel _card_load
    nop
