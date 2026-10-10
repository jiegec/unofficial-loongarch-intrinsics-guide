// load_v_tex_mip -- Fetches channel-masked components of one image level chosen by the resource operand.
//
// tex-carrier load: the T# tuple, coordinates, dmask, and the per-fragment window are provided by the texture contract; the CPU model passes the data words through.

dst[0] = src[0];
