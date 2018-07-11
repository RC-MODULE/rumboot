#ifndef REGS_GP_TIMERS_H
#define REGS_GP_TIMERS_H

/**
 * \defgroup regs_gp_timers General Purpose Timers
 * \ingroup regs
 *
 * Register offsets and constants
 * \code{.c}
 * #include <regs/regs_gp_timers.h>
 * \endcode
 *
 * @{
 */
#define ID                      0x000
#define VERSION                 0x004
#define FREE_RUN_L              0x008
#define FREE_RUN_H              0x00C
#define GLOBAL_NS               0x010
#define GLOBAL_S_L              0x014
#define GLOBAL_S_H              0x018
#define SW_RST                  0x01C
#define FIX_CMD                 0x020
#define INC_NS                  0x024
#define GLOBAL_ADJUST           0x028
#define MOVE_NS                 0x02C
#define MOVE_S                  0x030
#define SET_GLOBAL_NS           0x034
#define SET_GLOBAL_S_L          0x038
#define SET_GLOBAL_S_H          0x03C
#define ENABLE                  0x040

//  Probably there are more or less TMRs in current project
//  Watch user guide to determine this value
#define TMR_0_STATE             0x400
#define TMR_0_LIMIT             0x404
#define TMR_0_SYS_CNT_NS        0x408
#define TMR_0_SYS_CNT_S_L       0x40C
#define TMR_0_SYS_CNT_S_H       0x410
#define TMR_1_STATE             0x420
#define TMR_1_LIMIT             0x424
#define TMR_1_SYS_CNT_NS        0x428
#define TMR_1_SYS_CNT_S_L       0x42C
#define TMR_1_SYS_CNT_S_H       0x430
#define TMR_2_STATE             0x440
#define TMR_2_LIMIT             0x444
#define TMR_2_SYS_CNT_NS        0x448
#define TMR_2_SYS_CNT_S_L       0x44C
#define TMR_2_SYS_CNT_S_H       0x450
#define TMR_3_STATE             0x460
#define TMR_3_LIMIT             0x464
#define TMR_3_SYS_CNT_NS        0x468
#define TMR_3_SYS_CNT_S_L       0x46C
#define TMR_3_SYS_CNT_S_H       0x470
#define TMR_4_STATE             0x480
#define TMR_4_LIMIT             0x484
#define TMR_4_SYS_CNT_NS        0x488
#define TMR_4_SYS_CNT_S_L       0x48C
#define TMR_4_SYS_CNT_S_H       0x490
#define TMR_5_STATE             0x4A0
#define TMR_5_LIMIT             0x4A4
#define TMR_5_SYS_CNT_NS        0x4A8
#define TMR_5_SYS_CNT_S_L       0x4AC
#define TMR_5_SYS_CNT_S_H       0x4B0
 
/**
 * @}
 */

#endif /* end of include guard: REGS_GP_TIMERS_H */
