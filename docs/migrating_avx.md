# Migrating from AVX to LASX

AVX is a 256-bit SIMD extension to X86. It is possible to migrate existing AVX code to leverage LoongArch LASX extension by rewriting the intrinsics or instructions manually, or by using tools like [SIMD Everywhere](https://github.com/simd-everywhere/simde) to implement AVX intrinsics with LASX counterparts. But to unleash the full performance, you may want to port your code to LASX manually.

Thankfully, LASX intrinsics adopt the same type as AVX: you can use the following familiar types for SIMD:

- __m256: 256-bit vector of single precision floating point numbers
- __m256d: 256-bit vector of double precision floating point numbers
- __m256i: 256-bit vector of integers, which can be of any width

Here is a table of a mapping from AVX intrinsics to their LASX counterpart (WIP):

| AVX                         | LASX               |
|-----------------------------|--------------------|
| _mm256_abs_epi16            | __lasx_xvsigncov_h |
| _mm256_abs_epi32            | __lasx_xvsigncov_w |
| _mm256_abs_epi8             | __lasx_xvsigncov_b |
| _mm256_add_epi16            | __lasx_xvadd_h     |
| _mm256_add_epi32            | __lasx_xvadd_w     |
| _mm256_add_epi64            | __lasx_xvadd_d     |
| _mm256_add_epi8             | __lasx_xvadd_b     |
| _mm256_add_pd               | __lasx_xvfadd_d    |
| _mm256_add_ps               | __lasx_xvfadd_s    |
| _mm256_adds_epi16           | __lasx_xvsadd_h    |
| _mm256_adds_epi8            | __lasx_xvsadd_b    |
| _mm256_adds_epu16           | __lasx_xvsadd_hu   |
| _mm256_adds_epu8            | __lasx_xvsadd_bu   |
| _mm256_addsub_pd            |                    |
| _mm256_addsub_ps            |                    |
| _mm256_alignr_epi8          |                    |
| _mm256_and_pd               | __lasx_xvand_v     |
| _mm256_and_ps               | __lasx_xvand_v     |
| _mm256_and_si256            | __lasx_xvand_v     |
| _mm256_andnot_pd            | __lasx_xvandn_v    |
| _mm256_andnot_ps            | __lasx_xvandn_v    |
| _mm256_andnot_si256         | __lasx_xvandn_v    |
| _mm256_avg_epu16            | __lasx_xvavgr_hu   |
| _mm256_avg_epu8             | __lasx_xvavgr_bu   |
| _mm256_bcstnebf16_ps        |                    |
| _mm256_bcstnesh_ps          |                    |
| _mm256_blend_epi16          |                    |
| _mm256_blend_epi32          |                    |
| _mm256_blend_pd             |                    |
| _mm256_blend_ps             |                    |
| _mm256_blendv_epi8          |                    |
| _mm256_blendv_pd            |                    |
| _mm256_blendv_ps            |                    |
| _mm256_broadcast_pd         |                    |
| _mm256_broadcast_ps         |                    |
| _mm256_broadcast_sd         |                    |
| _mm256_broadcast_ss         |                    |
| _mm256_broadcastb_epi8      |                    |
| _mm256_broadcastd_epi32     |                    |
| _mm256_broadcastq_epi64     |                    |
| _mm256_broadcastsd_pd       |                    |
| _mm256_broadcastsi128_si256 |                    |
| _mm256_broadcastss_ps       |                    |
| _mm256_broadcastw_epi16     |                    |
| _mm256_bslli_epi128         |                    |
| _mm256_bsrli_epi128         |                    |
| _mm256_castpd128_pd256      |                    |
| _mm256_castpd256_pd128      |                    |
| _mm256_castpd_ps            |                    |
| _mm256_castpd_si256         |                    |
| _mm256_castps128_ps256      |                    |
| _mm256_castps256_ps128      |                    |
| _mm256_castps_pd            |                    |
| _mm256_castps_si256         |                    |
| _mm256_castsi128_si256      |                    |
| _mm256_castsi256_pd         |                    |
| _mm256_castsi256_ps         |                    |
| _mm256_castsi256_si128      |                    |
| _mm256_ceil_pd              |                    |
| _mm256_ceil_ps              |                    |
| _mm256_cmp_pd               |                    |
| _mm256_cmp_ps               |                    |
| _mm256_cmpeq_epi16          |                    |
| _mm256_cmpeq_epi32          |                    |
| _mm256_cmpeq_epi64          |                    |
| _mm256_cmpeq_epi8           |                    |
| _mm256_cmpgt_epi16          |                    |
| _mm256_cmpgt_epi32          |                    |
| _mm256_cmpgt_epi64          |                    |
| _mm256_cmpgt_epi8           |                    |
| _mm256_cvtepi16_epi32       |                    |
| _mm256_cvtepi16_epi64       |                    |
| _mm256_cvtepi32_epi64       |                    |
| _mm256_cvtepi32_pd          |                    |
| _mm256_cvtepi32_ps          |                    |
| _mm256_cvtepi8_epi16        |                    |
| _mm256_cvtepi8_epi32        |                    |
| _mm256_cvtepi8_epi64        |                    |
| _mm256_cvtepu16_epi32       |                    |
| _mm256_cvtepu16_epi64       |                    |
| _mm256_cvtepu32_epi64       |                    |
| _mm256_cvtepu8_epi16        |                    |
| _mm256_cvtepu8_epi32        |                    |
| _mm256_cvtepu8_epi64        |                    |
| _mm256_cvtneebf16_ps        |                    |
| _mm256_cvtneeph_ps          |                    |
| _mm256_cvtneobf16_ps        |                    |
| _mm256_cvtneoph_ps          |                    |
| _mm256_cvtneps_avx_pbh      |                    |
| _mm256_cvtneps_pbh          |                    |
| _mm256_cvtpd_epi32          |                    |
| _mm256_cvtpd_ps             |                    |
| _mm256_cvtph_ps             |                    |
| _mm256_cvtps_epi32          |                    |
| _mm256_cvtps_pd             |                    |
| _mm256_cvtps_ph             |                    |
| _mm256_cvtsd_f64            |                    |
| _mm256_cvtsi256_si32        |                    |
| _mm256_cvtss_f32            |                    |
| _mm256_cvttpd_epi32         |                    |
| _mm256_cvttps_epi32         |                    |
| _mm256_div_pd               | __lasx_xvfdiv_d    |
| _mm256_div_ps               | __lasx_xvfdiv_s    |
| _mm256_dp_ps                |                    |
| _mm256_dpbssd_epi32         |                    |
| _mm256_dpbssds_epi32        |                    |
| _mm256_dpbsud_epi32         |                    |
| _mm256_dpbsuds_epi32        |                    |
| _mm256_dpbusd_avx_epi32     |                    |
| _mm256_dpbusd_epi32         |                    |
| _mm256_dpbusds_avx_epi32    |                    |
| _mm256_dpbusds_epi32        |                    |
| _mm256_dpbuud_epi32         |                    |
| _mm256_dpbuuds_epi32        |                    |
| _mm256_dpwssd_avx_epi32     |                    |
| _mm256_dpwssd_epi32         |                    |
| _mm256_dpwssds_avx_epi32    |                    |
| _mm256_dpwssds_epi32        |                    |
| _mm256_dpwsud_epi32         |                    |
| _mm256_dpwsuds_epi32        |                    |
| _mm256_dpwusd_epi32         |                    |
| _mm256_dpwusds_epi32        |                    |
| _mm256_dpwuud_epi32         |                    |
| _mm256_dpwuuds_epi32        |                    |
| _mm256_extract_epi16        |                    |
| _mm256_extract_epi32        |                    |
| _mm256_extract_epi64        |                    |
| _mm256_extract_epi8         |                    |
| _mm256_extractf128_pd       |                    |
| _mm256_extractf128_ps       |                    |
| _mm256_extractf128_si256    |                    |
| _mm256_extracti128_si256    |                    |
| _mm256_floor_pd             |                    |
| _mm256_floor_ps             |                    |
| _mm256_fmadd_pd             |                    |
| _mm256_fmadd_ps             |                    |
| _mm256_fmaddsub_pd          |                    |
| _mm256_fmaddsub_ps          |                    |
| _mm256_fmsub_pd             |                    |
| _mm256_fmsub_ps             |                    |
| _mm256_fmsubadd_pd          |                    |
| _mm256_fmsubadd_ps          |                    |
| _mm256_fnmadd_pd            |                    |
| _mm256_fnmadd_ps            |                    |
| _mm256_fnmsub_pd            |                    |
| _mm256_fnmsub_ps            |                    |
| _mm256_hadd_epi16           |                    |
| _mm256_hadd_epi32           |                    |
| _mm256_hadd_pd              |                    |
| _mm256_hadd_ps              |                    |
| _mm256_hadds_epi16          |                    |
| _mm256_hsub_epi16           |                    |
| _mm256_hsub_epi32           |                    |
| _mm256_hsub_pd              |                    |
| _mm256_hsub_ps              |                    |
| _mm256_hsubs_epi16          |                    |
| _mm256_i32gather_epi32      |                    |
| _mm256_i32gather_epi64      |                    |
| _mm256_i32gather_pd         |                    |
| _mm256_i32gather_ps         |                    |
| _mm256_i64gather_epi32      |                    |
| _mm256_i64gather_epi64      |                    |
| _mm256_i64gather_pd         |                    |
| _mm256_i64gather_ps         |                    |
| _mm256_insert_epi16         |                    |
| _mm256_insert_epi32         |                    |
| _mm256_insert_epi64         |                    |
| _mm256_insert_epi8          |                    |
| _mm256_insertf128_pd        |                    |
| _mm256_insertf128_ps        |                    |
| _mm256_insertf128_si256     |                    |
| _mm256_inserti128_si256     |                    |
| _mm256_lddqu_si256          |                    |
| _mm256_load_pd              |                    |
| _mm256_load_ps              |                    |
| _mm256_load_si256           |                    |
| _mm256_loadu2_m128d         |                    |
| _mm256_loadu2_m128i         |                    |
| _mm256_loadu2_m128          |                    |
| _mm256_loadu_pd             |                    |
| _mm256_loadu_ps             |                    |
| _mm256_loadu_si256          |                    |
| _mm256_madd52hi_avx_epu64   |                    |
| _mm256_madd52hi_epu64       |                    |
| _mm256_madd52lo_avx_epu64   |                    |
| _mm256_madd52lo_epu64       |                    |
| _mm256_madd_epi16           |                    |
| _mm256_maddubs_epi16        |                    |
| _mm256_mask_i32gather_epi32 |                    |
| _mm256_mask_i32gather_epi64 |                    |
| _mm256_mask_i32gather_pd    |                    |
| _mm256_mask_i32gather_ps    |                    |
| _mm256_mask_i64gather_epi32 |                    |
| _mm256_mask_i64gather_epi64 |                    |
| _mm256_mask_i64gather_pd    |                    |
| _mm256_mask_i64gather_ps    |                    |
| _mm256_maskload_epi32       |                    |
| _mm256_maskload_epi64       |                    |
| _mm256_maskload_pd          |                    |
| _mm256_maskload_ps          |                    |
| _mm256_maskstore_epi32      |                    |
| _mm256_maskstore_epi64      |                    |
| _mm256_maskstore_pd         |                    |
| _mm256_maskstore_ps         |                    |
| _mm256_max_epi16            | __lasx_xvmax_h     |
| _mm256_max_epi32            | __lasx_xvmax_w     |
| _mm256_max_epi8             | __lasx_xvmax_b     |
| _mm256_max_epu16            | __lasx_xvmax_hu    |
| _mm256_max_epu32            | __lasx_xvmax_wu    |
| _mm256_max_epu8             | __lasx_xvmax_bu    |
| _mm256_max_pd               | __lasx_xvfmax_d    |
| _mm256_max_ps               | __lasx_xvfmax_s    |
| _mm256_min_epi16            | __lasx_xvmin_h     |
| _mm256_min_epi32            | __lasx_xvmin_w     |
| _mm256_min_epi8             | __lasx_xvmin_b     |
| _mm256_min_epu16            | __lasx_xvmin_hu    |
| _mm256_min_epu32            | __lasx_xvmin_wu    |
| _mm256_min_epu8             | __lasx_xvmin_bu    |
| _mm256_min_pd               | __lasx_xvfmin_d    |
| _mm256_min_ps               | __lasx_xvfmin_s    |
| _mm256_movedup_pd           |                    |
| _mm256_movehdup_ps          |                    |
| _mm256_moveldup_ps          |                    |
| _mm256_movemask_epi8        |                    |
| _mm256_movemask_pd          |                    |
| _mm256_movemask_ps          |                    |
| _mm256_mpsadbw_epu8         |                    |
| _mm256_mul_epi32            |                    |
| _mm256_mul_epu32            |                    |
| _mm256_mul_pd               | __lasx_xvfmul_d    |
| _mm256_mul_ps               | __lasx_xvfmul_s    |
| _mm256_mulhi_epi16          |                    |
| _mm256_mulhi_epu16          |                    |
| _mm256_mulhrs_epi16         |                    |
| _mm256_mullo_epi16          |                    |
| _mm256_mullo_epi32          |                    |
| _mm256_or_pd                |                    |
| _mm256_or_ps                |                    |
| _mm256_or_si256             | __lasx_xvor_v      |
| _mm256_packs_epi16          |                    |
| _mm256_packs_epi32          |                    |
| _mm256_packus_epi16         |                    |
| _mm256_packus_epi32         |                    |
| _mm256_permute2f128_pd      |                    |
| _mm256_permute2f128_ps      |                    |
| _mm256_permute2f128_si256   |                    |
| _mm256_permute2x128_si256   |                    |
| _mm256_permute4x64_epi64    |                    |
| _mm256_permute4x64_pd       |                    |
| _mm256_permute_pd           |                    |
| _mm256_permute_ps           |                    |
| _mm256_permutevar8x32_epi32 |                    |
| _mm256_permutevar8x32_ps    |                    |
| _mm256_permutevar_pd        |                    |
| _mm256_permutevar_ps        |                    |
| _mm256_rcp_ps               |                    |
| _mm256_round_pd             |                    |
| _mm256_round_ps             |                    |
| _mm256_rsqrt_ps             |                    |
| _mm256_sad_epu8             |                    |
| _mm256_set1_epi16           |                    |
| _mm256_set1_epi32           |                    |
| _mm256_set1_epi64x          |                    |
| _mm256_set1_epi8            |                    |
| _mm256_set1_pd              |                    |
| _mm256_set1_ps              |                    |
| _mm256_set_epi16            |                    |
| _mm256_set_epi32            |                    |
| _mm256_set_epi64x           |                    |
| _mm256_set_epi8             |                    |
| _mm256_set_m128d            |                    |
| _mm256_set_m128i            |                    |
| _mm256_set_m128             |                    |
| _mm256_set_pd               |                    |
| _mm256_set_ps               |                    |
| _mm256_setr_epi16           |                    |
| _mm256_setr_epi32           |                    |
| _mm256_setr_epi64x          |                    |
| _mm256_setr_epi8            |                    |
| _mm256_setr_m128d           |                    |
| _mm256_setr_m128i           |                    |
| _mm256_setr_m128            |                    |
| _mm256_setr_pd              |                    |
| _mm256_setr_ps              |                    |
| _mm256_setzero_pd           |                    |
| _mm256_setzero_ps           |                    |
| _mm256_setzero_si256        |                    |
| _mm256_sha512msg1_epi64     |                    |
| _mm256_sha512msg2_epi64     |                    |
| _mm256_sha512rnds2_epi64    |                    |
| _mm256_shuffle_epi32        |                    |
| _mm256_shuffle_epi8         |                    |
| _mm256_shuffle_pd           |                    |
| _mm256_shuffle_ps           |                    |
| _mm256_shufflehi_epi16      |                    |
| _mm256_shufflelo_epi16      |                    |
| _mm256_sign_epi16           |                    |
| _mm256_sign_epi32           |                    |
| _mm256_sign_epi8            |                    |
| _mm256_sll_epi16            | __lasx_xvsll_h     |
| _mm256_sll_epi32            | __lasx_xvsll_w     |
| _mm256_sll_epi64            | __lasx_xvsll_d     |
| _mm256_slli_epi16           | __lasx_xvslli_h    |
| _mm256_slli_epi32           | __lasx_xvslli_w    |
| _mm256_slli_epi64           | __lasx_xvslli_d    |
| _mm256_slli_si256           |                    |
| _mm256_sllv_epi32           |                    |
| _mm256_sllv_epi64           |                    |
| _mm256_sm4key4_epi32        |                    |
| _mm256_sm4rnds4_epi32       |                    |
| _mm256_sqrt_pd              |                    |
| _mm256_sqrt_ps              |                    |
| _mm256_sra_epi16            | __lasx_xvsra_h     |
| _mm256_sra_epi32            | __lasx_xvsra_w     |
| _mm256_srai_epi16           | __lasx_xvsrai_h    |
| _mm256_srai_epi32           | __lasx_xvsrai_w    |
| _mm256_srav_epi32           |                    |
| _mm256_srl_epi16            | __lasx_xvsrl_h     |
| _mm256_srl_epi32            | __lasx_xvsrl_w     |
| _mm256_srl_epi64            | __lasx_xvsrl_d     |
| _mm256_srli_epi16           | __lasx_xvsrli_h    |
| _mm256_srli_epi32           | __lasx_xvsrli_w    |
| _mm256_srli_epi64           | __lasx_xvsrli_d    |
| _mm256_srli_si256           |                    |
| _mm256_srlv_epi32           |                    |
| _mm256_srlv_epi64           |                    |
| _mm256_store_pd             |                    |
| _mm256_store_ps             |                    |
| _mm256_store_si256          |                    |
| _mm256_storeu2_m128d        |                    |
| _mm256_storeu2_m128i        |                    |
| _mm256_storeu2_m128         |                    |
| _mm256_storeu_pd            |                    |
| _mm256_storeu_ps            |                    |
| _mm256_storeu_si256         |                    |
| _mm256_stream_load_si256    |                    |
| _mm256_stream_pd            |                    |
| _mm256_stream_ps            |                    |
| _mm256_stream_si256         |                    |
| _mm256_sub_epi16            | __lasx_xvsub_h     |
| _mm256_sub_epi32            | __lasx_xvsub_w     |
| _mm256_sub_epi64            | __lasx_xvsub_d     |
| _mm256_sub_epi8             | __lasx_xvsub_b     |
| _mm256_sub_pd               | __lasx_xvfsub_d    |
| _mm256_sub_ps               | __lasx_xvfsub_s    |
| _mm256_subs_epi16           |                    |
| _mm256_subs_epi8            |                    |
| _mm256_subs_epu16           |                    |
| _mm256_subs_epu8            |                    |
| _mm256_testc_pd             |                    |
| _mm256_testc_ps             |                    |
| _mm256_testc_si256          |                    |
| _mm256_testnzc_pd           |                    |
| _mm256_testnzc_ps           |                    |
| _mm256_testnzc_si256        |                    |
| _mm256_testz_pd             |                    |
| _mm256_testz_ps             |                    |
| _mm256_testz_si256          |                    |
| _mm256_undefined_pd         |                    |
| _mm256_undefined_ps         |                    |
| _mm256_undefined_si256      |                    |
| _mm256_unpackhi_epi16       | __lasx_xvilvh_h    |
| _mm256_unpackhi_epi32       | __lasx_xvilvh_w    |
| _mm256_unpackhi_epi64       | __lasx_xvilvh_d    |
| _mm256_unpackhi_epi8        | __lasx_xvilvh_b    |
| _mm256_unpackhi_pd          |                    |
| _mm256_unpackhi_ps          |                    |
| _mm256_unpacklo_epi16       | __lasx_xvilvl_h    |
| _mm256_unpacklo_epi32       | __lasx_xvilvl_w    |
| _mm256_unpacklo_epi64       | __lasx_xvilvl_d    |
| _mm256_unpacklo_epi8        | __lasx_xvilvl_b    |
| _mm256_unpacklo_pd          |                    |
| _mm256_unpacklo_ps          |                    |
| _mm256_xor_pd               |                    |
| _mm256_xor_ps               |                    |
| _mm256_xor_si256            |                    |
| _mm256_zeroall              |                    |
| _mm256_zeroupper            |                    |
| _mm256_zextpd128_pd256      |                    |
| _mm256_zextps128_ps256      |                    |
| _mm256_zextsi128_si256      |                    |
| _mm_bcstnebf16_ps           |                    |
| _mm_bcstnesh_ps             |                    |
| _mm_blend_epi32             |                    |
| _mm_broadcast_ss            |                    |
| _mm_broadcastb_epi8         |                    |
| _mm_broadcastd_epi32        |                    |
| _mm_broadcastq_epi64        |                    |
| _mm_broadcastsd_pd          |                    |
| _mm_broadcastsi128_si256    |                    |
| _mm_broadcastss_ps          |                    |
| _mm_broadcastw_epi16        |                    |
| _mm_cmp_pd                  |                    |
| _mm_cmp_ps                  |                    |
| _mm_cmp_sd                  |                    |
| _mm_cmp_ss                  |                    |
| _mm_cvtneebf16_ps           |                    |
| _mm_cvtneeph_ps             |                    |
| _mm_cvtneobf16_ps           |                    |
| _mm_cvtneoph_ps             |                    |
| _mm_cvtneps_avx_pbh         |                    |
| _mm_cvtneps_pbh             |                    |
| _mm_cvtph_ps                |                    |
| _mm_cvtps_ph                |                    |
| _mm_dpbssd_epi32            |                    |
| _mm_dpbssds_epi32           |                    |
| _mm_dpbsud_epi32            |                    |
| _mm_dpbsuds_epi32           |                    |
| _mm_dpbusd_avx_epi32        |                    |
| _mm_dpbusd_epi32            |                    |
| _mm_dpbusds_avx_epi32       |                    |
| _mm_dpbusds_epi32           |                    |
| _mm_dpbuud_epi32            |                    |
| _mm_dpbuuds_epi32           |                    |
| _mm_dpwssd_avx_epi32        |                    |
| _mm_dpwssd_epi32            |                    |
| _mm_dpwssds_avx_epi32       |                    |
| _mm_dpwssds_epi32           |                    |
| _mm_dpwsud_epi32            |                    |
| _mm_dpwsuds_epi32           |                    |
| _mm_dpwusd_epi32            |                    |
| _mm_dpwusds_epi32           |                    |
| _mm_dpwuud_epi32            |                    |
| _mm_dpwuuds_epi32           |                    |
| _mm_fmadd_pd                |                    |
| _mm_fmadd_ps                |                    |
| _mm_fmadd_sd                |                    |
| _mm_fmadd_ss                |                    |
| _mm_fmaddsub_pd             |                    |
| _mm_fmaddsub_ps             |                    |
| _mm_fmsub_pd                |                    |
| _mm_fmsub_ps                |                    |
| _mm_fmsub_sd                |                    |
| _mm_fmsub_ss                |                    |
| _mm_fmsubadd_pd             |                    |
| _mm_fmsubadd_ps             |                    |
| _mm_fnmadd_pd               |                    |
| _mm_fnmadd_ps               |                    |
| _mm_fnmadd_sd               |                    |
| _mm_fnmadd_ss               |                    |
| _mm_fnmsub_pd               |                    |
| _mm_fnmsub_ps               |                    |
| _mm_fnmsub_sd               |                    |
| _mm_fnmsub_ss               |                    |
| _mm_i32gather_epi32         |                    |
| _mm_i32gather_epi64         |                    |
| _mm_i32gather_pd            |                    |
| _mm_i32gather_ps            |                    |
| _mm_i64gather_epi32         |                    |
| _mm_i64gather_epi64         |                    |
| _mm_i64gather_pd            |                    |
| _mm_i64gather_ps            |                    |
| _mm_madd52hi_avx_epu64      |                    |
| _mm_madd52hi_epu64          |                    |
| _mm_madd52lo_avx_epu64      |                    |
| _mm_madd52lo_epu64          |                    |
| _mm_mask_i32gather_epi32    |                    |
| _mm_mask_i32gather_epi64    |                    |
| _mm_mask_i32gather_pd       |                    |
| _mm_mask_i32gather_ps       |                    |
| _mm_mask_i64gather_epi32    |                    |
| _mm_mask_i64gather_epi64    |                    |
| _mm_mask_i64gather_pd       |                    |
| _mm_mask_i64gather_ps       |                    |
| _mm_maskload_epi32          |                    |
| _mm_maskload_epi64          |                    |
| _mm_maskload_pd             |                    |
| _mm_maskload_ps             |                    |
| _mm_maskstore_epi32         |                    |
| _mm_maskstore_epi64         |                    |
| _mm_maskstore_pd            |                    |
| _mm_maskstore_ps            |                    |
| _mm_permute_pd              |                    |
| _mm_permute_ps              |                    |
| _mm_permutevar_pd           |                    |
| _mm_permutevar_ps           |                    |
| _mm_sllv_epi32              |                    |
| _mm_sllv_epi64              |                    |
| _mm_sm3msg1_epi32           |                    |
| _mm_sm3msg2_epi32           |                    |
| _mm_sm3rnds2_epi32          |                    |
| _mm_sm4key4_epi32           |                    |
| _mm_sm4rnds4_epi32          |                    |
| _mm_srav_epi32              |                    |
| _mm_srlv_epi32              |                    |
| _mm_srlv_epi64              |                    |
| _mm_testc_pd                |                    |
| _mm_testc_ps                |                    |
| _mm_testnzc_pd              |                    |
| _mm_testnzc_ps              |                    |
| _mm_testz_pd                |                    |
| _mm_testz_ps                |                    |


The list of AVX intrinsics came from [Intel Intrinsics Guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.htm).
