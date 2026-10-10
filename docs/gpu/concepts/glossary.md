# Glossary

Terms used throughout the Loongson GPU documentation.  A term is defined once
here; chapters link back to this page instead of redefining it.  Values that
differ between chip generations are not repeated here — see the generation
profile under [Chips](../chips/lg200.md).

## Hardware structure

| Term | Meaning |
|------|---------|
| **Generation** | A chip family with its own instruction encodings, state layout and limits (LG200 today).  Every chapter states its generation scope; the profile page carries the values. |
| **Pipe** | A hardware command-processing engine with its own ring, command vocabulary and completion source (GPIPE, BPIPE, CPIPE, XDMA).  Not the same as the **Pipe Enables** state field, which is a rasterization configuration word. |
| **Ring** | A circular buffer of command dwords in GPU memory, driven by a write/read cursor pair. |
| **Indirect buffer (IB)** | A nested command stream referenced by an outer ring command instead of being inlined. |
| **Command processor (CP)** | The front end that consumes ring and IB commands.  Commands that target it are named `CP_*` in producer vocabulary. |
| **Wave** | A group of shader invocations executed together by one shader unit. |
| **Lane** | One invocation slot inside a wave. |
| **VGPR / SGPR** | Vector / scalar general-purpose register files of the shader unit. |
| **LDS** | Workgroup shared memory, addressed by the shared-memory instructions (`load_sm_*`, `store_sm_*`, `atomic_sm_*`). |
| **SIO** | Shader input/output: the channel-addressed interface between shader stages and the fixed-function pipeline. |

## Command and state

| Term | Meaning |
|------|---------|
| **ICMD32** | The kernel-level command word that configures a pipe, queue, MMU, interrupt or VRAM window. |
| **SCMD32** | The outer-ring command word consumed by the command processor. |
| **Envelope** | The fixed IB framing around one graphical state object (a header, the state object, a terminator and an end command). |
| **State object** | The 512-dword graphical payload: draw, raster, depth, blend, attachment and stage configuration. |
| **State offset** | An offset relative to the first dword of the state object.  The packet byte offset is the state offset plus the envelope header size. |
| **Shader-info record** | A per-stage 0x60-byte record holding resource sizes, the kernel entry address, descriptor roots and stage arguments. |
| **Root** | A shader-info address field that points at descriptor memory (not at payload data). |
| **Descriptor** | A fixed-size record that describes a resource: buffer, image, sampler or attribute. |
| **Pipeline state** | State that selects or configures fixed-function behaviour around a stage (topology, interpolation, attachments), as opposed to instruction operands. |

## Shader ISA

| Term | Meaning |
|------|---------|
| **Machine row** | One instruction variant: mnemonic plus the operand form it encodes.  The instruction table has one row per variant. |
| **Format family** | A group of rows that share an encoding layout (`vop`, `vop-imm`, `sop`, `flat`, `shared-memory`, `tex-buffer`, …). |
| **General source selector** | The `GJ`/`GK`/`GA` operand class: an 8-bit base selector (SGPR, `zero`, inline constant, special island) optionally extended by a namespace bit (VGPR) and raw-bit ABS/NEG modifiers. |
| **Special island** | Encodings in the general-source table that are not ordinary scalar registers and must not enter a scalar allocation pool. |
| **Predicate** | The `PJ` operand field that selects lane participation for one instruction. |
| **Modifier** | An encoded operation modifier (`M.*`), such as clamp, buffer control or channel mask — distinct from per-source raw-bit ABS/NEG. |
| **Reserved / ignored bits** | Encoding positions a row requires to be emitted at the base value (reserved) or that carry no operand meaning (ignored). |
| **Lane mask** | The set of lanes an instruction acts on; see the ISA semantics chapter for how a row expresses it. |
| **UNRESOLVED** | A property this documentation does not pin down.  It is written explicitly instead of being guessed. |
| **LOONGGPU_DELTA** | Behaviour that differs from the common convention of a comparable API or architecture, and is documented as a difference rather than as a general rule. |

## Documentation status

Every statement in this section is either a property of the architecture, a
property of the implementation, or marked `UNRESOLVED`.  There is no separate
"evidence tier": an instruction page documents the behaviour of its machine
row, and anything not established is called out as unresolved in place.
