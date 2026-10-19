// atomic_sm_add_u32 -- Atomically reads an unsigned 32-bit integer from workgroup shared memory, computes new = old + source modulo the operand width, and writes new back to the same location
// src[0] is the old shared-memory value read at the start of the operation;
// dst[0] returns the old value and dst[1] is the value written back.
u32 old = src[0];
dst[0] = old;
dst[1] = old + src[1];      // wraps at 32 bits
