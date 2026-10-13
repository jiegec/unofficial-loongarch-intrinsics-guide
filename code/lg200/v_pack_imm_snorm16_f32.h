// v_pack_imm_snorm16_f32 -- Packs an f32 lane value and a literal as signed normalized 16-bit fields.
//

// [HW 2026-10-13] the immediate row packs the LITERAL in the high half and the
// lane value in the low, as v_pack_imm_f16rtz_f32 does.  Packing src[0] alone left
// the literal out entirely and failed 170 cases.
dst[0] = (pack_snorm16_f32(src[1]) << 16) | pack_snorm16_f32(src[0]);
