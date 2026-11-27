/* hand-written R3000 assembly (not compiler output; see the note at its INCLUDE_ASM in md_sub.c) */

glabel Md_rtp
    addiu      $sp, $sp, -0x18
    addiu      $a0, $a0, -0xC
    lw         $a3, 0x0($a2)
    lhu        $a2, 0xA($a2)
    addiu      $a1, $a1, -0x6
    sw         $zero, 0x0($sp)
    sw         $zero, 0x4($sp)
    sw         $zero, 0x8($sp)
    sw         $s1, 0x14($sp)
    sw         $s0, 0x10($sp)
  .L8002D600:
    lwc2       $0, 0x0($a3)
    lwc2       $1, 0x4($a3)
    lwc2       $2, 0x8($a3)
    lwc2       $3, 0xC($a3)
    lwc2       $4, 0x10($a3)
    lwc2       $5, 0x14($a3)
    addiu      $a3, $a3, 0x18
    addiu      $a2, $a2, -0x1
    rtpt
    lw         $v0, 0x0($sp)
    lw         $s0, 0x4($sp)
    lw         $s1, 0x8($sp)
    addiu      $v1, $zero, 0x7FFF
    slt        $v0, $v1, $v0
    beqz       $v0, .L8002D644
     slt       $s0, $v1, $s0
    sh         $v1, 0x0($a1)
  .L8002D644:
    beqz       $s0, .L8002D650
     slt       $s1, $v1, $s1
    sh         $v1, 0x2($a1)
  .L8002D650:
    beqz       $s1, .L8002D65C
     addiu     $a0, $a0, 0xC
    sh         $v1, 0x4($a1)
  .L8002D65C:
    addiu      $a1, $a1, 0x6
    swc2       $17, 0x0($sp)
    swc2       $18, 0x4($sp)
    swc2       $19, 0x8($sp)
    swc2       $12, 0x0($a0)
    swc2       $13, 0x4($a0)
    swc2       $14, 0x8($a0)
    lhu        $v0, 0x0($sp)
    lhu        $s0, 0x4($sp)
    lhu        $s1, 0x8($sp)
    sh         $v0, 0x0($a1)
    sh         $s0, 0x2($a1)
    bnez       $a2, .L8002D600
     sh        $s1, 0x4($a1)
    lw         $v0, 0x0($sp)
    lw         $s0, 0x4($sp)
    lw         $s1, 0x8($sp)
    slt        $v0, $v1, $v0
    beqz       $v0, .L8002D6B0
     slt       $s0, $v1, $s0
    sh         $v1, 0x0($a1)
  .L8002D6B0:
    beqz       $s0, .L8002D6BC
     slt       $s1, $v1, $s1
    sh         $v1, 0x2($a1)
  .L8002D6BC:
    beqz       $s1, .L8002D6C8
     nop
    sh         $v1, 0x4($a1)
  .L8002D6C8:
    lw         $s1, 0x14($sp)
    lw         $s0, 0x10($sp)
    jr         $ra
     addiu     $sp, $sp, 0x18
endlabel Md_rtp
