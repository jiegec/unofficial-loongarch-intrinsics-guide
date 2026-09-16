// getlod -- Computes the texture level-of-detail values for the supplied coordinates, image descriptor, and sampler state and writes them to the vector destination
// [HW-observed] samp-carrier LOD query: the level estimates are computed from the coordinates and the T# gradient state; the CPU model passes the query words through.
dst[0] = src[0];
