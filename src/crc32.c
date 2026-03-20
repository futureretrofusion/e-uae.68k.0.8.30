/* Future Retro Fusion */
/*
 * UAE - The Un*x Amiga Emulator
 * CRC16/CRC32 helper
 * Patched Sept 2025 - FRF
 */

#include "sysconfig.h"
#include "sysdeps.h"
#include "crc32.h"

/* Static tables, built once on demand */
static uae_u32 crc_table32[256];
static uae_u16 crc_table16[256];
static int crc_tables_ready = 0;

static void make_crc_table(void)
{
    unsigned int n, k;
    for (n = 0; n < 256; n++) {
        uae_u32 c = (uae_u32)n;
        uae_u16 w = (uae_u16)(n << 8);
        for (k = 0; k < 8; k++) {
            c = (c >> 1) ^ ((c & 1) ? 0xEDB88320UL : 0);
            w = (uae_u16)((w << 1) ^ ((w & 0x8000) ? 0x1021 : 0));
        }
        crc_table32[n] = c;
        crc_table16[n] = w;
    }
    crc_tables_ready = 1;
}

uae_u32 get_crc32(const uae_u8 *buf, unsigned int len)
{
    if (!crc_tables_ready)
        make_crc_table();

    uae_u32 crc = 0xFFFFFFFFUL;
    while (len-- > 0) {
        crc = crc_table32[(crc ^ *buf++) & 0xFFU] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFFUL;
}

uae_u16 get_crc16(const uae_u8 *buf, unsigned int len)
{
    if (!crc_tables_ready)
        make_crc_table();

    uae_u16 crc = 0xFFFFU;
    while (len-- > 0) {
        crc = (uae_u16)((crc << 8) ^ crc_table16[((crc >> 8) ^ *buf++) & 0xFFU]);
    }
    return crc;
}
