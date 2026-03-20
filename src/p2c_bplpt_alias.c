/* Future Retro Fusion */
#include "sysconfig.h"
#include "sysdeps.h"
#include "uae.h"

/* The asm and C fast paths read bitplane pointers via `my_bplpt`.
   Provide real backing storage here. The project headers remap
   `bplpt` -> `my_bplpt` via a macro in amiga-kludges.h, so whenever
   the core writes to bplpt, it will actually write into this array. */

uae_u8 *my_bplpt_storage[8];     /* the 8 bitplane pointers */
uae_u8 **my_bplpt = my_bplpt_storage;
