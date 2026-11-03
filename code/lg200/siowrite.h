// siowrite -- Writes one channel of an SIO stream from a register.
//
// The destination is the STREAM, not a register: VJ8 and VK8 are both sources
// and the machine row has no destination field.  The body is the memory-carrier
// passthrough (the reference names the data word the carrier stages), so the row
// is run in observed mode - the word read back after the instruction is the word
// staged before it, and no vector of this row is a comparison.

dst[0] = src[0];
