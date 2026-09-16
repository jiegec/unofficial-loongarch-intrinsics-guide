// getresinfo -- Reads the image resource dimensions and level information selected by the coordinates and descriptor and writes the dmask-selected fields to the vector destination
// [HW-observed] tex-carrier info query: the resource dimensions and level fields are read from the T# descriptor window; the CPU model passes the query words through.
dst[0] = src[0];
