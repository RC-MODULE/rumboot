#ifndef REGS_MCLFIR_H_
#define REGS_MCLFIR_H_

#define MCLFIR_MC_ERR0                    0x00     //R 0x00000000
#define MCLFIR_MC_ERR1                    0x01     //R 0x00000000
#define MCLFIR_MC_ERR_MSK0                0x06     //R 0xFFFFFFFF
#define MCLFIR_MC_ERR_MSK1                0x07     //R 0xFFFFFFC0
#define MCLFIR_MC_ERR_ACTION0             0X0C     //RW 0x00000000
#define MCLFIR_MC_ERR_ACTION1             0X0D     //RW 0x00000000
#define MCLFIR_MC_ERR_WOF0                0X0E     //RW 0x00000000
#define MCLFIR_MC_ERR_WOF1                0X0F     //RW 0x00000000
#define MCLFIR_MC_ERR_AND0                0x02     // W N/A
#define MCLFIR_MC_ERR_AND1                0x03     // W N/A
#define MCLFIR_MC_ERR_OR0                 0x04     // W N/A
#define MCLFIR_MC_ERR_OR1                 0x05     // W N/A
#define MCLFIR_MC_ERR_MSK_AND0            0x08     // W N/A
#define MCLFIR_MC_ERR_MSK_AND1            0x09     // W N/A
#define MCLFIR_MC_ERR_MSK_OR0             0x0A     // W N/A
#define MCLFIR_MC_ERR_MSK_OR1             0x0B     // W N/A
#define MCLFIR_MC_LFIR                    0x10     // R 0x0000_0000
#define MCLFIR_MC_LFIR_AND                0x11     // W N/A
#define MCLFIR_MC_LFIR_OR                 0x12     // W N/A
#define MCLFIR_MC_LFIR_MSK                0x13     // R 0xC000_0000
#define MCLFIR_MC_LFIR_MSK_AND            0x14     // W N/A
#define MCLFIR_MC_LFIR_MSK_OR             0x15     // W N/A
#define MCLFIR_MC_CONSOL_STAT             0x20     // R 0x0000_0000
#define MCLFIR_MC_CONSOL_STAT_AND         0x21     // W N/A
#define MCLFIR_MC_CONSOL_STAT_OR          0x22     // W N/A
#define MCLFIR_MC_CONSOL_STAT_TRIG        0x23     // R 0x0000_0000
#define MCLFIR_MC_CONSOL_STAT_TRIG_AND    0x24     // W N/A
#define MCLFIR_MC_CONSOL_STAT_TRIG_OR     0x25     // W N/A
#define MCLFIR_MC_CONSOL_STAT_MSK         0x26     // R 0xFFF0_8000
#define MCLFIR_MC_CONSOL_STAT_MSK_AND     0x27     // W N/A
#define MCLFIR_MC_CONSOL_STAT_MSK_OR      0x28     // W N/A
#define MCLFIR_MC_CONSOL_STATSUM          0x30     // R 0x0000_0000
#define MCLFIR_MC_CONSOL_STATSUM_AND      0x31     // W N/A
#define MCLFIR_MC_CONSOL_STATSUM_OR       0x32     // W N/A
#define MCLFIR_MC_CONSOL_STATSUM_MSK      0x33     // R 0x8000_0000
#define MCLFIR_MC_CONSOL_STATSUM_MSK_AN   0x34     // W N/A
#define MCLFIR_MC_CONSOL_STATSUM_MSK_OR   0x35     // W N/A

#endif /* REGS_MCLFIR_H_ */
