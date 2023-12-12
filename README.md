# Unofficial LoongArch Intrinsics Guide

Visit <http://jia.je/unofficial-loongarch-intrinsics-guide/>

Arranged from QEMU implementation and [GCC Intrinsics](https://gcc.gnu.org/onlinedocs/gcc/LoongArch-SX-Vector-Intrinsics.html)

TODO List:

### vst

Vector Store

### vstelm.d/w/h/b

Vector Store Element

### vstx

Vector Store with Register Offset

### vseq/le/lt.b/h/w/d

Vector Set Equal/Less than or Equal/Less Than

### vsub.b/h/w/d

Vector Subtract

### vsubwev.h.b/w.h/d.w/q.d

Vector Subtract with Width Extension

### vaddwod.h.b/w.h/d.w/q.d
### vsubwod.h.b/w.h/d.w/q.d

### vsubwev.h.bu/w.hu/d.wu/q.du

Vector Subtract with Width Extension Unsigned

### vaddwod.h.bu/w.hu/d.wu/q.du
### vsubwod.h.bu/w.hu/d.wu/q.du

### vaddwod.h.bu.b/w.hu.h/q.wu.w/q.du.d

### vsadd.b/h/w/d

Vector Saturated Add

### vssub.b/h/w/d

Vector Saturated Subtract

### vsadd.bu/hu/wu/du

Vector Saturated Add Unsigned

### vssub.bu/hu/wu/du

Vector Saturated Subtract Unsigned

### vhaddw.h.b/w.h/d.w/q.d

Vector Half Add

### vhsubw.h.b/w.h/d.w/q.d

Vector Half Add

### vhaddw.hu.bu/wu.hu/du.wu/qu.du

Vector Half Add Unsigned

### vhsubw.hu.bu/wu.hu/du.wu/qu.du

Vector Half Sub Unsigned

### vavgr.b/h/w/d
### vavgr.bu/hu/wu/du

### vmax.b/h/w/d

Vector Maximum

### vmin.b/h/w/d

Vector Minimum

### vmax.bu/hu/wu/du

Vector Maximum Unsigned

### vmin.bu/hu/wu/du

Vector Minimum Unsigned

### vmul.b/h/w/d

Vector Multiplication

### vmuh.b/h/w/d/bu/hu/wu/du

Vector Multiplication High

### vmulwev.h.b/w.h/d.w/q.d

### vmulwod.h.b/w.h/d.w/q.d

### vmulwev.h.bu/w.hu/d.wu/q.du

### vmulwod.h.bu/w.hu/d.wu/q.du

### vmulwev.h.bu.b/w.hu.h/d.wu.d/q.du.d

### vmulwod.h.bu.b/w.hu.h/d.wu.d/q.du.d

### vmadd.b/h/w/d

### vmsub.b/h/w/d

### vmaddwev.h.b/w.h/d.w/q.d

### vmaddwod.h.b/w.h/d.w/q.d

### vmaddwev.h.bu/w.hu/d.wu/q.du

### vmaddwod.h.bu/w.hu/d.wu/q.du

### vdiv.b/h/w/d

### vmod.b/h/w/d

### vdiv.bu/hu/wu/du

### vmod.bu/hu/wu/du

### vsll.b/h/w/d

### vsrl.b/h/w/d

### vsra.b/h/w/d

### vrotr.b/h/w/d

### vsrlr.b/h/w/d

### vsrar.b/h/w/d

### vsrln.b.h/h.w/w.d

### vsran.b.h/h.w/w.d

### vsrlrn.b.h/h.w/w.d

### vsrarn.b.h/h.w/w.d

### vssrln.b.h/h.w/w.d

### vssran.b.h/h.w/w.d

### vssrlrn.b.h/h.w/w.d

### vssrarn.b.h/h.w/w.d

### vssrln.bu.h/hu.w/wu.d

### vssran.bu.h/hu.w/wu.d

### vssrlrn.bu.h/hu.w/wu.d

### vssrarn.bu.h/hu.w/wu.d

### vbitclr.b/h/w/d

### vbitset.b/h/w/d

### vbitrev.b/h/w/d

### vpackev.b/h/w/d

### vpackod.b/h/w/d

### vilvl.b/h/w/d

### vilvh.b/h/w/d

### vpickev.b/h/w/d

### vpickod.b/h/w/d

### vreplve.b/h/w/d

### vand.v

### vor.v

### vxor.v

### vnor.v

### vorn.v

### vfrstp.b/h

### vsub.q

### vsignconv.b/h/w/d

### vfadd.s/d

### vfsub.s/d

### vfmul.s/d

### vfdiv.s/d

### vfmax.s/d

### vfmin.s/d

### vfmaxa.s/d

### vfmina.s/d

### vfcvf.h.s/s.d

### vffint.s.l

### vffint.w.d

### vffintrm.w.d

### vffintrp.w.d

### vffintrz.w.d

### vffintrne.w.d

### vseqi.b/h/w/d

### vslei.b/h/w/d/bu/hu/wu/du

### vslti.b/h/w/d/bu/hu/wu/du

### vsubi.bu/hu/wu/du

### vbsll.v

### vbsrl.v

### vmaxi.b/h/w/d

### vmini.b/h/w/d

### vmaxi.bu/hu/wu/du

### vmini.bu/hu/wu/du

### vfrstpi.b/h

### vclo.b/h/w/d

### vclz.b/h/w/d

### vpcnt.b/h/w/d

### vneg.b/h/w/d

### vmskltz.b/h/w/d

### vmskgez.b

### vmsknz.b

### vseteqz.v

### vsetnez.v

### vsetanyeqz.b/h/w/d

### vsetanynez.b/h/w/d

### vflogb.s/d

### vfclass.s/d

### vfsqrt.s/d

### vfrecip.s/d

### vfrsqrt.s/d

### vfrecipe.s/d

### vfrsqrte.s/d

### vfrint.s/d

### vfrintrm.s/d

### vfrintrp.s/d

### vfrintrz.s/d

### vfrintrne.s/d

### vfcvtl.s.h

### vfcvth.s.h

### vfcvtl.d.s

### vfcvth.d.s

### vffint.s.w/wu/d.l/d.lu

### vffintl.d.w

### vffinth.d.w

### vftint.w.s/l.d

### vftintrm.w.s/l.d

### vftintrz.w.s/l.d

### vftintrne.w.s/l.d

### vftint.wu.s/lu.d

### vftintrz.wu.s/lu.d

### vftintl.l.s

### vftinth.l.s

### vftintrml.l.s

### vftintrmh.l.s

### vftintrpl.l.s

### vftintrph.l.s

### vftintrzl.l.s

### vftintrzh.l.s

### vftintrnel.l.s

### vftintrneh.l.s

### vexth.h.b/w.h/d.w/q.d/hu.bu/wu.hu/du.wu/qu.du

### vreplgr2vr.b/h/w/d

### vrotri.b/h/w/d

### vsrlri.b/h/w/d

### vsrari.b/h/w/d

### vinsgr2vr.b/h/w/d

### vpickve2gr.b/h/w/d/bu/hu/wu/du

### vreplvei.b/h/w/d

### vsllwil.h.b/w.h/d.w

### vextl.q.d

### vsllwil.hu.bu/wu.hu/du.wu

### vextl.qu.du

### vbitclri.b/h/w/d

### vbitseti.b/h/w/d

### vbitrevi.b/h/w/d

### vsat.b/h/w/d/bu/hu/wu/du

### vslli.b/h/w/d

### vsrli.b/h/w/d

### vsrai.b/h/w/d

### vsrlni.b.h/h.w/w.d/d.q

### vsrlrni.b.h/h.w/w.d/d.q

### vssrlni.b.h/h.w/w.d/d.q/bu.h/hu.w/wu.d/du.q

### vssrlrni.b.h/h.w/w.d/d.q/bu.h/bhu.w/wu.d/du.q

### vsrani.b.h/h.w/w.d/d.q

### vsrarni.b.h/h.w/w.d/d.q

### vssrani.b.h/h.w/w.d/d.q/bu.h/hu.w/wu.d/du.q

### vssrarni.b.h/h.w/w.d/d.q

### vssrarni.bu.h/hu.w/wu.d/du.q

### vextrins.d/w/h/b

### vshuf4i.b/h/w/d

### vbitseli.b

### vori.b

### vxori.b

### vnori.b

### vldi
