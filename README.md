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

### vsadd.b/h/w/d

Vector Saturated Add

### vssub.b/h/w/d

Vector Saturated Subtract

### vsadd.bu/hu/wu/du

Vector Saturated Add Unsigned

### vssub.bu/hu/wu/du

Vector Saturated Subtract Unsigned

### vmul.b/h/w/d

Vector Multiplication

### vmuh.b/h/w/d/bu/hu/wu/du

Vector Multiplication High

### vmadd.b/h/w/d

### vmsub.b/h/w/d

### vmaddwev.h.b/w.h/d.w/q.d

### vmaddwod.h.b/w.h/d.w/q.d

### vmaddwev.h.bu/w.hu/d.wu/q.du

### vmaddwod.h.bu/w.hu/d.wu/q.du

### vmod.b/h/w/d

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

### vpackev.b/h/w/d

### vpackod.b/h/w/d

### vpickev.b/h/w/d

### vpickod.b/h/w/d

### vreplve.b/h/w/d

### vfrstp.b/h

### vsignconv.b/h/w/d

### vfsub.s/d

### vfmul.s/d

### vfmax.s/d

### vfmin.s/d

### vfmaxa.s/d

### vfmina.s/d

### vfcvt.h.s

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

### vreplgr2vr.b/h/w/d

### vrotri.b/h/w/d

### vsrlri.b/h/w/d

### vsrari.b/h/w/d

### vpickve2gr.b/h/w/d/bu/hu/wu/du

### vreplvei.b/h/w/d

### vsllwil.h.b/w.h/d.w

### vsllwil.hu.bu/wu.hu/du.wu

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

### vshuf4i.b/h/w/d

### vldi
