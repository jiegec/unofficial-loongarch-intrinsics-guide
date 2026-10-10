// store_v_tex_mip -- Stores mask-selected payload components into a level of the descriptor-addressed image.
//
// tex-carrier store: the T# tuple, coordinates, dmask, and the per-fragment window are provided by the texture contract; the CPU model passes the data words through.

dst[0] = src[0];
