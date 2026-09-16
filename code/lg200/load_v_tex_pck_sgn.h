// load_v_tex_pck_sgn -- Loads the components selected by dmask from the image descriptor at the vector coordinates into the vector destination
// [HW-observed] tex-carrier load: the T# tuple, coordinates, dmask, and the per-fragment window are provided by the texture contract; the CPU model passes the data words through.
dst[0] = src[0];
