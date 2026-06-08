// x86_flag_verify.c — Verify .h models vs real x86_64, fixed flag init
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct { unsigned CF:1,_r1:1,PF:1,_p:1,AF:1,_r5:1,ZF:1,SF:1; unsigned _r:3,OF:1; } xf_t;
typedef struct { uint64_t gpr[32]; xf_t efl; } ms_t;
static ms_t M;
#define GPR M.gpr
#define EFLAGS M.efl
static int pe(uint8_t v){v^=v>>4;v&=0xf;return((0x6996u>>v)&1u)==0;}
#define parity_even pe
static uint32_t pk(xf_t f){return f.CF|(1u<<1)|(f.PF<<2)|(f.AF<<4)|(f.ZF<<6)|(f.SF<<7)|(f.OF<<11);}

// Set x86 flags to specified value, preserving reserved bits
static void wrfl(uint32_t f) {
    uint64_t v;
    __asm__ volatile(
        "pushfq; popq %0;"
        "andl $0xfffff72a, %k0;"    // clear writable flags, keep reserved
        "orl %k1, %k0;"
        "pushq %0; popfq"
        : "=&a"(v) : "r"(f & 0x8d5) : "cc","memory");
}
static uint32_t rdfl(void) {
    uint64_t f;
    __asm__ volatile("pushfq; popq %0" : "=r"(f) :: "memory");
    return (uint32_t)f;
}

// ── Model wrappers ──────────────────────────────────────────────────────
static void m_x86add_b(void){const unsigned rj=12,rk=13;
#include "x86add_b.h"
}
static void m_x86add_w(void){const unsigned rj=12,rk=13;
#include "x86add_w.h"
}
static void m_x86add_d(void){const unsigned rj=12,rk=13;
#include "x86add_d.h"
}
static void m_x86adc_b(void){const unsigned rj=12,rk=13;
#include "x86adc_b.h"
}
static void m_x86sub_w(void){const unsigned rj=12,rk=13;
#include "x86sub_w.h"
}
static void m_x86and_b(void){const unsigned rj=12,rk=13;
#include "x86and_b.h"
}
static void m_x86or_w(void){const unsigned rj=12,rk=13;
#include "x86or_w.h"
}
static void m_x86xor_w(void){const unsigned rj=12,rk=13;
#include "x86xor_w.h"
}
static void m_x86inc_b(void){const unsigned rj=12,rk=13;
#include "x86inc_b.h"
}
static void m_x86dec_w(void){const unsigned rj=12,rk=13;
#include "x86dec_w.h"
}
static void m_x86mul_w(void){const unsigned rj=12,rk=13;
#include "x86mul_w.h"
}
static void m_x86sll_w(void){const unsigned rj=12,rk=13;
#include "x86sll_w.h"
}
static void m_x86srl_w(void){const unsigned rj=12,rk=13;
#include "x86srl_w.h"
}
static void m_x86sra_w(void){const unsigned rj=12,rk=13;
#include "x86sra_w.h"
}
static void m_x86rotl_w(void){const unsigned rj=12,rk=13;
#include "x86rotl_w.h"
}
static void m_x86rotr_w(void){const unsigned rj=12,rk=13;
#include "x86rotr_w.h"
}

static int g_total,g_failed;
typedef void (*mfn_t)(void);

#define T(nm, fn, a, b, ief) do { \
    M.efl.CF=(ief>>0)&1;M.efl.PF=(ief>>2)&1;M.efl.AF=(ief>>4)&1; \
    M.efl.ZF=(ief>>6)&1;M.efl.SF=(ief>>7)&1;M.efl.OF=(ief>>11)&1; \
    M.gpr[12]=a;M.gpr[13]=b;fn();uint32_t m=pk(M.efl)&0x8d5; \
    wrfl(ief); \
    uint64_t dx=a,cx=b; \
    if(0){} \
    else if(!strcmp(nm,"x86add_b")) __asm__("addb %%cl,%%dl":"+d"(dx):"c"(cx):"cc"); \
    else if(!strcmp(nm,"x86add_w")) __asm__("addl %%ecx,%%edx":"+d"(dx):"c"(cx):"cc"); \
    else if(!strcmp(nm,"x86add_d")) __asm__("addq %%rcx,%%rdx":"+d"(dx):"c"(cx):"cc"); \
    else if(!strcmp(nm,"x86adc_b")) __asm__("adcb %%cl,%%dl":"+d"(dx):"c"(cx):"cc"); \
    else if(!strcmp(nm,"x86sub_w")) __asm__("subl %%ecx,%%edx":"+d"(dx):"c"(cx):"cc"); \
    else if(!strcmp(nm,"x86and_b")) __asm__("andb %%cl,%%dl":"+d"(dx):"c"(cx):"cc"); \
    else if(!strcmp(nm,"x86or_w"))  __asm__("orl %%ecx,%%edx":"+d"(dx):"c"(cx):"cc"); \
    else if(!strcmp(nm,"x86xor_w")) __asm__("xorl %%ecx,%%edx":"+d"(dx):"c"(cx):"cc"); \
    else if(!strcmp(nm,"x86inc_b")) __asm__("incb %%dl":"+d"(dx)::"cc"); \
    else if(!strcmp(nm,"x86dec_w")) __asm__("decl %%edx":"+d"(dx)::"cc"); \
    else if(!strcmp(nm,"x86mul_w")) __asm__("imull %%ecx,%%edx":"+d"(dx):"c"(cx):"cc"); \
    else if(!strcmp(nm,"x86sll_w")) __asm__("shll %%cl,%%edx":"+d"(dx):"c"(cx):"cc"); \
    else if(!strcmp(nm,"x86srl_w")) __asm__("shrl %%cl,%%edx":"+d"(dx):"c"(cx):"cc"); \
    else if(!strcmp(nm,"x86sra_w")) __asm__("sarl %%cl,%%edx":"+d"(dx):"c"(cx):"cc"); \
    else if(!strcmp(nm,"x86rotl_w"))__asm__("roll %%cl,%%edx":"+d"(dx):"c"(cx):"cc"); \
    else if(!strcmp(nm,"x86rotr_w"))__asm__("rorl %%cl,%%edx":"+d"(dx):"c"(cx):"cc"); \
    uint32_t h=rdfl()&0x8d5; \
    g_total++;int ok=(m==h);if(!ok)g_failed++; \
    printf("  [%s] %-30s model=0x%03x hw=0x%03x\n",ok?"PASS":"FAIL",nm,m,h); \
} while(0)

int main(void){
    printf("x86 Flag Verify — .h models vs x86_64 hardware\n");
    printf("===============================================\n\n");

    printf("--- x86 add ---\n");
    T("x86add_b",m_x86add_b,0,0,0); T("x86add_b",m_x86add_b,0xff,1,0); T("x86add_b",m_x86add_b,0x7f,1,0);
    T("x86add_w",m_x86add_w,0xffffffff,1,0); T("x86add_w",m_x86add_w,0x7fffffff,1,0);
    T("x86add_d",m_x86add_d,0x7fffffffffffffffULL,1,0);
    T("x86adc_b",m_x86adc_b,0,0,0x8d5); T("x86adc_b",m_x86adc_b,0xff,1,0x8d5);
    T("x86sub_w",m_x86sub_w,0,0,0); T("x86sub_w",m_x86sub_w,0,1,0);

    printf("\n--- x86 logic ---\n");
    T("x86and_b",m_x86and_b,0xFF,0x0F,0); T("x86or_w",m_x86or_w,0,0,0x8d5);
    T("x86xor_w",m_x86xor_w,0xFFFF,0xAAAA,0x8d5);

    printf("\n--- x86 inc/dec ---\n");
    T("x86inc_b",m_x86inc_b,0x7F,0,0); T("x86dec_w",m_x86dec_w,1,0,0);

    printf("\n--- x86 mul ---\n");
    T("x86mul_w",m_x86mul_w,2,2,0); T("x86mul_w",m_x86mul_w,0x8000,2,0);

    printf("\n--- x86 shift/rotate ---\n");
    T("x86sll_w",m_x86sll_w,1,1,0); T("x86srl_w",m_x86srl_w,1,1,0); T("x86sra_w",m_x86sra_w,0x80000000,1,0);
    T("x86rotl_w",m_x86rotl_w,0x12345678,1,0); T("x86rotr_w",m_x86rotr_w,1,1,0);

    printf("\n===============================================\n");
    printf("  TOTAL: %d  PASS: %d  FAIL: %d\n",g_total,g_total-g_failed,g_failed);
    return g_failed?1:0;
}
