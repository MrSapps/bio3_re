/* hand-written R3000 assembly (not compiler output; see the note at its INCLUDE_ASM in md_sub.c) */
/* Handwritten function */

glabel SortGT3_neo_uv
    addiu      $sp, $sp, -0x1C
    addu       $t0, $a0, $zero
    sw         $s0, 0x8($sp)
    sw         $s3, 0x18($sp)
    sw         $s2, 0x14($sp)
    sw         $s1, 0x10($sp)
    lw         $a0, 0x0($t0)
    nop
    lhu        $t4, 0x14($a0)
    addiu      $v0, $t0, 0x4
    beqz       $t4, .L8002D87C
     lui       $s0, (0xFF0000 >> 16)
    lw         $v1, 0xC($a0)
    lw         $t5, 0x4($a0)
    lw         $t9, 0x8($t0)
    lw         $s1, 0x8($v1)
    addiu      $t1, $v1, 0x8
    lwc2       $6, 0x0($v0)
    lb         $t7, -0x1($t1)
    and        $v0, $s1, $s0
    sra        $t6, $v0, 16
    sra        $t8, $s1, 24
    addu       $t3, $t7, $zero
  .L8002D734:
    sll        $a0, $t3, 2
    addu       $a0, $a0, $a2
    addu       $t2, $t6, $zero
    sll        $v1, $t2, 2
    addu       $v1, $v1, $a2
    addu       $t0, $t8, $zero
    sll        $v0, $t0, 2
    addu       $v0, $v0, $a2
    lw         $a0, 0x0($a0)
    lw         $v1, 0x0($v1)
    lw         $v0, 0x0($v0)
    mtc2       $a0, $12 /* handwritten instruction */
    mtc2       $v0, $14 /* handwritten instruction */
    mtc2       $v1, $13 /* handwritten instruction */
    addiu      $t1, $t1, 0xC
    addiu      $t4, $t4, -0x1
    nclip
    lw         $v1, 0x0($t1)
    lb         $t7, -0x1($t1)
    and        $v0, $v1, $s0
    sra        $t6, $v0, 16
    sra        $t8, $v1, 24
    swc2       $24, 0x0($sp)
    lw         $s1, 0x0($sp)
    sll        $a0, $t3, 1
    blez       $s1, .L8002D874
     addu      $a0, $a0, $a3
    sll        $v1, $t2, 1
    addu       $v1, $v1, $a3
    sll        $v0, $t0, 1
    addu       $v0, $v0, $a3
    lhu        $a0, 0x0($a0)
    lhu        $v1, 0x0($v1)
    lhu        $v0, 0x0($v0)
    mtc2       $a0, $17 /* handwritten instruction */
    mtc2       $v1, $18 /* handwritten instruction */
    mtc2       $v0, $19 /* handwritten instruction */
    sll        $a0, $t3, 3
    addu       $a0, $t5, $a0
    avsz3
    sll        $v1, $t2, 3
    addu       $v1, $t5, $v1
    sll        $v0, $t0, 3
    addu       $v0, $t5, $v0
    swc2       $7, 0x0($sp)
    swc2       $12, 0x8($a1)
    swc2       $13, 0x14($a1)
    swc2       $14, 0x20($a1)
    lwc2       $0, 0x0($a0)
    lwc2       $1, 0x4($a0)
    lwc2       $2, 0x0($v1)
    lwc2       $3, 0x4($v1)
    lwc2       $4, 0x0($v0)
    lwc2       $5, 0x4($v0)
    lui        $a0, (0xFFFFFF >> 16)
    ori        $a0, $a0, (0xFFFFFF & 0xFFFF)
    ncct
    lw         $s1, -0x14($t1)
    lw         $s2, -0x10($t1)
    lw         $s3, -0xC($t1)
    lw         $v0, 0x0($sp)
    and        $s2, $s2, $a0
    sw         $s1, 0xC($a1)
    sw         $s2, 0x18($a1)
    sw         $s3, 0x24($a1)
    sra        $v0, $v0, 3
    sll        $v0, $v0, 2
    addu       $v0, $t9, $v0
    lw         $v1, 0x0($v0)
    and        $a0, $a1, $a0
    sw         $v1, 0x0($sp)
    sw         $a0, 0x0($v0)
    lw         $s1, 0x0($sp)
    lui        $v1, (0x9000000 >> 16)
    or         $s1, $s1, $v1
    sw         $s1, 0x0($a1)
    swc2       $20, 0x4($a1)
    swc2       $21, 0x10($a1)
    swc2       $22, 0x1C($a1)
    addiu      $a1, $a1, 0x68
  .L8002D874:
    bnez       $t4, .L8002D734
     addu      $t3, $t7, $zero
  .L8002D87C:
    addu       $v0, $a1, $zero
    lw         $s0, 0x8($sp)
    lw         $s3, 0x18($sp)
    lw         $s2, 0x14($sp)
    lw         $s1, 0x10($sp)
    jr         $ra
     addiu     $sp, $sp, 0x1C
endlabel SortGT3_neo_uv
