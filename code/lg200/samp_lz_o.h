// samp_lz_o -- Samples the image through the sampler descriptor at the supplied coordinates and writes the dmask-selected components to the vector destination
// [HW-observed] samp-carrier fetch: the S#/T# tuples, coordinates, and dmask select the filtered result; the CPU model passes the fetched words through.
dst[0] = src[0];
