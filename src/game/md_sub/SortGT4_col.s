/* hand-written R3000 assembly (not compiler output; see the note at its INCLUDE_ASM in md_sub.c) */
/* Handwritten function */

glabel SortGT4_col
    addiu      $sp, $sp, -0x1C
    addu       $v0, $a0, $zero
    addu       $t0, $a1, $zero
    addu       $t9, $a2, $zero
    sw         $s2, 0x10($sp)
    lui        $s2, (0xFFFFFF >> 16)
    sw         $s1, 0xC($sp)
    lw         $v1, 0x0($v0)
    ori        $s2, $s2, (0xFFFFFF & 0xFFFF)
    lhu        $t4, 0x16($v1)
    sw         $s0, 0x8($sp)
    sw         $s3, 0x14($sp)
    sw         $s4, 0x18($sp)
    beqz       $t4, .L8002DFC0
     lui       $t8, (0xFF0000 >> 16)
    lw         $t1, 0x10($v1)
    lw         $s1, 0x8($v0)
    lw         $t3, 0xC($v0)
    lw         $a0, 0x8($t1)
    lw         $v1, 0xC($t1)
    and        $v0, $a0, $t8
    srl        $t7, $v0, 16
    sra        $t6, $a0, 24
    and        $v0, $v1, $t8
    srl        $t5, $v0, 16
    sra        $s0, $v1, 24
  .L8002DE74:
    sll        $a0, $t7, 2
    addu       $a0, $a0, $t9
    sll        $v1, $t6, 2
    addu       $v1, $v1, $t9
    sll        $v0, $t5, 2
    addu       $v0, $v0, $t9
    lw         $a0, 0x0($a0)
    lw         $v1, 0x0($v1)
    lw         $v0, 0x0($v0)
    mtc2       $a0, $12 /* handwritten instruction */
    mtc2       $v0, $14 /* handwritten instruction */
    mtc2       $v1, $13 /* handwritten instruction */
    addiu      $t1, $t1, 0x10
    addiu      $t4, $t4, -0x1
    nclip
    sll        $a2, $t7, 1
    sll        $a1, $t6, 1
    sll        $a0, $t5, 1
    addu       $t2, $s0, $zero
    addu       $a1, $a3, $a1
    lw         $v1, 0x8($t1)
    addu       $a0, $a3, $a0
    and        $v0, $v1, $t8
    sra        $t6, $v1, 24
    lw         $v1, 0xC($t1)
    srl        $t7, $v0, 16
    and        $v0, $v1, $t8
    srl        $t5, $v0, 16
    sra        $s0, $v1, 24
    swc2       $24, 0x0($sp)
    lw         $v0, 0x0($sp)
    addu       $v1, $a3, $a2
    blez       $v0, .L8002DFB8
     nop
    lh         $v1, 0x0($v1)
    lh         $v0, 0x0($a1)
    lh         $a0, 0x0($a0)
    addu       $v1, $v1, $v0
    addu       $v1, $v1, $a0
    sll        $a0, $t2, 1
    addu       $a0, $a0, $a3
    sll        $v0, $t2, 2
    addu       $v0, $v0, $t9
    lh         $a0, 0x0($a0)
    lw         $v0, 0x0($v0)
    addu       $v1, $v1, $a0
    sw         $v1, 0x0($sp)
    sw         $v0, 0x2C($t0)
    lw         $v0, 0x0($t3)
    lw         $v1, 0x4($t3)
    lw         $s3, 0x8($t3)
    lw         $s4, 0xC($t3)
    sw         $v0, 0x4($t0)
    sw         $v1, 0x10($t0)
    sw         $s3, 0x1C($t0)
    sw         $s4, 0x28($t0)
    lw         $v0, -0x10($t1)
    lw         $v1, -0xC($t1)
    lw         $s3, -0x8($t1)
    lw         $s4, -0x4($t1)
    sw         $v0, 0xC($t0)
    sw         $v1, 0x18($t0)
    sw         $s3, 0x24($t0)
    sw         $s4, 0x30($t0)
    lw         $v0, 0x0($sp)
    swc2       $12, 0x8($t0)
    swc2       $13, 0x14($t0)
    swc2       $14, 0x20($t0)
    sra        $v0, $v0, 7
    sll        $v0, $v0, 2
    addu       $v0, $s1, $v0
    lw         $v1, 0x0($v0)
    nop
    sw         $v1, 0x0($sp)
    and        $v1, $t0, $s2
    sw         $v1, 0x0($v0)
    lw         $v0, 0x0($sp)
    lui        $v1, (0xC000000 >> 16)
    or         $v0, $v0, $v1
    sw         $v0, 0x0($t0)
    addiu      $t0, $t0, 0x68
  .L8002DFB8:
    bnez       $t4, .L8002DE74
     addiu     $t3, $t3, 0x10
  .L8002DFC0:
    addu       $v0, $t0, $zero
    lw         $s4, 0x18($sp)
    lw         $s3, 0x14($sp)
    lw         $s2, 0x10($sp)
    lw         $s1, 0xC($sp)
    lw         $s0, 0x8($sp)
    jr         $ra
     addiu     $sp, $sp, 0x1C
endlabel SortGT4_col
