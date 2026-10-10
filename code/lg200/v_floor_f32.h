// v_floor_f32 -- Rounds each lane's f32 source toward negative infinity: `d = floor(a)`.
//

dst[0] = f32_floor(src[0], policy);
