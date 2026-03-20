/* Future Retro Fusion */
#ifndef FORCE_AMIGA_INCLUDED
#define FORCE_AMIGA_INCLUDED
/* force_amiga.h — injected via -include
   Only set a feature flag here. Do NOT include drawing.h from here. */

#ifndef UAE_FORCE_RGB444
#define UAE_FORCE_RGB444 1   /* 1 = force 12-bit clamp in drawing.h, 0 = normal */
#endif

/* Make sure the Amiga path in filesys.c is used, regardless of configure results */
#ifdef HAVE_DIRENT_H
#undef HAVE_DIRENT_H
#endif
#ifdef HAVE_SYS_DIR_H
#undef HAVE_SYS_DIR_H
#endif
#ifndef NO_DIRENT
#define NO_DIRENT 1
#endif
#endif /* FORCE_AMIGA_INCLUDED */
