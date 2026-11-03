// siowrite_imm -- Writes one channel of an SIO stream from a register, with an immediate channel slot.
//
// The destination is the STREAM, not a register (VJ8 is the data source and
// M.chan12 the slot; the row has no destination field).  As on siowrite the body
// is the memory-carrier passthrough and the row is run in observed mode: the
// readback is the staged data word, so it is not a comparison.

dst[0] = src[0];
