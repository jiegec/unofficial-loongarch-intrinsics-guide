// v_pack_snorm16_f32 -- Converts two f32 values to signed normalized 16-bit fields, packed into one word.
//
// [HW 2026-11-03] The title says two values and the encoding agrees - it carries
// two source fields, GJ11 and GK11 (lg200_codec.inc:4727, canonical
// "p0, v0, zero, v0") - but the body below reads one, and every case vector
// carries one input.  So the low half of the result is established by those
// vectors (1.0 -> 0x00007fff, -1.0 -> 0x00008001) and the high half is not: no
// vector has ever put a non-zero value in the second source, and no published
// rule says which half each source fills.  The carrier therefore finds no case
// row it can stage and the row is skipped on BOTH pipes - a model gap, not a
// hardware one.  Stating the high half needs a vector with two distinct sources
// and an independently derived order; that is UNRESOLVED and is not guessed
// here.

dst[0] = pack_snorm16_f32(src[0]);
