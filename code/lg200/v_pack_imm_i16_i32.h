// v_pack_imm_i16_i32 -- Packs source values from signed 32-bit integer representation into signed 16-bit integer fields in the 32-bit vector destination. Component placement follows the operand order; conversion uses the named normalization or rounding mode.
// The encoded immediate supplies the component value; component placement follows the operand order and the conversion uses the named normalization or rounding mode.
dst[0] = pack_i16_i32(src[0], src[1]);
