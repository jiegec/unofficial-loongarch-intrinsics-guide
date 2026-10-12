// load_s_buff_w1 -- Loads one 32-bit word from the buffer resource of a four-register record plus a 20-bit offset.
//
// The address contract is established (loonggpu Axis-1/U3, from mesa loongvk and
// the closed producer): the buffer form addresses through its four-register
// record, the same descriptor contract the buffer carrier hydrates.  This row
// stays observed-mode only because the SCALAR-destination form has no independent
// per-lane oracle, not because the memory contract is unknown.

dst[0] = src[0];
