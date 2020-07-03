#define assert(condition) \
  if(!(condition)) { \
    rumboot_printf("Assertion '%s' failed.\n", #condition); \
    rumboot_printf("TEST FAILED\n"); \
    return 1; \
  }


struct Reg_CONTROL {
  uint32_t rx_length,
           edq_mode,
           ss_source,
           sw_ss,
           length1,
           clock_divider,
           phase,
           polarity,
           frame_format,
           length0;
};

void print(struct Reg_CONTROL reg);


struct Reg_FIFO_WATERMARK {
  uint32_t tx_fifo_irq_level,
           rx_fifo_irq_level;
};


uint32_t bit_in_reg(uint32_t reg, int bit);
uint32_t bits_in_reg(uint32_t reg, int msb, int lsb);
int randomize(int min, int max);

uint32_t rand_control();
uint32_t rand_fifo_watermark();