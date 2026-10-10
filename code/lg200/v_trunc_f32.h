// v_trunc_f32 -- Rounds each f32 lane toward zero to an integral value: `d = trunc(a)`.
//

dst[0] = f32_trunc(src[0], policy);
