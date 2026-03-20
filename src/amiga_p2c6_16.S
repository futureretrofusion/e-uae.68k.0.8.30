/* Future Retro Fusion */
    .text
    .even
    .macro  GLOBL2 name
    .globl  \name
    .globl  _\name
    .endm

/* 6 planes -> 16 chunky bytes (8bpp indexes 0..63)
   Args (stack): 4(sp)=pack* (w[0..5]), 8(sp)=dst* (u8*) */
    GLOBL2 pfield_6p_pack16_to8b_asm
pfield_6p_pack16_to8b_asm:
_pfield_6p_pack16_to8b_asm:
    movem.l 4(sp),a0-a1         | a0=pack, a1=dst
    move.w  (a0)+,d0            | d0..d5 = p0..p5 (bit0..bit5)
    move.w  (a0)+,d1
    move.w  (a0)+,d2
    move.w  (a0)+,d3
    move.w  (a0)+,d4
    move.w  (a0)+,d5
    moveq   #15,d6              | 16 pixels - 1

1:  moveq   #0,d7
    add.w   d5,d5               | plane5 -> bit 5 (value 32)
    bcc.s   2f
    ori.b   #32,d7
2:  add.w   d4,d4               | plane4 -> 16
    bcc.s   3f
    ori.b   #16,d7
3:  add.w   d3,d3               | plane3 -> 8
    bcc.s   4f
    ori.b   #8,d7
4:  add.w   d2,d2               | plane2 -> 4
    bcc.s   5f
    ori.b   #4,d7
5:  add.w   d1,d1               | plane1 -> 2
    bcc.s   6f
    ori.b   #2,d7
6:  add.w   d0,d0               | plane0 -> 1
    bcc.s   7f
    ori.b   #1,d7
7:  move.b  d7,(a1)+
    dbra    d6,1b
    rts
