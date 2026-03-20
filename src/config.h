/* Future Retro Fusion */
#ifndef CONFIG_H
#define CONFIG_H

/* m68k-amigaos (big-endian, 32-bit ptrs) */
#define SIZEOF_CHAR 1
#define SIZEOF_SHORT 2
#define SIZEOF_INT 4
#define SIZEOF_LONG 4
#define SIZEOF_LONG_LONG 8
#define SIZEOF_VOID_P 4

/* Endianness */
#define WORDS_BIGENDIAN 1

/* Common headers available under clib2 */
#define HAVE_STRING_H 1
#define HAVE_STRINGS_H 1
#define HAVE_STDINT_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_DIRENT_H 1

#endif /* CONFIG_H */
