/* Future Retro Fusion */
#ifdef REGPARAM
#undef REGPARAM
#endif
#define REGPARAM
/* --- Branch prediction hints (used by generated cpuemu_x.c and others) --- */
#ifndef UAE_LIKELY
#  if defined(__GNUC__) || defined(__clang__)
#    define UAE_LIKELY(x)   __builtin_expect(!!(x), 1)
#    define UAE_UNLIKELY(x) __builtin_expect(!!(x), 0)
#  else
#    define UAE_LIKELY(x)   (x)
#    define UAE_UNLIKELY(x) (x)
#  endif
#endif
