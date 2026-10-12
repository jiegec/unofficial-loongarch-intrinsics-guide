// v_emit2 -- Emits geometry-stream vertex 2 from the geometry output state for each active lane.
//
// The semantics is the line above.  The geometry path is characterised by the
// loonggpu U6/U7 work (the hardware generates the invocations and the prefix
// carries the stream state), so the stream side effect here is established
// rather than unknown.  The body below is the register-level reduction: a
// per-lane value oracle cannot express a stream write, so the row is
// observed-mode.

dst[0] = src[0];
