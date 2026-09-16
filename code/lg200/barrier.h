// barrier -- Synchronizes participating lanes in the current workgroup and
// orders workgroup-visible memory accesses across the barrier.
// Workgroup-wide ordering effect only: participating lanes converge at the
// barrier and accesses before/after it are ordered for the workgroup. No
// register result is produced (oracle model: ordering effect).
return true;
