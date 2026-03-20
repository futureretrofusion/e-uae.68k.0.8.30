/* Future Retro Fusion */
#pragma once

/* Force no dirent on Amiga and provide our shim everywhere */
#undef HAVE_DIRENT_H
#undef HAVE_SYS_DIR_H
#define NO_DIRENT 1

/* Ensure REGPARAM never expands to regparm(4) */
#ifdef REGPARAM
# undef REGPARAM
#endif
#define REGPARAM

/* Pull in our dirent shim for all TUs */
#include "include/amiga_dirent_shim.h"

/* Optional: quiet the ERROR_BAD_NUMBER re-define if needed */
#ifndef ERROR_BAD_NUMBER
# define ERROR_BAD_NUMBER 6
#endif

/* Map optional image helpers to no-ops to avoid external libs */


/* Amiga toolchains sometimes lack stricmp; use POSIX name */
#ifndef stricmp
# define stricmp strcasecmp
#endif
