#include "common.h"

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
}