struct Reg_CONTROL {
  uint32_t rx_length : 6;
  uint32_t edq_mode : 2;
  uint32_t ss_source : 1;
  uint32_t sw_ss : 1;
  uint32_t length1 : 6;
  uint32_t clock_divider : 8;
  uint32_t phase : 1;
  uint32_t polarity : 1;
  uint32_t frame_format : 2;
  uint32_t length0 : 4;
} __attribute__((packed));


void print(struct Reg_CONTROL reg);


struct Reg_FIFO_WATERMARK {
  uint32_t tx_fifo_irq_level : 3;
  uint32_t rx_fifo_irq_level : 3;
} __attribute__((packed));


uint32_t rand_control();
uint32_t rand_fifo_watermark();