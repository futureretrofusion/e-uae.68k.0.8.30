/* Future Retro Fusion */
/* Tiny AGA -> ECS palette quantizer (choose 32 or 16 colors).
   Include this from custom.c AFTER drawing.h so xcolnr/xcolors exist. */
#ifndef UAE_TINY_PALETTE_H
#define UAE_TINY_PALETTE_H

#ifndef AGA_TARGET_COLORS
#define AGA_TARGET_COLORS 32   /* change to 16 for 4-bitplanes */
#endif
#if (AGA_TARGET_COLORS != 32) && (AGA_TARGET_COLORS != 16)
#error "AGA_TARGET_COLORS must be 32 or 16"
#endif

/* xcolnr/xcolors come from drawing.h */
STATIC_INLINE xcolnr xcolor_from_12(int v12) { return xcolors[v12 & 0x0FFF]; }

/* Prebuilt display palette in xcolors[] space (size 32 or 16). */
static xcolnr aga_tiny_pal[32];
static int    aga_tiny_pal_inited = 0;

/* Initialize R2-G2-B1 (32) or R2-G1-B1 (16) palette into xcolors[] */
STATIC_INLINE void init_aga_tiny_palette(void)
{
    static const uae_u8 R4_4[4] = { 1, 5, 10, 15 };
    static const uae_u8 G4_4[4] = { 1, 5, 10, 15 };
    static const uae_u8 B4_2[2] = { 3, 12 };
    static const uae_u8 G4_2[2] = { 3, 12 };

#if AGA_TARGET_COLORS == 32
    int idx = 0;
    for (int r=0; r<4; r++)
    for (int g=0; g<4; g++)
    for (int b=0; b<2; b++, idx++) {
        int v12 = (R4_4[r] << 8) | (G4_4[g] << 4) | (B4_2[b] << 0);
        aga_tiny_pal[idx] = xcolor_from_12(v12);
    }
#else /* 16 colors */
    int idx = 0;
    for (int r=0; r<4; r++)
    for (int g=0; g<2; g++)
    for (int b=0; b<2; b++, idx++) {
        int v12 = (R4_4[r] << 8) | (G4_2[g] << 4) | (B4_2[b] << 0);
        aga_tiny_pal[idx] = xcolor_from_12(v12);
    }
#endif
    aga_tiny_pal_inited = 1;
}

/* Bucket an 8-bit channel to 2-bit (0..3) or 1-bit (0..1). */
STATIC_INLINE int q2bits(int c8) { return c8 >> 6; }
STATIC_INLINE int q1bit (int c8) { return c8 >> 7; }

/* Map 24-bit AGA palette value to our tiny palette (returns host xcolor). */
STATIC_INLINE xcolnr aga24_to_tiny_xcolor(int v24)
{
    int r8 = (v24 >> 16) & 0xFF;
    int g8 = (v24 >>  8) & 0xFF;
    int b8 = (v24 >>  0) & 0xFF;
#if AGA_TARGET_COLORS == 32
    int idx = (q2bits(r8) << 3) | (q2bits(g8) << 1) | q1bit(b8);
#else
    int idx = (q2bits(r8) << 2) | (q1bit(g8) << 1) | q1bit(b8);
#endif
    if (!aga_tiny_pal_inited) init_aga_tiny_palette();
    return aga_tiny_pal[idx];
}

#endif /* UAE_TINY_PALETTE_H */
