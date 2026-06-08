// lbt_model.h — Shared types and helpers for LBT instruction pseudocode
//
// This header defines the machine state model (GPRs, flags, scratch registers)
// used in per-instruction .h pseudocode files.  The same .h files serve as:
//   1. Human-readable documentation pseudocode
//   2. Executable C oracle for hardware verification
//
// Flag behavior metadata (in @-comments inside each .h) is parsed by
// gen_lbt_headers.py to produce documentation tables and drive the fuzzer.

#ifndef LBT_MODEL_H
#define LBT_MODEL_H

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// ── Machine state (set by harness before pseudocode execution) ────────────

extern uint64_t GPR[32];
extern uint64_t FPR[32];   // $f0..$f31 (lower 64 bits)
extern uint64_t SCRATCH[4]; // $scr0..$scr3
extern uint32_t X87_TOP;    // 3-bit x87 stack top
extern bool X87_TM;         // x87 task-switch marker bit
extern uint16_t X87_TAG;    // x87 FPU tag word

// ── Bitfield flag types ───────────────────────────────────────────────────

typedef struct {
    unsigned CF : 1;     // bit  0 — carry / borrow
    unsigned _r1 : 1;    // bit  1 — reserved, always 1
    unsigned PF : 1;     // bit  2 — parity (even parity of low 8)
    unsigned _r3 : 1;    // bit  3 — reserved
    unsigned AF : 1;     // bit  4 — auxiliary carry (bit 3→4)
    unsigned _r5 : 1;    // bit  5 — reserved
    unsigned ZF : 1;     // bit  6 — zero
    unsigned SF : 1;     // bit  7 — sign
    unsigned _r8_10 : 3; // bits 8-10 — reserved
    unsigned OF : 1;     // bit 11 — signed overflow
} x86_flags_t;

typedef struct {
    unsigned V : 1; // bit 28 — signed overflow
    unsigned C : 1; // bit 29 — carry / borrow
    unsigned Z : 1; // bit 30 — zero
    unsigned N : 1; // bit 31 — negative / sign
} arm_flags_t;

extern x86_flags_t EFLAGS;
extern arm_flags_t ARMFLAGS;

// ── Bit-mask accessors (for packed read/write to hardware) ───────────────

static inline uint32_t x86_pack_flags(x86_flags_t f) {
    return ((uint32_t)f.CF << 0) | ((uint32_t)1 << 1) | ((uint32_t)f.PF << 2) | ((uint32_t)f.AF << 4) |
           ((uint32_t)f.ZF << 6) | ((uint32_t)f.SF << 7) | ((uint32_t)f.OF << 11);
}

static inline x86_flags_t x86_unpack_flags(uint32_t raw) {
    return (x86_flags_t){
        .CF = (raw >> 0) & 1,
        .PF = (raw >> 2) & 1,
        .AF = (raw >> 4) & 1,
        .ZF = (raw >> 6) & 1,
        .SF = (raw >> 7) & 1,
        .OF = (raw >> 11) & 1,
    };
}

static inline uint32_t arm_pack_flags(arm_flags_t f) {
    return ((uint32_t)f.V << 28) | ((uint32_t)f.C << 29) | ((uint32_t)f.Z << 30) | ((uint32_t)f.N << 31);
}

static inline arm_flags_t arm_unpack_flags(uint32_t raw) {
    return (arm_flags_t){
        .V = (raw >> 28) & 1,
        .C = (raw >> 29) & 1,
        .Z = (raw >> 30) & 1,
        .N = (raw >> 31) & 1,
    };
}

// ── Hardware I/O helpers (used by verify harness) ────────────────────────

static inline void hw_write_x86_flags(x86_flags_t f) {
    uint64_t v = x86_pack_flags(f);
    __asm__ volatile("x86mtflag %0, 0x3f\n\t" : : "r"(v) : "memory");
}

static inline x86_flags_t hw_read_x86_flags(void) {
    uint64_t v;
    __asm__ volatile("x86mfflag %0, 0x3f\n\t" : "=r"(v) : : "memory");
    return x86_unpack_flags((uint32_t)v);
}

static inline void hw_write_arm_flags(arm_flags_t f) {
    uint64_t v = arm_pack_flags(f);
    __asm__ volatile("armmtflag %0, 0x39\n\t" : : "r"(v) : "memory");
}

static inline arm_flags_t hw_read_arm_flags(void) {
    uint64_t v;
    __asm__ volatile("armmfflag %0, 0x39\n\t" : "=r"(v) : : "memory");
    return arm_unpack_flags((uint32_t)v);
}

static inline void hw_write_x87_top(unsigned top) {
    switch (top & 7) {
    case 0:
        __asm__ volatile("x86mttop 0\n\t" : : : "memory");
        break;
    case 1:
        __asm__ volatile("x86mttop 1\n\t" : : : "memory");
        break;
    case 2:
        __asm__ volatile("x86mttop 2\n\t" : : : "memory");
        break;
    case 3:
        __asm__ volatile("x86mttop 3\n\t" : : : "memory");
        break;
    case 4:
        __asm__ volatile("x86mttop 4\n\t" : : : "memory");
        break;
    case 5:
        __asm__ volatile("x86mttop 5\n\t" : : : "memory");
        break;
    case 6:
        __asm__ volatile("x86mttop 6\n\t" : : : "memory");
        break;
    case 7:
        __asm__ volatile("x86mttop 7\n\t" : : : "memory");
        break;
    }
}

static inline unsigned hw_read_x87_top(void) {
    uint64_t v;
    __asm__ volatile("x86mftop %0\n\t" : "=r"(v) : : "memory");
    return (unsigned)v;
}

static inline void hw_write_x87_tm(bool tm) {
    if (tm) {
        __asm__ volatile("x86settm\n\t" : : : "memory");
    } else {
        __asm__ volatile("x86clrtm\n\t" : : : "memory");
    }
}

static inline bool hw_read_x87_tm(void) {
    uint64_t v;
    __asm__ volatile("movfcsr2gr %0, $fcsr0\n\t" : "=r"(v) : : "memory");
    return ((v >> 6) & 1) != 0;
}

// ── Pseudocode helper functions ───────────────────────────────────────────

static inline uint64_t truncate(uint64_t v, unsigned w) { return w >= 64 ? v : (v & ((UINT64_C(1) << w) - 1)); }

static inline uint64_t sext(uint64_t v, unsigned w) {
    if (w >= 64)
        return v;
    uint64_t sign = UINT64_C(1) << (w - 1);
    return ((v & ((sign << 1) - 1)) ^ sign) - sign;
}

static inline bool parity_even(uint8_t v) {
    v ^= v >> 4;
    v &= 0xf;
    return ((0x6996u >> v) & 1u) == 0;
}

// ── ARM condition evaluation ──────────────────────────────────────────────

static inline bool arm_cond_holds(arm_flags_t f, unsigned cond) {
    bool n = f.N, z = f.Z, c = f.C, v = f.V;
    switch (cond & 0xf) {
    case 0:
        return z; // EQ
    case 1:
        return !z; // NE
    case 2:
        return c; // CS/HS
    case 3:
        return !c; // CC/LO
    case 4:
        return n; // MI
    case 5:
        return !n; // PL
    case 6:
        return v; // VS
    case 7:
        return !v; // VC
    case 8:
        return c && !z; // HI
    case 9:
        return !c || z; // LS
    case 10:
        return n == v; // GE
    case 11:
        return n != v; // LT
    case 12:
        return !z && n == v; // GT
    case 13:
        return z || n != v; // LE
    case 14:
        return true; // AL
    case 15:
        return true; // AL (tested on LA464/LA664)
    default:
        return false;
    }
}

// ── x86 condition evaluation ─────────────────────────────────────────────

static inline bool x86_cond_holds(x86_flags_t f, unsigned cond) {
    bool cf = f.CF, pf = f.PF, zf = f.ZF, sf = f.SF, of = f.OF;
    switch (cond & 0xf) {
    case 0:
        return !cf && !zf; // A / NBE
    case 1:
        return !cf; // AE / NB / NC
    case 2:
        return cf; // B / C / NAE
    case 3:
        return cf || zf; // BE / NA
    case 4:
        return zf; // E / Z
    case 5:
        return !zf; // NE / NZ
    case 6:
        return !zf && sf == of; // G / NLE
    case 7:
        return sf == of; // GE / NL
    case 8:
        return sf != of; // L / NGE
    case 9:
        return zf || sf != of; // LE / NG
    case 10:
        return sf; // S
    case 11:
        return !sf; // NS
    case 12:
        return of; // O
    case 13:
        return !of; // NO
    case 14:
        return pf; // P / PE
    case 15:
        return !pf; // NP / PO
    default:
        return false;
    }
}

// ── Verify engine helpers ─────────────────────────────────────────────────

typedef struct {
    const char* name;
    int total;
    int failed;
} suite_t;

static inline void suite_init(suite_t* s, const char* name) {
    s->name = name;
    s->total = 0;
    s->failed = 0;
    printf("\n=== %s ===\n", name);
}

static inline void suite_log(suite_t* s, bool ok, const char* fmt, ...) {
    s->total++;
    if (!ok)
        s->failed++;
    printf("  [%s] ", ok ? "PASS" : "FAIL");
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    putchar('\n');
}

static inline int suite_finish(suite_t* s) {
    printf("--- %s: %d/%d passed ---\n", s->name, s->total - s->failed, s->total);
    return s->failed;
}

#define EXPECT_TRUE(s, cond, ...) suite_log(&(s), (cond), __VA_ARGS__)
#define EXPECT_EQ(s, a, b, ...) suite_log(&(s), (a) == (b), __VA_ARGS__)

// ── Flag behavior enum ────────────────────────────────────────────────────

typedef enum {
    FB_SKIP,         // instruction doesn't touch this flag group
    FB_COMPUTE,      // computed from operands (no dependency on init value)
    FB_READ_COMPUTE, // reads init value, then computes and writes
    FB_PRESERVE,     // unconditionally passes through init value
    FB_FORCE_0,      // unconditionally writes 0
    FB_FORCE_1,      // unconditionally writes 1
    FB_COND,         // conditional: COMPUTE when condition true, else PRESERVE
} flag_kind_t;

typedef struct {
    flag_kind_t kind;
    const char* note;
    const char* cond_desc; // for FB_COND: description of the condition
} flag_decl_t;

typedef struct {
    const char* inst_name;
    const char* synopsis;
    const char* category;
    flag_decl_t cf, pf, af, zf, sf, of; // x86
    flag_decl_t N, Z, C, V;             // ARM
    const char* global_cond;            // e.g. "count == 0 → all PRESERVE"
} inst_meta_t;

#endif // LBT_MODEL_H
