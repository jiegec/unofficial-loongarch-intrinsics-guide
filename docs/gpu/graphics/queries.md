# Queries

The device counts samples and primitives and keeps a free-running clock. A query
is a pair of packets around a draw, each naming the 64-bit GPU address it writes.

## Selectors

A query packet's leading dword holds an opcode in bits [7:0] and a 24-bit field
in bits [31:8]. Under opcode `0x0c` (end) or `0x0d` (begin), that field is a
counter selector.

| Selector | Value | Counts |
|----------|------:|--------|
| Occlusion | `0x400000` | Covered samples; also the source of fragment-invocation counts. |
| Primitives generated | `0x2000` | Primitives formed by the last pre-rasterisation stage — geometry if present, else the input assembly — before clipping. |
| Primitives written | `0x20000` | Streamout records committed; delivered by the four-dword streamout-end word. |
| Transform-feedback primitives written | `0x200` | Listed as a selector; no packet carries it. UNRESOLVED. |

An "any samples" predicate has no selector of its own: both forms read
`0x400000` and the host reduces the count to a boolean. The generated counter
follows wave padding and precedes clipping, so it is neither the count submitted
nor the count that survives the frustum. `0x200` is a phase under `0x13` and a
selector under `0x0c` or `0x0d`, so a decoder MUST switch on the opcode first.

UNRESOLVED: the stamp rate of the occlusion counter. One source states one stamp
per covered sample; another states two per sample for a draw with no
depth/stencil attachment, regardless of the depth test. The readback applies no
scale in either case, so a bracket that mixes draws with and without such an
attachment cannot be normalized from the counter alone.

## Packets and placement

| Packet | Dwords | Words | Placement |
|--------|-------:|-------|-----------|
| Query begin | 1 | `(selector << 8) or 0x0d` | Head of the buffer, in front of the state record. |
| Query end | 3 | `(selector << 8) or 0x0c`, address low, address high | Behind the record's terminator `0x3`, in the buffer that carries the draw. |
| Timestamp sample | 3 | `(phase << 8) or 0x13`, address low, address high | Head of the buffer, paired with one behind the record. |
| Streamout end | 4 | `0x0200_000c` (`0x20000 << 8 or 0x0c`), address low, address high, `0x16` | At the record's end, where the end command would otherwise be. |

A buffer that carries a query packet carries at most one draw and MUST NOT also
carry a second draw's state record: the two fault together rather than serialize.
The begin packet needs no Submit word. In a buffer that carries a state record the
end packet stands behind `0x3`, either the record's own terminator or one of its
own; in a record-free buffer it follows the begin directly.

UNRESOLVED: how the end packet is framed against the end command `0xf05`. Three
framings are in use — the packet replaces the end command and the stream
re-appends one; the packet follows the end command behind a Submit word; the end
command stays and two zero dwords separate it from the packet. A driver SHOULD
replace the end command and re-append it last. The surrounding record vocabulary
is in [indirect buffers](../host/indirect-buffers.md).

## Begin and end

`0x0d` resets the counter. The reset applies when the packet is parsed, so
fragments of a still-running submission reach the counter after it and land
inside the bracket that just opened. `0x0c` samples the counter where it stands:
two samples measure an interval without touching it and can neither lose nor
gain work.

The counter is an accumulator — a draw's increments post when a later draw
completes, not when the sample executes. An end sample MUST follow the draw it
measures in the same buffer, with nothing but the end command and padding behind
it, and a draw-free buffer MUST NOT be used for an exact count. Counters are
device-global, so a bracket is exact only while no other client's work runs
inside it. A bracket spanning submissions keeps one begin, on the first, and one
end, on the last.

## Result buffer

Every packet carries its own result address and there is no shared result base.
The device writes eight bytes there and nothing else: no availability word, no
sequence number, no sentinel. Availability is the submission's fence, so a host
MUST keep the buffer resident and mapped until that fence signals, MUST NOT read
the word before it, and MUST give each outstanding query its own slot. One
bracket can own three slots — base, base + 8 for a second sample and base + 16
for a partial count — but that layout is a host convention, since every packet
names its own address.

The result address is an ordinary client virtual address. The device writes it,
so the range MUST be mapped writable, and the PTE path requires the executable
bit too. The mapping is rounded to the GPU page size, 4 KiB or 16 KiB as built.
Whether the device requires an eight-byte-aligned address is UNRESOLVED; every
placement in the sources is eight-byte aligned.

## Timestamps

Opcode `0x13` carries a 24-bit phase in bits [31:8]: `1` opens a bracket, `0x200`
closes it. Both write a 64-bit tick count at the address they carry. Two samples
eight bytes apart is a host layout: the device subtracts nothing, so an elapsed
time is the difference of the two words and the end phase MUST NOT be read as a
duration.

A free-running 64-bit counter is also readable from the host on this generation,
as an MMIO pair: `0x90` low half and `0x94` high half. Each access is 32 bits, so
a carry between the two reads yields a value 2^32 too low; whether the device
latches the high half when the low half is read is UNRESOLVED.

UNRESOLVED: the tick rate, and whether the packet samples the MMIO counter. The
only rate in the sources is 750 MHz, converted as ticks × 4 / 3 and applied as a
constant while the engine clock is scalable. Either the counter is a fixed
750 MHz clock independent of the engine, or the conversion is wrong at every
other clock level.

UNRESOLVED: whether a timestamp packet retires on its own; a buffer holding only
`0x13` packets is not established to complete. The producers also disagree on
the source of a host-visible timestamp — one samples the packet, the other says
the generation exposes no timestamp register and stamps the host clock. A driver
MUST NOT compare values from the two sources.