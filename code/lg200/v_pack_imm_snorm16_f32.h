// v_pack_imm_snorm16_f32 -- Packs source values from 32-bit floating-point value representation into signed normalized 16-bit value fields in the 32-bit vector destination. Component placement follows the operand order; conversion uses the named normalization or rounding mode.
// The encoded immediate supplies the component value; component placement follows the operand order and the conversion uses the named normalization or rounding mode.
dst[0] = pack_snorm16_f32(src[0]);
