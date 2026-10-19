// v_sin_f32 -- Computes the sine of the 32-bit floating-point source, with the source interpreted as a fraction of a full turn (angle = 2*pi*src), and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = f32_sin(src[0], policy);
