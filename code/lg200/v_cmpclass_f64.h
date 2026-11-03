// v_cmpclass_f64 -- Tests each f64 lane for membership in a class set: `d = (class(a) & m) != 0`.
//

// GJ11 is the value pair (x2, like every other f64 predicate row) and GK11 the
// class set, which is ONE word: the set is a class bitmask, not a second f64, and
// the vendor's own template prints that operand as a single register.  RD8 is
// one 32-bit lane mask, so there is no second result word to carry.

dst[0] = (u32)cmpclass_f64((u64)src[2], ((u64)src[0] | ((u64)src[0+1] << 32)));
