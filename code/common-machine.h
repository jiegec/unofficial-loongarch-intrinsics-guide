#ifndef _COMMON_MACHINE_H_
#define _COMMON_MACHINE_H_

#ifndef MACHINE_3C5000
#define MACHINE_3C5000 0
#endif

#ifndef MACHINE_2K1000LA
#define MACHINE_2K1000LA 0
#endif

#ifndef MACHINE_2K3000
#define MACHINE_2K3000 0
#endif

/* capabilities */

#ifndef MACHINE_HAS_LSX
#define MACHINE_HAS_LSX 1
#endif

#ifndef MACHINE_HAS_LASX
#if (!MACHINE_2K1000LA) && (!MACHINE_2K3000)
#define MACHINE_HAS_LASX 1
#endif
#endif

#endif /* _COMMON_MACHINE_H_ */
