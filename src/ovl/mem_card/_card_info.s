/* PsyQ libcard BIOS A0 jump stub (hand-written in the library) */

glabel _card_info
    addiu      $t2, $zero, 0xA0
    jr         $t2
     addiu     $t1, $zero, 0xAB
endlabel _card_info
    nop
