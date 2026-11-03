// getreg -- Reads the execution-environment register selected by the encoded immediate into a scalar destination.
//
// The encoding and the selector namespace are established (loonggpu U11, from
// the closed producer and the kernel-info block): the selector is the encoded
// 32-bit literal (IJ32), the result the scalar destination (RD8), and the
// namespace is launch-owned state - the group id (s11-s13), the local id
// (v0-v2) and the sizes, offsets and dimensions the kernarg block carries.
//
// The body names no value, and unlike the memory carriers it cannot: getreg has
// NO data source, so there is nowhere to stage a value under test and no case
// input can reach the result.  The earlier passthrough said the runner stages the
// value as the case input; that was never true of this row - the control carrier
// stages nothing for it and reads nothing back.  Its evidence is the control
// sentinel only.

dst[0] = 0u;
