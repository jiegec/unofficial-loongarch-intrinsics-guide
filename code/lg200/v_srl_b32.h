// v_srl_b32 -- Logically shifts a 32-bit lane value right, filling with zeros: `d = a >> (b & 31)`.
//

dst[0] = src[0] >> (src[1] & 31u);
