// v_firstv2s_b32 -- Copies the 32-bit value of the first active lane to a scalar register: `s = a[lane]`.
//
// [UNRESOLVED] The body below is a placeholder, not a model: this
// instruction's effect crosses lanes, and the reference oracle is per-lane, so
// it has nothing to say about which lane's value ends up where.

dst[0] = src[0];
