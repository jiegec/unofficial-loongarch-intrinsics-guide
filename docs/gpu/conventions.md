# Conventions

This chapter defines the notation and the normative vocabulary used throughout
the manual.

## Normative terms

The key words **MUST**, **MUST NOT**, **SHOULD**, **SHOULD NOT** and **MAY** are
used with the following meanings.

| Term | Meaning |
|------|---------|
| **MUST** | A requirement of the hardware or of the interface. A violation produces the described failure. |
| **MUST NOT** | A prohibited action. |
| **SHOULD** | A recommended practice. A violation is permitted only with a stated reason. |
| **MAY** | An optional behaviour. |
| Plain statement | A fact about the hardware or the interface. |

A statement about a driver convention rather than a hardware requirement is
introduced by **Host policy**. Host policy is not part of the hardware contract
and another driver may choose differently.

## Unresolved behaviour

A property that is not established is marked `UNRESOLVED` in place:

> UNRESOLVED: whether a misaligned flat access faults or is rounded down.

An unresolved item states the question and its consequence. It never states a
guess as a fact. The [UNRESOLVED register](appendices/unresolved.md) indexes
all such items.

## Numbers and bit ranges

| Notation | Meaning |
|----------|---------|
| `0x1234` | A hexadecimal value. |
| `[msb:lsb]` | An inclusive bit range, most significant bit first. |
| `[n]` | A single bit. |
| dword | A 32-bit word. |
| qword | A 64-bit word. |
| B, KiB, MiB, GiB | Byte, kibibyte (1024 B), mebibyte (1024 KiB), gibibyte (1024 MiB). |
| VA | A GPU virtual address. |
| PA | A physical address. |

Unless stated otherwise, an offset or a size is in bytes, an address is a byte
address, and a register index is decimal. Instruction words are little-endian:
the low word (`INST_LO`) is at the lower byte address.

## Register names

| Name | Meaning |
|------|---------|
| `sN` | Scalar general-purpose register N, shared by a wave. |
| `vN` | Vector general-purpose register N, private to a lane. |
| `pN` | Lane-mask register N, one bit per lane. |
| `SCC` | Scalar condition code, one bit per wave. |
| `PC` | Program counter. |

A tuple is written with a count: `s[a:b]` is a range of scalar registers,
`v[a:b]` a range of vector registers. A tuple base is aligned as described in
[Operands](isa/operands.md).

## Tables

A field table has the following columns unless stated otherwise:

| Column | Meaning |
|--------|---------|
| Bits | The field's position in the enclosing word or record. |
| Field | The field name. |
| Type | Encoding type: unsigned, signed, float pattern, address, enum, boolean. |
| Reset | Value written when the field is not otherwise programmed. |
| Description | The field's meaning. |

An enumeration is a separate table with `Value`, `Name` and `Meaning` columns.
`—` marks a field that has no value in the stated context. `N/A` and `etc.` are
not used.

## Cross-references

A chapter links to the one place where a fact is defined. A repeated value in
another chapter is a summary and carries a link to its definition. Bit positions
are stated once, in the field table; prose refers to the field by name.

## Figures

A figure is used only when it shows a relation that prose or a table cannot show
as clearly: containment, ownership, address-space layout or data flow. Every
figure has a caption that states what it adds, and the body refers to it.
