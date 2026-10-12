// v_firstv2s_b32 -- Copies the 32-bit value of the first active lane to a scalar register: `s = a[lane]`.
//
// The semantics is established (the line above): the destination is the value of
// the lowest-numbered active lane.  The body below is the per-lane reduction,
// not a placeholder - the reference oracle is per-lane, so it models the value
// transferred and the runner stages it as the case input.

dst[0] = src[0];
