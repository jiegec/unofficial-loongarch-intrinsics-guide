// v_fcvt_i32_f32 -- Converts each lane single-precision source to a signed 32-bit integer, truncating toward zero.
//
// [HW 2026-11-03] The title used to say "to the nearest" while the helper it calls
// (ops/softfloat.cpp:454) rounds TowardZero whatever the policy says: 3.5 gives 3 and -42.8
// gives -42.  None of the 41 vectors lands on a tie, so the row cannot tell the two rules
// apart, and every one of them was computed with the truncation the code performs.  Which
// rule the hardware uses is UNRESOLVED and is recorded here rather than decided by editing
// an expectation.
//

dst[0] = f32_i32(src[0], policy);
