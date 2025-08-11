#ifndef _COMMON_MACHINE_H_
#define _COMMON_MACHINE_H_

#ifndef MACHINE_3C5000
#define MACHINE_3C5000 0
#endif

#ifndef MACHINE_3A6000
#define MACHINE_3A6000 1
#endif

#ifndef MACHINE_3C6000
#define MACHINE_3C6000 0
#endif

#ifndef MACHINE_2K1000LA
#define MACHINE_2K1000LA 0
#endif

#ifndef MACHINE_2K3000
#define MACHINE_2K3000 0
#endif

/* uarches */
#define UARCH_LA264 (MACHINE_2K1000LA)
#define UARCH_LA364E (MACHINE_2K3000)
#define UARCH_LA464 (MACHINE_3C5000)
#define UARCH_LA664 (MACHINE_3A6000 || MACHINE_3C6000)

/* capabilities */

#ifndef MACHINE_HAS_LSX
#define MACHINE_HAS_LSX 1
#endif

#ifndef MACHINE_HAS_LASX
#if (!UARCH_LA264) && (!UARCH_LA364E)
#define MACHINE_HAS_LASX 1
#endif
#endif

#endif /* _COMMON_MACHINE_H_ */
