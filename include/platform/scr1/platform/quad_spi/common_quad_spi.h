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


uint32_t rand_control();
uint32_t rand_fifo_watermark();