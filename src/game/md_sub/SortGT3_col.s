/* hand-written R3000 assembly (not compiler output; see the note at its INCLUDE_ASM in md_sub.c) */
/* Handwritten function */

glabel SortGT3_col
    addiu      $sp, $sp, -0x10
    addu       $t7, $a0, $zero
    addu       $t0, $a1, $zero
    lui        $t8, (0xFFFFFF >> 16)
    lw         $v0, 0x0($t7)
    ori        $t8, $t8, (0xFFFFFF & 0xFFFF)
    lhu        $t3, 0x14($v0)
    sw         $s0, 0x8($sp)
    beqz       $t3, .L8002DDFC
     lui       $s0, (0xFF0000 >> 16)
    lw         $t1, 0xC($v0)
    lw         $t9, 0x8($t7)
    lw         $t2, 0xC($t7)
    lw         $v1, 0x8($t1)
    lb         $t6, 0x7($t1)
    and        $v0, $v1, $s0
    srl        $t4, $v0, 16
    sra        $t5, $v1, 24
  .L8002DCE0:
    sll        $a0, $t6, 2
    addu       $a0, $a0, $a2
    sll        $v1, $t4, 2
    addu       $v1, $v1, $a2
    sll        $v0, $t5, 2
    addu       $v0, $v0, $a2
    lw         $a0, 0x0($a0)
    lw         $v1, 0x0($v1)
    lw         $v0, 0x0($v0)
    mtc2       $a0, $12 /* handwritten instruction */
    mtc2       $v0, $14 /* handwritten instruction */
    mtc2       $v1, $13 /* handwritten instruction */
    addiu      $t1, $t1, 0xC
    addiu      $t3, $t3, -0x1
    nclip
    sll        $a1, $t6, 1
    sll        $a0, $t4, 1
    addu       $a1, $a3, $a1
    addu       $a0, $a3, $a0
    lw         $v1, 0x8($t1)
    lb         $t6, 0x7($t1)
    and        $v0, $v1, $s0
    srl        $t4, $v0, 16
    sll        $v0, $t5, 1
    sra        $t5, $v1, 24
    addu       $v1, $a3, $v0
    swc2       $24, 0x0($sp)
    lw         $v0, 0x0($sp)
    lhu        $a1, 0x0($a1)
    blez       $v0, .L8002DDF0
     nop
    lhu        $a0, 0x0($a0)
    lhu        $v1, 0x0($v1)
    mtc2       $a1, $17 /* handwritten instruction */
    mtc2       $a0, $18 /* handwritten instruction */
    mtc2       $v1, $19 /* handwritten instruction */
    lw         $v0, -0xC($t1)
    lw         $v1, 0x0($t2)
    avsz3
    sw         $v0, 0xC($t0)
    sw         $v1, 0x4($t0)
    lw         $v0, 0x4($t2)
    lw         $v1, 0x8($t2)
    sw         $v0, 0x10($t0)
    sw         $v1, 0x1C($t0)
    lw         $v0, -0x8($t1)
    lw         $v1, -0x4($t1)
    and        $v0, $v0, $t8
    sw         $v0, 0x18($t0)
    sw         $v1, 0x24($t0)
    swc2       $7, 0x0($sp)
    swc2       $12, 0x8($t0)
    lw         $v0, 0x0($sp)
    swc2       $13, 0x14($t0)
    swc2       $14, 0x20($t0)
    sra        $v0, $v0, 3
    sll        $v0, $v0, 2
    addu       $v0, $t9, $v0
    lw         $v1, 0x0($v0)
    nop
    sw         $v1, 0x0($sp)
    and        $v1, $t0, $t8
    sw         $v1, 0x0($v0)
    lw         $v0, 0x0($sp)
    lui        $v1, (0x9000000 >> 16)
    or         $v0, $v0, $v1
    sw         $v0, 0x0($t0)
    addiu      $t0, $t0, 0x68
  .L8002DDF0:
    bnez       $t3, .L8002DCE0
     addiu     $t2, $t2, 0xC
    sw         $t2, 0xC($t7)
  .L8002DDFC:
    addu       $v0, $t0, $zero
    lw         $s0, 0x8($sp)
    jr         $ra
     addiu     $sp, $sp, 0x10
endlabel SortGT3_col
