// s_movindex_b32 -- Reads a 32-bit value from execution-environment index state: `d = env[index]`.
//
// The encoding and the selector namespace are established the same way getreg's
// are (loonggpu U11): the selector is the encoded literal and the namespace is
// launch-owned index state.  The body is a deliberate passthrough, not a
// placeholder - the value is launch-owned and the offline oracle cannot
// synthesise it, so the runner stages it as the case input.

dst[0] = src[0];
