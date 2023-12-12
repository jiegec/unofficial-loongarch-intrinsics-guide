# Bitwise Operations

## __m128i __lsx_vbitsel_v (__m128i a, __m128i b, __m128i c)

### Synopsis

```c++
__m128i __lsx_vbitsel_v (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vbitsel.v vr, vr, vr
CPU Flags: LSX
```

### Description

Compute bitwise selection: for each bit position, if the bit in `c` equals to one, copy the bit from `b` to `dst`, otherwise copy from `a`.

### Operation

```c++
{% include 'vbitsel_v.h' %}
```

## __m128i __lsx_vbitseli_b (__m128i a, __m128i b, imm0_255 imm)

### Synopsis

```c++
__m128i __lsx_vbitseli_b (__m128i a, __m128i b, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vbitseli.b vr, vr, imm
CPU Flags: LSX
```

### Description

Compute bitwise selection: for each bit position, if the bit in `a` equals to one, copy the bit from `imm` to `dst`, otherwise copy from `b`.

### Operation

```c++
{% include 'vbitseli_b.h' %}
```

{{ vbitclr('b') }}
{{ vbitclr('h') }}
{{ vbitclr('w') }}
{{ vbitclr('d') }}

{{ vbitclri('b') }}
{{ vbitclri('h') }}
{{ vbitclri('w') }}
{{ vbitclri('d') }}

{{ vbitset('b') }}
{{ vbitset('h') }}
{{ vbitset('w') }}
{{ vbitset('d') }}

{{ vbitseti('b') }}
{{ vbitseti('h') }}
{{ vbitseti('w') }}
{{ vbitseti('d') }}

{{ vbitrev('b') }}
{{ vbitrev('h') }}
{{ vbitrev('w') }}
{{ vbitrev('d') }}

{{ vbitrevi('b') }}
{{ vbitrevi('h') }}
{{ vbitrevi('w') }}
{{ vbitrevi('d') }}

{{ vclo('b') }}
{{ vclo('h') }}
{{ vclo('w') }}
{{ vclo('d') }}

{{ vclz('b') }}
{{ vclz('h') }}
{{ vclz('w') }}
{{ vclz('d') }}

{{ vexth('h', 'b') }}
{{ vexth('hu', 'bu') }}
{{ vexth('w', 'h') }}
{{ vexth('wu', 'hu') }}
{{ vexth('d', 'w') }}
{{ vexth('du', 'wu') }}
{{ vexth('q', 'd') }}
{{ vexth('qu', 'du') }}

{{ vextl('q', 'd') }}
{{ vextl('qu', 'du') }}

{{ vextrins('b') }}
{{ vextrins('h') }}
{{ vextrins('w') }}
{{ vextrins('d') }}

{{ vpcnt('b') }}
{{ vpcnt('h') }}
{{ vpcnt('w') }}
{{ vpcnt('d') }}