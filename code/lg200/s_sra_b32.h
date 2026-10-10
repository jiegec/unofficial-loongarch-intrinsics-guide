// s_sra_b32 -- Arithmetically shifts a signed 32-bit scalar value right, filling with its sign bit: `d = (i32)a >> (b & 31)`.
//

dst[0] = sra_b32(src[0], src[1]);
