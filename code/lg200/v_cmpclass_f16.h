// v_cmpclass_f16 -- Tests each f16 lane for membership in a class set: `d = (class(a) & m) != 0`.
//

dst[0] = cmpclass_f16(src[1], src[0]);
