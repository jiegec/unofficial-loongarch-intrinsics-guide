// check -- Applies the encoded execution-domain check to pending K, VM, VS,
// and shared-memory work. The defined form is an ordering operation and
// produces no value result.
// [HW-observed] The carrier records check as survived vs terminated: the
// pending kernel/VM/VS/shared-memory work is ordered before the stage
// completes; there is no register output.
return true;
