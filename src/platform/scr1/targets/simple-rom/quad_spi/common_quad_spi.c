// #include <string.h>

// #include <rumboot/printf.h>

// #include <regs/quad_spi.h> // offset in quad_spi`s memory map
// #include <platform/quad_spi/common_quad_spi.h>
// // #include <platform/scr1/platform/quad_spi/common_quad_spi.h>




// void print(struct Reg_CONTROL reg) {
//   rumboot_printf(
//     "rx_length=%x\nedq_mode=%x\nss_source=%x\nsw_ss=%x\nlength1=%x\nclock_divider=%x\nphase=%x\npolarity=%x\nframe_format=%x\nlength0=%x\n", 
//     reg.rx_length, reg.edq_mode, reg.ss_source, reg.sw_ss, reg.length1, 
//     reg.clock_divider, reg.phase, reg.polarity, reg.frame_format, reg.length0
//   );
// }


// void print(struct Reg_FIFO_WATERMARK reg) {
//   rumboot_printf(
//     "tx_fifo_irq_level=%x\nrx_fifo_irq_level=%x\n",
//     reg.tx_fifo_irq_level,
//     reg.rx_fifo_irq_level
//   );
// }
