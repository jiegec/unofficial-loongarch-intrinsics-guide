// v_pack_imm_f16rtz_f32 -- Packs an f32 lane value and a literal as truncated f16 fields: `d = pack(f16rtz(a), f16rtz(imm))`.
//

dst[0] = pack_f16rtz_f32(src[0]);
