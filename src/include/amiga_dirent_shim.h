/* Future Retro Fusion */
#ifndef AMIGA_DIRENT_SHIM_H
#define AMIGA_DIRENT_SHIM_H

/* If libc provides <dirent.h> (clib2/newlib), use it and adapt names. */
#if defined(__CLIB2__) || defined(__NEWLIB__) || defined(HAVE_DIRENT_H)
  #include <dirent.h>

  /* UAE legacy code sometimes uses struct direct; map it to dirent. */
  #ifndef direct
  #define direct dirent
  #endif

  /* Some code expects my_readdir(d, space). */
  #ifndef my_readdir
  #define my_readdir(dirstream, space) readdir(dirstream)
  #endif

/* Otherwise: tiny no-op shim for non-POSIX builds. */
#else
  typedef void *DIR;
  struct direct { char d_name[256]; long d_ino; };
  static inline DIR opendir(const char *path) { (void)path; return (DIR)0; }
  static inline struct direct *readdir(DIR d) { (void)d; return (struct direct*)0; }
  static inline void closedir(DIR d) { (void)d; }
  #ifndef my_readdir
  #define my_readdir(dirstream, space) readdir(dirstream)
  #endif
#endif

#endif /* AMIGA_DIRENT_SHIM_H */
