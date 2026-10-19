// jpush -- Pushes a control-flow record onto the jump stack using the encoded
// predicate, scalar operands, and displacement.
// CPU model: src[0] carries the observed record value at the join point;
// stack layout, predicates, and reconvergence state are part of the
// execution contract and are not modeled at register level.
dst[0] = src[0];
