/*
 * Minimal generic platform configuration for Berkeley SoftFloat Release 3e
 * on a 64-bit little-endian GCC host (x86-64 / AArch64). This is the build
 * config file the library expects; it is local to this vendored copy.
 * The oracle runner is single-threaded, so THREAD_LOCAL stays empty.
 */
#define LITTLEENDIAN 1
#define INLINE_LEVEL 5
#define INLINE static inline
#define SOFTFLOAT_FAST_INT64 1
#define THREAD_LOCAL
