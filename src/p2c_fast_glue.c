/* Future Retro Fusion */
/* p2c_fast_glue.c — clean fast-path glue for 6bpl -> 8bpp (16px) ASM kernels.
 *
 * IMPORTANT: This file is intended to be *#included* from drawing.c*
 * so it can see the same static context (xlinebuffer, dp_for_drawing,
 * linetoscr_* fallbacks, etc.). Don't compile it as a separate TU.
 */

#include <stdint.h>

#ifndef CHUNK_PIXELS
#define CHUNK_PIXELS 16
#endif

#ifndef CHUNK_OFF
#define CHUNK_OFF(px) ((px) >> 4)      /* 16 pixels per 16-bit word */
#endif

/* Fallback prototypes live in drawing.c and are visible here because we #include. */
static int NOINLINE linetoscr_8 (int spix, int dpix, int stoppos);
#ifdef AGA
static int NOINLINE linetoscr_8_aga (int spix, int dpix, int stoppos);
#endif

/* Bitplane pointers provided by the core (often #defined to my_bplpt). */
extern uae_u8 *bplpt[8];

/* Core ASM kernel from your .a (declared in p2c6.h) */
extern void pfield_6p_16px_to8b_asm(const uint16_t *p0,
                                    const uint16_t *p1,
                                    const uint16_t *p2,
                                    const uint16_t *p3,
                                    const uint16_t *p4,
                                    const uint16_t *p5,
                                    uint8_t *dst);

static inline void do_p2c6_chunks(const uint16_t *p0,
                                  const uint16_t *p1,
                                  const uint16_t *p2,
                                  const uint16_t *p3,
                                  const uint16_t *p4,
                                  const uint16_t *p5,
                                  uint8_t *dst, int chunks)
{
    for (int i = 0; i < chunks; ++i) {
        pfield_6p_16px_to8b_asm(p0 + i, p1 + i, p2 + i, p3 + i, p4 + i, p5 + i,
                                dst + (i << 4));
    }
}

/* Fast path for plain 6bpl -> 8bpp (no HAM/EHB/DPF; no scaling). Works on ECS & AGA.
 * On AGA we also apply BPLCON4<15:8> XOR (same as linetoscr_8_aga does).
 */
static int NOINLINE linetoscr_8_fast (int spix, int dpix, int stoppos)
{
    uae_u8 *dst_base = (uae_u8*)xlinebuffer + dpix;
    if (stoppos <= dpix) return spix;

    /* Lead-in to 16px alignment (rare but keeps code simple & safe). */
    int mis = dpix & (CHUNK_PIXELS - 1);
    if (mis) {
        int upto = dpix + (CHUNK_PIXELS - mis);
        if (upto > stoppos) upto = stoppos;
        #ifdef AGA
        spix = linetoscr_8_aga(spix, dpix, upto);
        #else
        spix = linetoscr_8(spix, dpix, upto);
        #endif
        dpix = upto;
        dst_base = (uae_u8*)xlinebuffer + dpix;
    }

    int rem    = stoppos - dpix;
    int chunks = rem >> 4;

    if (chunks > 0) {
        const uint16_t *p0 = (const uint16_t*)(void*)bplpt[0] + CHUNK_OFF(dpix);
        const uint16_t *p1 = (const uint16_t*)(void*)bplpt[1] + CHUNK_OFF(dpix);
        const uint16_t *p2 = (const uint16_t*)(void*)bplpt[2] + CHUNK_OFF(dpix);
        const uint16_t *p3 = (const uint16_t*)(void*)bplpt[3] + CHUNK_OFF(dpix);
        const uint16_t *p4 = (const uint16_t*)(void*)bplpt[4] + CHUNK_OFF(dpix);
        const uint16_t *p5 = (const uint16_t*)(void*)bplpt[5] + CHUNK_OFF(dpix);

        do_p2c6_chunks(p0,p1,p2,p3,p4,p5, dst_base, chunks);

        #ifdef AGA
        /* Apply AGA XOR (BPLCON4 high byte) if enabled. */
        {
            uae_u8 xorv = (uae_u8)(dp_for_drawing->bplcon4 >> 8);
            if (xorv) {
                uae_u8 *p = dst_base;
                int npx = chunks << 4;
                while (npx--) *p++ ^= xorv;
            }
        }
        #endif

        dpix     += chunks << 4;
        dst_base += chunks << 4;
        spix     += chunks << 4;
    }

    /* Tail remainder (0..15 px) back to the standard path. */
    if (dpix < stoppos) {
        #ifdef AGA
        spix = linetoscr_8_aga(spix, dpix, stoppos);
        #else
        spix = linetoscr_8(spix, dpix, stoppos);
        #endif
    }
    return spix;
}
