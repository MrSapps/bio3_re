/* hand-written R3000 assembly (not compiler output; see the note at its INCLUDE_ASM in md_sub.c) */
/* Handwritten function */

glabel Skin_vtx0_old
    addiu      $sp, $sp, -0x60
    addu       $t2, $a0, $zero
    sw         $ra, 0x5C($sp)
    sw         $fp, 0x58($sp)
    sw         $s7, 0x54($sp)
    sw         $s6, 0x50($sp)
    sw         $s5, 0x4C($sp)
    sw         $s4, 0x48($sp)
    sw         $s3, 0x44($sp)
    sw         $s2, 0x40($sp)
    sw         $s1, 0x3C($sp)
    sw         $s0, 0x38($sp)
    lw         $s3, 0xC($a2)
    lw         $v0, 0x4($a2)
    lw         $s4, 0x10($a2)
    lw         $s6, 0x0($v0)
    lw         $t6, 0x0($a2)
    lw         $t7, 0x8($a2)
    lw         $t5, 0x110($t6)
    addu       $t3, $a1, $zero
    addu       $s1, $a3, $zero
    addiu      $s5, $zero, 0x7FFF
    addiu      $s2, $s1, 0x2
    sw         $t7, 0x24($sp)
    sw         $t6, 0x20($sp)
    sw         $t5, 0x1C($sp)
    sw         $zero, 0x10($sp)
    sw         $zero, 0x14($sp)
    sw         $zero, 0x18($sp)
  .L8002E38C:
    addu       $a2, $s3, $zero
    lbu        $a1, 0x0($s1)
    lw         $t7, 0x20($sp)
    lw         $a0, 0x24($sp)
    lw         $v1, 0x100($t7)
    sll        $s0, $a1, 1
    addu       $s0, $s0, $a1
    sll        $v0, $s0, 3
    addu       $v0, $v0, $a1
    sll        $v0, $v0, 3
    sw         $t1, 0x28($sp)
    sw         $t2, 0x2C($sp)
    sw         $t3, 0x30($sp)
    addu       $a1, $v1, $v0
    jal        CompM
     addiu     $a1, $a1, 0x3C
    lw         $t7, 0x1C($sp)
    sll        $s0, $s0, 1
    addu       $s0, $s0, $t7
    lh         $v0, 0x0($s0)
    lh         $v1, 0x2($s0)
    lh         $t6, 0x4($s0)
    sw         $v0, 0x0($s4)
    sw         $v1, 0x4($s4)
    sw         $t6, 0x8($s4)
    lw         $t4, 0x0($s3)
    lw         $t5, 0x4($s3)
    lw         $v0, 0x8($s3)
    lw         $v1, 0xC($s3)
    ctc2       $v0, $2 /* handwritten instruction */
    ctc2       $v1, $3 /* handwritten instruction */
    ctc2       $t4, $0 /* handwritten instruction */
    ctc2       $t5, $1 /* handwritten instruction */
    lw         $t6, 0x10($s3)
    lw         $t4, 0x14($s3)
    lw         $t5, 0x18($s3)
    lw         $v0, 0x1C($s3)
    ctc2       $t6, $4 /* handwritten instruction */
    ctc2       $t4, $5 /* handwritten instruction */
    ctc2       $t5, $6 /* handwritten instruction */
    ctc2       $v0, $7 /* handwritten instruction */
    lhu        $t5, 0x4($s4)
    lhu        $t4, 0x0($s4)
    sll        $t5, $t5, 16
    or         $t4, $t4, $t5
    mtc2       $t4, $0 /* handwritten instruction */
    lwc2       $1, 0x8($s4)
    lbu        $a1, -0x1($s2)
    addiu      $s2, $s2, 0x2
    mvmva      1, 0, 0, 0, 0
    addiu      $s1, $s1, 0x2
    addiu      $v0, $s3, 0x14
    swc2       $25, 0x0($v0)
    swc2       $26, 0x4($v0) /* handwritten instruction */
    swc2       $27, 0x8($v0) /* handwritten instruction */
    lw         $t4, 0x14($s3)
    lw         $t5, 0x18($s3)
    ctc2       $t4, $5 /* handwritten instruction */
    lw         $t6, 0x1C($s3)
    ctc2       $t5, $6 /* handwritten instruction */
    ctc2       $t6, $7 /* handwritten instruction */
    lw         $t3, 0x30($sp)
    lw         $t2, 0x2C($sp)
    lw         $t1, 0x28($sp)
  .L8002E48C:
    lbu        $a3, 0x0($s1)
    lbu        $t0, -0x1($s2)
    lbu        $a2, 0x0($s2)
    sll        $a0, $a3, 3
    addu       $a0, $s6, $a0
    sll        $v1, $t0, 3
    addu       $v1, $s6, $v1
    sll        $v0, $a2, 3
    addu       $v0, $s6, $v0
    lwc2       $0, 0x0($a0)
    lwc2       $1, 0x4($a0)
    lwc2       $2, 0x0($v1)
    lwc2       $3, 0x4($v1)
    lwc2       $4, 0x0($v0)
    lwc2       $5, 0x4($v0)
    addiu      $s2, $s2, 0x3
    addiu      $s1, $s1, 0x3
    addiu      $a1, $a1, -0x1
    rtpt
    lw         $v0, 0x10($sp)
    nop
    slt        $v0, $s5, $v0
    beqz       $v0, .L8002E4F0
     nop
    sh         $s5, 0x0($s7)
  .L8002E4F0:
    lw         $v0, 0x14($sp)
    nop
    slt        $v0, $s5, $v0
    beqz       $v0, .L8002E508
     nop
    sh         $s5, 0x0($fp)
  .L8002E508:
    lw         $v0, 0x18($sp)
    nop
    slt        $v0, $s5, $v0
    beqz       $v0, .L8002E520
     sll       $v0, $a3, 1
    sh         $s5, 0x0($t1)
  .L8002E520:
    addu       $s7, $t3, $v0
    sll        $v0, $t0, 1
    addu       $fp, $t3, $v0
    sll        $v0, $a2, 1
    addu       $t1, $t3, $v0
    sll        $a0, $a3, 2
    addu       $a0, $t2, $a0
    sll        $v1, $t0, 2
    addu       $v1, $t2, $v1
    sll        $v0, $a2, 2
    addu       $v0, $t2, $v0
    swc2       $12, 0x0($a0)
    swc2       $13, 0x0($v1)
    swc2       $14, 0x0($v0)
    addiu      $a0, $sp, 0x10
    addiu      $v1, $sp, 0x14
    addiu      $v0, $sp, 0x18
    swc2       $17, 0x0($a0)
    swc2       $18, 0x0($v1)
    swc2       $19, 0x0($v0)
    lhu        $v0, 0x10($sp)
    nop
    sh         $v0, 0x0($s7)
    lhu        $v0, 0x14($sp)
    nop
    sh         $v0, 0x0($fp)
    lhu        $v0, 0x18($sp)
    bnez       $a1, .L8002E48C
     sh        $v0, 0x0($t1)
    lbu        $v1, 0x0($s1)
    addiu      $v0, $zero, 0xFF
    bne        $v1, $v0, .L8002E38C
     addiu     $v1, $zero, 0x7FFF
    lw         $v0, 0x10($sp)
    nop
    slt        $v0, $v1, $v0
    beqz       $v0, .L8002E5BC
     nop
    sh         $v1, 0x0($s7)
  .L8002E5BC:
    lw         $v0, 0x14($sp)
    nop
    slt        $v0, $v1, $v0
    beqz       $v0, .L8002E5D4
     nop
    sh         $v1, 0x0($fp)
  .L8002E5D4:
    lw         $v0, 0x18($sp)
    nop
    slt        $v0, $v1, $v0
    beqz       $v0, .L8002E5EC
     nop
    sh         $v1, 0x0($t1)
  .L8002E5EC:
    lw         $ra, 0x5C($sp)
    lw         $fp, 0x58($sp)
    lw         $s7, 0x54($sp)
    lw         $s6, 0x50($sp)
    lw         $s5, 0x4C($sp)
    lw         $s4, 0x48($sp)
    lw         $s3, 0x44($sp)
    lw         $s2, 0x40($sp)
    lw         $s1, 0x3C($sp)
    lw         $s0, 0x38($sp)
    jr         $ra
     addiu     $sp, $sp, 0x60
endlabel Skin_vtx0_old
