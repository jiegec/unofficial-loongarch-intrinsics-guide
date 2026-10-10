// v_subb_u32 -- Subtracts the second u32 source from the first under a borrow state held in the third source.
//

dst[0] = (src[2] & 0x3fffffffu) == 0x3fffffffu
             ? src[0] - src[1] - 1u
             : (src[2] & 0x3fffffffu) == 0u
                   ? src[0] - src[1]
                   : (src[0] - src[1]) | (src[0] - src[1] - 1u);
