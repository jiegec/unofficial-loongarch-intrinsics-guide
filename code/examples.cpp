#include "common.h"

void test() {
  PRINT(__lsx_vmsknz_b(__m128i{0x1122334455667788, 0x99aabbccddeeff00}));
  PRINT(__lsx_vmsknz_b(__m128i{0x0000111100000000, 0x0011000011111111}));
  PRINT(__lsx_vmskgez_b(__m128i{0x1122334455667788, 0x99aabbccddeeff00}));
  PRINT(__lsx_vmskgez_b(__m128i{0x0000808000000000, 0x0081000081716151}));
  PRINT(__lsx_vmskltz_b(__m128i{0x1122334455667788, 0x99aabbccddeeff00}));
  PRINT(__lsx_vmskltz_b(__m128i{0x0000808000000000, 0x0081000081716151}));
  PRINT(__lsx_vmskltz_h(__m128i{0x1122334455667788, 0x99aabbccddeeff00}));
  PRINT(__lsx_vmskltz_h(__m128i{0x0000808000000000, 0x0081000081716151}));
  PRINT(__lsx_vmskltz_w(__m128i{0x1122334455667788, 0x99aabbccddeeff00}));
  PRINT(__lsx_vmskltz_w(__m128i{0x0000808000000000, 0x0081000081716151}));
  PRINT(__lsx_vmskltz_d(__m128i{0x1122334455667788, 0x99aabbccddeeff00}));
  PRINT(__lsx_vmskltz_d(__m128i{0x0000808000000000, 0x0081000081716151}));
}