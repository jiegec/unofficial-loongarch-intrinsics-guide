// getreg -- Reads the execution-environment register selected by the encoded immediate into a scalar destination.
//
// The encoding and the selector namespace are established (loonggpu U11, from
// the closed producer and the kernel-info block): the selector is the encoded
// 32-bit literal (IJ32), the result the scalar destination (RD8), and the
// namespace is launch-owned state - the group id (s11-s13), the local id
// (v0-v2) and the sizes, offsets and dimensions the kernarg block carries.
//
// The body is a deliberate passthrough, not a placeholder: the hardware result
// is launch-owned state the offline oracle cannot synthesise, so the runner
// stages the value under test as the case input and this reference copies it -
// the same observed-mode contract the memory carriers use.

dst[0] = src[0];
