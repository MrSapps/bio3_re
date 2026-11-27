/* hand-written R3000 assembly (not compiler output; see the note at its INCLUDE_ASM in md_sub.c) */
/* Handwritten function */

glabel SortGT3_neo
    addiu      $sp, $sp, -0x28
    addu       $t0, $a0, $zero
    sw         $s4, 0x18($sp)
    lw         $a0, 0x0($t0)
    lui        $s4, (0xFFFFFF >> 16)
    sw         $s6, 0x20($sp)
    ori        $s4, $s4, (0xFFFFFF & 0xFFFF)
    sw         $s5, 0x1C($sp)
    lhu        $t4, 0x14($a0)
    sw         $s3, 0x14($sp)
    sw         $s2, 0x10($sp)
    sw         $s1, 0xC($sp)
    sw         $s0, 0x8($sp)
    beqz       $t4, .L8002DC74
     lui       $s6, (0xFF0000 >> 16)
    addiu      $v0, $t0, 0x4
    lw         $v1, 0xC($a0)
    lw         $t5, 0x4($a0)
    lw         $s5, 0x8($t0)
    addiu      $t3, $v1, 0x8
    lwc2       $6, 0x0($v0)
    lw         $v1, 0x8($v1)
    lb         $t2, -0x1($t3)
    and        $v0, $v1, $s6
    sra        $t0, $v0, 16
    sra        $t1, $v1, 24
    sll        $v0, $t2, 2
    addu       $s3, $a2, $v0
    sll        $v0, $t0, 2
    addu       $s2, $a2, $v0
    sll        $v0, $t1, 2
    addu       $s1, $a2, $v0
    sll        $v0, $t2, 1
    addu       $s0, $a3, $v0
    sll        $v0, $t0, 1
    addu       $t9, $a3, $v0
    sll        $v0, $t1, 1
    addu       $t6, $a3, $v0
  .L8002DB44:
    lw         $s3, 0x0($s3)
    lw         $s2, 0x0($s2)
    lw         $s1, 0x0($s1)
    mtc2       $s3, $12 /* handwritten instruction */
    mtc2       $s1, $14 /* handwritten instruction */
    mtc2       $s2, $13 /* handwritten instruction */
    lhu        $s0, 0x0($s0)
    lhu        $t9, 0x0($t9)
    lhu        $t6, 0x0($t6)
    mtc2       $s0, $17 /* handwritten instruction */
    mtc2       $t9, $18 /* handwritten instruction */
    mtc2       $t6, $19 /* handwritten instruction */
    addiu      $t3, $t3, 0xC
    addiu      $t4, $t4, -0x1
    nclip
    addu       $a0, $t2, $zero
    addu       $t8, $t0, $zero
    addu       $t7, $t1, $zero
    lw         $v1, 0x0($t3)
    lb         $t2, -0x1($t3)
    and        $v0, $v1, $s6
    sra        $t0, $v0, 16
    sra        $t1, $v1, 24
    sll        $v0, $t2, 2
    addu       $s3, $a2, $v0
    sll        $v0, $t0, 2
    addu       $s2, $a2, $v0
    sll        $v0, $t1, 2
    addu       $s1, $a2, $v0
    sll        $v0, $t2, 1
    addu       $s0, $a3, $v0
    sll        $v0, $t0, 1
    addu       $t9, $a3, $v0
    sll        $v0, $t1, 1
    addu       $t6, $a3, $v0
    swc2       $24, 0x0($sp)
    lw         $v0, 0x0($sp)
    sll        $a0, $a0, 3
    blez       $v0, .L8002DC6C
     nop
    avsz3
    addu       $a0, $t5, $a0
    sll        $v1, $t8, 3
    addu       $v1, $t5, $v1
    sll        $v0, $t7, 3
    addu       $v0, $t5, $v0
    swc2       $7, 0x0($sp)
    swc2       $12, 0x8($a1)
    swc2       $13, 0x14($a1)
    swc2       $14, 0x20($a1)
    lwc2       $4, 0x0($v0)
    lwc2       $5, 0x4($v0)
    lwc2       $0, 0x0($a0)
    lw         $v0, 0x0($sp)
    lwc2       $1, 0x4($a0)
    lwc2       $2, 0x0($v1)
    lwc2       $3, 0x4($v1)
    sra        $v0, $v0, 3
    sll        $v0, $v0, 2
    ncct
    addu       $v0, $s5, $v0
    lw         $v1, 0x0($v0)
    nop
    sw         $v1, 0x0($sp)
    and        $v1, $a1, $s4
    sw         $v1, 0x0($v0)
    lw         $v0, 0x0($sp)
    lui        $v1, (0x9000000 >> 16)
    and        $v0, $v0, $s4
    or         $v0, $v0, $v1
    sw         $v0, 0x0($a1)
    swc2       $20, 0x4($a1)
    swc2       $21, 0x10($a1)
    swc2       $22, 0x1C($a1)
  .L8002DC6C:
    bnez       $t4, .L8002DB44
     addiu     $a1, $a1, 0x50
  .L8002DC74:
    lw         $s6, 0x20($sp)
    lw         $s5, 0x1C($sp)
    lw         $s4, 0x18($sp)
    lw         $s3, 0x14($sp)
    lw         $s2, 0x10($sp)
    lw         $s1, 0xC($sp)
    lw         $s0, 0x8($sp)
    jr         $ra
     addiu     $sp, $sp, 0x28
endlabel SortGT3_neo
