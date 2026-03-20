/* Future Retro Fusion */
    .text
    .even

    /* ---------- helpers ---------- */
    /* Create public + underscore alias for toolchains that prefix '_' */
    .macro  GLOBL2 name
    .globl  \name
    .globl  _\name
    .endm

/* ================================================================
   AGA: 8 planes -> 16 chunky bytes (8bpp), no stretch
   Args: 4(sp)=pack* (8 words w[0..7]), 8(sp)=dst* (u8*)
   pack words shift left each pixel; leftmost pixel = bit15 of each plane
   ================================================================ */
    GLOBL2 pfield_8p_pack16_to8b_asm
pfield_8p_pack16_to8b_asm:
_pfield_8p_pack16_to8b_asm:
    movem.l 4(sp),a0-a1           | a0=pack, a1=dst
    move.w  (a0)+,d0              | d0..d3 = p0..p3
    move.w  (a0)+,d1
    move.w  (a0)+,d2
    move.w  (a0)+,d3
    move.w  (a0)+,d5              | d5     = p5
    moveq   #15,d6                | d6 = loop counter (16 pixels-1)
    move.w  8(a0),d4              | temp load p4; we’ll use memory for p4/6/7
    move.w  12(a0),d4             | prime cache lines
    move.w  14(a0),d4

.Lp2c8_loop:
    moveq   #0,d7                 | d7 = pixel accumulator

    | plane7 (mem): add.w shifts into C; store back for next pixel
    move.w  14(a0),d4
    add.w   d4,d4
    move.w  d4,14(a0)
    bcc.s   1f
    ori.b   #128,d7
1:
    | plane6 (mem)
    move.w  12(a0),d4
    add.w   d4,d4
    move.w  d4,12(a0)
    bcc.s   2f
    ori.b   #64,d7
2:
    | plane5 (reg d5)
    add.w   d5,d5
    bcc.s   3f
    ori.b   #32,d7
3:
    | plane4 (mem)
    move.w  8(a0),d4
    add.w   d4,d4
    move.w  d4,8(a0)
    bcc.s   4f
    ori.b   #16,d7
4:
    | plane3..0 (regs d3..d0)
    add.w   d3,d3
    bcc.s   5f
    ori.b   #8,d7
5:
    add.w   d2,d2
    bcc.s   6f
    ori.b   #4,d7
6:
    add.w   d1,d1
    bcc.s   7f
    ori.b   #2,d7
7:
    add.w   d0,d0
    bcc.s   8f
    ori.b   #1,d7
8:
    move.b  d7,(a1)+

    dbra    d6,.Lp2c8_loop
    rts

/* ================================================================
   AGA: 8 planes -> 32 chunky bytes (8bpp), stretch ×2 horizontally
   ================================================================ */
    GLOBL2 pfield_8p_pack16_to8b_stretch1_asm
pfield_8p_pack16_to8b_stretch1_asm:
_pfield_8p_pack16_to8b_stretch1_asm:
    movem.l 4(sp),a0-a1           | a0=pack, a1=dst
    move.w  (a0)+,d0
    move.w  (a0)+,d1
    move.w  (a0)+,d2
    move.w  (a0)+,d3
    move.w  (a0)+,d5
    moveq   #15,d6
    move.w  8(a0),d4
    move.w  12(a0),d4
    move.w  14(a0),d4
.Lp2c8_st_loop:
    moveq   #0,d7
    move.w  14(a0),d4
    add.w   d4,d4
    move.w  d4,14(a0)
    bcc.s   9f
    ori.b   #128,d7
9:
    move.w  12(a0),d4
    add.w   d4,d4
    move.w  d4,12(a0)
    bcc.s   10f
    ori.b   #64,d7
10:
    add.w   d5,d5
    bcc.s   11f
    ori.b   #32,d7
11:
    move.w  8(a0),d4
    add.w   d4,d4
    move.w  d4,8(a0)
    bcc.s   12f
    ori.b   #16,d7
12:
    add.w   d3,d3
    bcc.s   13f
    ori.b   #8,d7
13:
    add.w   d2,d2
    bcc.s   14f
    ori.b   #4,d7
14:
    add.w   d1,d1
    bcc.s   15f
    ori.b   #2,d7
15:
    add.w   d0,d0
    bcc.s   16f
    ori.b   #1,d7
16:
    move.b  d7,(a1)+
    move.b  d7,(a1)+
    dbra    d6,.Lp2c8_st_loop
    rts

/* ================================================================
   AGA: 8 planes -> 8 bytes (8bpp), shrink ÷2 horizontally
   (compute 2 pixels; store the first; discard the second)
   ================================================================ */
    GLOBL2 pfield_8p_pack16_to8b_shrink1_asm
pfield_8p_pack16_to8b_shrink1_asm:
_pfield_8p_pack16_to8b_shrink1_asm:
    movem.l 4(sp),a0-a1           | a0=pack, a1=dst
    move.w  (a0)+,d0
    move.w  (a0)+,d1
    move.w  (a0)+,d2
    move.w  (a0)+,d3
    move.w  (a0)+,d5
    moveq   #7,d6                 | 8 stores (16 px processed)
    move.w  8(a0),d4
    move.w  12(a0),d4
    move.w  14(a0),d4

.Lp2c8_sh_loop:
    | pixel A (store)
    moveq   #0,d7
    move.w  14(a0),d4
    add.w   d4,d4
    move.w  d4,14(a0)
    bcc.s   20f
    ori.b   #128,d7
20:
    move.w  12(a0),d4
    add.w   d4,d4
    move.w  d4,12(a0)
    bcc.s   21f
    ori.b   #64,d7
21:
    add.w   d5,d5
    bcc.s   22f
    ori.b   #32,d7
22:
    move.w  8(a0),d4
    add.w   d4,d4
    move.w  d4,8(a0)
    bcc.s   23f
    ori.b   #16,d7
23:
    add.w   d3,d3
    bcc.s   24f
    ori.b   #8,d7
24:
    add.w   d2,d2
    bcc.s   25f
    ori.b   #4,d7
25:
    add.w   d1,d1
    bcc.s   26f
    ori.b   #2,d7
26:
    add.w   d0,d0
    bcc.s   27f
    ori.b   #1,d7
27:
    move.b  d7,(a1)+

    | pixel B (discard)
    moveq   #0,d7
    move.w  14(a0),d4
    add.w   d4,d4
    move.w  d4,14(a0)
    bcc.s   30f
    ori.b   #128,d7
30:
    move.w  12(a0),d4
    add.w   d4,d4
    move.w  d4,12(a0)
    bcc.s   31f
    ori.b   #64,d7
31:
    add.w   d5,d5
    bcc.s   32f
    ori.b   #32,d7
32:
    move.w  8(a0),d4
    add.w   d4,d4
    move.w  d4,8(a0)
    bcc.s   33f
    ori.b   #16,d7
33:
    add.w   d3,d3
    bcc.s   34f
    ori.b   #8,d7
34:
    add.w   d2,d2
    bcc.s   35f
    ori.b   #4,d7
35:
    add.w   d1,d1
    bcc.s   36f
    ori.b   #2,d7
36:
    add.w   d0,d0
    bcc.s   37f
    ori.b   #1,d7
37:
    dbra    d6,.Lp2c8_sh_loop
    rts

/* ================================================================
   020+: 8 planes -> 16 pixels expanded via palette (16bpp)
   Args: 4(sp)=pack*, 8(sp)=dst16*, 12(sp)=pal16*(256x u16)
   ================================================================ */
    GLOBL2 pfield_8p_pack16_to16b_asm_020
pfield_8p_pack16_to16b_asm_020:
_pfield_8p_pack16_to16b_asm_020:
    movem.l 4(sp),a0-a2           | a0=pack, a1=dst16, a2=pal16
    move.w  (a0)+,d0
    move.w  (a0)+,d1
    move.w  (a0)+,d2
    move.w  (a0)+,d3
    move.w  (a0)+,d5
    moveq   #15,d6
    move.w  8(a0),d4
    move.w  12(a0),d4
    move.w  14(a0),d4
.Lp2c8_16b_loop:
    moveq   #0,d7
    move.w  14(a0),d4
    add.w   d4,d4
    move.w  d4,14(a0)
    bcc.s   40f
    ori.b   #128,d7
40:
    move.w  12(a0),d4
    add.w   d4,d4
    move.w  d4,12(a0)
    bcc.s   41f
    ori.b   #64,d7
41:
    add.w   d5,d5
    bcc.s   42f
    ori.b   #32,d7
42:
    move.w  8(a0),d4
    add.w   d4,d4
    move.w  d4,8(a0)
    bcc.s   43f
    ori.b   #16,d7
43:
    add.w   d3,d3
    bcc.s   44f
    ori.b   #8,d7
44:
    add.w   d2,d2
    bcc.s   45f
    ori.b   #4,d7
45:
    add.w   d1,d1
    bcc.s   46f
    ori.b   #2,d7
46:
    add.w   d0,d0
    bcc.s   47f
    ori.b   #1,d7
47:
    move.w  (a2,d7.w*2),(a1)+
    dbra    d6,.Lp2c8_16b_loop
    rts

/* ================================================================
   020+: 8 planes -> 16 pixels expanded via palette (32bpp)
   Args: 4(sp)=pack*, 8(sp)=dst32*, 12(sp)=pal32*(256x u32)
   ================================================================ */
    GLOBL2 pfield_8p_pack16_to32b_asm_020
pfield_8p_pack16_to32b_asm_020:
_pfield_8p_pack16_to32b_asm_020:
    movem.l 4(sp),a0-a2
    move.w  (a0)+,d0
    move.w  (a0)+,d1
    move.w  (a0)+,d2
    move.w  (a0)+,d3
    move.w  (a0)+,d5
    moveq   #15,d6
    move.w  8(a0),d4
    move.w  12(a0),d4
    move.w  14(a0),d4
.Lp2c8_32b_loop:
    moveq   #0,d7
    move.w  14(a0),d4
    add.w   d4,d4
    move.w  d4,14(a0)
    bcc.s   50f
    ori.b   #128,d7
50:
    move.w  12(a0),d4
    add.w   d4,d4
    move.w  d4,12(a0)
    bcc.s   51f
    ori.b   #64,d7
51:
    add.w   d5,d5
    bcc.s   52f
    ori.b   #32,d7
52:
    move.w  8(a0),d4
    add.w   d4,d4
    move.w  d4,8(a0)
    bcc.s   53f
    ori.b   #16,d7
53:
    add.w   d3,d3
    bcc.s   54f
    ori.b   #8,d7
54:
    add.w   d2,d2
    bcc.s   55f
    ori.b   #4,d7
55:
    add.w   d1,d1
    bcc.s   56f
    ori.b   #2,d7
56:
    add.w   d0,d0
    bcc.s   57f
    ori.b   #1,d7
57:
    move.l  (a2,d7.w*4),(a1)+
    dbra    d6,.Lp2c8_32b_loop
    rts
