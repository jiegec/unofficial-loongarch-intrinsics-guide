// lbt_verify.c — LBT flag verification framework.
// Model wrappers and test cases are auto-generated in gen_verify.c.
#include "lbt_model.h"
#include <stdio.h>
#include <string.h>

uint64_t  GPR[32];
uint64_t  FPR[32];
uint64_t  SCRATCH[4];
uint32_t  X87_TOP;
bool      X87_TM;
uint16_t  X87_TAG;
x86_flags_t EFLAGS;
arm_flags_t ARMFLAGS;

static int g_total, g_failed;

static void check_x86(const char *name, x86_flags_t model, x86_flags_t hw) {
    g_total++;
    uint32_t m = x86_pack_flags(model), h = x86_pack_flags(hw);
    int ok = (m & 0x8d5) == (h & 0x8d5);
    printf("  [%s] %-35s model=0x%03x hw=0x%03x\n",
           ok ? "PASS" : "FAIL", name, m & 0x8d5, h & 0x8d5);
    if (!ok) g_failed++;
}

static void check_arm(const char *name, arm_flags_t model, arm_flags_t hw) {
    g_total++;
    uint32_t m = arm_pack_flags(model), h = arm_pack_flags(hw);
    int ok = (m & 0xF0000000) == (h & 0xF0000000);
    printf("  [%s] %-35s model=0x%08x hw=0x%08x\n",
           ok ? "PASS" : "FAIL", name, m, h);
    if (!ok) g_failed++;
}

static void check_u64(const char *name, const char *field,
                      uint64_t model, uint64_t hw) {
    g_total++;
    int ok = model == hw;
    printf("  [%s] %-35s %-8s model=0x%016" PRIx64 " hw=0x%016" PRIx64 "\n",
           ok ? "PASS" : "FAIL", name, field, model, hw);
    if (!ok) g_failed++;
}

static void check_u32(const char *name, const char *field,
                      uint32_t model, uint32_t hw) {
    g_total++;
    int ok = model == hw;
    printf("  [%s] %-35s %-8s model=0x%08x hw=0x%08x\n",
           ok ? "PASS" : "FAIL", name, field, model, hw);
    if (!ok) g_failed++;
}

static void check_bool(const char *name, const char *field,
                       bool model, bool hw) {
    g_total++;
    int ok = model == hw;
    printf("  [%s] %-35s %-8s model=%u hw=%u\n",
           ok ? "PASS" : "FAIL", name, field,
           (unsigned)model, (unsigned)hw);
    if (!ok) g_failed++;
}

static void model_prepare(uint64_t rj_val, uint64_t rk_val, uint64_t scratch0,
                          x86_flags_t x86_flags, arm_flags_t arm_flags,
                          unsigned top, bool tm) {
    memset(GPR, 0, sizeof(GPR));
    memset(SCRATCH, 0, sizeof(SCRATCH));
    GPR[12] = rj_val;
    GPR[13] = rk_val;
    SCRATCH[0] = scratch0;
    EFLAGS = x86_flags;
    ARMFLAGS = arm_flags;
    X87_TOP = top & 7;
    X87_TM = tm;
}

static void hw_prepare_x86_last(x86_flags_t x86_flags, arm_flags_t arm_flags,
                                unsigned top, bool tm) {
    hw_write_x87_top(top & 7);
    hw_write_x87_tm(tm);
    hw_write_arm_flags(arm_flags);
    hw_write_x86_flags(x86_flags);
}

static void hw_prepare_arm_last(x86_flags_t x86_flags, arm_flags_t arm_flags,
                                unsigned top, bool tm) {
    hw_write_x87_top(top & 7);
    hw_write_x87_tm(tm);
    hw_write_x86_flags(x86_flags);
    hw_write_arm_flags(arm_flags);
}

static void hw_prepare_x87_last(x86_flags_t x86_flags, arm_flags_t arm_flags,
                                unsigned top, bool tm) {
    hw_write_x86_flags(x86_flags);
    hw_write_arm_flags(arm_flags);
    hw_write_x87_top(top & 7);
    hw_write_x87_tm(tm);
}

// ── Verification macros ──────────────────────────────────────────────────

#define T_X86(name, asm_text, rj_val, rk_val, init_ef, model_fn) do { \
    EFLAGS = init_ef; GPR[12] = rj_val; GPR[13] = rk_val; \
    model_fn(); \
    x86_flags_t m = EFLAGS; \
    EFLAGS = init_ef; uint64_t a = rj_val, b = rk_val; \
    hw_write_x86_flags(init_ef); \
    __asm__ volatile(asm_text : "+r"(a), "+r"(b) : : "memory"); \
    check_x86(name, m, hw_read_x86_flags()); \
} while(0)

#define T_ARM(name, asm_text, rj_val, rk_val, init_arm, model_fn) do { \
    ARMFLAGS = init_arm; GPR[12] = rj_val; GPR[13] = rk_val; \
    model_fn(); \
    arm_flags_t m = ARMFLAGS; \
    ARMFLAGS = init_arm; uint64_t a = rj_val, b = rk_val; \
    hw_write_arm_flags(init_arm); \
    __asm__ volatile(asm_text : "+r"(a), "+r"(b) : : "memory"); \
    check_arm(name, m, hw_read_arm_flags()); \
} while(0)

// ── Generated test cases ─────────────────────────────────────────────────
#include "gen_verify.c"
#ifdef LBT_WITH_PROBE
#include "gen_probe_lbt.c"
#endif

// ── Main ─────────────────────────────────────────────────────────────────
int main(int argc, char **argv) {
    bool run_probe = argc > 1 && strcmp(argv[1], "--probe") == 0;
    printf("=== LBT Verify: .h model vs hardware ===\n");
    gen_verify_main();

    if (run_probe) {
#ifdef LBT_WITH_PROBE
        printf("\n=== LBT Probe: undefined flag discovery ===\n");
        gen_probe_main();
#else
        fprintf(stderr, "lbt_verify was built without probe support; use make lbt_probe\n");
        return 2;
#endif
    }

    printf("\n=== Summary ===\n");
    printf("  %d tests, %d passed, %d FAILED\n", g_total, g_total - g_failed, g_failed);
    return g_failed ? 1 : 0;
}
