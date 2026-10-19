// store_v_tex -- Stores the dmask-selected vector components to the image descriptor at the vector coordinates
// [HW-observed] tex-carrier store: the T# tuple, coordinates, dmask, and the per-fragment window are provided by the texture contract; the CPU model passes the data words through.
dst[0] = src[0];
