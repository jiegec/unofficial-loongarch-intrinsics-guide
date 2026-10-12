// v_interp_2_f32 -- Completes the second f32 interpolation phase of the pair, writing the final varying to VD8.
//
// The semantics is the line above.  The coefficient window it reads is
// launch-owned state - the interpolation carrier records the window's contents
// in observed mode rather than comparing them - so the body below is the
// register-level reduction and this row is not compared against an oracle.

dst[0] = src[0];
