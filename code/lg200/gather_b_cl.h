// gather_b_cl -- Gathers the selected component from the texture footprint addressed by the image and sampler descriptors and writes the gathered values to the vector destination
// [HW-observed] samp-carrier fetch: the S#/T# tuples, coordinates, and dmask select the filtered result; the CPU model passes the fetched words through.
dst[0] = src[0];
