// v_s2v_b32 -- Broadcasts one scalar value to every lane of the vector destination.
//
// The semantics is established (the line above): every active lane receives the
// scalar source.  The body below is the per-lane reduction of that effect, not a
// placeholder - the reference oracle is per-lane, so it can only model the value
// a lane receives, which is the same for every lane; the runner stages that
// value as the case input, the observed-mode contract the memory carriers use.

dst[0] = src[0];
