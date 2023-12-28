#include "common.h"

// some intrinsics are defined as macros, and they don't linke extra commas
#define COMMA ,

void test() {
  // Bitwise Operations
  PRINT(__lsx_vbitsel_v(__m128i{0x1122334455667788, 0x99aabbccddeeff00},
                        __m128i{0xabababababababab, 0x1234123443214321},
                        __m128i{0xffff0000aaaabbbb, 0x1111222233334444}));
  PRINT(__lasx_xvbitsel_v(__m256i{0x1122334455667788, 0x99aabbccddeeff00,
                                  0xabcdef1212341234, 0xaabbaabbddeeddee},
                          __m256i{0xabababababababab, 0x1234123443214321,
                                  0x1234123443214321, 0x5678567856785678},
                          __m256i{0xffff0000aaaabbbb, 0x1111222233334444,
                                  0x00000000ffffffff, 0xffffffff00000000}));
  PRINT(__lsx_vbitseli_b(__m128i{0x1122334455667788 COMMA 0x99aabbccddeeff00},
                         __m128i{0xabababababababab COMMA 0x1234123443214321},
                         0x12));
  PRINT(__lasx_xvbitseli_b(
      __m256i{
          0x1122334455667788 COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee},
      __m256i{
          0xabababababababab COMMA 0x1234123443214321 COMMA 0x1234123443214321 COMMA 0x5678567856785678},
      0x12));

  PRINT(__lsx_vbitclr_b(__m128i{0xffffffffffffffff, 0x99aabbccddeeff00},
                        __m128i{0xabababababababab, 0x1234123443214321}));
  PRINT(__lasx_xvbitclr_b(__m256i{0xffffffffffffffff, 0x99aabbccddeeff00,
                                  0xabcdef1212341234, 0xaabbaabbddeeddee},
                          __m256i{0xabababababababab, 0x1234123443214321,
                                  0x1234123443214321, 0x5678567856785678}));

  PRINT(__lsx_vbitclr_h(__m128i{0xffffffffffffffff, 0x99aabbccddeeff00},
                        __m128i{0xabababababababab, 0x1234123443214321}));
  PRINT(__lasx_xvbitclr_h(__m256i{0xffffffffffffffff, 0x99aabbccddeeff00,
                                  0xabcdef1212341234, 0xaabbaabbddeeddee},
                          __m256i{0xabababababababab, 0x1234123443214321,
                                  0x1234123443214321, 0x5678567856785678}));

  PRINT(__lsx_vbitclr_w(__m128i{0xffffffffffffffff, 0x99aabbccddeeff00},
                        __m128i{0xabababababababab, 0x1234123443214321}));
  PRINT(__lasx_xvbitclr_w(__m256i{0xffffffffffffffff, 0x99aabbccddeeff00,
                                  0xabcdef1212341234, 0xaabbaabbddeeddee},
                          __m256i{0xabababababababab, 0x1234123443214321,
                                  0x1234123443214321, 0x5678567856785678}));

  PRINT(__lsx_vbitclr_d(__m128i{0xffffffffffffffff, 0x99aabbccddeeff00},
                        __m128i{0xabababababababab, 0x1234123443214321}));
  PRINT(__lasx_xvbitclr_d(__m256i{0xffffffffffffffff, 0x99aabbccddeeff00,
                                  0xabcdef1212341234, 0xaabbaabbddeeddee},
                          __m256i{0xabababababababab, 0x1234123443214321,
                                  0x1234123443214321, 0x5678567856785678}));

  PRINT(__lsx_vbitclri_b(__m128i{0xffffffffffffffff COMMA 0x99aabbccddeeff00},
                         1));
  PRINT(__lasx_xvbitclri_b(
      __m256i{
          0xffffffffffffffff COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee},
      1));

  PRINT(__lsx_vbitclri_h(__m128i{0xffffffffffffffff COMMA 0x99aabbccddeeff00},
                         1));
  PRINT(__lasx_xvbitclri_h(
      __m256i{
          0xffffffffffffffff COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee},
      1));

  PRINT(__lsx_vbitclri_w(__m128i{0xffffffffffffffff COMMA 0x99aabbccddeeff00},
                         1));
  PRINT(__lasx_xvbitclri_w(
      __m256i{
          0xffffffffffffffff COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee},
      1));

  PRINT(__lsx_vbitclri_d(__m128i{0xffffffffffffffff COMMA 0x99aabbccddeeff00},
                         1));
  PRINT(__lasx_xvbitclri_d(
      __m256i{
          0xffffffffffffffff COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee},
      1));

  PRINT(__lsx_vbitset_b(__m128i{0x0000000000000000, 0x99aabbccddeeff00},
                        __m128i{0xabababababababab, 0x1234123443214321}));
  PRINT(__lasx_xvbitset_b(__m256i{0x0000000000000000, 0x99aabbccddeeff00,
                                  0xabcdef1212341234, 0xaabbaabbddeeddee},
                          __m256i{0xabababababababab, 0x1234123443214321,
                                  0x1234123443214321, 0x5678567856785678}));

  PRINT(__lsx_vbitset_h(__m128i{0x0000000000000000, 0x99aabbccddeeff00},
                        __m128i{0xabababababababab, 0x1234123443214321}));
  PRINT(__lasx_xvbitset_h(__m256i{0x0000000000000000, 0x99aabbccddeeff00,
                                  0xabcdef1212341234, 0xaabbaabbddeeddee},
                          __m256i{0xabababababababab, 0x1234123443214321,
                                  0x1234123443214321, 0x5678567856785678}));

  PRINT(__lsx_vbitset_w(__m128i{0x0000000000000000, 0x99aabbccddeeff00},
                        __m128i{0xabababababababab, 0x1234123443214321}));
  PRINT(__lasx_xvbitset_w(__m256i{0x0000000000000000, 0x99aabbccddeeff00,
                                  0xabcdef1212341234, 0xaabbaabbddeeddee},
                          __m256i{0xabababababababab, 0x1234123443214321,
                                  0x1234123443214321, 0x5678567856785678}));

  PRINT(__lsx_vbitset_d(__m128i{0x0000000000000000, 0x99aabbccddeeff00},
                        __m128i{0xabababababababab, 0x1234123443214321}));
  PRINT(__lasx_xvbitset_d(__m256i{0x0000000000000000, 0x99aabbccddeeff00,
                                  0xabcdef1212341234, 0xaabbaabbddeeddee},
                          __m256i{0xabababababababab, 0x1234123443214321,
                                  0x1234123443214321, 0x5678567856785678}));

  PRINT(__lsx_vbitseti_b(__m128i{0x0000000000000000 COMMA 0x99aabbccddeeff00},
                         1));
  PRINT(__lasx_xvbitseti_b(
      __m256i{
          0x0000000000000000 COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee},
      1));

  PRINT(__lsx_vbitseti_h(__m128i{0x0000000000000000 COMMA 0x99aabbccddeeff00},
                         1));
  PRINT(__lasx_xvbitseti_h(
      __m256i{
          0x0000000000000000 COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee},
      1));

  PRINT(__lsx_vbitseti_w(__m128i{0x0000000000000000 COMMA 0x99aabbccddeeff00},
                         1));
  PRINT(__lasx_xvbitseti_w(
      __m256i{
          0x0000000000000000 COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee},
      1));

  PRINT(__lsx_vbitseti_d(__m128i{0x0000000000000000 COMMA 0x99aabbccddeeff00},
                         1));
  PRINT(__lasx_xvbitseti_d(
      __m256i{
          0x0000000000000000 COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee},
      1));

  PRINT(__lsx_vbitrev_b(__m128i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00},
                        __m128i{0xabababababababab, 0x1234123443214321}));
  PRINT(__lasx_xvbitrev_b(__m256i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00,
                                  0xabcdef1212341234, 0xaabbaabbddeeddee},
                          __m256i{0xabababababababab, 0x1234123443214321,
                                  0x1234123443214321, 0x5678567856785678}));

  PRINT(__lsx_vbitrev_h(__m128i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00},
                        __m128i{0xabababababababab, 0x1234123443214321}));
  PRINT(__lasx_xvbitrev_h(__m256i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00,
                                  0xabcdef1212341234, 0xaabbaabbddeeddee},
                          __m256i{0xabababababababab, 0x1234123443214321,
                                  0x1234123443214321, 0x5678567856785678}));

  PRINT(__lsx_vbitrev_w(__m128i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00},
                        __m128i{0xabababababababab, 0x1234123443214321}));
  PRINT(__lasx_xvbitrev_w(__m256i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00,
                                  0xabcdef1212341234, 0xaabbaabbddeeddee},
                          __m256i{0xabababababababab, 0x1234123443214321,
                                  0x1234123443214321, 0x5678567856785678}));

  PRINT(__lsx_vbitrev_d(__m128i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00},
                        __m128i{0xabababababababab, 0x1234123443214321}));
  PRINT(__lasx_xvbitrev_d(__m256i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00,
                                  0xabcdef1212341234, 0xaabbaabbddeeddee},
                          __m256i{0xabababababababab, 0x1234123443214321,
                                  0x1234123443214321, 0x5678567856785678}));

  PRINT(__lsx_vbitrevi_b(__m128i{0x0f0f0f0f0f0f0f0f COMMA 0x99aabbccddeeff00},
                         1));
  PRINT(__lasx_xvbitrevi_b(
      __m256i{
          0x0f0f0f0f0f0f0f0f COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee},
      1));

  PRINT(__lsx_vbitrevi_h(__m128i{0x0f0f0f0f0f0f0f0f COMMA 0x99aabbccddeeff00},
                         1));
  PRINT(__lasx_xvbitrevi_h(
      __m256i{
          0x0f0f0f0f0f0f0f0f COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee},
      1));

  PRINT(__lsx_vbitrevi_w(__m128i{0x0f0f0f0f0f0f0f0f COMMA 0x99aabbccddeeff00},
                         1));
  PRINT(__lasx_xvbitrevi_w(
      __m256i{
          0x0f0f0f0f0f0f0f0f COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee},
      1));

  PRINT(__lsx_vbitrevi_d(__m128i{0x0f0f0f0f0f0f0f0f COMMA 0x99aabbccddeeff00},
                         1));
  PRINT(__lasx_xvbitrevi_d(
      __m256i{
          0x0f0f0f0f0f0f0f0f COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee},
      1));

  PRINT(__lsx_vclo_b(__m128i{0x1122334455667788 COMMA 0x99aabbccddeeff00}));
  PRINT(__lasx_xvclo_b(__m256i{
      0x1122334455667788 COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee}));

  PRINT(__lsx_vclo_h(__m128i{0x1122334455667788 COMMA 0x99aabbccddeeff00}));
  PRINT(__lasx_xvclo_h(__m256i{
      0x1122334455667788 COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee}));

  PRINT(__lsx_vclo_w(__m128i{0x1122334455667788 COMMA 0x99aabbccddeeff00}));
  PRINT(__lasx_xvclo_w(__m256i{
      0x1122334455667788 COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee}));

  PRINT(__lsx_vclo_d(__m128i{0x1122334455667788 COMMA 0x99aabbccddeeff00}));
  PRINT(__lasx_xvclo_d(__m256i{
      0x1122334455667788 COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee}));

  PRINT(__lsx_vclz_b(__m128i{0x1122334455667788 COMMA 0x99aabbccddeeff00}));
  PRINT(__lasx_xvclz_b(__m256i{
      0x1122334455667788 COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee}));

  PRINT(__lsx_vclz_h(__m128i{0x1122334455667788 COMMA 0x99aabbccddeeff00}));
  PRINT(__lasx_xvclz_h(__m256i{
      0x1122334455667788 COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee}));

  PRINT(__lsx_vclz_w(__m128i{0x1122334455667788 COMMA 0x99aabbccddeeff00}));
  PRINT(__lasx_xvclz_w(__m256i{
      0x1122334455667788 COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee}));

  PRINT(__lsx_vclz_d(__m128i{0x1122334455667788 COMMA 0x99aabbccddeeff00}));
  PRINT(__lasx_xvclz_d(__m256i{
      0x1122334455667788 COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee}));

  PRINT(__lsx_vpcnt_b(__m128i{0x1122334455667788 COMMA 0x99aabbccddeeff00}));
  PRINT(__lasx_xvpcnt_b(__m256i{
      0x1122334455667788 COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee}));

  PRINT(__lsx_vpcnt_h(__m128i{0x1122334455667788 COMMA 0x99aabbccddeeff00}));
  PRINT(__lasx_xvpcnt_h(__m256i{
      0x1122334455667788 COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee}));

  PRINT(__lsx_vpcnt_w(__m128i{0x1122334455667788 COMMA 0x99aabbccddeeff00}));
  PRINT(__lasx_xvpcnt_w(__m256i{
      0x1122334455667788 COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee}));

  PRINT(__lsx_vpcnt_d(__m128i{0x1122334455667788 COMMA 0x99aabbccddeeff00}));
  PRINT(__lasx_xvpcnt_d(__m256i{
      0x1122334455667788 COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee}));

  // Misc
  PRINT(__lsx_vmsknz_b(__m128i{0x1122334455667788, 0x99aabbccddeeff00}));
  PRINT(__lsx_vmsknz_b(__m128i{0x0000111100000000, 0x0011000011111111}));
  PRINT(__lasx_xvmsknz_b(__m256i{0x1122334455667788, 0x99aabbccddeeff00,
                                 0xabababab12121212, 0x1234567812345678}));
  PRINT(__lasx_xvmsknz_b(__m256i{0x0000111100000000, 0x0011000011111111,
                                 0x1111000001010101, 0x0000000000000000}));

  PRINT(__lsx_vmskgez_b(__m128i{0x1122334455667788, 0x99aabbccddeeff00}));
  PRINT(__lsx_vmskgez_b(__m128i{0x0000808000000000, 0x0081000081716151}));
  PRINT(__lasx_xvmskgez_b(__m256i{0x1122334455667788, 0x99aabbccddeeff00,
                                  0xabababab12121212, 0x1234567812345678}));
  PRINT(__lasx_xvmskgez_b(__m256i{0x0000191100000000, 0x00a1000011b11c11,
                                  0x1181000008010101, 0x0000000000000000}));

  PRINT(__lsx_vmskltz_b(__m128i{0x1122334455667788, 0x99aabbccddeeff00}));
  PRINT(__lsx_vmskltz_b(__m128i{0x0000808000000000, 0x0081000081716151}));
  PRINT(__lasx_xvmskltz_b(__m256i{0x1122334455667788, 0x99aabbccddeeff00,
                                  0xabababab12121212, 0x1234567812345678}));
  PRINT(__lasx_xvmskltz_b(__m256i{0x0000118100000000, 0x0081000081111118,
                                  0x1181000001010801, 0x0000000000000000}));

  PRINT(__lsx_vmskltz_h(__m128i{0x1122334455667788, 0x99aabbccddeeff00}));
  PRINT(__lsx_vmskltz_h(__m128i{0x0000808000000000, 0x0081000081716151}));
  PRINT(__lasx_xvmskltz_h(__m256i{0x1122334455667788, 0x99aabbccddeeff00,
                                  0xabababab12121212, 0x1234567812345678}));
  PRINT(__lasx_xvmskltz_h(__m256i{0x0000818100000000, 0x0018000018181881,
                                  0x1181000008080808, 0x0000000000000000}));

  PRINT(__lsx_vmskltz_w(__m128i{0x1122334455667788, 0x99aabbccddeeff00}));
  PRINT(__lsx_vmskltz_w(__m128i{0x0000808000000000, 0x0081000081716151}));
  PRINT(__lasx_xvmskltz_w(__m256i{0x1122334455667788, 0x99aabbccddeeff00,
                                  0xabababab12121212, 0x1234567812345678}));
  PRINT(__lasx_xvmskltz_w(__m256i{0x0000811100000000, 0x0018000081111111,
                                  0x8111000001010108, 0x0000000000000000}));

  PRINT(__lsx_vmskltz_d(__m128i{0x1122334455667788, 0x99aabbccddeeff00}));
  PRINT(__lsx_vmskltz_d(__m128i{0x0000808000000000, 0x0081000081716151}));
  PRINT(__lasx_xvmskltz_d(__m256i{0x1122334455667788, 0x99aabbccddeeff00,
                                  0xabababab12121212, 0x1234567812345678}));
  PRINT(__lasx_xvmskltz_d(__m256i{0x0000111800000000, 0x0081000081111111,
                                  0x8111000008010101, 0x0000000000000000}));

  // Shuffling
  PRINT(__lsx_vshuf_b(__m128i{0x1122334455667788, 0x99aabbccddeeff00},
                      __m128i{0xabcdef1314156678, 0x1234123443214321},
                      __m128i{0x0011021304050607, 0x0811120213031404}));
  PRINT(__lasx_xvshuf_b(__m256i{0x1122334455667788, 0x99aabbccddeeff00,
                                0xabcdef1212341234, 0xaabbaabbddeeddee},
                        __m256i{0xabcdef1314156678, 0x1234123443214321,
                                0x1234123443214321, 0x5678567856785678},
                        __m256i{0x1f1f00001a0a1b0b, 0x1111120213031404,
                                0x0102030405060708, 0x1112131405060708}));

  PRINT(__lsx_vshuf_h(__m128i{0x0001000200030004, 0x0005000a000b000c},
                      __m128i{0x1122334455667788, 0x99aabbccddeeff00},
                      __m128i{0xabcdef1314156678, 0x1234123443214321}));
  PRINT(__lasx_xvshuf_h(__m256i{0x0001000200030004, 0x0005000a000b000c,
                                0x000f000e00010002, 0x0008000900020001},
                        __m256i{0x1122334455667788, 0x99aabbccddeeff00,
                                0xabcdef1212341234, 0xaabbaabbddeeddee},
                        __m256i{0xabcdef1314156678, 0x1234123443214321,
                                0x1234123443214321, 0x5678567856785678}));

  PRINT(__lsx_vshuf_w(__m128i{0x0000000200000004, 0x0000000700000005},
                      __m128i{0x1122334455667788, 0x99aabbccddeeff00},
                      __m128i{0xabcdef1314156678, 0x1234123443214321}));
  PRINT(__lasx_xvshuf_w(__m256i{0x0000000200000004, 0x0000000700000005,
                                0x0000000100000003, 0x0000000400000000},
                        __m256i{0x1122334455667788, 0x99aabbccddeeff00,
                                0xabcdef1212341234, 0xaabbaabbddeeddee},
                        __m256i{0xabcdef1314156678, 0x1234123443214321,
                                0x1234123443214321, 0x5678567856785678}));

  PRINT(__lsx_vshuf_d(__m128i{0x0000000000000001, 0x0000000000000002},
                      __m128i{0x1122334455667788, 0x99aabbccddeeff00},
                      __m128i{0xabcdef1314156678, 0x1234123443214321}));
  PRINT(__lasx_xvshuf_d(__m256i{0x0000000000000000, 0x0000000000000003,
                                0x0000000000000002, 0x0000000000000001},
                        __m256i{0x1122334455667788, 0x99aabbccddeeff00,
                                0xabcdef1212341234, 0xaabbaabbddeeddee},
                        __m256i{0xabcdef1314156678, 0x1234123443214321,
                                0x1234123443214321, 0x5678567856785678}));

  PRINT(__lsx_vshuf4i_b(__m128i{0xabcdef1314156678 COMMA 0x1234123443214321},
                        0x12));
  PRINT(__lasx_xvshuf4i_b(
      __m256i{
          0xabcdef1314156678 COMMA 0x1234123443214321 COMMA 0x1234123443214321 COMMA 0x5678567856785678},
      0x12));

  PRINT(__lsx_vshuf4i_h(__m128i{0xabcdef1314156678 COMMA 0x1234123443214321},
                        0x12));
  PRINT(__lasx_xvshuf4i_h(
      __m256i{
          0xabcdef1314156678 COMMA 0x1234123443214321 COMMA 0x1234123443214321 COMMA 0x5678567856785678},
      0x12));

  PRINT(__lsx_vshuf4i_w(__m128i{0xabcdef1314156678 COMMA 0x1234123443214321},
                        0x12));
  PRINT(__lasx_xvshuf4i_w(
      __m256i{
          0xabcdef1314156678 COMMA 0x1234123443214321 COMMA 0x1234123443214321 COMMA 0x5678567856785678},
      0x12));

  PRINT(__lsx_vshuf4i_d(__m128i{0x1122334455667788 COMMA 0x99aabbccddeeff00},
                        __m128i{0xabcdef1314156678 COMMA 0x1234123443214321},
                        0x12));
  PRINT(__lasx_xvshuf4i_d(
      __m256i{
          0x1122334455667788 COMMA 0x99aabbccddeeff00 COMMA 0xabcdef1212341234 COMMA 0xaabbaabbddeeddee},
      __m256i{
          0xabcdef1314156678 COMMA 0x1234123443214321 COMMA 0x1234123443214321 COMMA 0x5678567856785678},
      0x12));
}