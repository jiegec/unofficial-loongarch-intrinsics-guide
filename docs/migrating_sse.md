# Migrating from SSE to LSX

SSE is a 128-bit SIMD extension to X86. It is possible to migrate existing SSE code to leverage LoongArch LSX extension by rewriting the intrinsics or instructions manually, or by using tools like [SIMD Everywhere](https://github.com/simd-everywhere/simde) to implement SSE intrinsics with LSX counterparts. But to unleash the full performance, you may want to port your code to LSX manually.

Thankfully, LSX intrinsics adopt the same type as SSE: you can use the following familiar types for SIMD:

- __m128: 128-bit vector of single precision floating point numbers
- __m128d: 128-bit vector of double precision floating point numbers
- __m128i: 128-bit vector of integers, which can be of any width

Here is a table of a mapping from SSE intrinsics to their LSX counterpart (WIP):

| SSE                     | LSX                                   |
|-------------------------|---------------------------------------|
| _mm_abs_epi16           |                                       |
| _mm_abs_epi32           |                                       |
| _mm_abs_epi8            |                                       |
| _mm_add_epi16           | __lsx_vadd_h                          |
| _mm_add_epi32           | __lsx_vadd_w                          |
| _mm_add_epi64           | __lsx_vadd_d                          |
| _mm_add_epi8            | __lsx_vadd_b                          |
| _mm_add_pd              | __lsx_vfadd_d                         |
| _mm_add_ps              | __lsx_vfadd_s                         |
| _mm_add_sd              |                                       |
| _mm_add_ss              |                                       |
| _mm_adds_epi16          | __lsx_vsadd_h                         |
| _mm_adds_epi8           | __lsx_vsadd_b                         |
| _mm_adds_epu16          | __lsx_vsadd_hu                        |
| _mm_adds_epu8           | __lsx_vsadd_bu                        |
| _mm_addsub_pd           |                                       |
| _mm_addsub_ps           |                                       |
| _mm_alignr_epi8         |                                       |
| _mm_and_pd              | __lsx_vand_v                          |
| _mm_and_ps              | __lsx_vand_v                          |
| _mm_and_si128           | __lsx_vand_v                          |
| _mm_andnot_pd           | __lsx_vandn_v                         |
| _mm_andnot_ps           | __lsx_vandn_v                         |
| _mm_andnot_si128        | __lsx_vandn_v                         |
| _mm_avg_epu16           | __lsx_vavgr_hu                        |
| _mm_avg_epu8            | __lsx_vavgr_bu                        |
| _mm_blend_epi16         |                                       |
| _mm_blend_pd            |                                       |
| _mm_blend_ps            |                                       |
| _mm_blendv_epi8         |                                       |
| _mm_blendv_pd           |                                       |
| _mm_blendv_ps           |                                       |
| _mm_bslli_si128         | __lsx_vbsll_v                         |
| _mm_bsrli_si128         | __lsx_vbsrl_v                         |
| _mm_castpd_ps           |                                       |
| _mm_castpd_si128        |                                       |
| _mm_castps_pd           |                                       |
| _mm_castps_si128        |                                       |
| _mm_castsi128_pd        |                                       |
| _mm_castsi128_ps        |                                       |
| _mm_ceil_pd             |                                       |
| _mm_ceil_ps             |                                       |
| _mm_ceil_sd             |                                       |
| _mm_ceil_ss             |                                       |
| _mm_cmpeq_epi16         | __lsx_vseq_h                          |
| _mm_cmpeq_epi32         | __lsx_vseq_w                          |
| _mm_cmpeq_epi64         | __lsx_vseq_d                          |
| _mm_cmpeq_epi8          | __lsx_vseq_b                          |
| _mm_cmpeq_pd            | __lsx_vfcmp_ceq_d                     |
| _mm_cmpeq_ps            | __lsx_vfcmp_ceq_s                     |
| _mm_cmpeq_sd            |                                       |
| _mm_cmpeq_ss            |                                       |
| _mm_cmpestra            |                                       |
| _mm_cmpestrc            |                                       |
| _mm_cmpestri            |                                       |
| _mm_cmpestrm            |                                       |
| _mm_cmpestro            |                                       |
| _mm_cmpestrs            |                                       |
| _mm_cmpestrz            |                                       |
| _mm_cmpge_pd            | __lsx_vfcmp_cle_d                     |
| _mm_cmpge_ps            | __lsx_vfcmp_cle_s                     |
| _mm_cmpge_sd            |                                       |
| _mm_cmpge_ss            |                                       |
| _mm_cmpgt_epi16         | __lsx_vslt_h                          |
| _mm_cmpgt_epi32         | __lsx_vslt_w                          |
| _mm_cmpgt_epi64         | __lsx_vslt_d                          |
| _mm_cmpgt_epi8          | __lsx_vslt_b                          |
| _mm_cmpgt_pd            | __lsx_vfcmp_clt_d                     |
| _mm_cmpgt_ps            | __lsx_vfcmp_clt_s                     |
| _mm_cmpgt_sd            |                                       |
| _mm_cmpgt_ss            |                                       |
| _mm_cmpistra            |                                       |
| _mm_cmpistrc            |                                       |
| _mm_cmpistri            |                                       |
| _mm_cmpistrm            |                                       |
| _mm_cmpistro            |                                       |
| _mm_cmpistrs            |                                       |
| _mm_cmpistrz            |                                       |
| _mm_cmple_pd            | __lsx_vfcmp_cle_d                     |
| _mm_cmple_ps            | __lsx_vfcmp_cle_s                     |
| _mm_cmple_sd            |                                       |
| _mm_cmple_ss            |                                       |
| _mm_cmplt_epi16         | __lsx_vslt_h                          |
| _mm_cmplt_epi32         | __lsx_vslt_w                          |
| _mm_cmplt_epi8          | __lsx_vslt_b                          |
| _mm_cmplt_pd            | __lsx_vfcmp_clt_d                     |
| _mm_cmplt_ps            | __lsx_vfcmp_clt_s                     |
| _mm_cmplt_sd            |                                       |
| _mm_cmplt_ss            |                                       |
| _mm_cmpneq_pd           | __lsx_vfcmp_cune_d                    |
| _mm_cmpneq_ps           | __lsx_vfcmp_cune_s                    |
| _mm_cmpneq_sd           |                                       |
| _mm_cmpneq_ss           |                                       |
| _mm_cmpnge_pd           |                                       |
| _mm_cmpnge_ps           |                                       |
| _mm_cmpnge_sd           |                                       |
| _mm_cmpnge_ss           |                                       |
| _mm_cmpngt_pd           |                                       |
| _mm_cmpngt_ps           |                                       |
| _mm_cmpngt_sd           |                                       |
| _mm_cmpngt_ss           |                                       |
| _mm_cmpnle_pd           |                                       |
| _mm_cmpnle_ps           |                                       |
| _mm_cmpnle_sd           |                                       |
| _mm_cmpnle_ss           |                                       |
| _mm_cmpnlt_pd           |                                       |
| _mm_cmpnlt_ps           |                                       |
| _mm_cmpnlt_sd           |                                       |
| _mm_cmpnlt_ss           |                                       |
| _mm_cmpord_pd           | __lsx_vfcmp_cor_d                     |
| _mm_cmpord_ps           | __lsx_vfcmp_cor_s                     |
| _mm_cmpord_sd           |                                       |
| _mm_cmpord_ss           |                                       |
| _mm_cmpunord_pd         | __lsx_vfcmp_cun_d                     |
| _mm_cmpunord_ps         | __lsx_vfcmp_cun_s                     |
| _mm_cmpunord_sd         |                                       |
| _mm_cmpunord_ss         |                                       |
| _mm_comieq_sd           |                                       |
| _mm_comieq_ss           |                                       |
| _mm_comige_sd           |                                       |
| _mm_comige_ss           |                                       |
| _mm_comigt_sd           |                                       |
| _mm_comigt_ss           |                                       |
| _mm_comile_sd           |                                       |
| _mm_comile_ss           |                                       |
| _mm_comilt_sd           |                                       |
| _mm_comilt_ss           |                                       |
| _mm_comineq_sd          |                                       |
| _mm_comineq_ss          |                                       |
| _mm_cvt_pi2ps           |                                       |
| _mm_cvt_ps2pi           |                                       |
| _mm_cvt_si2ss           |                                       |
| _mm_cvt_ss2si           |                                       |
| _mm_cvtepi16_epi32      |                                       |
| _mm_cvtepi16_epi64      |                                       |
| _mm_cvtepi32_epi64      |                                       |
| _mm_cvtepi32_pd         |                                       |
| _mm_cvtepi32_ps         |                                       |
| _mm_cvtepi8_epi16       |                                       |
| _mm_cvtepi8_epi32       |                                       |
| _mm_cvtepi8_epi64       |                                       |
| _mm_cvtepu16_epi32      |                                       |
| _mm_cvtepu16_epi64      |                                       |
| _mm_cvtepu32_epi64      |                                       |
| _mm_cvtepu8_epi16       |                                       |
| _mm_cvtepu8_epi32       |                                       |
| _mm_cvtepu8_epi64       |                                       |
| _mm_cvtpd_epi32         |                                       |
| _mm_cvtpd_pi32          |                                       |
| _mm_cvtpd_ps            |                                       |
| _mm_cvtpi16_ps          |                                       |
| _mm_cvtpi32_pd          |                                       |
| _mm_cvtpi32_ps          |                                       |
| _mm_cvtpi32x2_ps        |                                       |
| _mm_cvtpi8_ps           |                                       |
| _mm_cvtps_epi32         |                                       |
| _mm_cvtps_pd            |                                       |
| _mm_cvtps_pi16          |                                       |
| _mm_cvtps_pi32          |                                       |
| _mm_cvtps_pi8           |                                       |
| _mm_cvtpu16_ps          |                                       |
| _mm_cvtpu8_ps           |                                       |
| _mm_cvtsd_f64           |                                       |
| _mm_cvtsd_si32          |                                       |
| _mm_cvtsd_si64          |                                       |
| _mm_cvtsd_si64x         |                                       |
| _mm_cvtsd_ss            |                                       |
| _mm_cvtsi128_si32       |                                       |
| _mm_cvtsi128_si64       |                                       |
| _mm_cvtsi128_si64x      |                                       |
| _mm_cvtsi32_sd          |                                       |
| _mm_cvtsi32_si128       |                                       |
| _mm_cvtsi32_ss          |                                       |
| _mm_cvtsi64_sd          |                                       |
| _mm_cvtsi64_si128       |                                       |
| _mm_cvtsi64_ss          |                                       |
| _mm_cvtsi64x_sd         |                                       |
| _mm_cvtsi64x_si128      |                                       |
| _mm_cvtss_f32           |                                       |
| _mm_cvtss_sd            |                                       |
| _mm_cvtss_si32          |                                       |
| _mm_cvtss_si64          |                                       |
| _mm_cvtt_ps2pi          |                                       |
| _mm_cvtt_ss2si          |                                       |
| _mm_cvttpd_epi32        |                                       |
| _mm_cvttpd_pi32         |                                       |
| _mm_cvttps_epi32        |                                       |
| _mm_cvttps_pi32         |                                       |
| _mm_cvttsd_si32         |                                       |
| _mm_cvttsd_si64         |                                       |
| _mm_cvttsd_si64x        |                                       |
| _mm_cvttss_si32         |                                       |
| _mm_cvttss_si64         |                                       |
| _mm_div_pd              | __lsx_vfdiv_d                         |
| _mm_div_ps              | __lsx_vfdiv_s                         |
| _mm_div_sd              |                                       |
| _mm_div_ss              |                                       |
| _mm_dp_pd               |                                       |
| _mm_dp_ps               |                                       |
| _mm_extract_epi16       |                                       |
| _mm_extract_epi32       |                                       |
| _mm_extract_epi64       |                                       |
| _mm_extract_epi8        |                                       |
| _mm_extract_ps          |                                       |
| _mm_floor_pd            |                                       |
| _mm_floor_ps            |                                       |
| _mm_floor_sd            |                                       |
| _mm_floor_ss            |                                       |
| _mm_hadd_epi16          |                                       |
| _mm_hadd_epi32          |                                       |
| _mm_hadd_pd             |                                       |
| _mm_hadd_ps             |                                       |
| _mm_hadds_epi16         |                                       |
| _mm_hsub_epi16          |                                       |
| _mm_hsub_epi32          |                                       |
| _mm_hsub_pd             |                                       |
| _mm_hsub_ps             |                                       |
| _mm_hsubs_epi16         |                                       |
| _mm_insert_epi16        |                                       |
| _mm_insert_epi32        |                                       |
| _mm_insert_epi64        |                                       |
| _mm_insert_epi8         |                                       |
| _mm_insert_ps           |                                       |
| _mm_lddqu_si128         |                                       |
| _mm_load_pd             | __lsx_vld                             |
| _mm_load_pd1            |                                       |
| _mm_load_ps             | __lsx_vld                             |
| _mm_load_ps1            |                                       |
| _mm_load_sd             |                                       |
| _mm_load_si128          |                                       |
| _mm_load_ss             |                                       |
| _mm_load1_pd            | __lsx_vldrepl_d                       |
| _mm_load1_ps            | __lsx_vldrepl_w                       |
| _mm_loaddup_pd          |                                       |
| _mm_loadh_pd            |                                       |
| _mm_loadh_pi            |                                       |
| _mm_loadl_epi64         |                                       |
| _mm_loadl_pd            |                                       |
| _mm_loadl_pi            |                                       |
| _mm_loadr_pd            |                                       |
| _mm_loadr_ps            | __lsx_vld + __lsx_vshuf4i_w           |
| _mm_loadu_pd            | __lsx_vld                             |
| _mm_loadu_ps            | __lsx_vld                             |
| _mm_loadu_si128         |                                       |
| _mm_loadu_si16          |                                       |
| _mm_loadu_si32          |                                       |
| _mm_loadu_si64          |                                       |
| _mm_madd_epi16          |                                       |
| _mm_maddubs_epi16       |                                       |
| _mm_maskmoveu_si128     |                                       |
| _mm_max_epi16           |                                       |
| _mm_max_epi32           |                                       |
| _mm_max_epi8            |                                       |
| _mm_max_epu16           |                                       |
| _mm_max_epu32           |                                       |
| _mm_max_epu8            |                                       |
| _mm_max_pd              | __lsx_vfmax_d                         |
| _mm_max_ps              | __lsx_vfmax_s                         |
| _mm_max_sd              |                                       |
| _mm_max_ss              |                                       |
| _mm_min_epi16           |                                       |
| _mm_min_epi32           |                                       |
| _mm_min_epi8            |                                       |
| _mm_min_epu16           |                                       |
| _mm_min_epu32           |                                       |
| _mm_min_epu8            |                                       |
| _mm_min_pd              | __lsx_vfmin_d                         |
| _mm_min_ps              | __lsx_vfmin_s                         |
| _mm_min_sd              |                                       |
| _mm_min_ss              |                                       |
| _mm_minpos_epu16        |                                       |
| _mm_move_epi64          |                                       |
| _mm_move_sd             | __lsx_vextrins_d                      |
| _mm_move_ss             | __lsx_vextrins_w                      |
| _mm_movedup_pd          |                                       |
| _mm_movehdup_ps         |                                       |
| _mm_movehl_ps           | __lsx_vilvh_d                         |
| _mm_moveldup_ps         |                                       |
| _mm_movelh_ps           | __lsx_vilvl_d                         |
| _mm_movemask_epi8       |                                       |
| _mm_movemask_pd         |                                       |
| _mm_movemask_ps         | __lsx_vmskltz_w + __lsx_vpickve2gr_wu |
| _mm_movepi64_pi64       |                                       |
| _mm_movpi64_epi64       |                                       |
| _mm_mpsadbw_epu8        |                                       |
| _mm_mul_epi32           |                                       |
| _mm_mul_epu32           |                                       |
| _mm_mul_pd              | __lsx_vfmul_d                         |
| _mm_mul_ps              | __lsx_vfmul_s                         |
| _mm_mul_sd              |                                       |
| _mm_mul_ss              |                                       |
| _mm_mulhi_epi16         |                                       |
| _mm_mulhi_epu16         |                                       |
| _mm_mulhrs_epi16        |                                       |
| _mm_mullo_epi16         |                                       |
| _mm_mullo_epi32         |                                       |
| _mm_or_pd               | __lsx_vor_v                           |
| _mm_or_ps               | __lsx_vor_v                           |
| _mm_or_si128            | __lsx_vor_v                           |
| _mm_packs_epi16         |                                       |
| _mm_packs_epi32         |                                       |
| _mm_packus_epi16        |                                       |
| _mm_packus_epi32        |                                       |
| _mm_rcp_ps              | __lsx_vfrecip_s                       |
| _mm_rcp_ss              |                                       |
| _mm_round_pd            | __lsx_vfrintr*_d                      |
| _mm_round_ps            | __lsx_vfrintr*_s                      |
| _mm_round_sd            |                                       |
| _mm_round_ss            |                                       |
| _mm_rsqrt_ps            | __lsx_vfrsqrt_s                       |
| _mm_rsqrt_ss            |                                       |
| _mm_sad_epu8            |                                       |
| _mm_set_epi16           |                                       |
| _mm_set_epi32           |                                       |
| _mm_set_epi64           |                                       |
| _mm_set_epi64x          |                                       |
| _mm_set_epi8            |                                       |
| _mm_set_pd              |                                       |
| _mm_set_pd1             | __lsx_vdrepl_d/__lsx_vreplgr2vd_d     |
| _mm_set_ps              |                                       |
| _mm_set_ps1             | __lsx_vdrepl_w/__lsx_vreplgr2vr_w     |
| _mm_set_sd              |                                       |
| _mm_set_ss              |                                       |
| _mm_set1_epi16          |                                       |
| _mm_set1_epi32          |                                       |
| _mm_set1_epi64          |                                       |
| _mm_set1_epi64x         |                                       |
| _mm_set1_epi8           |                                       |
| _mm_set1_pd             |                                       |
| _mm_set1_ps             |                                       |
| _mm_setr_epi16          |                                       |
| _mm_setr_epi32          |                                       |
| _mm_setr_epi64          |                                       |
| _mm_setr_epi8           |                                       |
| _mm_setr_pd             |                                       |
| _mm_setr_ps             |                                       |
| _mm_setzero_pd          |                                       |
| _mm_setzero_ps          |                                       |
| _mm_setzero_si128       |                                       |
| _mm_shuffle_epi32       |                                       |
| _mm_shuffle_epi8        |                                       |
| _mm_shuffle_pd          |                                       |
| _mm_shuffle_ps          |                                       |
| _mm_shufflehi_epi16     |                                       |
| _mm_shufflelo_epi16     |                                       |
| _mm_sign_epi16          |                                       |
| _mm_sign_epi32          |                                       |
| _mm_sign_epi8           |                                       |
| _mm_sll_epi16           | __lsx_vsll_h                          |
| _mm_sll_epi32           | __lsx_vsll_w                          |
| _mm_sll_epi64           | __lsx_vsll_d                          |
| _mm_slli_epi16          | __lsx_vslli_h                         |
| _mm_slli_epi32          | __lsx_vslli_w                         |
| _mm_slli_epi64          | __lsx_vslli_d                         |
| _mm_slli_si128          |                                       |
| _mm_sqrt_pd             | __lsx_vfsqrt_d                        |
| _mm_sqrt_ps             | __lsx_vfsqrt_s                        |
| _mm_sqrt_sd             |                                       |
| _mm_sqrt_ss             |                                       |
| _mm_sra_epi16           | __lsx_vsra_h                          |
| _mm_sra_epi32           | __lsx_vsra_w                          |
| _mm_srai_epi16          | __lsx_vsrai_h                         |
| _mm_srai_epi32          | __lsx_vsrai_w                         |
| _mm_srl_epi16           | __lsx_vsrl_h                          |
| _mm_srl_epi32           | __lsx_vsrl_w                          |
| _mm_srl_epi64           | __lsx_vsrl_d                          |
| _mm_srli_epi16          | __lsx_vsrli_h                         |
| _mm_srli_epi32          | __lsx_vsrli_w                         |
| _mm_srli_epi64          | __lsx_vsrli_d                         |
| _mm_srli_si128          |                                       |
| _mm_store_pd            | __lsx_vst                             |
| _mm_store_pd1           |                                       |
| _mm_store_ps            | __lsx_vst                             |
| _mm_store_ps1           |                                       |
| _mm_store_sd            |                                       |
| _mm_store_si128         |                                       |
| _mm_store_ss            | __lsx_vstelm_w                        |
| _mm_store1_pd           | __lsx_vreplvei_d + __lsx_vst          |
| _mm_store1_ps           | __lsx_vreplvei_w + __lsx_vst          |
| _mm_storeh_pd           |                                       |
| _mm_storeh_pi           |                                       |
| _mm_storel_epi64        |                                       |
| _mm_storel_pd           |                                       |
| _mm_storel_pi           |                                       |
| _mm_storer_pd           |                                       |
| _mm_storer_ps           | __lsx_vshuf4i_w + __lsx_vst           |
| _mm_storeu_pd           | __lsx_vst                             |
| _mm_storeu_ps           | __lsx_vst                             |
| _mm_storeu_si128        |                                       |
| _mm_storeu_si16         |                                       |
| _mm_storeu_si32         |                                       |
| _mm_storeu_si64         |                                       |
| _mm_stream_load_si128   |                                       |
| _mm_stream_pd           |                                       |
| _mm_stream_ps           |                                       |
| _mm_stream_si128        |                                       |
| _mm_sub_epi16           | __lsx_vsub_h                          |
| _mm_sub_epi32           | __lsx_vsub_w                          |
| _mm_sub_epi64           | __lsx_vsub_d                          |
| _mm_sub_epi8            | __lsx_vsub_b                          |
| _mm_sub_pd              | __lsx_vfsub_s                         |
| _mm_sub_ps              | __lsx_vfsub_s                         |
| _mm_sub_sd              |                                       |
| _mm_sub_ss              |                                       |
| _mm_subs_epi16          |                                       |
| _mm_subs_epi8           |                                       |
| _mm_subs_epu16          |                                       |
| _mm_subs_epu8           |                                       |
| _mm_test_all_ones       |                                       |
| _mm_test_all_zeros      |                                       |
| _mm_test_mix_ones_zeros |                                       |
| _mm_testc_si128         |                                       |
| _mm_testnzc_si128       |                                       |
| _mm_testz_si128         |                                       |
| _MM_TRANSPOSE4_PS       |                                       |
| _mm_ucomieq_sd          |                                       |
| _mm_ucomieq_ss          |                                       |
| _mm_ucomige_sd          |                                       |
| _mm_ucomige_ss          |                                       |
| _mm_ucomigt_sd          |                                       |
| _mm_ucomigt_ss          |                                       |
| _mm_ucomile_sd          |                                       |
| _mm_ucomile_ss          |                                       |
| _mm_ucomilt_sd          |                                       |
| _mm_ucomilt_ss          |                                       |
| _mm_ucomineq_sd         |                                       |
| _mm_ucomineq_ss         |                                       |
| _mm_undefined_pd        |                                       |
| _mm_undefined_ps        |                                       |
| _mm_undefined_si128     |                                       |
| _mm_unpackhi_epi16      | __lsx_vilvh_h                         |
| _mm_unpackhi_epi32      | __lsx_vilvh_w                         |
| _mm_unpackhi_epi64      | __lsx_vilvh_d                         |
| _mm_unpackhi_epi8       | __lsx_vilvh_b                         |
| _mm_unpackhi_pd         | __lsx_vilvh_d                         |
| _mm_unpackhi_ps         | __lsx_vilvh_w                         |
| _mm_unpacklo_epi16      | __lsx_vilvl_h                         |
| _mm_unpacklo_epi32      | __lsx_vilvl_w                         |
| _mm_unpacklo_epi64      | __lsx_vilvl_d                         |
| _mm_unpacklo_epi8       | __lsx_vilvl_b                         |
| _mm_unpacklo_pd         | __lsx_vilvl_d                         |
| _mm_unpacklo_ps         | __lsx_vilvl_w                         |
| _mm_xor_pd              | __lsx_vxor_v                          |
| _mm_xor_ps              | __lsx_vxor_v                          |
| _mm_xor_si128           | __lsx_vxor_v                          |

The list of SSE intrinsics came from [Intel Intrinsics Guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.htm).