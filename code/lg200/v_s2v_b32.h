// v_s2v_b32 -- Broadcasts one scalar value to every lane of the vector destination.
//
// [UNRESOLVED] The body below is a placeholder, not a model: this
// instruction's effect crosses lanes, and the reference oracle is per-lane, so
// it has nothing to say about which lane's value ends up where.

dst[0] = src[0];
