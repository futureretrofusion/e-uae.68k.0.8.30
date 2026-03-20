/* Future Retro Fusion */
#ifndef AMIGA_PATHFIX_H
#define AMIGA_PATHFIX_H

#include <string.h>
#include <stddef.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <utime.h>
#include <limits.h>

/* tiny local strlcpy */
static inline void apf_strlcpy(char *dst, const char *src, size_t n) {
    if (!n) return;
    size_t i = 0;
    while (i + 1 < n && src[i]) { dst[i] = src[i]; ++i; }
    dst[i] = 0;
}

/* Convert "VOL:dir/sub" -> "/VOL/dir/sub" (used only on Amiga builds) */
static inline void apf_amiga_to_posix_path(const char *in, char *out, size_t outsz) {
    const char *colon = strchr(in, ':');
    if (!colon) { apf_strlcpy(out, in, outsz); return; }
    size_t vol = (size_t)(colon - in);
    if (outsz < vol + 2) { if (outsz) out[0] = 0; return; }
    out[0] = '/';
    memcpy(out + 1, in, vol);
    out[1 + vol] = '/';
    const char *rest = colon + 1;
    while (*rest == '/') rest++;
    size_t j = 2 + vol;
    while (*rest && j + 1 < outsz) {
        char c = *rest++;
        out[j++] = (c == '\\') ? '/' : c;
    }
    out[j] = 0;
}

#if defined(__AMIGA__) || defined(AMIGA) || defined(__amigaos__) || defined(__AMIGAOS__)
#  define APF_FIX(in, buf) (apf_amiga_to_posix_path((in), (buf), sizeof(buf)), (buf))
#else
#  define APF_FIX(in, buf) (in)
#endif

/* --- thin wrappers for common POSIX calls (auto-fix path) --- */
static inline int  apf_stat     (const char *p, struct stat *st)       { char b[1024]; return stat    (APF_FIX(p,b), st); }
static inline int  apf_lstat    (const char *p, struct stat *st)       { char b[1024]; return lstat   (APF_FIX(p,b), st); }
static inline int  apf_chmod    (const char *p, mode_t m)              { char b[1024]; return chmod   (APF_FIX(p,b), m); }
static inline int  apf_unlink   (const char *p)                        { char b[1024]; return unlink  (APF_FIX(p,b)); }
static inline int  apf_access   (const char *p, int mode)              { char b[1024]; return access  (APF_FIX(p,b), mode); }
static inline int  apf_truncate (const char *p, off_t len)             { char b[1024]; return truncate(APF_FIX(p,b), len); }
static inline int  apf_utime    (const char *p, const struct utimbuf *t){char b[1024]; return utime   (APF_FIX(p,b), t); }
static inline int  apf_mkdir    (const char *p, mode_t m)              { char b[1024]; return mkdir   (APF_FIX(p,b), m); }
static inline int  apf_rmdir    (const char *p)                        { char b[1024]; return rmdir   (APF_FIX(p,b)); }
static inline DIR* apf_opendir  (const char *p)                        { char b[1024]; return opendir (APF_FIX(p,b)); }
static inline FILE*apf_fopen    (const char *p, const char *mode)      { char b[1024]; return fopen   (APF_FIX(p,b), mode); }
static inline int  apf_rename   (const char *a, const char *b2)        { char bA[1024], bB[1024]; return rename(APF_FIX(a,bA), APF_FIX(b2,bB)); }
static inline int  apf_link     (const char *a, const char *b2)        { char bA[1024], bB[1024]; return link  (APF_FIX(a,bA), APF_FIX(b2,bB)); }
static inline char*apf_realpath (const char *p, char *r)               { char b[PATH_MAX]; return realpath(APF_FIX(p,b), r); }

/* open/creat need varargs; use a GNU statement-expression macro to preserve arity */
#define APF_OPEN(path, flags, ...) ({ char _b[1024]; open (APF_FIX((path), _b), (flags), ##__VA_ARGS__); })
#define APF_CREAT(path, mode)      ({ char _b[1024]; creat(APF_FIX((path), _b), (mode)); })

#endif /* AMIGA_PATHFIX_H */
