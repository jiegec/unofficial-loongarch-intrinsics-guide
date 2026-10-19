// v_cubet_f32 -- Computes the cube-map T coordinate from the direction vector and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = cubet(src[0], src[1], src[2]);
