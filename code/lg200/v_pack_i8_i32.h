// v_pack_i8_i32 -- Clamps the first source to the signed 8-bit range and inserts it into a byte of the third source.
//

dst[0] = pack_i8_i32(src[0], src[1], src[2]);
