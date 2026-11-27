/* hand-written R3000 assembly (not compiler output; see the note at its INCLUDE_ASM in md_sub.c) */
/* Handwritten function */

glabel SortGT4_neo_uv
    addiu      $sp, $sp, -0x24
    sw         $s2, 0x10($sp)
    sw         $s1, 0xC($sp)
    sw         $s0, 0x8($sp)
    sw         $s3, 0x18($sp)
    sw         $s4, 0x1C($sp)
    sw         $s5, 0x20($sp)
    lw         $v1, 0x0($a0)
    addu       $t0, $a1, $zero
    lhu        $t6, 0x16($v1)
    addu       $t0, $a1, $zero
    beqz       $t6, .L8002DA88
     addiu     $s2, $zero, 0xFF
    lw         $v0, 0x10($v1)
    lw         $t5, 0x4($v1)
    lw         $s1, 0x8($a0)
    lbu        $t9, 0xE($v0)
    lb         $s0, 0xF($v0)
    addiu      $t2, $v0, 0xC
    addiu      $v0, $a0, 0x4
    lbu        $t7, -0x2($t2)
    lb         $t8, -0x1($t2)
    lwc2       $6, 0x0($v0)
    addu       $a1, $t7, $zero
  .L8002D8F8:
    sll        $a0, $a1, 2
    addu       $a0, $a0, $a2
    addu       $t4, $t8, $zero
    sll        $v1, $t4, 2
    addu       $v1, $v1, $a2
    addu       $t3, $t9, $zero
    sll        $v0, $t3, 2
    addu       $v0, $v0, $a2
    lw         $a0, 0x0($a0)
    lw         $v1, 0x0($v1)
    lw         $v0, 0x0($v0)
    mtc2       $a0, $12 /* handwritten instruction */
    mtc2       $v0, $14 /* handwritten instruction */
    mtc2       $v1, $13 /* handwritten instruction */
    addiu      $t2, $t2, 0x10
    addiu      $t6, $t6, -0x1
    nclip
    addu       $t1, $s0, $zero
    lw         $v1, -0x4($t2)
    lw         $a0, 0x0($t2)
    sra        $v0, $v1, 16
    and        $t7, $v0, $s2
    sra        $t8, $v1, 24
    sra        $v0, $a0, 16
    and        $t9, $v0, $s2
    sra        $s0, $a0, 24
    swc2       $24, 0x0($sp)
    lw         $v1, 0x0($sp)
    sll        $v0, $t1, 3
    blez       $v1, .L8002DA80
     addu      $v0, $t5, $v0
    lwc2       $0, 0x0($v0)
    lwc2       $1, 0x4($v0)
    sll        $v1, $a1, 1
    addu       $v1, $v1, $a3
    nccs
    sll        $a0, $t3, 1
    addu       $a0, $a0, $a3
    sll        $a1, $a1, 3
    addu       $a1, $t5, $a1
    sll        $v0, $t4, 1
    addu       $v0, $v0, $a3
    lh         $v1, 0x0($v1)
    lh         $v0, 0x0($v0)
    lh         $a0, 0x0($a0)
    addu       $v1, $v1, $v0
    sll        $v0, $t1, 1
    addu       $v0, $v0, $a3
    addu       $v1, $v1, $a0
    sll        $a0, $t4, 3
    lh         $v0, 0x0($v0)
    addu       $a0, $t5, $a0
    addu       $v1, $v1, $v0
    sw         $v1, 0x0($sp)
    sll        $v1, $t3, 3
    addu       $v1, $t5, $v1
    addiu      $s5, $t0, 0x28
    swc2       $12, 0x8($t0)
    swc2       $13, 0x14($t0)
    swc2       $14, 0x20($t0)
    swc2       $22, 0x0($s5)
    lwc2       $0, 0x0($a1)
    lwc2       $1, 0x4($a1)
    lwc2       $2, 0x0($a0)
    lwc2       $3, 0x4($a0)
    lwc2       $4, 0x0($v1)
    lwc2       $5, 0x4($v1)
    sll        $v0, $t1, 2
    addu       $v0, $v0, $a2
    ncct
    lw         $v0, 0x0($v0)
    lw         $v1, -0x1C($t2)
    lw         $s3, -0x18($t2)
    lw         $s4, -0x14($t2)
    lw         $s5, -0x10($t2)
    sw         $v0, 0x2C($t0)
    sw         $v1, 0xC($t0)
    sw         $s3, 0x18($t0)
    sw         $s4, 0x24($t0)
    sw         $s5, 0x30($t0)
    lw         $v0, 0x0($sp)
    lui        $v1, (0xFFFFFF >> 16)
    ori        $v1, $v1, (0xFFFFFF & 0xFFFF)
    sra        $v0, $v0, 7
    sll        $v0, $v0, 2
    addu       $v0, $s1, $v0
    lw         $a0, 0x0($v0)
    and        $v1, $t0, $v1
    sw         $a0, 0x0($sp)
    sw         $v1, 0x0($v0)
    lw         $v0, 0x0($sp)
    lui        $v1, (0xC000000 >> 16)
    or         $v0, $v0, $v1
    sw         $v0, 0x0($t0)
    swc2       $20, 0x4($t0)
    swc2       $21, 0x10($t0)
    swc2       $22, 0x1C($t0)
    addiu      $t0, $t0, 0x68
  .L8002DA80:
    bnez       $t6, .L8002D8F8
     addu      $a1, $t7, $zero
  .L8002DA88:
    addu       $v0, $t0, $zero
    lw         $s2, 0x10($sp)
    lw         $s1, 0xC($sp)
    lw         $s0, 0x8($sp)
    lw         $s3, 0x18($sp)
    lw         $s4, 0x1C($sp)
    lw         $s5, 0x20($sp)
    jr         $ra
     addiu     $sp, $sp, 0x24
endlabel SortGT4_neo_uv
