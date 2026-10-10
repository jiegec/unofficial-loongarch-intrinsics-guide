// v_cmpclass_f32 -- Tests each f32 lane for membership in a class set: `d = (class(a) & m) != 0`.
//

dst[0] = cmpclass_f32(src[1], src[0]);
