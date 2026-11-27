/* hand-written R3000 assembly (not compiler output; see the note at its INCLUDE_ASM in md_sub.c) */
/* Handwritten function */

glabel SortGT3_sabun
    addiu      $sp, $sp, -0x1C
    sw         $s0, 0x8($sp)
    sw         $s1, 0xC($sp)
    sw         $s2, 0x10($sp)
    sw         $s3, 0x14($sp)
    sw         $s4, 0x18($sp)
    lw         $v0, 0x0($a0)
    lw         $t9, 0x8($a0)
    lw         $t1, 0xC($v0)
    lw         $t3, 0xC($a0)
    lhu        $t2, 0x14($v0)
    addu       $t0, $a1, $zero
    beqz       $t2, .L8002E140
     addu      $t7, $a2, $zero
    addu       $t8, $a3, $zero
  .L8002E01C:
    lb         $a1, 0x7($t1)
    lbu        $a0, 0xA($t1)
    lb         $v1, 0xB($t1)
    sll        $t4, $a1, 2
    addu       $a3, $t4, $t7
    sll        $t5, $a0, 2
    addu       $a2, $t5, $t7
    sll        $t6, $v1, 2
    addu       $v0, $t6, $t7
    lw         $a3, 0x0($a3)
    lw         $a2, 0x0($a2)
    lw         $v0, 0x0($v0)
    mtc2       $a3, $12 /* handwritten instruction */
    mtc2       $v0, $14 /* handwritten instruction */
    mtc2       $a2, $13 /* handwritten instruction */
    addiu      $t1, $t1, 0xC
    addiu      $t2, $t2, -0x1
    nclip
    sll        $a1, $a1, 1
    addu       $a1, $t8, $a1
    sll        $a0, $a0, 1
    addu       $a0, $t8, $a0
    sll        $v1, $v1, 1
    addu       $v1, $t8, $v1
    lhu        $a1, 0x0($a1)
    lhu        $a0, 0x0($a0)
    swc2       $24, 0x0($sp)
    lw         $v0, 0x0($sp)
    addu       $s0, $t5, $t3
    blez       $v0, .L8002E138
     lhu       $v1, 0x0($v1)
    mtc2       $a1, $17 /* handwritten instruction */
    mtc2       $a0, $18 /* handwritten instruction */
    mtc2       $v1, $19 /* handwritten instruction */
    addu       $v0, $t4, $t3
    addu       $s1, $t6, $t3
    avsz3
    lw         $v0, 0x0($v0)
    lw         $s0, 0x0($s0)
    lw         $s1, 0x0($s1)
    lw         $s2, -0xC($t1)
    lw         $s3, -0x8($t1)
    lw         $s4, -0x4($t1)
    lui        $v1, (0x34000000 >> 16)
    lui        $a0, (0xFFFFFF >> 16)
    or         $v0, $v0, $v1
    ori        $a0, $a0, (0xFFFFFF & 0xFFFF)
    and        $s3, $s3, $a0
    sw         $v0, 0x4($t0)
    sw         $s0, 0x10($t0)
    sw         $s1, 0x1C($t0)
    sw         $s2, 0xC($t0)
    sw         $s3, 0x18($t0)
    sw         $s4, 0x24($t0)
    swc2       $7, 0x0($sp)
    swc2       $12, 0x8($t0)
    swc2       $13, 0x14($t0)
    swc2       $14, 0x20($t0)
    lw         $v0, 0x0($sp)
    and        $a0, $t0, $a0
    sra        $v0, $v0, 3
    sll        $v0, $v0, 2
    addu       $v0, $t9, $v0
    lw         $v1, 0x0($v0)
    sw         $a0, 0x0($v0)
    sw         $v1, 0x0($sp)
    lw         $s0, 0x0($sp)
    lui        $v1, (0x9000000 >> 16)
    or         $s0, $s0, $v1
    sw         $s0, 0x0($t0)
    addiu      $t0, $t0, 0x68
  .L8002E138:
    bnez       $t2, .L8002E01C
     nop
  .L8002E140:
    addu       $v0, $t0, $zero
    lw         $s0, 0x8($sp)
    lw         $s1, 0xC($sp)
    lw         $s2, 0x10($sp)
    lw         $s3, 0x14($sp)
    lw         $s4, 0x18($sp)
    jr         $ra
     addiu     $sp, $sp, 0x1C
endlabel SortGT3_sabun
