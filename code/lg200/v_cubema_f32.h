// v_cubema_f32 -- Computes the signed major-axis term for cube-map coordinate selection and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = test::oracle_cubema(src[0], src[1], src[2]);
