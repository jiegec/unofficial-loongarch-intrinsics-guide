// s_cmp_eq_u64 -- Compares the two unsigned 64-bit integer operands with
// the equality predicate, and writes 0xffff for true or zero for false to
// SCC or the scalar destination.
// [HW-observed 2026-08] LOONGGPU_DELTA: on the PS carrier the u64 compare
// rows execute as (src[1] pair high word != 0) (launch-state dependent
// observation); the nominal 64-bit comparison below is the CPU model.
dst[0] = cmp_eq_u64(((u64)src[0] | ((u64)src[0+1] << 32)),
                    ((u64)src[2] | ((u64)src[2+1] << 32)), policy);
