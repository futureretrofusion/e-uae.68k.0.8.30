/* Future Retro Fusion */
#include "config.h"   /* must come before sysdeps.h */
#include "sysdeps.h"

/* ---- libdisk stubs ---- */
int  libdisk_open(const char *name, unsigned int drv) { (void)name; (void)drv; return 0; }
void libdisk_close(unsigned int drv) { (void)drv; }

int  libdisk_loadtrack(uae_u16 *mfmbuf, uae_u16 *tracktiming,
                       unsigned int drv, int tr, int rev,
                       int *tracklen, unsigned int *multi_revolution,
                       int *skipoffset)
{
    (void)mfmbuf; (void)tracktiming; (void)drv; (void)tr; (void)rev;
    if (tracklen) *tracklen = 0;
    if (multi_revolution) *multi_revolution = 0U;
    if (skipoffset) *skipoffset = 0;
    return -1;
}

int  libdisk_loadrevolution(uae_u16 *mfmbuf, unsigned int drv,
                            uae_u16 *tracktiming, int tr, int rev,
                            int *tracklen, unsigned int *multi_revolution,
                            int *skipoffset)
{
    (void)mfmbuf; (void)drv; (void)tracktiming; (void)tr; (void)rev;
    if (tracklen) *tracklen = 0;
    if (multi_revolution) *multi_revolution = 0U;
    if (skipoffset) *skipoffset = 0;
    return -1;
}

/* ---- SuperCard Pro stubs ---- */
int  scp_open (const char *name, unsigned int drv) { (void)name; (void)drv; return 0; }
void scp_close(unsigned int drv) { (void)drv; }

int  scp_loadtrack(uae_u16 *mfmbuf, uae_u16 *tracktiming,
                   unsigned int drv, int tr, int rev,
                   int *tracklen, unsigned int *multi_revolution,
                   int *skipoffset)
{
    (void)mfmbuf; (void)tracktiming; (void)drv; (void)tr; (void)rev;
    if (tracklen) *tracklen = 0;
    if (multi_revolution) *multi_revolution = 0U;
    if (skipoffset) *skipoffset = 0;
    return -1;
}

void scp_loadrevolution(uae_u16 *mfmbuf, unsigned int drv,
                        uae_u16 *tracktiming, int tr, int rev,
                        int *tracklen, unsigned int *multi_revolution,
                        int *skipoffset)
{
    (void)mfmbuf; (void)drv; (void)tracktiming; (void)tr; (void)rev;
    if (tracklen) *tracklen = 0;
    if (multi_revolution) *multi_revolution = 0U;
    if (skipoffset) *skipoffset = 0;
}
