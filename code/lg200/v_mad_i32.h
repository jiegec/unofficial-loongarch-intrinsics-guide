// v_mad_i32 -- Multiplies two signed 32-bit integers and adds a third: `d = a * b + c`, wrapped modulo 2^32.
//

dst[0] = mad_i32(src[0], src[1], src[2]);
