/* hand-written R3000 assembly (not compiler output; see the note at its INCLUDE_ASM in md_sub.c) */
/* Handwritten function */

glabel SortGT4_sabun
    addiu      $sp, $sp, -0x1C
    addu       $t0, $a1, $zero
    lw         $v0, 0x0($a0)
    addu       $t8, $a2, $zero
    lhu        $t3, 0x16($v0)
    sw         $s0, 0x8($sp)
    sw         $s1, 0x10($sp)
    sw         $s2, 0x14($sp)
    sw         $s3, 0x18($sp)
    beqz       $t3, .L8002E2FC
     addu      $t9, $a3, $zero
    lw         $t1, 0x10($v0)
    lw         $s0, 0x8($a0)
    lw         $t2, 0xC($a0)
    lui        $v1, (0xFF0000 >> 16)
  .L8002E19C:
    lw         $a0, 0x8($t1)
    lw         $v0, 0xC($t1)
    and        $a1, $a0, $v1
    sra        $a1, $a1, 16
    sra        $a0, $a0, 24
    and        $v1, $v0, $v1
    sra        $v1, $v1, 16
    sra        $t4, $v0, 24
    sll        $t5, $a1, 2
    addu       $a3, $t5, $t8
    sll        $t6, $a0, 2
    addu       $a2, $t6, $t8
    sll        $t7, $v1, 2
    addu       $v0, $t7, $t8
    lw         $a3, 0x0($a3)
    lw         $a2, 0x0($a2)
    lw         $v0, 0x0($v0)
    mtc2       $a3, $12 /* handwritten instruction */
    mtc2       $v0, $14 /* handwritten instruction */
    mtc2       $a2, $13 /* handwritten instruction */
    addiu      $t1, $t1, 0x10
    addiu      $t3, $t3, -0x1
    nclip
    sll        $a1, $a1, 1
    addu       $a2, $t9, $a1
    sll        $a0, $a0, 1
    addu       $a1, $t9, $a0
    sll        $v1, $v1, 1
    addu       $a0, $t9, $v1
    lh         $v1, 0x0($a1)
    sll        $a1, $t4, 2
    swc2       $24, 0x0($sp)
    lw         $v0, 0x0($sp)
    lh         $a0, 0x0($a0)
    lh         $s1, 0x0($a2)
    blez       $v0, .L8002E2F4
     addu      $v0, $s1, $v1
    addu       $v0, $v0, $a0
    sll        $v1, $t4, 1
    addu       $v1, $v1, $t9
    addu       $a0, $a1, $t8
    lh         $v1, 0x0($v1)
    lw         $a0, 0x0($a0)
    addu       $v0, $v0, $v1
    sw         $v0, 0x0($sp)
    sw         $a0, 0x2C($t0)
    addu       $v0, $t5, $t2
    addu       $s1, $t6, $t2
    addu       $s2, $t7, $t2
    addu       $a1, $a1, $t2
    lw         $v0, 0x0($v0)
    lw         $s1, 0x0($s1)
    lw         $s2, 0x0($s2)
    lw         $s3, 0x0($a1)
    lui        $v1, (0x3C000000 >> 16)
    or         $v0, $v0, $v1
    sw         $v0, 0x4($t0)
    sw         $s1, 0x10($t0)
    sw         $s2, 0x1C($t0)
    sw         $s3, 0x28($t0)
    lw         $v0, -0x10($t1)
    lw         $s1, -0xC($t1)
    lw         $s2, -0x8($t1)
    lw         $s3, -0x4($t1)
    sw         $v0, 0xC($t0)
    sw         $s1, 0x18($t0)
    sw         $s2, 0x24($t0)
    sw         $s3, 0x30($t0)
    swc2       $12, 0x8($t0)
    swc2       $13, 0x14($t0)
    swc2       $14, 0x20($t0)
    lui        $v1, (0xFFFFFF >> 16)
    lw         $v0, 0x0($sp)
    ori        $v1, $v1, (0xFFFFFF & 0xFFFF)
    sra        $v0, $v0, 7
    sll        $v0, $v0, 2
    addu       $v0, $s0, $v0
    lw         $a0, 0x0($v0)
    and        $v1, $t0, $v1
    sw         $a0, 0x0($sp)
    sw         $v1, 0x0($v0)
    lw         $v0, 0x0($sp)
    lui        $v1, (0xC000000 >> 16)
    or         $v0, $v0, $v1
    sw         $v0, 0x0($t0)
    addiu      $t0, $t0, 0x68
  .L8002E2F4:
    bnez       $t3, .L8002E19C
     lui       $v1, (0xFF0000 >> 16)
  .L8002E2FC:
    addu       $v0, $t0, $zero
    lw         $s0, 0x8($sp)
    lw         $s1, 0x10($sp)
    lw         $s2, 0x14($sp)
    lw         $s3, 0x18($sp)
    jr         $ra
     addiu     $sp, $sp, 0x1C
endlabel SortGT4_sabun
