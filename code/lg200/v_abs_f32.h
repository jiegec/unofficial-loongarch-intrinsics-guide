// v_abs_f32 -- Computes the absolute value of the 32-bit floating-point source and writes the result to the vector destination
// Clears the sign bit; all other bits pass through unchanged.
dst[0] = src[0] & 0x7fffffffu;
