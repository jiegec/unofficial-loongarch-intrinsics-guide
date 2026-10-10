// load_v_tex_mip_pck_sgn -- Fetches masked components of one image level with signed component interpretation.
//
// tex-carrier load: the T# tuple, coordinates, dmask, and the per-fragment window are provided by the texture contract; the CPU model passes the data words through.

dst[0] = src[0];
