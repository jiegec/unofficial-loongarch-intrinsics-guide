// v_cmp_neq_f64 -- Compares two f64 values for inequality and writes a lane mask: `d = (a != b) ? 0xffff : 0x0000`.
//
// [HW 2026-11-03] The line above states the DOCUMENTED intent; what this row's oracle
// implements is the measured behaviour, and the two are not the same.  A 144-point
// two-dimensional sweep (2026-10-13, ops/oracle.cpp:1054) found that this row compares the
// HIGH half of a with the LOW half of b as two f32 values - flushing a denormal b half first -
// and that this shape matches all 144 cells where a whole-f64 comparison does not.  The rows
// are their own encodings, not a family: each of the eight uses a different construction, and
// the f32 and f16 siblings really do compare whole values.  So a reader who implements from
// this sentence alone will disagree with the hardware; the oracle is the authority here, and
// the sentence is kept because it is what the ISA page claims.

dst[0] = cmp_neq_f64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), policy);
