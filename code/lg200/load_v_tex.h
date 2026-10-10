// load_v_tex -- Fetches channel-masked image components through a descriptor into four registers.
//
// tex-carrier load: the T# tuple, coordinates, dmask, and the per-fragment window are provided by the texture contract; the CPU model passes the data words through.

dst[0] = src[0];
