#!/usr/bin/env python3
"""gen_lbt_verify.py — complete boundary-covering test case generator.

Covers all points from design review:
  1. Logic AF FORCE_0 explicit
  2. ADD/SUB: CF×OF grid, cross-width AF/PF, CF/OF 2D
  3. Shift: operand data patterns (0x00,0xFF,0x55,0xAA,sign-only), OF at c=1
  4. Rotate: OF=0/1 at c=1 explicit, preserve labels
  5. MUL: SF/ZF/AF/PF FORCE_0 explicit
  6. ADC: input interference (other flag bits must not affect result)
  7. Cross-width AF (bit3→4) and PF (low 8 bits) independence
  8. Shift count masking: c=width→0, c=width+1→1
"""

import argparse
import sys, os
from collections import defaultdict
from pathlib import Path

HEADER_DIR = Path(__file__).resolve().parent
OUTPUT_C = HEADER_DIR / "gen_verify.c"
OUTPUT_PROBE_C = HEADER_DIR / "gen_probe_lbt.c"
OUTPUT_PROBE_DIR = HEADER_DIR / "gen_probe_lbt_chunks"
OUTPUT_X86_C = HEADER_DIR / "gen_verify_x86.c"

W = {"b":8,"h":16,"w":32,"d":64,"wu":32,"du":64,"bu":8,"hu":16}
FLAG_BITS = {"CF":0x001,"PF":0x004,"AF":0x010,"ZF":0x040,"SF":0x080,"OF":0x800}
FLAG_ORDER = ("CF","PF","AF","ZF","SF","OF")
FLAG_MASK_ALL = sum(FLAG_BITS.values())
def wmax(b): return (1<<b)-1
def smax(b): return (1<<(b-1))-1
def smin(b): return -(1<<(b-1))
def stem(n): return n.split(".")[0]
def suf(n): return n.split(".")[-1] if "." in n else ""
def width(n): return W.get(suf(n),32)

def has_flag_meta(name):
    p = HEADER_DIR / (name.replace(".","_") + ".h")
    if not p.exists(): return False
    for l in open(p):
        s=l.strip()
        if s.startswith("// @flag ") and not s.startswith("// @flag: none"):
            return True
    return False

# ── Mw / TV ──────────────────────────────────────────────────────────────
class Mw:
    def __init__(s,name,rj=True,rk=False,rd=False,rd_val=12,imm_val=None,cond_val=None,
                 mask_val=None,top_val=None,crd_val=None,crj_val=None):
        s.name=name;s.rj=rj;s.rk=rk;s.rd=rd;s.rd_val=rd_val
        s.imm_val=imm_val;s.cond_val=cond_val;s.mask_val=mask_val
        s.top_val=top_val;s.crd_val=crd_val;s.crj_val=crj_val
    @property
    def h_file(self): return self.name.replace(".","_")+".h"
    def decl(self):
        p=[]
        if self.rd:p.append(f"rd={self.rd_val}")
        if self.rj:p.append("rj=12")
        if self.rk:p.append("rk=13")
        if self.imm_val is not None:p.append(f"imm={self.imm_val}")
        if self.cond_val is not None:p.append(f"cond={self.cond_val}")
        if self.mask_val is not None:p.append(f"mask={self.mask_val}")
        if self.top_val is not None:p.append(f"top={self.top_val}")
        if self.crd_val is not None:p.append(f"crd={self.crd_val}")
        if self.crj_val is not None:p.append(f"crj={self.crj_val}")
        return "const unsigned "+",".join(p)+";" if p else ""
    def vars(self):
        p=[]
        if self.rd:p.append("rd")
        if self.rj:p.append("rj")
        if self.rk:p.append("rk")
        if self.imm_val is not None:p.append("imm")
        if self.cond_val is not None:p.append("cond")
        if self.mask_val is not None:p.append("mask")
        if self.top_val is not None:p.append("top")
        if self.crd_val is not None:p.append("crd")
        if self.crj_val is not None:p.append("crj")
        return p

def mw_for(name):
    st=stem(name); is_arm=name.startswith("arm"); is_imm=st.endswith("i")
    if is_arm:
        if st in ("armnot","armrrx") or st.startswith("armmov."):
            return Mw(name,rj=True,rk=False,rd=False,cond_val=14)
        if st=="armmove": return Mw(name,rj=True,rk=False,rd=True,cond_val=14)
        if st=="setarmj": return Mw(name,rj=False,rk=False,rd=True,cond_val=14)
        if is_imm: return Mw(name,rj=True,rk=False,rd=False,imm_val=0,cond_val=14)
        return Mw(name,rj=True,rk=True,rd=False,cond_val=14)
    if st in ("x86inc","x86dec"): return Mw(name,rj=True,rk=False,rd=False)
    if st in ("adc","sbc","rotr","rcr"): return Mw(name,rj=True,rk=True,rd=True)
    if st in ("rotri","rcri"): return Mw(name,rj=True,rk=False,rd=True,imm_val=0)
    if st == "addu12i": return Mw(name,rj=True,rk=False,rd=True,imm_val=0)
    if is_imm: return Mw(name,rj=True,rk=True,rd=False,imm_val=0)
    if st in ("setx86j","setx86loope","setx86loopne","x86mfflag","movscr2gr","x86mftop"):
        return Mw(name,rj=True,rk=False,rd=True)
    return Mw(name,rj=True,rk=True,rd=False)

class TV:
    def __init__(s,label,rj,rk=0,efl=None,armfl=None,asm_t="",
                 is_arm=False,imm_val=None,mw=None):
        s.label=label;s.rj=rj;s.rk=rk;s.efl=efl;s.armfl=armfl
        s.asm_t=asm_t;s.is_arm=is_arm;s.imm_val=imm_val;s.mw=mw

class StateTV:
    def __init__(s,label,asm_t,rj=0,rk=0,efl=None,armfl=None,scratch0=0,
                 top=0,tm=False,mw=None,checks=None):
        s.label=label;s.asm_t=asm_t;s.rj=rj;s.rk=rk;s.efl=efl;s.armfl=armfl
        s.scratch0=scratch0;s.top=top;s.tm=tm;s.mw=mw;s.checks=checks or ("gpr12",)

# ── Helpers ──────────────────────────────────────────────────────────────
def _dd(tvs,bits):
    seen,out=set(),[]; M=wmax(bits)
    for t in tvs:
        t.rj&=M;t.rk&=M
        k=(t.rj,t.rk,str(t.efl),str(t.armfl),t.imm_val)
        if k not in seen:seen.add(k);out.append(t)
    return out

EF0={}; EF1={"CF":1,"PF":1,"AF":1,"ZF":1,"SF":1,"OF":1}
EF_OF={"OF":1}; EF_AF={"AF":1}; EF_PF={"PF":1}
EF_CF={"CF":1}; EF_no_CF={"PF":1,"AF":1,"ZF":1,"SF":1,"OF":1}  # all except CF

AF=(1,1,1,0); AF0=(0,0,0,0); afC=(1,1,1,1)

def f_ef(d):
    d=d or {}; return f"{{.CF={d.get('CF',0)},.PF={d.get('PF',0)},.AF={d.get('AF',0)},.ZF={d.get('ZF',0)},.SF={d.get('SF',0)},.OF={d.get('OF',0)}}}"
def f_af(b):
    if isinstance(b,(list,tuple)):n,z,c,v=b
    elif isinstance(b,dict):n=b.get("N",0);z=b.get("Z",0);c=b.get("C",0);v=b.get("V",0)
    else:n=z=c=v=0
    return f"{{.N={n},.Z={z},.C={c},.V={v}}}"

def flag_mask(*names):
    m = 0
    for n in names:
        m |= FLAG_BITS[n]
    return m

# ═══════════════════════════════════════════════════════════════
# Complete generators — design review v4
# ═══════════════════════════════════════════════════════════════

def gen_addsub(n):
    """Binary add/sub. CF×OF grid, cross-width AF/PF (#2, #7)."""
    b=width(n); M=wmax(b); sub="sub" in stem(n); a=f"{n} %0,%1"

    # Base set: 2 per flag (0/1)
    if sub:
        base = [
            TV("ZF=1 eq",42,42,asm_t=a),     TV("ZF=0 diff",5,2,asm_t=a),
            TV("CF=1 borrow",0,1,asm_t=a),   TV("CF=0 no borrow",1,0,asm_t=a),
            TV("OF=1 INT_MIN-1",smin(b)&M,1,asm_t=a), TV("OF=0 simple",1,2,asm_t=a),
            TV("AF=1 nibble",0x10,1,asm_t=a),TV("AF=0 no nib",0x11,1,asm_t=a),
            TV("SF=1 neg",0,1,asm_t=a),       TV("SF=0 pos",1,0,asm_t=a),
            TV("PF=1 even 3-1",3,1,asm_t=a),  TV("PF=0 odd 4-1",4,1,asm_t=a),
        ]
    else:
        base = [
            TV("ZF=1 0+0",0,0,asm_t=a),       TV("ZF=0 1+2",1,2,asm_t=a),
            TV("CF=1 MAX+1",M,1,asm_t=a),     TV("CF=0 MAX-1+1",M-1,1,asm_t=a),
            TV("OF=1 INT_MAX+1",smax(b),1,asm_t=a), TV("OF=1 INT_MIN+INT_MIN",smin(b)&M,smin(b)&M,asm_t=a),
            TV("OF=0 simple",1,2,asm_t=a),
            TV("AF=1 0x0F+1",0x0F,1,asm_t=a),TV("AF=0 0x0E+1",0x0E,1,asm_t=a),
            TV("SF=1 MSB",M,0,asm_t=a),       TV("SF=0 noMSB",1,0,asm_t=a),
            TV("PF=1 1+1",1,1,asm_t=a),       TV("PF=0 1+2",1,2,asm_t=a),
            TV("PF=1 result=0",0,0,asm_t=a),
        ]

    # CF×OF grid: 4 combos (#2)
    if sub:
        grid = [
            TV("CF=1 OF=1 borrow+ovf",smin(b)&M,1,asm_t=a),
            TV("CF=1 OF=0 borrow noovf",1,3,asm_t=a),
            TV("CF=0 OF=1 ovf no borrow",42,0x80 if b==8 else 0x8000 if b==16 else 0x80000000,asm_t=a),
        ]
    else:
        grid = [
            TV("CF=1 OF=1 carry+ovf",smax(b),smax(b),asm_t=a),
            TV("CF=1 OF=0 carry noovf",M,0,asm_t=a),
            TV("CF=0 OF=1 ovf no carry",smax(b),1,asm_t=a),
        ]

    # Cross-width AF: .w/.d explicit low-nibble-only (#7)
    af_extra = []
    if b>=16 and not sub:
        af_extra = [
            TV("AF=1 .w/.d nibble only",0x0F,1,asm_t=a),
            TV("AF=0 .w/.d nibble only",0x0E,1,asm_t=a),
        ]
    elif b>=16 and sub:
        af_extra = [
            TV("AF=1 .w/.d nibble sub",0x10,1,asm_t=a),
        ]

    # Cross-width PF: .w/.d low-8-only (#7)
    pf_extra = []
    if b==32 or b==64:
        pf_extra = [
            TV("PF=1 .w/.d low8 even hi junk",0xFF00FF00,0x00FF00FF,asm_t=a),
            TV("PF=0 .w/.d low8 odd hi junk",0xFF00FF01,0x00FF00FE,asm_t=a),
        ]

    return _dd(base + grid + af_extra + pf_extra, b)


def gen_adcsbc(n):
    """ADC/SBC: CF READ_COMPUTE. Input interference (#6), CF×OF grid (#2)."""
    b=width(n); M=wmax(b); a=f"{n} %0,%1"
    t = [
        # CF_in=0 basic
        TV("CF_in=0 ZF=1",0,0,efl=EF0,asm_t=a),
        TV("CF_in=0 ZF=0",1,2,efl=EF0,asm_t=a),
        TV("CF_in=0 carry",M,1,efl=EF0,asm_t=a),
        TV("CF_in=0 OF=1",smax(b),1,efl=EF0,asm_t=a),
        TV("CF_in=0 OF=0",1,2,efl=EF0,asm_t=a),
        # CF_in=1
        TV("CF_in=1 result+1",0,0,efl=EF_CF,asm_t=a),
        TV("CF_in=1 carry",M-1,1,efl=EF_CF,asm_t=a),
        TV("CF_in=1 OF=1",smax(b)-1,1,efl=EF_CF,asm_t=a),
        # Input interference (#6): other flag bits set, only CF matters
        TV("interfere other flags 1",0,0,efl=EF_no_CF,asm_t=a),
        TV("interfere other flags 0",0,0,efl=EF1,asm_t=a),
        # Other flags: AF/SF/PF
        TV("AF=1 nibble",0x0F,1,efl=EF0,asm_t=a),
        TV("AF=0 no nibble",0x0E,1,efl=EF0,asm_t=a),
        TV("SF=1 neg",M,0,efl=EF0,asm_t=a),
        TV("SF=0 pos",1,0,efl=EF0,asm_t=a),
        TV("PF=1 even",3,3,efl=EF0,asm_t=a),
        TV("PF=0 odd",1,2,efl=EF0,asm_t=a),
    ]
    # SBC-specific: AF erratum tests
    # LBT x86sbc computes AF = (lhs & 0xf) < (rhs & 0xf), which does NOT
    # incorporate CF input.  This diverges from x86 SBB when CF=1 and the
    # low nibbles would borrow across the nibble boundary after subtracting CF.
    # The canonical trigger: rj=0, rk=0, CF=1 → x86 AF=1, LBT AF=0.
    if "sbc" in stem(n):
        t.extend([
            TV("SBC AF erratum rj=0 rk=0 CF=1",0,0,efl=EF_CF,asm_t=a),
            TV("SBC AF erratum rj=0 rk=1 CF=1",0,1,efl=EF_CF,asm_t=a),
            TV("SBC AF erratum rj=0x10 rk=0 CF=1",0x10,0,efl=EF_CF,asm_t=a),
            TV("SBC AF no-erratum nibble< CF=1",0x10,0x0f,efl=EF_CF,asm_t=a),
        ])
    return _dd(t,b)


def gen_logic(n):
    """AND/OR/XOR. AF FORCE_0 explicit (#1), cross-width PF (#7)."""
    b=width(n); M=wmax(b); a=f"{n} %0,%1"
    stm = stem(n)
    t = [
        # CF/AF/OF FORCE_0: explicit per-flag check
        TV("CF=0 forced",M,M,asm_t=a),
        TV("AF=0 forced",M,M,asm_t=a),
        TV("OF=0 forced",M,M,asm_t=a),
        TV("CF/AF/OF=0 all zero",0,0,asm_t=a),
        # AF must stay 0 even with nibble boundary operands (#1)
        TV("AF=0 nibble boundary",0x0F,0x01,asm_t=a),
        # ZF
        TV("ZF=0",M,1,asm_t=a),
        TV("ZF=1 PF=1",0,0,asm_t=a),
        # PF
        TV("PF=0 odd parity",M,2,asm_t=a),
        TV("PF=1 even parity",1,1,asm_t=a),
        # SF
        TV("SF=1 MSB set",0x80 if b==8 else M,0x80 if b==8 else M,asm_t=a),
        TV("SF=0 MSB clear",1,1,asm_t=a),
    ]
    # Cross-width PF: .w/.d construct specific low 8-bit parity (#7)
    if b in (16,32,64):
        t.append(TV("PF low8 only w/d",0xFF00,0x00FF,asm_t=a))
        t.append(TV("PF low8 odd w/d",0xFF01,0x00FE,asm_t=a))
    return _dd(t,b)


def gen_incdec(n):
    """INC/DEC: CF PRESERVE, 5 COMPUTE."""
    b=width(n); M=wmax(b); inc="inc" in stem(n); a=f"{n} %0"
    cf1={"CF":1}
    return _dd([
        TV("CF pres 1→1",0,efl=cf1,asm_t=a),
        TV("CF pres 0→0",0,efl=EF0,asm_t=a),
        TV("CF pres w/other flags",0,efl=EF1,asm_t=a),
        TV("ZF=1",M if inc else 1,efl=EF0,asm_t=a),
        TV("ZF=0",0 if inc else 2,efl=EF0,asm_t=a),
        TV("OF=1 overflow",smax(b) if inc else smin(b)&M,efl=EF0,asm_t=a),
        TV("OF=0 no ovf",1 if inc else 2,efl=EF0,asm_t=a),
        TV("AF=1 nibble",0x0F if inc else 0x10,efl=EF0,asm_t=a),
        TV("AF=0 no nibble",0x0E if inc else 0x11,efl=EF0,asm_t=a),
        TV("SF=1 neg",smax(b) if inc else (smin(b)&M)-1,efl=EF0,asm_t=a),
        TV("SF=0 pos",0 if inc else (smax(b)+1)&M,efl=EF0,asm_t=a),
        TV("PF=1 even",M if inc else 3,efl=EF0,asm_t=a),
        TV("PF=0 odd",0 if inc else 4,efl=EF0,asm_t=a),
    ],b)


def gen_mul(n):
    """MUL. SF/ZF/AF/PF FORCE_0 explicit (#5)."""
    b=width(n); M=wmax(b); a=f"{n} %0,%1"
    return _dd([
        # CF/OF overflow
        TV("CF=OF=1 max*max",M,M,asm_t=a),
        TV("CF=OF=1 INT_MIN*2",1<<(b-1),2,asm_t=a),
        TV("CF=OF=0 2*2",2,2,asm_t=a),
        TV("CF=OF=0 1*1",1,1,asm_t=a),
        # SF/ZF/AF/PF forced to 0 (#5) — explicit per-flag check
        TV("SF=0 forced",1,1,asm_t=a),
        TV("ZF=0 forced 2*2",2,2,asm_t=a),
        TV("AF=0 forced",M,M,asm_t=a),
        TV("PF=0 forced",1,1,asm_t=a),
        # All-zero input → all flags 0
        TV("0*0→all=0",0,0,asm_t=a),
        TV("1*0→all=0",1,0,asm_t=a),
    ],b)


def gen_shift(n):
    """SHL/SHR/SAR. Data patterns (#3), count masking (#8), OF at c=1 (#3)."""
    b=width(n); M=wmax(b); st=stem(n); imm=st.endswith("i")
    k="l" if "sll" in st else ("a" if "sra" in st else "r")
    efA=EF1; t=[]

    # Data patterns for operand boundaries (#3)
    sig_only = 1 << (b-1)
    patterns = [
        ("all-0", 0),
        ("all-1", M),
        ("sign-only", sig_only),
        ("0x55", 0x55 if b==8 else 0x5555 if b==16 else 0x55555555),
        ("0xAA", 0xAA if b==8 else 0xAAAA if b==16 else 0xAAAAAAAA),
    ]

    if imm:
        max_imm = (1 << (3 if b==8 else 4 if b==16 else 5 if b==32 else 6)) - 1
        ivs = sorted(set([0,1,min(b//2,max_imm),min(b-1,max_imm)]))
        for iv in ivs:
            at=f"{n} %0,{iv}"
            mw=Mw(n,rj=True,rk=True,rd=False,imm_val=iv)
            if iv==0:
                t.append(TV("c=0 all preserved",0x55,asm_t=at,efl=efA,imm_val=iv,mw=mw))
            elif iv==1:
                if k=="l":
                    t.append(TV("c=1 CF=1 OF=1 all-1",M,asm_t=at,imm_val=iv,mw=mw))
                    t.append(TV("c=1 CF=0 OF=0 sign",sig_only>>1,asm_t=at,imm_val=iv,mw=mw))
                elif k=="r":
                    t.append(TV("c=1 CF=1 OF=MSB 0xAA",0xAA if b==8 else 0xAAAA,asm_t=at,imm_val=iv,mw=mw))
                    t.append(TV("c=1 CF=0 OF=0 0x55",0x55 if b==8 else 0x5555,asm_t=at,imm_val=iv,mw=mw))
                elif k=="a":
                    t.append(TV("c=1 sra sign-ext",M,asm_t=at,imm_val=iv,mw=mw))
                    t.append(TV("c=1 sra no-ext",1,asm_t=at,imm_val=iv,mw=mw))
                # PF at c=1
                t.append(TV("c=1 PF odd 3",3,asm_t=at,imm_val=iv,mw=mw))
                t.append(TV("c=1 PF even 0",0,asm_t=at,imm_val=iv,mw=mw))
                # AF preserved (#3)
                t.append(TV("c=1 AF pres",0,asm_t=at,efl={"AF":1},imm_val=iv,mw=mw))
            else:
                for pn, pv in patterns[:2]:  # all-0 and all-1 for other counts
                    t.append(TV(f"c={iv} {pn}",pv,asm_t=at,imm_val=iv,mw=mw))
    else:
        # Register variant
        t.append(TV("c=0 all preserved",0x55,rk=0,efl=efA,asm_t=f"{n} %0,%1"))
        # Count masking (#8): c=width→effective 0
        t.append(TV(f"c={b}→eff0 preserved",0x55,rk=b,efl=efA,asm_t=f"{n} %0,%1"))
        if k=="l":
            t.append(TV("c=1 CF=1 OF=1 all-1",M,rk=1,asm_t=f"{n} %0,%1"))
            t.append(TV("c=1 CF=0 OF=0 sign",sig_only>>1,rk=1,asm_t=f"{n} %0,%1"))
        elif k=="r":
            t.append(TV("c=1 CF=bit0 OF=MSB",1,rk=1,asm_t=f"{n} %0,%1"))
            t.append(TV("c=1 CF=0 OF=0",sig_only,rk=1,asm_t=f"{n} %0,%1"))
        elif k=="a":
            t.append(TV("c=1 sra sign-ext",M,rk=1,asm_t=f"{n} %0,%1"))
            t.append(TV("c=1 sra no-ext OF=0",1,rk=1,asm_t=f"{n} %0,%1"))
        # PF/AF at c=1
        t.append(TV("c=1 PF odd",3,rk=1,asm_t=f"{n} %0,%1"))
        t.append(TV("c=1 PF even",0,rk=1,asm_t=f"{n} %0,%1"))
        t.append(TV("c=1 AF pres",0,rk=1,efl={"AF":1},asm_t=f"{n} %0,%1"))
        # Data patterns at typical counts (#3)
        for pn, pv in patterns:
            t.append(TV(f"c=2 {pn}",pv,rk=2,asm_t=f"{n} %0,%1"))
        # Count masking (#8): c=width+1→effective 1
        t.append(TV(f"c={b+1}→eff1",1,rk=b+1,asm_t=f"{n} %0,%1"))
    return _dd(t,b)


def gen_rotate(n):
    """ROL/ROR. OF=0/1 explicit at c=1 (#4), preserve labels clear (#4)."""
    b=width(n); M=wmax(b); st=stem(n); left="rotl" in st; imm=st.endswith("i")
    efA=EF1; t=[]
    if imm:
        max_imm = (1 << (3 if b==8 else 4 if b==16 else 5 if b==32 else 6)) - 1
        ivs = sorted(set([0,1,min(b//2,max_imm),min(b-1,max_imm)]))
        for iv in ivs:
            at=f"{n} %0,{iv}"
            mw=Mw(n,rj=True,rk=True,rd=False,imm_val=iv)
            if iv==0:
                t.append(TV("c=0 CF=MSB/LSB",M if left else 1,asm_t=at,imm_val=iv,mw=mw))
                t.append(TV("c=0 CF=0",0,asm_t=at,imm_val=iv,mw=mw))
                t.append(TV("c=0 flags pres(PF/ZF/SF/AF)",0x12,asm_t=at,efl=efA,imm_val=iv,mw=mw))
            elif iv==1:
                # OF=1 at c=1 (#4): left→MSB changes, right→top two bits differ
                t.append(TV("c=1 OF=1",0x80 if b==8 else 0x8000 if b==16 else 0x80000000,asm_t=at,imm_val=iv,mw=mw))
                t.append(TV("c=1 OF=0",0,asm_t=at,imm_val=iv,mw=mw))
                t.append(TV("c=1 flags pres(PF/ZF/SF/AF)",0x12,asm_t=at,efl=efA,imm_val=iv,mw=mw))
                t.append(TV("c=1 AF pres",0x12,asm_t=at,efl={"AF":1},imm_val=iv,mw=mw))
            else:
                t.append(TV(f"c={iv} flags pres",0x12,asm_t=at,efl=efA,imm_val=iv,mw=mw))
                t.append(TV(f"c={iv} CF check",0x34 if iv<0x100 else 0x3456,asm_t=at,imm_val=iv,mw=mw))
    else:
        t.append(TV("c=0 CF=MSB/LSB",M if left else 1,rk=0,asm_t=f"{n} %0,%1"))
        t.append(TV("c=0 CF=0",0,rk=0,asm_t=f"{n} %0,%1"))
        t.append(TV("c=0 flags pres(PF/ZF/SF/AF)",0x12,rk=0,efl=efA,asm_t=f"{n} %0,%1"))
        # c=1 OF (#4)
        t.append(TV("c=1 OF=1",0x80 if b==8 else 0x8000 if b==16 else 0x80000000,rk=1,asm_t=f"{n} %0,%1"))
        t.append(TV("c=1 OF=0",0,rk=1,asm_t=f"{n} %0,%1"))
        t.append(TV("c=1 flags pres(PF/ZF/SF/AF)",0x12,rk=1,efl=efA,asm_t=f"{n} %0,%1"))
        t.append(TV("c=1 AF pres",0x12,rk=1,efl={"AF":1},asm_t=f"{n} %0,%1"))
        for cv in [b//2, b]:
            t.append(TV(f"c={cv} CF",0x34 if cv<0x100 else 0x3456,rk=cv,asm_t=f"{n} %0,%1"))
            t.append(TV(f"c={cv} flags pres",0x12,rk=cv,efl=efA,asm_t=f"{n} %0,%1"))
    return _dd(t,b)


def gen_rcr_rcl(n):
    """RCL/RCR. Count masking (#8), CF_in=0/1 at each boundary."""
    b=width(n); M=wmax(b); st=stem(n); imm=st.endswith("i")
    efA=EF1; efC=EF_CF; m1=b+1; t=[]
    if imm:
        max_imm = (1 << (3 if b==8 else 4 if b==16 else 5 if b==32 else 6)) - 1
        ivs = sorted(set([0,1,min(b//2,max_imm),min(b-1,max_imm)]))
        for iv in ivs:
            at=f"{n} %0,{iv}"
            mw=Mw(n,rj=True,rk=True,rd=False,imm_val=iv)
            ne=iv%(b+1)
            if ne==0:
                t.append(TV("n=0 all pres",0x55,asm_t=at,efl=efA,imm_val=iv,mw=mw))
            else:
                t.append(TV(f"n={ne} CF_in=0",0x55,asm_t=at,imm_val=iv,mw=mw))
                t.append(TV(f"n={ne} CF_in=1",0x55,efl=efC,asm_t=at,imm_val=iv,mw=mw))
                t.append(TV(f"n={ne} flags pres",0x55,efl=efA,asm_t=at,imm_val=iv,mw=mw))
                if ne==1:
                    t.append(TV("n=1 OF=1",M,asm_t=at,imm_val=iv,mw=mw))
                    t.append(TV("n=1 OF=0",0,asm_t=at,imm_val=iv,mw=mw))
    else:
        t.append(TV("n=0 c=0 all pres",0x55,rk=0,efl=efA,asm_t=f"{n} %0,%1"))
        for cv in [1,b//2,b-1,b,b+1,b+2]:
            ne=cv%(b+1)
            if ne==0:
                t.append(TV(f"c={cv}→n=0 all pres",0x55,rk=cv,efl=efA,asm_t=f"{n} %0,%1"))
            else:
                t.append(TV(f"c={cv}n={ne} CF_in=0",0x55,rk=cv,asm_t=f"{n} %0,%1"))
                t.append(TV(f"c={cv}n={ne} CF_in=1",0x55,rk=cv,efl=efC,asm_t=f"{n} %0,%1"))
                t.append(TV(f"c={cv}n={ne} flags pres",0x55,rk=cv,efl=efA,asm_t=f"{n} %0,%1"))
                if ne==1:
                    t.append(TV(f"c={cv}n=1 OF=1",M,rk=cv,asm_t=f"{n} %0,%1"))
                    t.append(TV(f"c={cv}n=1 OF=0",0,rk=cv,asm_t=f"{n} %0,%1"))
    return _dd(t,b)


def gen_arm_arith(n):
    """ARM add/sub/adc/sbc."""
    M=wmax(32); sub="sub" in stem(n) or "sbc" in stem(n)
    a=f"{n} %0,%1,14"
    t=[TV("N0Z1C0V0",0,0,armfl=AF0,is_arm=True,asm_t=a),
       TV("N1Z0C1V0",M,1,armfl=AF,is_arm=True,asm_t=a),
       TV("N0Z0C0V0",1,2,armfl=AF,is_arm=True,asm_t=a),
       TV("V1 overflow",0x7FFFFFFF,1,armfl=AF,is_arm=True,asm_t=a),
       TV("V0 no ovf",1,2,armfl=AF,is_arm=True,asm_t=a),
       TV("N0Z0 mid",0x55555555,0x11111111,armfl=AF,is_arm=True,asm_t=a),
       TV("Z1 equal",42,42 if sub else 0,42 if sub else 0,armfl=AF,is_arm=True,asm_t=a),
       TV("C0 no carry",1,2,armfl=AF,is_arm=True,asm_t=a),
       # cond=false: EQ(Z==1) with Z=0 → writes suppressed, flags preserved
       TV("cond=0(EQ) Z=0 N0C1V1 flags pres",0x12345678,0x87654321,
          armfl={"N":0,"Z":0,"C":1,"V":1},is_arm=True,asm_t=f"{n} %0,%1,0",
          mw=Mw(n,rj=True,rk=True,rd=False,cond_val=0))]
    if sub: t.append(TV("C1 borrow",0,1,armfl=AF,is_arm=True,asm_t=a))
    return _dd(t,32)

def gen_arm_logic(n):
    M=wmax(32); a=f"{n} %0,%1,14"
    return _dd([
        TV("N1Z0",M,0,armfl=AF,is_arm=True,asm_t=a),
        TV("Z1N0",0,0,armfl=AF,is_arm=True,asm_t=a),
        TV("N0Z0",1,2,armfl=AF,is_arm=True,asm_t=a),
        TV("C1V1→pres",0,M,armfl=afC,is_arm=True,asm_t=a),
        TV("C0V0→pres",0,0,armfl=AF0,is_arm=True,asm_t=a),
        # cond=false: EQ(Z==1) with Z=0 → writes suppressed, C/V preserved
        TV("cond=0(EQ) C1V1 pres",0x12345678,0x87654321,
           armfl={"N":0,"Z":0,"C":1,"V":1},is_arm=True,asm_t=f"{n} %0,%1,0",
           mw=Mw(n,rj=True,rk=True,rd=False,cond_val=0)),
    ],32)

def gen_arm_unary(n):
    M=wmax(32); a=f"{n} %0,14"
    return _dd([
        TV("N1Z0",M,0,armfl=AF,is_arm=True,asm_t=a),
        TV("Z1N0",0,0,armfl=AF,is_arm=True,asm_t=a),
        TV("N0Z0",1,0,armfl=AF,is_arm=True,asm_t=a),
        TV("C,V pres set",0,0,armfl=afC,is_arm=True,asm_t=a),
        TV("C,V pres clr",0,0,armfl=AF0,is_arm=True,asm_t=a),
        # cond=false: EQ(Z==1) with Z=0 → writes suppressed, C/V preserved
        TV("cond=0(EQ) C1V1 pres",0x55555555,0,
           armfl={"N":0,"Z":0,"C":1,"V":1},is_arm=True,asm_t=f"{n} %0,0",
           mw=Mw(n,rj=True,rk=False,rd=False,cond_val=0)),
    ],32)

def gen_arm_shift(n):
    b=32; AFv=afC; imm="i" in stem(n)
    a=f"{n} %0,%1,14"
    t=[]
    if imm:
        for iv in [0,1,b//2,b-1]:
            at=f"{n} %0,{iv},14"
            mw=Mw(n,rj=True,rk=False,rd=False,imm_val=iv,cond_val=14)
            if iv==0:
                t.append(TV("c=0 C=1 pres",0x55,iv,armfl=AFv,is_arm=True,asm_t=at,mw=mw))
                t.append(TV("c=0 C=0 pres",0x55,iv,armfl=AF0,is_arm=True,asm_t=at,mw=mw))
            else:t.append(TV(f"c={iv}",0x55,iv,armfl=AF,is_arm=True,asm_t=at,mw=mw))
        # cond=false: imm variant
        t.append(TV("cond=0(EQ) c=0 C0 pres",0x55,0,
           armfl={"N":0,"Z":0,"C":0,"V":0},is_arm=True,asm_t=f"{n} %0,0,0",
           mw=Mw(n,rj=True,rk=False,rd=False,imm_val=0,cond_val=0)))
    else:
        for cv in [0,1,b//2,b-1,b,b+1]:
            if cv==0:
                t.append(TV("c=0 C=1 pres",0x55,0,armfl=AFv,is_arm=True,asm_t=a))
                t.append(TV("c=0 C=0 pres",0x55,0,armfl=AF0,is_arm=True,asm_t=a))
            else:t.append(TV(f"c={cv}",0x55,cv,armfl=AF,is_arm=True,asm_t=a))
        # cond=false: reg variant
        t.append(TV("cond=0(EQ) c=0 C0 pres",0x55,0,
           armfl={"N":0,"Z":0,"C":0,"V":0},is_arm=True,asm_t=f"{n} %0,%1,0",
           mw=Mw(n,rj=True,rk=True,rd=False,cond_val=0)))
    mw=Mw(n,rj=True,rk=False,rd=False,imm_val=1,cond_val=14) if imm else None
    t.append(TV("V pres set",0x55,1,armfl=AFv,is_arm=True,
               asm_t=f"{n} %0,{'1' if imm else '%1'},14",mw=mw))
    t.append(TV("V pres clr",0x55,1,armfl=AF0,is_arm=True,
               asm_t=f"{n} %0,{'1' if imm else '%1'},14",mw=mw))
    return _dd(t,b)

def gen_armmove(n):
    return _dd([TV("move 0x42",0x42,0,armfl=AF,is_arm=True,asm_t=f"{n} %0,%1,14"),
                TV("move 0",0,0,armfl=AF0,is_arm=True,asm_t=f"{n} %0,%1,14"),
                TV("move all-1",0xFFFFFFFFFFFFFFFF,0,armfl=afC,is_arm=True,asm_t=f"{n} %0,%1,14")],32)

def state_mw_for(name, tv):
    st = stem(name)
    if tv and tv.mw:
        return tv.mw
    if st in ("movgr2scr","movscr2gr"):
        return Mw(name, rj=(st == "movgr2scr"), rk=False, rd=(st == "movscr2gr"))
    if st in ("x86clrtm","x86settm","x86inctop","x86dectop"):
        return Mw(name, rj=False, rk=False, rd=False)
    if st == "x86mttop":
        return Mw(name, rj=False, rk=False, rd=False)
    if st in ("x86mftop","x86mfflag","armmfflag","setarmj","setx86j","setx86loope","setx86loopne"):
        return Mw(name, rj=(st in ("setx86loope","setx86loopne")), rk=False, rd=True)
    if st in ("x86mtflag","armmtflag"):
        return Mw(name, rj=True, rk=False, rd=False)
    if st == "addu12i":
        return Mw(name, rj=True, rk=False, rd=True)
    if st in ("adc","sbc","rotr","rcr"):
        return Mw(name, rj=True, rk=True, rd=True)
    if st in ("rotri","rcri"):
        return Mw(name, rj=True, rk=False, rd=True, imm_val=0)
    if st.startswith("armmov."):
        return Mw(name, rj=True, rk=False, rd=False, cond_val=14)
    return mw_for(name)

def gen_state_base_result(n):
    b = width(n); M = wmax(b); st = stem(n)
    vals = [
        ("basic", 0x12, 0x34, 0),
        ("cf-in", M, 0, 1),
        ("sign-ext", 1 << (b - 1), 0, 0),
    ]
    out = []
    for label, rjv, rkv, cf in vals:
        ef = {"CF": cf} if cf else EF0
        if st == "adc":
            out.append(StateTV(label, f"{n} %0,%0,%1", rj=rjv, rk=rkv, efl=ef, checks=("gpr12","eflags")))
        elif st == "sbc":
            out.append(StateTV(label, f"{n} %0,%0,%1", rj=rjv, rk=rkv, efl=ef, checks=("gpr12","eflags")))
    return out

def gen_state_addu12i(n):
    out = []
    for imm, asm_imm, rjv in [(0, 0, 0x1234), (1, 1, 0), (15, 15, 0), (16, -16, 0), (31, -1, 0x1000)]:
        mw = Mw(n, rj=True, rk=False, rd=True, imm_val=imm)
        out.append(StateTV(f"imm={imm}", f"{n} %0,%0,{asm_imm}", rj=rjv, mw=mw, checks=("gpr12","eflags")))
    return out

def gen_state_rotr(n):
    b = width(n); st = stem(n); imm = st.endswith("i")
    counts = [0, 1, b // 2, b - 1]
    out = []
    for c in counts:
        if imm:
            mw = Mw(n, rj=True, rk=False, rd=True, imm_val=c)
            out.append(StateTV(f"c={c}", f"{n} %0,%0,{c}", rj=0x8123456789ABCDEF, mw=mw, checks=("gpr12","eflags")))
        else:
            out.append(StateTV(f"c={c}", f"{n} %0,%0,%1", rj=0x8123456789ABCDEF, rk=c, checks=("gpr12","eflags")))
    return out

def gen_state_rcr(n):
    b = width(n); st = stem(n); imm = st.endswith("i")
    counts = [0, 1, b // 2, b - 1] if imm else [0, 1, b, b + 1]
    out = []
    for c in counts:
        for cf in [0, 1]:
            ef = {"CF": cf} if cf else EF0
            if imm:
                mw = Mw(n, rj=True, rk=False, rd=True, imm_val=c)
                out.append(StateTV(f"c={c} CF={cf}", f"{n} %0,%0,{c}", rj=0x8123456789ABCDEF, efl=ef, mw=mw, checks=("gpr12","eflags")))
            else:
                out.append(StateTV(f"c={c} CF={cf}", f"{n} %0,%0,%1", rj=0x8123456789ABCDEF, rk=c, efl=ef, checks=("gpr12","eflags")))
    return out

def gen_state_scratch(n):
    st = stem(n)
    if st == "movgr2scr":
        return [StateTV("scr0", "movgr2scr $scr0,%0\\n\\tmovscr2gr %2,$scr0",
                        rj=0x123456789abcdef0, checks=("scratch0",), mw=Mw(n, rj=True, rk=False, crd_val=0))]
    if st == "movscr2gr":
        return [StateTV("scr0", "movgr2scr $scr0,%2\\n\\tmovscr2gr %0,$scr0",
                        scratch0=0xfedcba9876543210, checks=("gpr12",), mw=Mw(n, rj=False, rk=False, rd=True, crj_val=0))]
    return []

def gen_state_flags_move(n):
    st = stem(n)
    if st == "x86mfflag":
        out = [StateTV("all", "x86mfflag %0,0x3f", efl=EF1, checks=("gpr12","eflags"), mw=Mw(n, rj=False, rd=True, mask_val=0x3f)),
               StateTV("of-only", "x86mfflag %0,0x20", efl=EF1, checks=("gpr12","eflags"), mw=Mw(n, rj=False, rd=True, mask_val=0x20))]
        # Individual flag bit masks
        for mask, flag, bitpos in [(0x01,"CF",0),(0x02,"PF",2),(0x04,"AF",4),(0x08,"ZF",6),(0x10,"SF",7)]:
            ef = {flag:1}
            mw = Mw(n, rj=False, rd=True, mask_val=mask)
            out.append(StateTV(f"mask=0x{mask:02x}({flag})", f"x86mfflag %0,0x{mask:02x}",
                               efl=ef, mw=mw, checks=("gpr12","eflags")))
        return out
    if st == "x86mtflag":
        out = [StateTV("all", "x86mtflag %0,0x3f", rj=FLAG_MASK_ALL, checks=("eflags",), mw=Mw(n, rj=True, rk=False, mask_val=0x3f)),
               StateTV("of-only", "x86mtflag %0,0x20", rj=FLAG_MASK_ALL, checks=("eflags",), mw=Mw(n, rj=True, rk=False, mask_val=0x20))]
        for mask, flag, bitpos in [(0x01,"CF",0),(0x02,"PF",2),(0x04,"AF",4),(0x08,"ZF",6),(0x10,"SF",7)]:
            mw = Mw(n, rj=True, rk=False, mask_val=mask)
            out.append(StateTV(f"mask=0x{mask:02x}({flag})", f"x86mtflag %0,0x{mask:02x}",
                               rj=1 << bitpos, mw=mw, checks=("eflags",)))
        return out
    if st == "armmfflag":
        out = [StateTV("all", "armmfflag %0,0x39", armfl=afC, checks=("gpr12","armflags"), mw=Mw(n, rj=False, rd=True, mask_val=0x39)),
               StateTV("v-only", "armmfflag %0,0x20", armfl=afC, checks=("gpr12","armflags"), mw=Mw(n, rj=False, rd=True, mask_val=0x20))]
        # Individual ARM flag masks: mask bit 0=C, bit 3=Z, bit 4=N, bit 5=V
        for mask, af in [(0x01,{"N":0,"Z":0,"C":1,"V":0}),(0x08,{"N":0,"Z":1,"C":0,"V":0}),(0x10,{"N":1,"Z":0,"C":0,"V":0})]:
            mw = Mw(n, rj=False, rd=True, mask_val=mask)
            flagname = "C" if mask==0x01 else "Z" if mask==0x08 else "N"
            out.append(StateTV(f"mask=0x{mask:02x}({flagname})", f"armmfflag %0,0x{mask:02x}",
                               armfl=af, mw=mw, checks=("gpr12","armflags")))
        return out
    if st == "armmtflag":
        out = [StateTV("all", "armmtflag %0,0x39", rj=0xF0000000, checks=("armflags",), mw=Mw(n, rj=True, rk=False, mask_val=0x39)),
               StateTV("v-only", "armmtflag %0,0x20", rj=0xF0000000, checks=("armflags",), mw=Mw(n, rj=True, rk=False, mask_val=0x20))]
        for mask, gpr_bit, flagname in [(0x01,29,"C"),(0x08,30,"Z"),(0x10,31,"N")]:
            mw = Mw(n, rj=True, rk=False, mask_val=mask)
            out.append(StateTV(f"mask=0x{mask:02x}({flagname})", f"armmtflag %0,0x{mask:02x}",
                               rj=1 << gpr_bit, mw=mw, checks=("armflags",)))
        return out
    return []

def gen_state_top_tm(n):
    st = stem(n)
    if st == "x86mftop":
        return [StateTV("top=0", "x86mftop %0", top=0, checks=("gpr12","top")),
                StateTV("top=7", "x86mftop %0", top=7, checks=("gpr12","top"))]
    if st == "x86mttop":
        out = []
        for top in [0, 1, 7]:
            mw = Mw(n, rj=False, rk=False, rd=False, top_val=top)
            out.append(StateTV(f"top={top}", f"x86mttop {top}", top=3, mw=mw, checks=("top",)))
        return out
    if st == "x86inctop":
        return [StateTV("0->1", "x86inctop", top=0, checks=("top",)),
                StateTV("7->0", "x86inctop", top=7, checks=("top",))]
    if st == "x86dectop":
        return [StateTV("1->0", "x86dectop", top=1, checks=("top",)),
                StateTV("0->7", "x86dectop", top=0, checks=("top",))]
    if st == "x86settm":
        return [StateTV("set", "x86settm", tm=False, checks=("tm",))]
    if st == "x86clrtm":
        return [StateTV("clear", "x86clrtm", tm=True, checks=("tm",))]
    return []

def gen_state_setcond(n):
    st = stem(n)
    if st == "setx86j":
        # All 16 x86 condition codes: cond → (true_flags, false_flags)
        # True: condition holds.  False: condition does not hold.
        conds = [
            (0,  {"OF":1}, {"OF":0}),   # O  (overflow)
            (1,  {"OF":0}, {"OF":1}),   # NO (no overflow)
            (2,  {"CF":1}, {"CF":0}),   # B  (below)
            (3,  {"CF":0}, {"CF":1}),   # AE (above/equal)
            (4,  {"ZF":1}, {"ZF":0}),   # E  (equal)
            (5,  {"ZF":0}, {"ZF":1}),   # NE (not equal)
            (6,  {"CF":1},      {"CF":0,"ZF":0}),  # BE (below/equal)
            (7,  {"CF":0,"ZF":0},{"CF":1}),  # A  (above)
            (8,  {"SF":1}, {"SF":0}),   # S  (sign)
            (9,  {"SF":0}, {"SF":1}),   # NS (not sign)
            (10, {"PF":1}, {"PF":0}),   # P  (parity even)
            (11, {"PF":0}, {"PF":1}),   # NP (parity odd)
            (12, {"SF":1,"OF":0},{"SF":1,"OF":1}),  # L  (less, SF≠OF)
            (13, {"SF":1,"OF":1},{"SF":1,"OF":0}),  # GE (greater/equal, SF=OF)
            (14, {"ZF":1},      {"ZF":0,"SF":0,"OF":0}),  # LE (ZF=1 or SF≠OF)
            (15, {"ZF":0,"SF":0,"OF":0},{"ZF":1}),  # G  (ZF=0 and SF=OF)
        ]
        out = []
        for cond, true_ef, false_ef in conds:
            mw = Mw(n, rj=False, rk=False, rd=True, cond_val=cond)
            out.append(StateTV(f"cond={cond} true", f"setx86j %0,{cond}",
                               efl=true_ef, mw=mw, checks=("gpr12","eflags")))
            # false case: verify GPR[rd]=0
            out.append(StateTV(f"cond={cond} false", f"setx86j %0,{cond}",
                               efl=false_ef, mw=mw, checks=("gpr12","eflags")))
        return out
    if st in ("setx86loope","setx86loopne"):
        out = []
        for rjv, zf in [(0,0), (1,0), (0,1), (1,1)]:
            ef = {"ZF": zf} if zf else EF0
            out.append(StateTV(f"rj={rjv} ZF={zf}", f"{n} %0,%0", rj=rjv, efl=ef, checks=("gpr12","eflags")))
        return out
    if st == "setarmj":
        # All 16 ARM condition codes: cond → (true_flags, false_flags)
        conds = [
            (0,  {"N":0,"Z":1,"C":0,"V":0}, {"N":0,"Z":0,"C":0,"V":0}),  # EQ  Z=1
            (1,  {"N":0,"Z":0,"C":0,"V":0}, {"N":0,"Z":1,"C":0,"V":0}),  # NE  Z=0
            (2,  {"N":0,"Z":0,"C":1,"V":0}, {"N":0,"Z":0,"C":0,"V":0}),  # CS  C=1
            (3,  {"N":0,"Z":0,"C":0,"V":0}, {"N":0,"Z":0,"C":1,"V":0}),  # CC  C=0
            (4,  {"N":1,"Z":0,"C":0,"V":0}, {"N":0,"Z":0,"C":0,"V":0}),  # MI  N=1
            (5,  {"N":0,"Z":0,"C":0,"V":0}, {"N":1,"Z":0,"C":0,"V":0}),  # PL  N=0
            (6,  {"N":0,"Z":0,"C":0,"V":1}, {"N":0,"Z":0,"C":0,"V":0}),  # VS  V=1
            (7,  {"N":0,"Z":0,"C":0,"V":0}, {"N":0,"Z":0,"C":0,"V":1}),  # VC  V=0
            (8,  {"N":0,"Z":0,"C":1,"V":0}, {"N":0,"Z":0,"C":0,"V":0}),  # HI  C=1 & Z=0
            (9,  {"N":0,"Z":1,"C":0,"V":0}, {"N":0,"Z":0,"C":0,"V":0}),  # LS  C=0 or Z=1
            (10, {"N":1,"Z":0,"C":0,"V":1}, {"N":1,"Z":0,"C":0,"V":0}),  # GE  N==V
            (11, {"N":1,"Z":0,"C":0,"V":0}, {"N":1,"Z":0,"C":0,"V":1}),  # LT  N!=V
            (12, {"N":1,"Z":0,"C":0,"V":1}, {"N":0,"Z":1,"C":0,"V":0}),  # GT  Z=0 & N==V
            (13, {"N":1,"Z":0,"C":0,"V":0}, {"N":1,"Z":1,"C":0,"V":0}),  # LE  Z=1 or N!=V
            (14, {"N":0,"Z":0,"C":0,"V":0}, {"N":0,"Z":0,"C":0,"V":0}),  # AL  always (no false)
            (15, {"N":0,"Z":0,"C":0,"V":0}, {"N":0,"Z":0,"C":0,"V":0}),  # AL  always (no false)
        ]
        out = []
        for cond, true_af, false_af in conds:
            mw = Mw(n, rj=False, rk=False, rd=True, cond_val=cond)
            out.append(StateTV(f"cond={cond} true", f"setarmj %0,{cond}",
                               armfl=true_af, mw=mw, checks=("gpr12","armflags")))
            if cond < 14:  # AL conditions have no false path
                out.append(StateTV(f"cond={cond} false", f"setarmj %0,{cond}",
                                   armfl=false_af, mw=mw, checks=("gpr12","armflags")))
        return out
    return []

def gen_state_armmove(n):
    """armmove: conditional GPR move.  Uses rj=12, rd=13 to test write
    suppression when cond is false.  _a0 holds the initial rd value (from rk),
    _a1 holds the rj (source) value.  After cond-true asm, _a0 == rj_source;
    after cond-false asm, _a0 == initial rd."""
    out = []
    # cond=true: move value from rj to rd
    mw_true = Mw(n, rj=True, rk=False, rd=True, rd_val=13, cond_val=14)
    out.append(StateTV("cond=14 move 0x42", "armmove %0,%1,14",
                       rj=0x42, rk=0xDEAD, mw=mw_true, checks=("gpr13",)))
    out.append(StateTV("cond=14 move 0", "armmove %0,%1,14",
                       rj=0, rk=0xDEAD, mw=mw_true, checks=("gpr13",)))
    out.append(StateTV("cond=14 move all-1", "armmove %0,%1,14",
                       rj=0xFFFFFFFFFFFFFFFF, rk=0xDEAD, mw=mw_true, checks=("gpr13",)))
    # cond=false: rd should keep its initial value
    mw_false = Mw(n, rj=True, rk=False, rd=True, rd_val=13, cond_val=0)
    out.append(StateTV("cond=0(EQ) Z=0 rd unchanged",
                       "armmove %0,%1,0", rj=0x42, rk=0xDEAD,
                       armfl={"N":0,"Z":0,"C":0,"V":0},
                       mw=mw_false, checks=("gpr13","armflags")))
    return out

def gen_state_armmov(n):
    if n.startswith("armmov."):
        return [StateTV("zero", f"{n} %0,14", rj=0, armfl=AF0, checks=("armflags",)),
                StateTV("sign", f"{n} %0,14", rj=0x8000000000000000, armfl=afC, checks=("armflags",))]
    return []

def gen_state_fcvt(n):
    """FCVT: convert between double and x87 extended precision via FPRs."""
    out = []
    # Common double test values
    ld_d_tests = [
        (0x0000000000000000, "zero"),
        (0x3FF0000000000000, "one"),
        (0xBFF0000000000000, "neg_one"),
        (0x7FF0000000000000, "inf"),
        (0x7FF8000000000000, "nan"),
        (0x7FF0000000000001, "snan"),
        (0x0000000000000001, "min_denorm"),
        (0x7FEFFFFFFFFFFFFF, "max_normal"),
    ]
    if n == "fcvt.d.ld":
        asm = "movgr2fr.d $f12,%0\\n\\tmovgr2fr.d $f13,%1\\n\\tfcvt.d.ld $f14,$f12,$f13\\n\\tmovfr2gr.d %0,$f14"
        cases = [
            (0x0000000000000000, 0x0000, "zero"),
            (0x8000000000000000, 0x3FFF, "one"),
            (0x8000000000000000, 0xBFFF, "neg_one"),
            (0x8000000000000000, 0x7FFF, "inf"),
            (0xC000000000000000, 0x7FFF, "nan"),
            (0x8000000000000000, 0x0001, "underflow_to_zero"),
        ]
        for lo, hi, lbl in cases:
            mw = Mw(n, rj=True, rk=True, rd=True, rd_val=14)
            out.append(StateTV(lbl, asm, rj=lo, rk=hi, mw=mw, checks=("fpr14",)))
    elif n == "fcvt.ld.d":
        asm = "movgr2fr.d $f12,%0\\n\\tfcvt.ld.d $f14,$f12\\n\\tmovfr2gr.d %0,$f14"
        mw = Mw(n, rj=True, rk=False, rd=True, rd_val=14)
        for val, lbl in ld_d_tests:
            out.append(StateTV(lbl, asm, rj=val, mw=mw, checks=("fpr14",)))
    elif n == "fcvt.ud.d":
        asm = "movgr2fr.d $f12,%0\\n\\tfcvt.ud.d $f14,$f12\\n\\tmovfr2gr.d %0,$f14"
        mw = Mw(n, rj=True, rk=False, rd=True, rd_val=14)
        for val, lbl in ld_d_tests:
            out.append(StateTV(lbl, asm, rj=val, mw=mw, checks=("fpr14",)))
    return out

def state_generator(name):
    st = stem(name)
    if st in ("adc","sbc"):
        return gen_state_base_result
    if st == "addu12i":
        return gen_state_addu12i
    if st in ("rotr","rotri"):
        return gen_state_rotr
    if st in ("rcr","rcri"):
        return gen_state_rcr
    if st in ("movgr2scr","movscr2gr"):
        return gen_state_scratch
    if st in ("x86mfflag","x86mtflag","armmfflag","armmtflag"):
        return gen_state_flags_move
    if st in ("x86mftop","x86mttop","x86inctop","x86dectop","x86settm","x86clrtm"):
        return gen_state_top_tm
    if st in ("setx86j","setx86loope","setx86loopne","setarmj"):
        return gen_state_setcond
    if st == "armmove":
        return gen_state_armmove
    if name.startswith("armmov."):
        return gen_state_armmov
    if st == "fcvt":
        return gen_state_fcvt
    return None

FM={
    "x86add":gen_addsub,"x86sub":gen_addsub,"x86adc":gen_adcsbc,"x86sbc":gen_adcsbc,
    "x86and":gen_logic,"x86or":gen_logic,"x86xor":gen_logic,
    "x86inc":gen_incdec,"x86dec":gen_incdec,"x86mul":gen_mul,
    "x86sll":gen_shift,"x86srl":gen_shift,"x86sra":gen_shift,
    "x86slli":gen_shift,"x86srli":gen_shift,"x86srai":gen_shift,
    "x86rotl":gen_rotate,"x86rotr":gen_rotate,
    "x86rotli":gen_rotate,"x86rotri":gen_rotate,
    "x86rcl":gen_rcr_rcl,"x86rcr":gen_rcr_rcl,
    "x86rcli":gen_rcr_rcl,"x86rcri":gen_rcr_rcl,
    "armadd":gen_arm_arith,"armsub":gen_arm_arith,
    "armadc":gen_arm_arith,"armsbc":gen_arm_arith,
    "armand":gen_arm_logic,"armor":gen_arm_logic,"armxor":gen_arm_logic,
    "armnot":gen_arm_unary,"armmov":gen_arm_unary,
    "armrrx":gen_arm_unary,"armmove":gen_armmove,
    "armsll":gen_arm_shift,"armsrl":gen_arm_shift,"armsra":gen_arm_shift,"armrotr":gen_arm_shift,
    "armslli":gen_arm_shift,"armsrli":gen_arm_shift,"armsrai":gen_arm_shift,"armrotri":gen_arm_shift,
}
def gf(n):
    st=stem(n)
    if st in FM:return FM[st]
    for p in sorted(FM,reverse=True,key=len):
        if st.startswith(p):return FM[p]
    return None

def fmt(v):
    if v<256:return str(v)
    if v<=0xFFFFFFFF:return f"0x{v:X}ULL"
    return f"0x{v:X}ULL"

X86_OP = {
    "x86add":"add","x86sub":"sub","x86adc":"adc","x86sbc":"sbb",
    "x86and":"and","x86or":"or","x86xor":"xor","x86test":"test",
    "x86inc":"inc","x86dec":"dec","x86mul":"mul",
    "x86div":"div","x86idiv":"idiv",
    "x86sll":"shl","x86slli":"shl",
    "x86srl":"shr","x86srli":"shr",
    "x86sra":"sar","x86srai":"sar",
    "x86rotl":"rol","x86rotli":"rol",
    "x86rotr":"ror","x86rotri":"ror",
    "x86rcl":"rcl","x86rcli":"rcl",
    "x86rcr":"rcr","x86rcri":"rcr",
    "x86bt":"bt","x86btc":"btc","x86btr":"btr","x86bts":"bts",
    "x86bsf":"bsf","x86bsr":"bsr",
    "x86shld":"shld","x86shrd":"shrd",
    "x86inc":"inc","x86dec":"dec",
}

X86_SUFFIX = {"b":"b","h":"w","w":"l","d":"q","bu":"b","hu":"w","wu":"l","du":"q"}
X86_CTYPE = {"b":"uint8_t","h":"uint16_t","w":"uint32_t","d":"uint64_t",
             "bu":"uint8_t","hu":"uint16_t","wu":"uint32_t","du":"uint64_t"}
X86_WIDTH_SUFFIX = {"b":"b","h":"h","w":"w","d":"d","bu":"b","hu":"h","wu":"w","du":"d"}
NATIVE_PROBE_ONLY = {
    "x86test": ("b","h","w","d"),
    "x86bt": ("h","w","d"),
    "x86btc": ("h","w","d"),
    "x86btr": ("h","w","d"),
    "x86bts": ("h","w","d"),
    "x86bsf": ("h","w","d"),
    "x86bsr": ("h","w","d"),
    "x86div": ("b","h","w","d"),
    "x86idiv": ("b","h","w","d"),
    "x86shld": ("h","w","d"),
    "x86shrd": ("h","w","d"),
}

def x86_asm_op(name):
    st = stem(name)
    if st == "x86mul":
        op = "mul" if suf(name) in ("bu","hu","wu","du") else "imul"
        return op + X86_SUFFIX.get(suf(name), "l")
    if st in X86_OP:
        return X86_OP[st] + X86_SUFFIX.get(suf(name), "l")
    return None

def x86_supported(name):
    st = stem(name)
    if not name.startswith("x86"):
        return False
    if st in ("x86add","x86sub") and suf(name) in ("wu","du"):
        return False
    if st in ("x86bt","x86btc","x86btr","x86bts","x86bsf","x86bsr","x86shld","x86shrd") and suf(name) == "b":
        return False
    return st in X86_OP

def x86_asm_text(name, tv):
    op = x86_asm_op(name)
    st = stem(name)
    if op is None:
        return None
    if st == "x86mul":
        return op
    if st in ("x86div","x86idiv"):
        return op
    if st in ("x86inc","x86dec"):
        return f"{op} %[a]"
    if st in ("x86shld","x86shrd"):
        return f"{op} %b[c], %[b], %[a]"
    if st in ("x86slli","x86srli","x86srai","x86rotli","x86rotri","x86rcli","x86rcri"):
        return f"{op} ${tv.mw.imm_val if tv.mw and tv.mw.imm_val is not None else tv.imm_val}, %[a]"
    if st in ("x86sll","x86srl","x86sra","x86rotl","x86rotr","x86rcl","x86rcr"):
        return f"{op} %b[c], %[a]"
    return f"{op} %[b], %[a]"

def x86_defined_mask(name, tv):
    st = stem(name)
    if st in ("x86add","x86sub","x86adc","x86sbc"):
        return FLAG_MASK_ALL
    if st in ("x86and","x86or","x86xor"):
        return flag_mask("CF","PF","ZF","SF","OF")
    if st in ("x86inc","x86dec"):
        return FLAG_MASK_ALL
    if st == "x86mul":
        return flag_mask("CF","OF")
    if st in ("x86sll","x86slli","x86srl","x86srli","x86sra","x86srai"):
        count = masked_count(name, tv)
        if count == 0:
            return FLAG_MASK_ALL
        m = flag_mask("PF","ZF","SF")
        if count <= width(name):
            m |= FLAG_BITS["CF"]
        if count == 1:
            m |= FLAG_BITS["OF"]
        return m
    if st in ("x86rotl","x86rotli","x86rotr","x86rotri"):
        raw = masked_count(name, tv)
        preserved = flag_mask("PF","AF","ZF","SF")
        if raw == 0:
            return FLAG_MASK_ALL
        m = preserved | FLAG_BITS["CF"]
        if raw == 1:
            m |= FLAG_BITS["OF"]
        return m
    if st in ("x86rcl","x86rcli","x86rcr","x86rcri"):
        count = effective_count(name, tv)
        raw = masked_count(name, tv)
        preserved = flag_mask("PF","AF","ZF","SF")
        if count == 0:
            if raw == 0:
                return FLAG_MASK_ALL
            return preserved | FLAG_BITS["CF"]
        m = preserved | FLAG_BITS["CF"]
        if raw == 1:
            m |= FLAG_BITS["OF"]
        return m
    return 0

def raw_count(name, tv):
    st = stem(name)
    if st in ("x86slli","x86srli","x86srai","x86rotli","x86rotri","x86rcli","x86rcri"):
        if tv.mw and tv.mw.imm_val is not None:
            return tv.mw.imm_val
        return tv.imm_val or 0
    return tv.rk

def masked_count(name, tv):
    c = raw_count(name, tv)
    return c & (0x3f if width(name) == 64 else 0x1f)

def effective_count(name, tv):
    c = masked_count(name, tv)
    b = width(name)
    st = stem(name)
    if st in ("x86rotl","x86rotli","x86rotr","x86rotri"):
        return c % b
    if st in ("x86rcl","x86rcli","x86rcr","x86rcri") and b in (8,16):
        return c % (b + 1)
    return c

UNDEF_SCOPE = {
    "x86and": ("AF",),
    "x86or": ("AF",),
    "x86test": ("AF",),
    "x86xor": ("AF",),
    "x86mul": ("PF","AF","ZF","SF"),
    "x86div": ("CF","PF","AF","ZF","SF","OF"),
    "x86idiv": ("CF","PF","AF","ZF","SF","OF"),
    "x86sll": ("AF","OF"),
    "x86slli": ("AF","OF"),
    "x86srl": ("AF","OF"),
    "x86srli": ("AF","OF"),
    "x86sra": ("AF","OF"),
    "x86srai": ("AF","OF"),
    "x86rotl": ("OF",),
    "x86rotli": ("OF",),
    "x86rotr": ("OF",),
    "x86rotri": ("OF",),
    "x86rcl": ("OF",),
    "x86rcli": ("OF",),
    "x86rcr": ("OF",),
    "x86rcri": ("OF",),
    "x86bt": ("PF","AF","ZF","SF","OF"),
    "x86btc": ("PF","AF","ZF","SF","OF"),
    "x86btr": ("PF","AF","ZF","SF","OF"),
    "x86bts": ("PF","AF","ZF","SF","OF"),
    "x86bsf": ("CF","PF","AF","SF","OF"),
    "x86bsr": ("CF","PF","AF","SF","OF"),
    "x86shld": ("CF","PF","AF","ZF","SF","OF"),
    "x86shrd": ("CF","PF","AF","ZF","SF","OF"),
}

def lbt_imm_max(bits):
    return (1 << (3 if bits == 8 else 4 if bits == 16 else 5 if bits == 32 else 6)) - 1

def native_count_classes(bits):
    mask_max = 63 if bits == 64 else 31
    aliases = [mask_max + 1, mask_max + 2, 63, 64, 65, 127, 128, 255]
    return sorted(set(list(range(mask_max + 1)) + aliases))

def native_count_edges(bits):
    mask_max = 63 if bits == 64 else 31
    return sorted(set([0, 1, 2, bits - 1, bits, bits + 1, mask_max, mask_max + 1, mask_max + 2, 63, 64, 65, 127, 255]))

def probe_counts_for(name, native=False):
    b = width(name)
    st = stem(name)
    if st in ("x86slli","x86srli","x86srai","x86rotli","x86rotri","x86rcli","x86rcri") and not native:
        return list(range(lbt_imm_max(b) + 1))
    if st in ("x86shld","x86shrd"):
        return native_count_edges(b)
    return native_count_classes(b)

def bit_offsets_for(bits):
    return sorted(set([0, 1, 2, 3, bits - 1, bits, bits + 1, 15, 16, 31, 32, 63, 64, 65]))

def probe_vectors_for(name, native=False):
    st = stem(name)
    if st not in UNDEF_SCOPE:
        return []
    b = width(name)
    M = wmax(b)
    vals = [0,1,2,3,0x0f,0x10,0x55,0xaa,M,(1 << (b - 1))]
    vals = [v & M for v in vals]
    vals = list(dict.fromkeys(vals))
    init_flags = [EF0, EF1, {"CF":1}, {"PF":1}, {"AF":1}, {"ZF":1}, {"SF":1}, {"OF":1}]
    out = []
    if st in ("x86and","x86or","x86xor","x86mul"):
        for a in vals:
            for bval in vals[:6]:
                for ef in init_flags:
                    out.append(TV(f"probe a={fmt(a)} b={fmt(bval)} init={flag_label(ef)}", a, bval, efl=ef, asm_t=""))
    elif st == "x86test":
        for a in vals:
            for bval in vals[:6]:
                for ef in init_flags:
                    out.append(TV(f"probe a={fmt(a)} b={fmt(bval)} init={flag_label(ef)}", a, bval, efl=ef, asm_t=""))
    elif st in ("x86bt","x86btc","x86btr","x86bts"):
        for a in vals:
            for bit in bit_offsets_for(b):
                for ef in init_flags:
                    out.append(TV(f"probe a={fmt(a)} b={bit} init={flag_label(ef)}", a, bit, efl=ef, asm_t=""))
    elif st in ("x86bsf","x86bsr"):
        srcs = [0, 1, 2, 3, 0x10, 0x80, 0x8000, 0x80000000, 1 << (b - 1), M]
        srcs = list(dict.fromkeys(v & M for v in srcs))
        dests = [0, 0x55, M]
        for src in srcs:
            for dst in dests:
                for ef in init_flags:
                    out.append(TV(f"probe a={fmt(dst)} b={fmt(src)} init={flag_label(ef)}", dst, src, efl=ef, asm_t=""))
    elif st in ("x86div","x86idiv"):
        if st == "x86div":
            pairs = [(0, 1), (1, 1), (3, 2), (7, 3), (0x7f, 3), (M >> 1, 7)]
        else:
            pairs = [(0, 1), (1, 1), (M, 1), (3, 2), ((-7) & M, 3), (0x7f, 5)]
        for a, bval in pairs:
            for ef in init_flags:
                out.append(TV(f"probe a={fmt(a & M)} b={fmt(bval & M)} init={flag_label(ef)}", a, bval, efl=ef, asm_t=""))
    elif st in ("x86shld","x86shrd"):
        srcs = [0, 1, M]
        for a in [0, 1, M, 1 << (b - 1)]:
            for bval in srcs:
                for c in probe_counts_for(name, native=True):
                    for ef in init_flags:
                        out.append(TV(f"probe a={fmt(a)} b={fmt(bval & M)} c={c} init={flag_label(ef)}", a, bval, efl=ef, asm_t="", imm_val=c))
    elif st in ("x86sll","x86srl","x86sra","x86rotl","x86rotr","x86rcl","x86rcr"):
        for a in vals:
            for c in probe_counts_for(name, native=native):
                for ef in init_flags:
                    out.append(TV(f"probe a={fmt(a)} c={c} init={flag_label(ef)}", a, c, efl=ef, asm_t=""))
    elif st in ("x86slli","x86srli","x86srai","x86rotli","x86rotri","x86rcli","x86rcri"):
        counts = probe_counts_for(name, native=native)
        for a in vals:
            for c in counts:
                mw = Mw(name, rj=True, rk=False, rd=False, imm_val=c)
                for ef in init_flags:
                    out.append(TV(f"probe a={fmt(a)} c={c} init={flag_label(ef)}", a, 0, efl=ef, asm_t="", imm_val=c, mw=mw))
    return _dd(out, width(name))

def flag_label(ef):
    ef = ef or {}
    if not ef:
        return "0"
    return "".join(k for k in FLAG_ORDER if ef.get(k,0)) or "0"

def collect_all_tvs(include_arm=True):
    flagged=set()
    for p in sorted(HEADER_DIR.glob("*.h")):
        if p.name=="lbt_model.h":continue
        name=p.stem.replace("_",".")
        if has_flag_meta(name):flagged.add(name)
    all_tvs=[];sk=0
    for name in sorted(flagged):
        if not include_arm and not x86_supported(name):
            continue
        fn=gf(name)
        if fn is None:sk+=1;continue
        tvs=fn(name)
        if not tvs:sk+=1;continue
        for tv in tvs:all_tvs.append((name,tv))
    return all_tvs, sk

def collect_state_tvs():
    out=[];sk=0
    for p in sorted(HEADER_DIR.glob("*.h")):
        if p.name=="lbt_model.h":continue
        name=p.stem.replace("_",".")
        fn=state_generator(name)
        if fn is None:
            continue
        tvs=fn(name)
        if not tvs:
            sk+=1;continue
        for tv in tvs:out.append((name,tv))
    return out, sk

def wrapper_key(name, mw):
    parts=[name]
    if mw.imm_val is not None:parts.append(f"imm{mw.imm_val}")
    if mw.cond_val is not None:parts.append(f"cond{mw.cond_val}")
    if mw.mask_val is not None:parts.append(f"mask{mw.mask_val}")
    if mw.top_val is not None:parts.append(f"top{mw.top_val}")
    if mw.crd_val is not None:parts.append(f"crd{mw.crd_val}")
    if mw.crj_val is not None:parts.append(f"crj{mw.crj_val}")
    return "__".join(parts)

def emit(all_tvs, state_tvs=None):
    state_tvs = state_tvs or []
    L=lambda x:lines.append(x);lines=[]
    L("// Auto-generated by gen_lbt_verify.py")
    L("")
    wrappers={}
    for name,tv in all_tvs:
        dmw=mw_for(name)
        if name not in wrappers:wrappers[name]=tv.mw if tv.mw else dmw
        wtv = tv.mw if tv.mw else dmw
        # Create separate wrapper if imm or cond differs from default
        if wtv.imm_val is not None and wtv.imm_val != dmw.imm_val:
            wk=f"{name}__imm{wtv.imm_val}";wrappers[wk]=wtv;tv._wkey=wk
        elif wtv.cond_val is not None and wtv.cond_val != dmw.cond_val:
            wk=f"{name}__cond{wtv.cond_val}";wrappers[wk]=wtv;tv._wkey=wk
        else:tv._wkey=name
    for name,tv in state_tvs:
        mw=state_mw_for(name,tv)
        wk=wrapper_key(name,mw)
        wrappers[wk]=mw
        tv._wkey=wk
    L("// ── Model wrappers ──")
    for key,mw in sorted(wrappers.items()):
        safe=key.replace(".","_")
        decl=mw.decl()
        L(f"static void gv_{safe}(void){{ {decl}")
        for v in mw.vars():
            L(f"(void){v};")
        L(f'#include "{mw.h_file}"')
        L("}")
    L("")
    def wname(tv,name):
        return f"gv_{getattr(tv,'_wkey',name).replace('.','_')}"
    def hw_prepare_for(name, tv):
        st = stem(name)
        if "fpr14" in tv.checks or "top" in tv.checks or "tm" in tv.checks:
            return "hw_prepare_x87_last"
        if name.startswith("arm") or st == "setarmj":
            return "hw_prepare_arm_last"
        return "hw_prepare_x86_last"
    groups=defaultdict(list)
    for name,tv in all_tvs:groups[name].append(tv)
    for name in sorted(groups):
        tvs=groups[name];is_arm=tvs[0].is_arm
        macro="T_ARM" if is_arm else "T_X86"
        safe=name.replace(".","_")
        L(f"static void run_{safe}(void){{")
        for tv in tvs:
            rs=fmt(tv.rj);ks=fmt(tv.rk);w=wname(tv,name)
            if is_arm:
                af=f_af(tv.armfl)
                L(f'  {{ arm_flags_t _a={af};')
                L(f'  {macro}("{name} {tv.label}","{tv.asm_t}",{rs},{ks},_a,{w}); }}')
            else:
                ef=f_ef(tv.efl)
                L(f'  {{ x86_flags_t _e={ef};')
                L(f'  {macro}("{name} {tv.label}","{tv.asm_t}",{rs},{ks},_e,{w}); }}')
        L("}");L("")
    state_groups=defaultdict(list)
    for name,tv in state_tvs:state_groups[name].append(tv)
    for name in sorted(state_groups):
        safe=name.replace(".","_")
        L(f"static void run_state_{safe}(void){{")
        for tv in state_groups[name]:
            label=c_escape(f"{name} {tv.label}")
            ef=f_ef(tv.efl)
            af=f_af(tv.armfl)
            rs=fmt(tv.rj)
            ks=fmt(tv.rk)
            ss=fmt(tv.scratch0)
            top=tv.top & 7
            tm="true" if tv.tm else "false"
            w=wname(tv,name)
            is_fpr = "fpr14" in tv.checks
            L(f'  {{ x86_flags_t _e={ef}; arm_flags_t _a={af};')
            L(f'    model_prepare({rs},{ks},{ss},_e,_a,{top},{tm});')
            if is_fpr:
                L('    FPR[12] = GPR[12];')   # copy GPR → FPR for fcvt inputs
                L('    FPR[13] = GPR[13];')
            L(f'    {w}();')
            if is_fpr:
                L('    uint64_t _mfpr14=FPR[14];')  # model result from FPR[fd]
            L('    uint64_t _mgpr12=GPR[12], _mgpr13=GPR[13], _mscr0=SCRATCH[0];')
            L('    uint32_t _mef=x86_pack_flags(EFLAGS), _maf=arm_pack_flags(ARMFLAGS);')
            L('    unsigned _mtop=X87_TOP; bool _mtm=X87_TM;')
            L('    (void)_mgpr12; (void)_mgpr13; (void)_mscr0; (void)_mef; (void)_maf; (void)_mtop; (void)_mtm;')
            L(f'    {hw_prepare_for(name, tv)}(_e,_a,{top},{tm});')
            if "gpr13" in tv.checks:
                # armmove: _a0 = rd (rk_val), _a1 = rj (rj_val) — operand swap
                L(f'    uint64_t _a0={ks}, _a1={rs}, _scr0={ss};')
            else:
                L(f'    uint64_t _a0={rs}, _a1={ks}, _scr0={ss};')
            L(f'    __asm__ volatile("{tv.asm_t}" : "+r"(_a0), "+r"(_a1), "+r"(_scr0) : : "memory");')
            if is_fpr:
                L(f'    check_u64("{label}","FPR[rd]",_mfpr14,_a0);')
            elif "gpr13" in tv.checks:
                L(f'    check_u64("{label}","GPR[rd]",_mgpr13,_a0);')
            elif "gpr12" in tv.checks:
                L(f'    check_u64("{label}","GPR[rd]",_mgpr12,_a0);')
            if "scratch0" in tv.checks:
                L(f'    check_u64("{label}","SCR0",_mscr0,_scr0);')
            if "eflags" in tv.checks:
                L(f'    check_u32("{label}","EFLAGS",_mef & 0x8d5,x86_pack_flags(hw_read_x86_flags()) & 0x8d5);')
            if "armflags" in tv.checks:
                L(f'    check_u32("{label}","ARMFLAGS",_maf,arm_pack_flags(hw_read_arm_flags()));')
            if "top" in tv.checks:
                L(f'    check_u64("{label}","TOP",_mtop,hw_read_x87_top() & 7);')
            if "tm" in tv.checks:
                L(f'    check_bool("{label}","TM",_mtm,hw_read_x87_tm());')
            L("  }")
        L("}");L("")
    L("void gen_verify_main(void){")
    all_instrs=set(groups)|set(state_groups)
    L(f'  printf("\\n=== Generated (%d instrs) ===\\n",{len(all_instrs)});')
    for name in sorted(groups):
        L(f'  printf("\\n--- {name} ---\\n");')
        L(f"  run_{name.replace('.','_')}();")
        if name in state_groups:
            L(f"  run_state_{name.replace('.','_')}();")
    for name in sorted(set(state_groups)-set(groups)):
        L(f'  printf("\\n--- {name} ---\\n");')
        L(f"  run_state_{name.replace('.','_')}();")
    L("}")
    return "\n".join(lines)

def lbt_probe_runtime_prelude():
    return "\n".join([
        "// Auto-generated by gen_lbt_verify.py",
        '#include "../lbt_model.h"',
        "#include <stdio.h>",
        "",
        "static void probe_x86(const char *name, x86_flags_t init, x86_flags_t hw) {",
        "    uint32_t h = x86_pack_flags(hw);",
        "    uint32_t i = x86_pack_flags(init);",
        '    printf("PROBE|%s|init=0x%03x|hw=0x%03x\\n", name, i & 0x8d5, h & 0x8d5);',
        "}",
        "",
        "#define P_X86(name, asm_text, rj_val, rk_val, init_ef) do { \\",
        "    x86_flags_t _ef = init_ef; EFLAGS = _ef; \\",
        "    uint64_t a = rj_val, b = rk_val; \\",
        "    hw_write_x86_flags(_ef); \\",
        "    __asm__ volatile(asm_text : \"+r\"(a), \"+r\"(b) : : \"memory\"); \\",
        "    probe_x86(name, _ef, hw_read_x86_flags()); \\",
        "} while(0)",
        "",
    ])

def write_lbt_probe_chunks(probe_groups):
    OUTPUT_PROBE_DIR.mkdir(parents=True, exist_ok=True)
    keep = set()
    names = []
    for name in sorted(probe_groups):
        safe = name.replace(".", "_")
        path = OUTPUT_PROBE_DIR / f"{safe}.c"
        keep.add(path.name)
        names.append((name, safe))
        lines = [lbt_probe_runtime_prelude(), f"void gen_probe_{safe}(void){{"]
        lines.append(f'  printf("\\n--- probe {name} ---\\n");')
        for tv in probe_groups[name]:
            ef = f_ef(tv.efl)
            rs = fmt(tv.rj)
            ks = fmt(tv.rk)
            asm_t = lbt_probe_asm_text(name, tv)
            label = c_escape(f"{name} {tv.label}")
            lines.append(f'  {{ x86_flags_t _e={ef}; P_X86("{label}","{asm_t}",{rs},{ks},_e); }}')
        lines.append("}")
        path.write_text("\n".join(lines) + "\n")
    for stale in OUTPUT_PROBE_DIR.glob("*.c"):
        if stale.name not in keep:
            stale.unlink()
    return names

def emit_lbt_probe():
    L=lambda x:lines.append(x);lines=[]
    L("// Auto-generated by gen_lbt_verify.py")
    L("#include <stdio.h>")
    L("")
    probe_groups = collect_probe_groups(native=False)
    chunk_names = write_lbt_probe_chunks(probe_groups)
    for _, safe in chunk_names:
        L(f"void gen_probe_{safe}(void);")
    L("")
    L("void gen_probe_main(void){")
    L(f'  printf("\\n=== Undefined flag probe (%d instrs) ===\\n",{len(probe_groups)});')
    for _, safe in chunk_names:
        L(f"  gen_probe_{safe}();")
    L("}")
    return "\n".join(lines)

def collect_probe_groups(native=False):
    groups = {}
    for p in sorted(HEADER_DIR.glob("x86*.h")):
        name = p.stem.replace("_",".")
        if not x86_supported(name):
            continue
        tvs = probe_vectors_for(name, native=native)
        if tvs:
            groups[name] = tvs
    if native:
        for st, suffixes in sorted(NATIVE_PROBE_ONLY.items()):
            for sfx in suffixes:
                name = f"{st}.{sfx}"
                tvs = probe_vectors_for(name, native=True)
                if tvs:
                    groups[name] = tvs
    return groups

def lbt_probe_asm_text(name, tv):
    st = stem(name)
    if st in ("x86inc","x86dec"):
        return f"{name} %0"
    if st in ("x86slli","x86srli","x86srai","x86rotli","x86rotri","x86rcli","x86rcri"):
        imm = tv.mw.imm_val if tv.mw and tv.mw.imm_val is not None else (tv.imm_val or 0)
        return f"{name} %0,{imm}"
    return f"{name} %0,%1"

def c_escape(s):
    return s.replace("\\", "\\\\").replace('"', '\\"')

def emit_x86(all_tvs):
    L=lambda x:lines.append(x);lines=[]
    groups=defaultdict(list)
    for name,tv in all_tvs:
        if x86_supported(name):
            groups[name].append(tv)

    L("// Auto-generated by gen_lbt_verify.py --x86")
    L("#include <stdint.h>")
    L("#include <stdio.h>")
    L("")
    L(f"#define FLAG_MASK_ALL 0x{FLAG_MASK_ALL:03x}ULL")
    L("static unsigned g_total, g_failed;")
    L("")
    L("static void check_flags(const char *name, uint64_t model, uint64_t hw, uint64_t mask){")
    L("    g_total++;")
    L("    int ok = ((model ^ hw) & mask) == 0;")
    L("    if (!ok) g_failed++;")
    L("    printf(\"CHECK|%s|%s|mask=0x%03llx|model=0x%03llx|hw=0x%03llx\\n\",")
    L("           ok ? \"PASS\" : \"FAIL\", name, (unsigned long long)(mask & FLAG_MASK_ALL),")
    L("           (unsigned long long)(model & FLAG_MASK_ALL), (unsigned long long)(hw & FLAG_MASK_ALL));")
    L("}")
    L("")
    L("static void probe_flags(const char *name, uint64_t init, uint64_t hw){")
    L("    printf(\"PROBE|%s|init=0x%03llx|hw=0x%03llx\\n\", name,")
    L("           (unsigned long long)(init & FLAG_MASK_ALL), (unsigned long long)(hw & FLAG_MASK_ALL));")
    L("}")
    L("")

    emitted = 0
    for name in sorted(groups):
        tvs = groups[name]
        safe = name.replace(".","_")
        L(f"static void run_check_{safe}(void){{")
        for i,tv in enumerate(tvs):
            asm_t = x86_asm_text(name, tv)
            if asm_t is None:
                continue
            mask = x86_defined_mask(name, tv)
            if mask == 0:
                continue
            suffix = suf(name)
            ctype = X86_CTYPE.get(suffix, "uint64_t")
            rj = fmt(tv.rj & wmax(width(name)))
            rk = fmt(tv.rk & wmax(width(name)))
            init = ef_raw(tv.efl)
            model = lbt_model_flags(name, tv) & FLAG_MASK_ALL
            label = c_escape(f"{name} {tv.label}")
            emit_x86_case(L, name, ctype, asm_t, rj, rk, init, f"check_flags(\"{label}\",0x{model:03x}ULL,_hw,0x{mask:03x}ULL);", tv.imm_val)
        L("}")
        emitted += 1
        L("")

    probe_groups = collect_probe_groups(native=True)
    for name in sorted(probe_groups):
        safe = name.replace(".","_")
        L(f"static void run_probe_{safe}(void){{")
        for tv in probe_groups[name]:
            asm_t = x86_asm_text(name, tv)
            if asm_t is None:
                continue
            suffix = suf(name)
            ctype = X86_CTYPE.get(suffix, "uint64_t")
            rj = fmt(tv.rj & wmax(width(name)))
            rk = fmt(tv.rk & wmax(width(name)))
            init = ef_raw(tv.efl)
            label = c_escape(f"{name} {tv.label}")
            emit_x86_case(L, name, ctype, asm_t, rj, rk, init, f"probe_flags(\"{label}\",0x{init:03x}ULL,_hw);", tv.imm_val)
        L("}")
        L("")

    L("int main(void){")
    L(f'    printf("INFO|mode=x86|verify_instrs={len(groups)}|probe_instrs={len(probe_groups)}\\n");')
    for name in sorted(groups):
        L(f"    run_check_{name.replace('.','_')}();")
    for name in sorted(probe_groups):
        L(f"    run_probe_{name.replace('.','_')}();")
    L('    printf("SUMMARY|tests=%u|passed=%u|failed=%u\\n", g_total, g_total - g_failed, g_failed);')
    L("    return g_failed ? 1 : 0;")
    L("}")
    return "\n".join(lines)

def emit_x86_case(L, name, ctype, asm_t, rj, rk, init, after, imm_val=None):
    st = stem(name)
    suffix = suf(name)
    if st == "x86mul":
        emit_x86_mul_case(L, suffix, asm_t, rj, rk, init, after)
        return
    if st in ("x86div","x86idiv"):
        emit_x86_div_case(L, suffix, asm_t, rj, rk, init, after)
        return

    setup = "pushq %[init]\\n\\tpopfq\\n\\t"
    finish = "\\n\\tpushfq\\n\\tpopq %[hw]"
    if "%[b]" in asm_t and ("%[c]" in asm_t or "%b[c]" in asm_t):
        cval = 0 if imm_val is None else imm_val
        L(f'    {{ {ctype} a=({ctype}){rj}, b=({ctype}){rk}; uint8_t c=(uint8_t){cval}; uint64_t _init=0x{init:03x}ULL, _hw; __asm__ volatile("{setup}{asm_t}{finish}" : [a] "+&q"(a), [hw] "=r"(_hw) : [b] "q"(b), [c] "c"(c), [init] "r"(_init) : "cc","memory"); {after} }}')
    elif "%[b]" in asm_t:
        L(f'    {{ {ctype} a=({ctype}){rj}, b=({ctype}){rk}; uint64_t _init=0x{init:03x}ULL, _hw; __asm__ volatile("{setup}{asm_t}{finish}" : [a] "+&q"(a), [hw] "=r"(_hw) : [b] "q"(b), [init] "r"(_init) : "cc","memory"); {after} }}')
    elif "%[c]" in asm_t or "%b[c]" in asm_t:
        L(f'    {{ {ctype} a=({ctype}){rj}; uint8_t c=(uint8_t){rk}; uint64_t _init=0x{init:03x}ULL, _hw; __asm__ volatile("{setup}{asm_t}{finish}" : [a] "+&q"(a), [hw] "=r"(_hw) : [c] "c"(c), [init] "r"(_init) : "cc","memory"); {after} }}')
    else:
        L(f'    {{ {ctype} a=({ctype}){rj}; uint64_t _init=0x{init:03x}ULL, _hw; __asm__ volatile("{setup}{asm_t}{finish}" : [a] "+&q"(a), [hw] "=r"(_hw) : [init] "r"(_init) : "cc","memory"); {after} }}')

def emit_x86_mul_case(L, suffix, op, rj, rk, init, after):
    signed = suffix in ("b","h","w","d")
    rhs_type = X86_CTYPE.get(suffix, "uint64_t")
    if suffix in ("b","bu"):
        ax_type = "uint16_t"
        dx_out = ""
        dx_constraint = ""
    elif suffix in ("h","hu"):
        ax_type = "uint16_t"
        dx_out = "uint16_t dx;"
        dx_constraint = ', "=&d"(dx)'
    elif suffix in ("w","wu"):
        ax_type = "uint32_t"
        dx_out = "uint32_t dx;"
        dx_constraint = ', "=&d"(dx)'
    else:
        ax_type = "uint64_t"
        dx_out = "uint64_t dx;"
        dx_constraint = ', "=&d"(dx)'
    setup = "pushq %[init]\\n\\tpopfq\\n\\t"
    finish = "\\n\\tpushfq\\n\\tpopq %[hw]"
    # Put the explicit multiplier in RBX so it cannot alias the implicit AX/EAX/RAX operand.
    L(f'    {{ {ax_type} ax=({ax_type})({rj}); {rhs_type} b=({rhs_type})({rk}); {dx_out} uint64_t _init=0x{init:03x}ULL, _hw; __asm__ volatile("{setup}{op} %[b]{finish}" : "+&a"(ax){dx_constraint}, [hw] "=r"(_hw) : [b] "b"(b), [init] "r"(_init) : "cc","memory"); {after} }}')

def emit_x86_div_case(L, suffix, op, rj, rk, init, after):
    signed = op.startswith("idiv")
    setup = "pushq %[init]\\n\\tpopfq\\n\\t"
    finish = "\\n\\tpushfq\\n\\tpopq %[hw]"
    if suffix == "b":
        div_type = "int8_t" if signed else "uint8_t"
        ax_init = f"(uint16_t)(int16_t)(int8_t)((uint8_t)({rj}))" if signed else f"(uint16_t)(uint8_t)({rj})"
        L(f'    {{ uint16_t ax={ax_init}; {div_type} b=({div_type})({rk}); uint64_t _init=0x{init:03x}ULL, _hw; __asm__ volatile("{setup}{op} %[b]{finish}" : "+&a"(ax), [hw] "=r"(_hw) : [b] "b"(b), [init] "r"(_init) : "cc","memory"); {after} }}')
        return
    if suffix == "h":
        reg_type = "uint16_t"; div_type = "int16_t" if signed else "uint16_t"
        sign_hi = "0xffffu" if signed else "0u"
        hi_expr = f"(({reg_type})({rj}) & 0x8000u) ? {sign_hi} : 0u" if signed else "0u"
    elif suffix == "w":
        reg_type = "uint32_t"; div_type = "int32_t" if signed else "uint32_t"
        sign_hi = "0xffffffffu" if signed else "0u"
        hi_expr = f"(({reg_type})({rj}) & 0x80000000u) ? {sign_hi} : 0u" if signed else "0u"
    else:
        reg_type = "uint64_t"; div_type = "int64_t" if signed else "uint64_t"
        sign_hi = "0xffffffffffffffffULL" if signed else "0u"
        hi_expr = f"(({reg_type})({rj}) & 0x8000000000000000ULL) ? {sign_hi} : 0u" if signed else "0u"
    L(f'    {{ {reg_type} ax=({reg_type})({rj}), dx=({reg_type})({hi_expr}); {div_type} b=({div_type})({rk}); uint64_t _init=0x{init:03x}ULL, _hw; __asm__ volatile("{setup}{op} %[b]{finish}" : "+&a"(ax), "+&d"(dx), [hw] "=r"(_hw) : [b] "b"(b), [init] "r"(_init) : "cc","memory"); {after} }}')

def ef_raw(ef):
    ef = ef or {}
    raw = 0
    for flag, bit in FLAG_BITS.items():
        if ef.get(flag, 0):
            raw |= bit
    return raw

def lbt_model_flags(name, tv):
    st = stem(name)
    b = width(name)
    M = wmax(b)
    lhs = tv.rj & M
    rhs = tv.rk & M
    f = ef_raw(tv.efl)
    def setf(flag, val):
        nonlocal f
        if val: f |= FLAG_BITS[flag]
        else: f &= ~FLAG_BITS[flag]
    def getf(flag):
        return 1 if (f & FLAG_BITS[flag]) else 0
    def szp(r):
        setf("PF", parity_even_py(r & 0xff))
        setf("ZF", (r & M) == 0)
        setf("SF", (r >> (b - 1)) & 1)

    if st in ("x86add","x86adc"):
        carry = getf("CF") if st == "x86adc" else 0
        res = (lhs + rhs + carry) & M
        full = lhs + rhs + carry
        setf("CF", full > M)
        setf("AF", ((lhs ^ rhs ^ res) & 0x10) != 0)
        if suf(name) in ("wu","du"):
            setf("OF", 0)
        else:
            setf("OF", ((~(lhs ^ rhs)) & (lhs ^ res) & (1 << (b - 1))) != 0)
        szp(res)
    elif st in ("x86sub","x86sbc"):
        borrow = getf("CF") if st == "x86sbc" else 0
        subtr = rhs + borrow
        res = (lhs - subtr) & M
        setf("CF", lhs < subtr)
        if st == "x86sbc":
            # LBT hardware AF formula: does NOT incorporate CF input.
            # This diverges from Intel x86 SBB spec when CF=1.
            # Running --x86 mode on real x86 will FAIL for cases with CF=1,
            # exposing the erratum.
            setf("AF", (lhs & 0xf) < (rhs & 0xf))
        else:
            setf("AF", ((lhs ^ rhs ^ res) & 0x10) != 0)
        if suf(name) in ("wu","du"):
            setf("OF", 0)
        else:
            setf("OF", ((lhs ^ rhs) & (lhs ^ res) & (1 << (b - 1))) != 0)
        szp(res)
    elif st in ("x86and","x86or","x86xor"):
        if st == "x86and": res = lhs & rhs
        elif st == "x86or": res = lhs | rhs
        else: res = lhs ^ rhs
        setf("CF", 0); setf("OF", 0); setf("AF", 0)
        szp(res)
    elif st in ("x86inc","x86dec"):
        delta = 1 if st == "x86inc" else -1
        res = (lhs + delta) & M
        setf("AF", ((lhs ^ abs(delta) ^ res) & 0x10) != 0)
        if st == "x86inc":
            setf("OF", lhs == ((1 << (b - 1)) - 1))
        else:
            setf("OF", lhs == (1 << (b - 1)))
        szp(res)
    elif st == "x86mul":
        signed = suf(name) in ("b","h","w","d")
        if signed:
            a = sign_extend_py(lhs, b); c = sign_extend_py(rhs, b); prod = a * c
            overflow = prod < -(1 << (b - 1)) or prod > ((1 << (b - 1)) - 1)
        else:
            prod = lhs * rhs
            overflow = (prod >> b) != 0
        setf("CF", overflow); setf("OF", overflow)
        setf("PF", 0); setf("AF", 0); setf("ZF", 0); setf("SF", 0)
    elif st in ("x86sll","x86slli","x86srl","x86srli","x86sra","x86srai"):
        c = effective_count(name, tv)
        raw = raw_count(name, tv) & (0x3f if b == 64 else 0x1f)
        if c == 0:
            return f
        if "sll" in st:
            carry = 0 if c > b else ((lhs >> (b - c)) & 1)
            res = 0 if c >= b else ((lhs << c) & M)
            if raw == 1: setf("OF", ((lhs ^ res) & (1 << (b - 1))) != 0)
        elif "srl" in st:
            carry = 0 if c > b else ((lhs >> (c - 1)) & 1)
            res = 0 if c >= b else (lhs >> c)
            if raw == 1: setf("OF", (lhs >> (b - 1)) & 1)
        else:
            carry = 0 if c > b else ((lhs >> (c - 1)) & 1)
            res = sar_py(lhs, b, c)
            if raw == 1: setf("OF", 0)
        setf("CF", carry); szp(res)
    elif st in ("x86rotl","x86rotli","x86rotr","x86rotri"):
        c = raw_count(name, tv) & (0x3f if b == 64 else 0x1f)
        n = c % b
        if "rotl" in st:
            carry = (lhs >> (b - 1)) & 1 if n == 0 else (lhs >> (b - n)) & 1
            res = lhs if n == 0 else (((lhs << n) | (lhs >> (b - n))) & M)
            if c == 1: setf("OF", ((lhs ^ res) & (1 << (b - 1))) != 0)
        else:
            carry = 0 if n == 0 else (lhs >> (n - 1)) & 1
            res = lhs if n == 0 else (((lhs >> n) | (lhs << (b - n))) & M)
            if c == 1: setf("OF", (((res >> (b - 1)) ^ (res >> (b - 2))) & 1) != 0)
        setf("CF", carry)
    elif st in ("x86rcl","x86rcli","x86rcr","x86rcri"):
        c = raw_count(name, tv) & (0x3f if b == 64 else 0x1f)
        n = c % (b + 1) if b in (8,16) else c
        if n == 0:
            return f
        carry_in = getf("CF")
        if "rcl" in st:
            carry = (lhs >> (b - n)) & 1
            res = ((lhs << 1) | carry_in) & M
            if c == 1: setf("OF", (((res >> (b - 1)) & 1) != carry))
        else:
            carry = (lhs >> ((n - 1) % (b + 1))) & 1
            res = ((lhs >> 1) | (carry_in << (b - 1))) & M
            if c == 1: setf("OF", (((res >> (b - 1)) ^ (res >> (b - 2))) & 1) != 0)
        setf("CF", carry)
    return f

def parity_even_py(v):
    return (bin(v & 0xff).count("1") % 2) == 0

def sign_extend_py(v, bits):
    sign = 1 << (bits - 1)
    return (v ^ sign) - sign

def sar_py(v, bits, count):
    signed = sign_extend_py(v, bits)
    return (signed >> count) & wmax(bits)

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--x86", action="store_true", help="generate native x86_64 verify/probe runner")
    args = ap.parse_args()

    all_tvs, sk = collect_all_tvs(include_arm=not args.x86)
    ni=len(set(n for n,_ in all_tvs))
    if args.x86:
        print(f"x86 instrs:{ni} vectors:{len(all_tvs)} skipped:{sk}")
        c=emit_x86(all_tvs)
        with open(OUTPUT_X86_C,"w") as f:f.write(c)
        print(f"→ {OUTPUT_X86_C} ({len(c)} bytes)")
    else:
        state_tvs, state_sk = collect_state_tvs()
        total_instrs = len(set(n for n,_ in all_tvs) | set(n for n,_ in state_tvs))
        print(f"Instrs:{total_instrs} flag_vectors:{len(all_tvs)} state_vectors:{len(state_tvs)} skipped:{sk + state_sk}")
        c=emit(all_tvs, state_tvs)
        with open(OUTPUT_C,"w") as f:f.write(c)
        print(f"→ {OUTPUT_C} ({len(c)} bytes)")
        pc=emit_lbt_probe()
        with open(OUTPUT_PROBE_C,"w") as f:f.write(pc)
        print(f"→ {OUTPUT_PROBE_C} ({len(pc)} bytes)")

if __name__=="__main__":
    main()
