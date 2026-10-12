// check -- Orders the pending kernel, VM, vector-store and shared-memory work named by the encoded domain bits; no value result.
//
// The domain bits are X0_1 (bit 0) = k (kernel/scalar), M.vm1 (bit 1) = vm
// (vector memory), X1_2[0] (bit 2) = vs (vector store) and X1_2[1] (bit 3)
// = sm (shared memory).  The vendor reference producer prints them by name
// in its own assembly (llc -march=lggpu: check k|vm|vs|sm(15), check k(1),
// check vm(2), check vs(4)), and its cached kernels carry the values
// 0x1/0x2/0x3/0x4/0x8.  A zero operand is not part of any producer-emitted
// contract: the vendor emits only the named non-zero forms (its own runtime
// headers use check k|vm|vs|sm(15) for every __threadfence* variant), and the
// observed 0x0 instances occur only where no listed class is pending.  The
// producer also emits check convergently - before the block's execution mask is
// narrowed - consistent with it not being a lane rendezvous.
//

return true;
