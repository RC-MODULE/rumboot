#ifndef __REGS_NMC_DBG_H__
#define __REGS_NMC_DBG_H__

/* nmc-gcc asm compiler don't understand hex numbers in it's patterns
   with math operations. */

#define NMC_DBG_ITR_LOW     64
#define NMC_DBG_ITR_HIGH    72
#define NMC_DBG_DSCRset     112
#define NMC_DBG_DSCRreset   120
#define NMC_DBG_DSCR        136
#define NMC_DBG_DRCR        144
#define NMC_DBG_DTRRX       192
#define NMC_DBG_DTRTX       200
#define NMC_DBG_IRNR        208
#define NMC_DBG_BVR0        256
#define NMC_DBG_BVR1        264
#define NMC_DBG_BVR2        272
#define NMC_DBG_BVR3        280
#define NMC_DBG_BVR4        288
#define NMC_DBG_BVR5        296
#define NMC_DBG_BVR6        304
#define NMC_DBG_BVR7        312
#define NMC_DBG_DRAR        512
#define NMC_DBG_DSAR        1024
#define NMC_DBG_SFTRST      1280
#define NMC_DBG_SFTNMI      1288
#define NMC_DBG_SFTSTARTIAG 1296
#define NMC_DBG_LAR         4016
#define NMC_DBG_LSR         4032
#define NMC_DBG_PID4        8144
#define NMC_DBG_PID0        8160
#define NMC_DBG_PID1        8164
#define NMC_DBG_PID2        8168
#define NMC_DBG_PID3        8172
#define NMC_DBG_CID0        8176
#define NMC_DBG_CID1        8180
#define NMC_DBG_CID2        8184
#define NMC_DBG_CID3        8188

#define GET_NMC_ADDR(x,y) (x+y)>>2

#endif
