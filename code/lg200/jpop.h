// jpop -- Pops a control-flow record from the jump stack and resumes the
// selected path under the encoded predicate and stack operands.
// CPU model: the execution environment supplies the popped record value in
// src[0]; stack layout, predicates, and reconvergence state are part of the
// execution contract and are not modeled at register level.
dst[0] = src[0];
