// v_cmp_imm_nle_f32 -- Compares each f32 lane with the literal under the not-less-or-equal predicate, an unordered pair reporting true: `d = ((a > b) || unordered) ? 0xffff : 0x0000`.
//

// [HW 2026-10-13] the _std variant compares the raw bits, but this row flushes like the
// non-immediate one: nle(0x0, 0x80000001) comes back false, which is only true if the
// -denormal operand became -0 first, so 0 <= -0 holds and nle is its negation.  Fourteen
// cases all disagreed in that direction.
dst[0] = cmp_nle_f32(src[0], src[1], policy);
