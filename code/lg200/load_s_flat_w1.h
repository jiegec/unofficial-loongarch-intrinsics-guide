// load_s_flat_w1 -- Loads one 32-bit word from a flat address pair plus a 20-bit offset.
//
// The address contract is established (loonggpu Axis-1/U3, from mesa loongvk and
// the closed producer): a flat access carries the FULL absolute 64-bit VA in the
// vector-address pair with a literal-zero scalar base pair - in compute s[6:7] is
// the kernarg pointer, not a segment base.  This row stays observed-mode only
// because the SCALAR-destination form has no independent per-lane oracle, not
// because the memory contract is unknown.

dst[0] = src[0];
