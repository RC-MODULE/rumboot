
/* --- Peripheral Base Addresses --- */

#define DIT0_BASE      0x01012000
#define DIT1_BASE      0x01013000
#define DIT2_BASE      0x01014000
#define DIT3_BASE      0x01015000

#define TSNS_BASE      0x01019000

#define GPIO0_BASE     0x01036000
#define GPIO1_BASE     0x01037000
#define GPIO2_BASE     0x01038000




/* --- GPIO (PL061) --- */

#define GPIO_DIR                 0x0400
#define GPIO_DATA                0x03FC



/* --- TIMER (ARM SP804) --- */

#define TIMER0_BASE    0x0
#define TIMER1_BASE    0x020

#define TIMER_LOAD     0x000
#define TIMER_VALUE    0x004
#define TIMER_CONTROL  0x008
#define TIMER_CLEAR    0x00C
#define TIMER_RIS      0x010
#define TIMER_MIS      0x014
#define TIMER_BGLOAD   0x018

#define TIMER_ITCR     0xF00
#define TIMER_ITOP     0xF04

#define TIMER_PERIPHID0  0xFE0
#define TIMER_PERIPHID1  0xFE4
#define TIMER_PERIPHID2  0xFE8
#define TIMER_PERIPHID3  0xFEC
#define TIMER_PCELLID0   0xFF0
#define TIMER_PCELLID1   0xFF4
#define TIMER_PCELLID2   0xFF8
#define TIMER_PCELLID3   0xFFC

/* --- WDOG (ARM SP805) --- */

#define WDOG_LOAD    0x000
#define WDOG_VALUE   0x004
#define WDOG_CONTROL 0x008
#define WDOG_INTCLR  0x00C
#define WDOG_RIS     0x010
#define WDOG_MIS     0x014
#define WDOG_LOCK    0xC00

#define WDOG_ITCR  0xF00
#define WDOG_ITOP  0xF04

#define WDOG_PERIPHID0  0xFE0
#define WDOG_PERIPHID1  0xFE4
#define WDOG_PERIPHID2  0xFE8
#define WDOG_PERIPHID3  0xFEC
#define WDOG_PCELLID0   0xFF0
#define WDOG_PCELLID1   0xFF4
#define WDOG_PCELLID2   0xFF8
#define WDOG_PCELLID3   0xFFC

/* --- TSNS (Module) ----*/

#define TSNS_PWDN     0x000
#define TSNS_START    0x004
#define TSNS_IR       0x008
#define TSNS_IM       0x00C
#define TSNS_CLKDIV   0x010
#define TSNS_LEVEL    0x014
#define TSNS_DATA     0x018
#define TSNS_ENZCALIB 0x01C

