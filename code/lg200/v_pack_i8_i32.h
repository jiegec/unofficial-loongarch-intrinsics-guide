// v_pack_i8_i32 -- Packs the signed 32-bit source into a signed 8-bit field selected by the third source position operator
// [HW-verified 2026-08-25] true shape is a THREE-source pack: byte
// (src1 % 4) of src2 is replaced by the full 32-bit signed src0 saturated
// to S8 [-128, 127].  src1 acts as the byte-position selector (byte =
// 8*(src1 mod 4); 0x1->byte1, 0x2->byte2, 0x3->byte3, 0x4->byte0 again);
// the other three bytes of src2 pass through untouched.  Saturation
// evidence: 0x80->0x7f, 0x100->0x7f, 0x7fffffff->0x7f (positive clamp),
// 0xffffffff->0xff (-1), 0x80000000->0x80, 0xffffff01->0x80 (negative
// clamp).  The older two-source model (((a&0xff)|(b&0xff)<<8) ignored src2
// and mispredicted every non-zero-c row.
dst[0] = pack_i8_i32(src[0], src[1], src[2]);
