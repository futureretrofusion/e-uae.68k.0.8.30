/* Future Retro Fusion */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Fallback when the DMS library isn't linked on this target.
   Simply report "not supported". */
int DMS_Process_File(const char *in, const char *out)
{
    (void)in; (void)out;
    return -1;
}
