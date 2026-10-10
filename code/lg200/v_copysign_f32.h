// v_copysign_f32 -- Combines the magnitude of one f32 value with the sign bit of another: `d = {sign(b), magnitude(a)}`.
//

dst[0] = f32_copysign(src[0], src[1], policy);
