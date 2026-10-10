// v_sra_b32 -- Arithmetically shifts a signed 32-bit lane value right: `d = a >> (b & 31)`.
//

dst[0] = sra_b32(src[0], src[1]);
