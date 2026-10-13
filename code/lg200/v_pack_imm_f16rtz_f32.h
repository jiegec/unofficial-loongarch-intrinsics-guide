// v_pack_imm_f16rtz_f32 -- Packs an f32 lane value and a literal as truncated f16 fields: `d = pack(f16rtz(a), f16rtz(imm))`.
//

// [HW 2026-10-13] which field the literal takes was UNRESOLVED.  Measured: the
// literal goes high and the converted value low - a source of +inf with a
// literal of 1.0 gives 0x3c007c00, of NaN gives 0x3c007e00, of -NaN 0x3c00fe00.
dst[0] = (pack_f16rtz_f32(src[1]) << 16) | pack_f16rtz_f32(src[0]);
