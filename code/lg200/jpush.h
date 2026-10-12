// jpush -- Pushes a control-flow record onto the jump stack for a nested region.
//
// The semantics is the line above.  The body below is the register-level
// reduction the per-lane reference can express: the jump stack is control flow,
// which a per-lane value oracle does not model, so this row is run in observed
// mode rather than compared.

dst[0] = src[0];
