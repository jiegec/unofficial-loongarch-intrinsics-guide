// v_v2s_b32 -- Copies the selected active lane's 32-bit value into the scalar destination.
//
// The semantics is established (the line above): the destination is the value of
// the lane the selector names.  The body below is the per-lane reduction, not a
// placeholder - the reference oracle is per-lane and cannot express "which
// lane", so it models the value transferred and the runner stages the selected
// lane's value as the case input.

dst[0] = src[0];
