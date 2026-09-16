// v_cubeface_f32 -- Selects the cube-map face associated with the three-dimensional direction and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = test::oracle_cubeface(src[0], src[1], src[2]);
