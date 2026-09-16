// v_pack_u8_u32 -- Packs the unsigned 32-bit source into an unsigned 8-bit field selected by the third source position operator
// [HW-verified 2026-08-25] true shape is a THREE-source pack: byte
// (src1 % 4) of src2 is replaced by the U8-saturated src0 where the
// hardware applies min(src0, 0xff) on the RAW 32-bit bits - in-range
// values pass (0x7f->0x7f, 0xaa->0xaa), anything >= 0x100 or negative
// wraps huge and becomes 0xff (0x100->0xff, 0xffffffff->0xff); there is
// no zero floor.  src1 selects the byte (8*(src1 mod 4)); src2 supplies
// the three untouched bytes.
dst[0] = pack_u8_u32(src[0], src[1], src[2]);
