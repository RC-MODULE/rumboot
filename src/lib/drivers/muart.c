#include <stdint.h>
#include <stdbool.h>

#include <devices/muart.h>
#include <regs/regs_muart.h>
#include <rumboot/io.h>
#include <rumboot/timer.h>
#include <rumboot/printf.h>
#include <devices/mdma_chan_api.h>

static uint32_t calc_bdiv(uint32_t baud_rate, uint8_t n)
{
        return (400 * 1000000 / baud_rate) / 8;
}
void set_reg(uint32_t base, uint32_t reg_offset, uint32_t value)
{
	iowrite32(value, base + reg_offset);
}
// better set_bit
void set_bit(uint32_t base, uint32_t reg_offset, uint32_t bit_pos)
{
        uint32_t reg = ioread32(base + reg_offset);

        reg |= (1 << bit_pos);
        iowrite32(reg, base + reg_offset);
}
void reset_bit(uint32_t base, uint32_t reg_offset, uint32_t bit_pos)
{
        uint32_t reg = ioread32(base + reg_offset);

        reg &= ~(1 << bit_pos);
        iowrite32(reg, base + reg_offset);
}
/*static void set_field(uint32_t base, uint32_t reg_offset, uint32_t bit_pos,
                      uint32_t mask)
{
	uint32_t reg = ioread32(base + reg_offset);

	reg |= (mask << bit_pos);
	iowrite32(reg, base + reg_offset);
}*/
// not needed set_bit is enough
/*
static void reset_field(uint32_t base, uint32_t reg_offset, uint32_t bit_pos,
                        uint32_t mask)
{
	uint32_t reg = ioread32(base + reg_offset);

	reg &= ~(mask << bit_pos);
	iowrite32(reg, base + reg_offset);
}
*/

void muart_init(const uint32_t base, const struct muart_conf *conf)
{
        uint32_t ctrl = 0;
        uint32_t bdiv = ioread32(base + MUART_BDIV);
/*
rumboot_printf("CONF dum= %x\n", conf->dum);
rumboot_printf("CONF wlen= %x\n", conf->wlen);
rumboot_printf("CONF stp2= %x\n", conf->stp2);
rumboot_printf("CONF is_even= %x\n", conf->is_even);
rumboot_printf("CONF is_parity= %x\n", conf->is_parity);
rumboot_printf("CONF mode= %x\n", conf->mode);
rumboot_printf("CONF is_loopback= %x\n", conf->is_loopback);
rumboot_printf("CONF baud_rate= %x\n", conf->baud_rate);
rumboot_printf("CONF is_dma= %x\n", conf->is_dma);
*/


	ctrl |= (conf->wlen << MUART_WLEN_i);
	ctrl |= (conf->stp2 << MUART_STP2_i);

	if (conf->is_even) ctrl |= (1 << MUART_EPS_i);
	if (conf->is_loopback) ctrl |= (1 << MUART_LBE_i);
        if (conf->is_parity) ctrl |= (1 << MUART_PEN_i);

	if( conf->mode == RS_232 ) {
		ctrl |= (1 << MUART_RTSen_i) | (1 << MUART_CTSen_i);
                ctrl |= (conf->mode << MUART_MDS_i);
        }else if( conf->mode == RS_485) {
		ctrl |= (1 << MUART_RTSen_i) & ~(1 << MUART_CTSen_i);
                ctrl |= (conf->mode << MUART_MDS_i);
        }else if( conf->mode == RS_422) {
            NULL; // 0 in filds RTSen, CTSen, MDS
        }

        if (conf->is_dma) {
            ctrl &= ~(1 << MUART_APB_MD);
            ctrl |= (1 << MUART_DUM_i);
        } else ctrl |= (1 << MUART_APB_MD);

	set_reg(base, MUART_CTRL, ctrl);

    bdiv = calc_bdiv(conf->baud_rate,8);
    set_reg(base, MUART_BDIV, bdiv);
}

void muart_enable(uint32_t base)
{
        set_bit(base, MUART_CTRL, MUART_MEN_i);
}

void muart_disable(uint32_t base)
{
        reset_bit(base, MUART_CTRL, MUART_MEN_i);
}

// APB
void muart_write_char(uint32_t base, char ch)
{
    while ((ioread32(base + MUART_FIFO_STATE) & 0x7ff0000) >= 0x3ff0000) {
        rumboot_printf(" muart_write_char fifo state: %x\n",
                       ioread32(base + MUART_FIFO_STATE));
    };
    iowrite8((uint8_t)ch, base + MUART_DTRANS);
}

char muart_read_char(uint32_t base)
{
	char ch = '\0';
        uint32_t reg;

        while (((reg=ioread32(base + MUART_FIFO_STATE)) & 0xfff) == 0) {
            rumboot_printf("muart_read_char fifo state: %x\n", reg);
        }
	ch = ioread8(base + MUART_DREC);
	return ch;
}

void muart_write_data_apb(uint32_t base, volatile void* data, size_t size)
{
        while (size--) {
                muart_write_char(base, *(char*)data );
                (char*) data++;
        }
}

void muart_read_data_apb(uint32_t base, volatile void* data, size_t size)
{
        while (size--) {
        *(char*)data = muart_read_char(base);
                (char*)data++;
        }
}

// AXI
struct mdma_chan *muart_set_chan_mdma_W(uint32_t base, void *data, size_t len)
{
    struct mdma_chan *chan_wr;
    struct mdma_cfg cfg;
    int  ret = 0;
 //   uint32_t reg;

    //CONFIG DMA
    cfg.mode = MDMA_CHAN_POLLING;
    cfg.desc_kind = LONG_DESC;  //NORMAL_DESC;
    cfg.heap_id = 1;
    cfg.burst_width = MDMA_BURST_WIDTH4; // !!!!!
    cfg.event_indx = -1;
 //   cfg.add_info = false;  //true for custom !!!!!
    cfg.add_info = true;  //true for custom !!!!!
    cfg.addr_inc = true;

    chan_wr = mdma_chan_create((void *)(base + RX_CHANNEL_W), &cfg);
    if (!chan_wr) { //goto test_exit_2;
       ret = -2;       goto to_exit;
    }
    if (mdma_trans_create(chan_wr, MDMA_TRANS_QUEUE)) {
        ret = -4;        goto to_exit;
    }
        // Init and add description to write
    if (mdma_trans_add_group(chan_wr, data, len, 0, true)) {
          ret = -7;          goto to_exit;
    }
    if (mdma_chan_config(chan_wr)) {
        ret = -8;        goto to_exit;
    }

to_exit:
    if(ret < 0) {
            mdma_chan_destroy(chan_wr);
            chan_wr=NULL;
    }
    return chan_wr;
}

struct mdma_chan * muart_set_chan_mdma_R(uint32_t base, void *data, size_t len)
{
    struct mdma_chan *chan_rd;
    struct mdma_cfg cfg;
    int  ret = 0;
 //   uint32_t reg;

    //CONFIG DMA
    cfg.mode = MDMA_CHAN_POLLING;
    cfg.desc_kind = LONG_DESC;  //NORMAL_DESC;
    cfg.heap_id = 1;
    cfg.burst_width = MDMA_BURST_WIDTH4; // !!!!!
    cfg.event_indx = -1;
 //   cfg.add_info = false;  //true for custom !!!!!
    cfg.add_info = true;  //true for custom !!!!!
    cfg.addr_inc = true;

    chan_rd = mdma_chan_create((void *)(base + TX_CHANNEL_R), &cfg);
    if (!chan_rd) {
       ret = -1;
       goto to_exit;
    }
    if (mdma_trans_create(chan_rd, MDMA_TRANS_QUEUE)) {
        ret = -3;
        goto to_exit;
    }
        // Init and add description to read/write
    if (mdma_trans_add_group(chan_rd, data, len, 0, true)) {
          ret = -7;
          goto to_exit;
    }
    // Preparation to start. Setting reg’s
    if (mdma_chan_config(chan_rd) ) {
        ret = -8;
        goto to_exit;
    }

to_exit:
     if(ret < 0){
        mdma_chan_destroy(chan_rd);
        chan_rd=NULL;
     }
     return chan_rd;
}

void muart_finish_transmit_mdma(struct mdma_chan *chan_rd,
                               struct mdma_chan *chan_wr)
{
        mdma_chan_reset(chan_rd);
        mdma_chan_reset(chan_wr);

        mdma_chan_destroy(chan_wr);
        mdma_chan_destroy(chan_rd);
}

///////////////////////////////////////////////////////////////
int muart_transmit_data_throught_mdma(
        uint32_t src_base, uint32_t dst_base,
        void *src_addr, void *dst_addr, size_t len)
{

    struct mdma_chan *chan_rd;
    struct mdma_chan *chan_wr;
    struct mdma_cfg cfg;
    int i, state=0, ret = 0;
    uint32_t reg;

    //CONFIG DMA
    cfg.mode = MDMA_CHAN_POLLING;
    cfg.desc_kind = NORMAL_DESC;
    cfg.heap_id = 1;
    cfg.burst_width = MDMA_BURST_WIDTH4; // !!!!!
    cfg.event_indx = -1;
 //   cfg.add_info = false;  //true for custom !!!!!
    cfg.add_info = true;  //true for custom !!!!!
    cfg.addr_inc = true;

    chan_rd = mdma_chan_create((void *)(src_base + TX_CHANNEL_R), &cfg);
    if (!chan_rd) {
       ret = -1;
       goto test_exit_1;
    }
    chan_wr = mdma_chan_create((void *)(dst_base + RX_CHANNEL_W), &cfg);
    if (!chan_wr) {
       ret = -2;
       goto test_exit_2;
    }
    if (mdma_trans_create(chan_rd, MDMA_TRANS_QUEUE)) {
        ret = -3;
        goto test_exit_2;
    }
    if (mdma_trans_create(chan_wr, MDMA_TRANS_QUEUE)) {
        ret = -4;
        goto test_exit_2;
    }

        // Init and add description to read/write
    if (mdma_trans_add_group(chan_rd, src_addr, len, 0, true) ||
        mdma_trans_add_group(chan_wr, dst_addr, len, 0, true)) {
          ret = -7;
          goto test_exit_2;
    }
    // Preparation to start. Setting reg’s
    if (mdma_chan_config(chan_rd) || mdma_chan_config(chan_wr)) {
        ret = -8;
        goto test_exit_2;
    }
    rumboot_printf("write start====\n");
        if (mdma_chan_start(chan_wr)) {  // uart --> dma
            ret = -9;
            goto test_exit_5;
        }
    udelay(1);
    reg = ioread32(src_base + MUART_FIFO_STATE);
    rumboot_printf("read MUART_FIFO_STATE=%x \n", reg);
    reg = ioread32(src_base + MUART_STATUS);
    rumboot_printf("read MUART_STATUS = %x\n", reg);
     reg = ioread32(dst_base + MUART_FIFO_STATE);
     rumboot_printf("write MUART_FIFO_STATE=%x \n", reg);
     reg = ioread32(dst_base + MUART_STATUS);
     rumboot_printf("write MUART_STATUS = %x\n", reg);

    rumboot_printf("read start====\n");
        if (mdma_chan_start(chan_rd)) { //dma --> uart
            ret = -9;
            goto test_exit_5;
        }
    udelay(1);
    reg = ioread32(src_base + MUART_FIFO_STATE);
    rumboot_printf("read MUART_FIFO_STATE=%x \n", reg);
    reg = ioread32(src_base + MUART_STATUS);
    rumboot_printf("read MUART_STATUS = %x\n", reg);
     reg = ioread32(dst_base + MUART_FIFO_STATE);
     rumboot_printf("write MUART_FIFO_STATE=%x \n", reg);
     reg = ioread32(dst_base + MUART_STATUS);
     rumboot_printf("write MUART_STATUS = %x\n", reg);
    if (mdma_chan_wait_trans_end(chan_wr, 1000)) {
        ret = -10;
        goto test_exit_5;
    }
    state = mdma_chan_get_state(chan_wr);
    if (state != MDMA_CHAN_DUMMY) {
         ret = -11;
         goto test_exit_5;
    }

    state = mdma_chan_get_state(chan_rd);
    if (state != MDMA_CHAN_DUMMY) {
         ret = -12;
         goto test_exit_5;
    }
    for (i = 0; i < len; i++) {
       if (*((char *)(dst_addr + i)) != *((char *)(src_addr + i))) {
          ret = -13;
          break;
        }
    }
test_exit_5:
        mdma_chan_reset(chan_rd);
        mdma_chan_reset(chan_wr);

test_exit_2:
        mdma_chan_destroy(chan_wr);

test_exit_1:
        mdma_chan_destroy(chan_rd);

//        return state;
        return ret;
}


void muart_dump(uint32_t base)
{
    uint32_t reg;

    rumboot_printf("======= muart_dump ======   base: %x\n", base);
/*    reg = ioread32(base + MUART_ID);
    rumboot_printf("MUART_ID = %x\n", reg);
    reg = ioread32(base + MUART_VERSION);
    rumboot_printf("MUART_VERSION = %x\n", reg);
    reg = ioread32(base + MUART_SW_RST);
    rumboot_printf("MUART_SW_RST = %x\n", reg);
    reg = ioread32(base + MUART_GEN_STATUS);
    rumboot_printf("MUART_GEN_STATUS = %x\n", reg);
    reg = ioread32(base + MUART_FIFO_STATE);
    rumboot_printf("MUART_FIFO_STATE = %x\n", reg);
    reg = ioread32(base + MUART_STATUS);
    rumboot_printf("MUART_STATUS = %x\n", reg);
    reg = ioread32(base + MUART_DTRANS);
    rumboot_printf("MUART_DTRANS = %x\n", reg);
*/
//    reg = ioread32(base + MUART_DREC);
//    rumboot_printf("MUART_DREC = %x\n", reg);
    reg = ioread32(base + MUART_BDIV);
    rumboot_printf("MUART_BDIV = %x\n", reg);
    reg = ioread32(base + MUART_FIFOWM);
    rumboot_printf("MUART_FIFOWM = %x\n", reg);
    reg = ioread32(base + MUART_CTRL);
    rumboot_printf("MUART_CTRL = %x\n", reg);
 //   rumboot_printf("MEN LBE APB MDS RTS CTS RTSP PEN EPS SPS\n");
 //   rumboot_printf(" %x  %x  %x  %x  %x  %x  %x   %x  %x  %x \n",
 //                  reg)

    reg = ioread32(base + MUART_MASK);
    rumboot_printf("MUART_MASK = %x\n", reg);
    reg = ioread32(base + MUART_TIMEOUT);
    rumboot_printf("MUART_TIMEOUT = %x\n", reg);
    rumboot_printf("=======END muart_dump ====== base: %x \n", base);
}

void muart_soft_reset(uint32_t base)
{
//  rumboot_printf("===== RESET start \n");
  set_reg(base, MUART_SW_RST, 0x1);

  while (ioread32(base + MUART_SW_RST) & 1) {};
}
