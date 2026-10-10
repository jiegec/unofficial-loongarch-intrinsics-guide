#pragma once

struct FpPolicy;

#include "softfloat.hpp"

#include <cstdint>
#include <vector>

namespace lgpu::test {

struct AddU32Result {
  std::uint32_t sum;
  bool carry;
};

struct AddCU32Result {
  std::uint32_t sum;
  bool carry;
};struct SubU32Result {
  std::uint32_t diff;
  bool borrow;
};

AddU32Result oracle_v_add_u32(std::uint32_t a, std::uint32_t b);
AddCU32Result oracle_v_addc_u32(std::uint32_t a, std::uint32_t b, std::uint32_t cin);
SubU32Result oracle_v_sub_u32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_v_and_b32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_v_or_b32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_v_xor_b32(std::uint32_t a, std::uint32_t b);

std::uint32_t oracle_v_add_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_v_sub_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_v_abs_i32(std::uint32_t a);
std::uint32_t oracle_v_clz_u32(std::uint32_t a);
std::uint32_t oracle_v_bcnt1_u32(std::uint32_t a);
std::uint32_t oracle_s_add_u32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_v_fcvt_f32_u32(std::uint32_t a);
std::uint32_t oracle_v_pack_f16rtz_f32(std::uint32_t a, std::uint32_t b);

// Integer/scalar family. Shift counts use the low five bits. Signed
// arithmetic is two's-complement bit-exact.
std::uint32_t oracle_sll_b32(std::uint32_t a, std::uint32_t n);
std::uint32_t oracle_srl_b32(std::uint32_t a, std::uint32_t n);
std::uint32_t oracle_sra_b32(std::uint32_t a, std::uint32_t n);
std::uint32_t oracle_rotr_b32(std::uint32_t a, std::uint32_t n);
std::uint32_t oracle_bfrev_b32(std::uint32_t a);
std::uint32_t oracle_bfm_b32(std::uint32_t mask, std::uint32_t offset);
std::uint32_t oracle_bfe_u32(std::uint32_t a, std::uint32_t offset, std::uint32_t width);
std::uint32_t oracle_bfe_i32(std::uint32_t a, std::uint32_t offset, std::uint32_t width);
std::uint32_t oracle_bitalign_b32(std::uint32_t lo, std::uint32_t hi, std::uint32_t n);
std::uint32_t oracle_bytealign_b32(std::uint32_t lo, std::uint32_t hi, std::uint32_t n);
std::uint32_t oracle_byteperm_b32(std::uint32_t a, std::uint32_t b, std::uint32_t sel);
std::uint32_t oracle_min_u32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_max_u32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_min_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_max_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_mul_u32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_mul_hi_u32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_mul_hi_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_mul24_u32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_mul24_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_mul24_hi_u32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_mul24_hi_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_absdiff_u32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_absdiff_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_subrev_u32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_not_b32(std::uint32_t a);
std::uint32_t oracle_nand_b32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_nor_b32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_xnor_b32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_andn1_b32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_andn2_b32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_orn1_b32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_orn2_b32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_ineg_i32(std::uint32_t a);
std::uint32_t oracle_ctz_u32(std::uint32_t a);
std::uint32_t oracle_clo_u32(std::uint32_t a);
std::uint32_t oracle_cto_u32(std::uint32_t a);
std::uint32_t oracle_clb_u32(std::uint32_t a);
std::uint32_t oracle_bcnt0_u32(std::uint32_t a);
std::uint32_t oracle_pack_u8_u32(std::uint32_t a, std::uint32_t b,
                               std::uint32_t c);
std::uint32_t oracle_pack_i8_i32(std::uint32_t a, std::uint32_t b,
                                 std::uint32_t c);
std::uint32_t oracle_pack_u16_u32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_pack_i16_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_sext_i32_i8(std::uint32_t a);
std::uint32_t oracle_sext_i32_i16(std::uint32_t a);
std::uint32_t oracle_bitset0_b32(std::uint32_t a, std::uint32_t bit);
std::uint32_t oracle_bitset1_b32(std::uint32_t a, std::uint32_t bit);
std::uint32_t oracle_min3_u32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t oracle_max3_u32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t oracle_med3_u32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t oracle_min3_i32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t oracle_max3_i32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t oracle_med3_i32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t oracle_passthrough(std::uint32_t a);
std::uint32_t oracle_v_mma(std::uint32_t a, std::uint32_t b, std::uint32_t c,
                           bool signed_bytes);
// v_interp_1_f32 dst = s1 * (s0 + 1) in f32.
std::uint32_t oracle_v_interp_1_f32(std::uint32_t s0, std::uint32_t s1);
std::uint64_t oracle_passthrough64(std::uint64_t a);
std::uint32_t oracle_cubema(std::uint32_t s0, std::uint32_t s1, std::uint32_t s2);
std::uint32_t oracle_cubeface(std::uint32_t s0, std::uint32_t s1, std::uint32_t s2);
std::uint32_t oracle_cubes(std::uint32_t s0, std::uint32_t s1, std::uint32_t s2);
std::uint32_t oracle_cubet(std::uint32_t s0, std::uint32_t s1, std::uint32_t s2);
std::uint32_t oracle_bitcmprev0(std::uint32_t a, std::uint32_t bit);
std::uint32_t oracle_bitcmprev1(std::uint32_t a, std::uint32_t bit);
std::uint32_t oracle_quadmask(std::uint32_t a);
std::uint32_t oracle_wqm(std::uint32_t a);
std::uint32_t oracle_s_select_b32(std::uint32_t scc, std::uint32_t a,
                                  std::uint32_t b);
std::uint32_t oracle_sllrev_b32(std::uint32_t count, std::uint32_t data);
std::uint32_t oracle_srlrev_b32(std::uint32_t count, std::uint32_t data);
std::uint32_t oracle_srarev_b32(std::uint32_t count, std::uint32_t data);
std::uint32_t oracle_sub_u32_value(std::uint32_t a, std::uint32_t b);

// v_cmp / s_cmp predicate family. All FP oracles are bit-exact and
// host-FP-mode independent (sign-magnitude integer comparison of the IEEE
// bit patterns; NaN is unordered; +0 == -0). 16-bit operands use the low
// low 16 bits of each 32-bit operand; high bits are ignored.
std::uint32_t oracle_cmp_eq_f16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_eq_f32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_eq_i16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_eq_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_eq_u16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_eq_u32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_ne_u32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_lt_u32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_le_u32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_gt_u32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_ge_u32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_lt_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_le_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_gt_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_ge_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_lt_u16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_le_u16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_gt_u16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_ge_u16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_lt_i16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_le_i16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_gt_i16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_ge_i16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_neq_u16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_neq_i16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_neq_u32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_neq_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_neq_f16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_neq_f32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_lt_f16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_lt_f32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_le_f16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_le_f32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_gt_f16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_gt_f32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_ge_f16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_ge_f32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_lg_f16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_lg_f32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_nlt_f16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_nlt_f32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_nle_f16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_nle_f32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_ngt_f16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_ngt_f32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_ngt_f32_std(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_nle_f32_std(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_nge_f16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_nge_f32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_nlg_f16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_nlg_f32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_o_f16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_o_f32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_u_f16(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_u_f32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_tru(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmp_fals(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_cmpclass_f16(std::uint32_t mask, std::uint32_t v);
std::uint32_t oracle_cmpclass_f32(std::uint32_t mask, std::uint32_t v);

// 64-bit pair64 "both" oracles: return 1/0 as a u64 dword pair.
std::uint32_t oracle_cmp_eq_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_eq_i64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_eq_u64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_ge_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_ge_i64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_ge_u64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_gt_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_gt_i64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_gt_u64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_le_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_le_i64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_le_u64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_lt_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_lt_i64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_lt_u64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_neq_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_neq_i64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_neq_u64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_lg_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_nlt_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_nle_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_ngt_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_nge_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_nlg_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_o_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_u_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_tru(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_fals(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint64_t oracle_cmpclass_f64(std::uint64_t mask, std::uint64_t v);

// Scalar s_cmp pair64 "in" oracles: (lo, hi) input pairs, single-dword
// result; the FP policy argument is carried by the wrapper but unused.
std::uint32_t oracle_cmp_eq_u64(std::uint64_t a, std::uint64_t b,
                                const FpPolicy& policy);
std::uint32_t oracle_cmp_ne_u64(std::uint64_t a, std::uint64_t b,
                                const FpPolicy& policy);

// Scalar carry/borrow/rotate forms (SCC carry-out not modeled; the third
// input is the deterministic SCC carry/borrow-in state).
std::uint32_t oracle_s_addc_u32(std::uint32_t src, std::uint32_t imm,
                                std::uint32_t carry_in);
std::uint32_t oracle_s_subb_u32(std::uint32_t src, std::uint32_t imm,
                                std::uint32_t borrow_in);
std::uint32_t oracle_s_subbrev_u32(std::uint32_t src, std::uint32_t imm,
                                   std::uint32_t borrow_in);
std::uint32_t oracle_rotrrev_b32(std::uint32_t count, std::uint32_t data);

// Atomic RMW oracles (sm/buff/flat/tex): all return the old value read back;
// the memory side effect is modeled per the operation text in the registry
// entry. 64-bit forms take (lo, hi) pairs as u64 values.

// 32-bit binary RMW.
std::uint32_t oracle_sm_add_u32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_sm_and_b32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_sm_dec_u32(std::uint32_t old, std::uint32_t bound);
std::uint32_t oracle_sm_inc_u32(std::uint32_t old, std::uint32_t bound);
std::uint32_t oracle_sm_max_i32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_sm_max_u32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_sm_min_i32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_sm_min_u32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_sm_or_b32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_sm_rsub_u32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_sm_sub_u32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_sm_swap_b32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_sm_xor_b32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_buff_cmpswap_b32(std::uint32_t old, std::uint32_t cmp,
                                        std::uint32_t swap);
std::uint32_t oracle_v_buff_dec_u32(std::uint32_t old, std::uint32_t bound);
std::uint32_t oracle_v_buff_inc_u32(std::uint32_t old, std::uint32_t bound);
std::uint32_t oracle_v_buff_sub_i32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_flat_add_i32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_flat_and_b32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_flat_cmpswap_b32(std::uint32_t old, std::uint32_t cmp,
                                        std::uint32_t swap);
std::uint32_t oracle_v_flat_dec_u32(std::uint32_t old, std::uint32_t bound);
std::uint32_t oracle_v_flat_inc_u32(std::uint32_t old, std::uint32_t bound);
std::uint32_t oracle_v_flat_max_i32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_flat_max_u32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_flat_min_i32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_flat_min_u32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_flat_or_b32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_flat_sub_i32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_flat_swap_b32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_flat_xor_b32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_tex_add_i32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_tex_and_b32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_tex_cmpswap_b32(std::uint32_t old, std::uint32_t cmp,
                                       std::uint32_t swap);
std::uint32_t oracle_v_tex_dec_u32(std::uint32_t old, std::uint32_t bound);
std::uint32_t oracle_v_tex_inc_u32(std::uint32_t old, std::uint32_t bound);
std::uint32_t oracle_v_tex_max_i32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_tex_max_u32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_tex_min_i32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_tex_min_u32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_tex_or_b32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_tex_sub_i32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_tex_swap_b32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_tex_xor_b32(std::uint32_t old, std::uint32_t src);

// 32-bit conditional RMW; floating NaN comparisons are false.
std::uint32_t oracle_sm_cmpswap_eq_u32(std::uint32_t old, std::uint32_t cmp,
                                       std::uint32_t swap);
std::uint32_t oracle_sm_cmpswap_gt_f32(std::uint32_t old, std::uint32_t data);
std::uint32_t oracle_sm_cmpswap_lt_f32(std::uint32_t old, std::uint32_t data);

// 64-bit binary RMW.
std::uint64_t oracle_sm_add_u64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_sm_and_b64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_sm_dec_u64(std::uint64_t old, std::uint64_t bound);
std::uint64_t oracle_sm_inc_u64(std::uint64_t old, std::uint64_t bound);
std::uint64_t oracle_sm_max_i64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_sm_max_u64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_sm_min_i64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_sm_min_u64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_sm_or_b64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_sm_rsub_u64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_sm_sub_u64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_sm_swap_b64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_sm_xor_b64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_v_buff_add_i64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_v_buff_and_b64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_v_buff_cmpswap_b64(std::uint64_t old, std::uint64_t cmp,
                                        std::uint64_t swap);
std::uint64_t oracle_v_buff_dec_u64(std::uint64_t old, std::uint64_t bound);
std::uint64_t oracle_v_buff_inc_u64(std::uint64_t old, std::uint64_t bound);
std::uint64_t oracle_v_buff_max_i64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_v_buff_max_u64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_v_buff_min_i64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_v_buff_min_u64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_v_buff_or_b64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_v_buff_sub_i64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_v_buff_swap_b64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_v_buff_xor_b64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_v_flat_add_i64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_v_flat_and_b64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_v_flat_cmpswap_b64(std::uint64_t old, std::uint64_t cmp,
                                        std::uint64_t swap);
std::uint64_t oracle_v_flat_dec_u64(std::uint64_t old, std::uint64_t bound);
std::uint64_t oracle_v_flat_inc_u64(std::uint64_t old, std::uint64_t bound);
std::uint64_t oracle_v_flat_max_i64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_v_flat_max_u64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_v_flat_min_i64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_v_flat_min_u64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_v_flat_or_b64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_v_flat_sub_i64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_v_flat_swap_b64(std::uint64_t old, std::uint64_t src);
std::uint64_t oracle_v_flat_xor_b64(std::uint64_t old, std::uint64_t src);

// 64-bit conditional RMW.
std::uint64_t oracle_sm_cmpswap_eq_u64(std::uint64_t old, std::uint64_t cmp,
                                       std::uint64_t swap);
std::uint64_t oracle_sm_cmpswap_gt_f64(std::uint64_t old, std::uint64_t data);
std::uint64_t oracle_sm_cmpswap_lt_f64(std::uint64_t old, std::uint64_t data);

std::uint32_t oracle_atomic_v_buff_add_i32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_atomic_v_buff_and_b32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_atomic_v_buff_max_i32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_atomic_v_buff_max_u32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_atomic_v_buff_min_i32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_atomic_v_buff_min_u32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_atomic_v_buff_or_b32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_atomic_v_buff_swap_b32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_atomic_v_buff_xor_b32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_bfi_b32(std::uint32_t a, std::uint32_t b, std::uint32_t mask);
std::uint32_t oracle_bitcmp0_b32(std::uint32_t a, std::uint32_t bit);
std::uint32_t oracle_bitcmp1_b32(std::uint32_t a, std::uint32_t bit);
std::uint32_t oracle_dp4a_i8_i8_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_dp4a_i8_u8_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_dp4a_u8_i8_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_dp4a_u8_u8_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_fcvt_f16_i16(std::uint32_t a);
std::uint32_t oracle_fcvt_f16_u16(std::uint32_t a);
std::uint32_t oracle_fcvt_f32_u80(std::uint32_t a);
std::uint32_t oracle_fcvt_f32_u81(std::uint32_t a);
std::uint32_t oracle_fcvt_f32_u82(std::uint32_t a);
std::uint32_t oracle_fcvt_f32_u83(std::uint32_t a);
std::uint32_t oracle_fcvt_f32off_i4(std::uint32_t a);
std::uint32_t oracle_fcvt_i16_f16(std::uint32_t a);
std::uint32_t oracle_fcvt_i32flr_f32(std::uint32_t a);
std::uint32_t oracle_fcvt_i32rpi_f32(std::uint32_t a);
std::uint32_t oracle_fcvt_u16_f16(std::uint32_t a);
std::uint32_t oracle_frexp_exp_i32_f32(std::uint32_t a);
std::uint32_t oracle_frexp_exp_i32_f64(std::uint64_t a);
std::uint32_t oracle_frexp_mant_f32(std::uint32_t a);
std::uint32_t oracle_mad24_i32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t oracle_mad24_u32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t oracle_mad_i32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t oracle_mad_u32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t oracle_mbcnt_u32(std::uint32_t v, std::uint32_t mask);
std::uint32_t oracle_movindex_b32(std::uint32_t a, std::uint32_t);
std::uint32_t oracle_pack_snorm16_f32(std::uint32_t a);
std::uint32_t oracle_pack_u8_f32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t oracle_pack_unorm16_f32(std::uint32_t a);
std::uint32_t oracle_s_sub_u32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_select_b32(std::uint32_t a, std::uint32_t b, std::uint32_t sel);
std::uint32_t oracle_threadid_u32(std::uint32_t);
std::uint32_t oracle_v_subb_u32(std::uint32_t a, std::uint32_t b, std::uint32_t borrow);
std::uint32_t oracle_v_subbrev_u32(std::uint32_t a, std::uint32_t b, std::uint32_t borrow);
std::uint32_t sf_cos32(std::uint32_t a, const FpPolicy&);
std::uint32_t sf_exp2_32(std::uint32_t a, const FpPolicy&);
std::uint32_t sf_fma32(std::uint32_t a, std::uint32_t b, std::uint32_t c,
                      const FpPolicy&);
std::uint32_t sf_ldexp32(std::uint32_t a, std::uint32_t e,
                     const FpPolicy&);
std::uint32_t sf_log2_32(std::uint32_t a, const FpPolicy&);
std::uint32_t sf_rcp32(std::uint32_t a, const FpPolicy&);
std::uint32_t sf_rsq32(std::uint32_t a, const FpPolicy&);
std::uint32_t sf_sin32(std::uint32_t a, const FpPolicy&);
std::uint64_t oracle_frexp_mant_f64(std::uint64_t a);
std::uint64_t oracle_sll_b64(std::uint64_t a, std::uint32_t n);
std::uint64_t oracle_sra_b64(std::uint64_t a, std::uint32_t n);
std::uint64_t oracle_srl_b64(std::uint64_t a, std::uint32_t n);
std::uint64_t sf_ceil64(std::uint64_t a, const FpPolicy&);
std::uint64_t sf_floor64(std::uint64_t a, const FpPolicy&);
std::uint64_t sf_fract64(std::uint64_t a, const FpPolicy&);
std::uint64_t sf_rcp64(std::uint64_t a, const FpPolicy&);
std::uint64_t sf_rndne64(std::uint64_t a, const FpPolicy&);
std::uint64_t sf_rsq64(std::uint64_t a, const FpPolicy&);
std::uint64_t sf_trunc64(std::uint64_t a, const FpPolicy&);
// 64-bit compare predicates: 32-bit mask result overloads.
std::uint32_t oracle_cmp_eq_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_eq_i64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_ge_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_ge_i64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_ge_u64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_gt_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_gt_i64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_gt_u64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_le_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_le_i64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_le_u64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_lg_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_lt_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_lt_i64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_lt_u64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_neq_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_neq_i64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_neq_u64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_nge_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_ngt_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_nle_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_nlg_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_nlt_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_o_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);
std::uint32_t oracle_cmp_u_f64(std::uint64_t a, std::uint64_t b,
                          const FpPolicy& policy);

std::uint32_t oracle_mad_i32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t oracle_mad_u32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t oracle_mad24_i32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t oracle_mad24_u32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t oracle_bfi_b32(std::uint32_t a, std::uint32_t b, std::uint32_t mask);
std::uint32_t oracle_mbcnt_u32(std::uint32_t v, std::uint32_t mask);
std::uint32_t oracle_threadid_u32(std::uint32_t);;
std::uint32_t oracle_select_b32(std::uint32_t a, std::uint32_t b, std::uint32_t sel);
std::uint32_t oracle_movindex_b32(std::uint32_t a, std::uint32_t);;
std::uint32_t oracle_s_sub_u32(std::uint32_t a, std::uint32_t b);;
std::uint32_t oracle_bitcmp0_b32(std::uint32_t a, std::uint32_t bit);
std::uint32_t oracle_bitcmp1_b32(std::uint32_t a, std::uint32_t bit);
std::uint64_t oracle_sll_b64(std::uint64_t a, std::uint32_t n);
std::uint64_t oracle_sra_b64(std::uint64_t a, std::uint32_t n);
std::uint64_t oracle_srl_b64(std::uint64_t a, std::uint32_t n);
std::uint32_t oracle_fcvt_f16_i16(std::uint32_t a);
std::uint32_t oracle_fcvt_f16_u16(std::uint32_t a);
std::uint32_t oracle_fcvt_i16_f16(std::uint32_t a);
std::uint32_t oracle_fcvt_u16_f16(std::uint32_t a);
std::uint32_t oracle_fcvt_f32off_i4(std::uint32_t a);;
std::uint32_t oracle_fcvt_i32flr_f32(std::uint32_t a);
std::uint32_t oracle_fcvt_i32rpi_f32(std::uint32_t a);
std::uint32_t oracle_fcvt_f32_u80(std::uint32_t a);
std::uint32_t oracle_fcvt_f32_u81(std::uint32_t a);
std::uint32_t oracle_fcvt_f32_u82(std::uint32_t a);
std::uint32_t oracle_fcvt_f32_u83(std::uint32_t a);
std::uint32_t oracle_pack_snorm16_f32(std::uint32_t a);
std::uint32_t oracle_pack_unorm16_f32(std::uint32_t a);
std::uint32_t oracle_pack_u8_f32(std::uint32_t a);
std::uint32_t oracle_frexp_mant_f32(std::uint32_t a);
std::uint32_t oracle_frexp_exp_i32_f32(std::uint32_t a);
std::uint64_t oracle_frexp_mant_f64(std::uint64_t a);
std::uint32_t oracle_frexp_exp_i32_f64(std::uint64_t a);
std::uint32_t oracle_dp4a_i8_i8_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_dp4a_i8_u8_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_dp4a_u8_i8_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_dp4a_u8_u8_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t sf_rcp32(std::uint32_t a);
std::uint32_t sf_rsq32(std::uint32_t a);
std::uint32_t sf_sin32(std::uint32_t a);
std::uint32_t sf_cos32(std::uint32_t a);
std::uint32_t sf_exp2_32(std::uint32_t a);
std::uint32_t sf_log2_32(std::uint32_t a);
std::uint64_t sf_rcp64(std::uint64_t a);
std::uint64_t sf_rsq64(std::uint64_t a);
std::uint64_t sf_floor64(std::uint64_t a);
std::uint64_t sf_ceil64(std::uint64_t a);
std::uint64_t sf_trunc64(std::uint64_t a);
std::uint64_t sf_rndne64(std::uint64_t a);
std::uint64_t sf_fract64(std::uint64_t a);
std::uint32_t sf_fma32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t sf_ldexp32(std::uint32_t a, std::uint32_t e);

std::uint32_t oracle_mad_i32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t oracle_mad_u32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t oracle_mad24_i32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t oracle_mad24_u32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t oracle_bfi_b32(std::uint32_t a, std::uint32_t b, std::uint32_t mask);
std::uint32_t oracle_mbcnt_u32(std::uint32_t v, std::uint32_t mask);
std::uint32_t oracle_threadid_u32(std::uint32_t);
std::uint32_t oracle_select_b32(std::uint32_t a, std::uint32_t b, std::uint32_t sel);
std::uint32_t oracle_movindex_b32(std::uint32_t a, std::uint32_t);
std::uint32_t oracle_s_sub_u32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_bitcmp0_b32(std::uint32_t a, std::uint32_t bit);
std::uint32_t oracle_bitcmp1_b32(std::uint32_t a, std::uint32_t bit);
std::uint64_t oracle_sll_b64(std::uint64_t a, std::uint32_t n);
std::uint64_t oracle_sra_b64(std::uint64_t a, std::uint32_t n);
std::uint64_t oracle_srl_b64(std::uint64_t a, std::uint32_t n);
std::uint32_t oracle_fcvt_f16_i16(std::uint32_t a);
std::uint32_t oracle_fcvt_f16_u16(std::uint32_t a);
std::uint32_t oracle_fcvt_i16_f16(std::uint32_t a);
std::uint32_t oracle_fcvt_u16_f16(std::uint32_t a);
std::uint32_t oracle_fcvt_f32off_i4(std::uint32_t a);
std::uint32_t oracle_fcvt_i32flr_f32(std::uint32_t a);
std::uint32_t oracle_fcvt_i32rpi_f32(std::uint32_t a);
std::uint32_t oracle_fcvt_f32_u80(std::uint32_t a);
std::uint32_t oracle_fcvt_f32_u81(std::uint32_t a);
std::uint32_t oracle_fcvt_f32_u82(std::uint32_t a);
std::uint32_t oracle_fcvt_f32_u83(std::uint32_t a);
std::uint32_t oracle_pack_snorm16_f32(std::uint32_t a);
std::uint32_t oracle_pack_unorm16_f32(std::uint32_t a);
std::uint32_t oracle_pack_u8_f32(std::uint32_t a);
std::uint32_t oracle_frexp_mant_f32(std::uint32_t a);
std::uint32_t oracle_frexp_exp_i32_f32(std::uint32_t a);
std::uint64_t oracle_frexp_mant_f64(std::uint64_t a);
std::uint32_t oracle_frexp_exp_i32_f64(std::uint64_t a);
std::uint32_t oracle_dp4a_i8_i8_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_dp4a_i8_u8_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_dp4a_u8_i8_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t oracle_dp4a_u8_u8_i32(std::uint32_t a, std::uint32_t b);
std::uint32_t sf_rcp32(std::uint32_t a);
std::uint32_t sf_rsq32(std::uint32_t a);
std::uint32_t sf_sin32(std::uint32_t a);
std::uint32_t sf_cos32(std::uint32_t a);
std::uint32_t sf_exp2_32(std::uint32_t a);
std::uint32_t sf_log2_32(std::uint32_t a);
std::uint64_t sf_rcp64(std::uint64_t a);
std::uint64_t sf_rsq64(std::uint64_t a);
std::uint64_t sf_floor64(std::uint64_t a);
std::uint64_t sf_ceil64(std::uint64_t a);
std::uint64_t sf_trunc64(std::uint64_t a);
std::uint64_t sf_rndne64(std::uint64_t a);
std::uint64_t sf_fract64(std::uint64_t a);
std::uint32_t sf_fma32(std::uint32_t a, std::uint32_t b, std::uint32_t c);
std::uint32_t sf_ldexp32(std::uint32_t a, std::uint32_t e);

std::uint32_t oracle_atomic_v_buff_add_i32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_atomic_v_buff_and_b32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_atomic_v_buff_max_i32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_atomic_v_buff_max_u32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_atomic_v_buff_min_i32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_atomic_v_buff_min_u32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_atomic_v_buff_or_b32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_atomic_v_buff_swap_b32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_atomic_v_buff_xor_b32(std::uint32_t old, std::uint32_t src);
std::uint32_t oracle_v_subb_u32(std::uint32_t a, std::uint32_t b, std::uint32_t borrow);
std::uint32_t oracle_v_subbrev_u32(std::uint32_t a, std::uint32_t b, std::uint32_t borrow);
std::uint32_t oracle_pack_f16rtz_f32(std::uint32_t a);
std::uint32_t oracle_copysign_imm_f32(std::uint32_t src, std::uint32_t imm,
                             const FpPolicy&);
}  // namespace lgpu::test
