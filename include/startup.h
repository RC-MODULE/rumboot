
/* --- Mode bits in PSRs --- */

#define Mode_USR    0x10
#define Mode_FIQ    0x11
#define Mode_IRQ    0x12
#define Mode_SVC    0x13
#define Mode_MON    0x16
#define Mode_ABT    0x17
#define Mode_UND    0x1B
#define Mode_SYS    0x1F

/* --- Interrupt disable flags in PSRs --- */

#define I_Bit   0x80  /* when I bit is set, IRQ is disabled */
#define F_Bit   0x40  /* when F bit is set, FIQ is disabled */
#define A_Bit   0x100 /* when A bit is set, Imbrecise Data Abort Interrupts disabled */


/* --- Stack at the end of AMB0 --- */

#define FIQ_Size              256*2
#define IRQ_Size              256*2
#define SVC_Size              256*2
#define ABT_Size              256*1
#define UND_Size              256*1
#define USR_Size              256*2

/* --- USR and SYS mode stack shared --- */

#define MON_Size              256*1
#define TotalStack_Size       FIQ_Size + IRQ_Size + SVC_Size + ABT_Size + UND_Size + USR_Size + MON_Size
#define Heap_Size             0x4000 /* 16Kb */

/* --- Calculate adresses, put stack at AMB0 bank end --- */

#define Stack_Top             0x00080000
#define Stack_Base            Stack_Top - TotalStack_Size
#define Heap_Base             Stack_Base - Heap_Size

/* --- Stack addresses --- */

#define SVC_Stack             Stack_Top
#define IRQ_Stack             SVC_Stack - SVC_Size
#define FIQ_Stack             IRQ_Stack - IRQ_Size
#define USR_Stack             FIQ_Stack - FIQ_Size
#define UND_Stack             USR_Stack - USR_Size
#define ABT_Stack             UND_Stack - UND_Size
#define MON_Stack             ABT_Stack - ABT_Size
