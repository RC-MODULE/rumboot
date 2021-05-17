#include <devices/ugly/greth.h>
#include <regs/regs_greth.h>

void greth_set_mac(uint32_t base_addr, greth_mac_t* mac)
{
#ifdef ETH_DEBUG
    rumboot_printf("Setting MAC: 0x%x%x\n", mac->mac_msb, mac->mac_lsb);
#endif
    iowrite32((uint32_t) mac->mac_msb, base_addr + MAC_MSB);
    iowrite32(           mac->mac_lsb, base_addr + MAC_LSB);
}

void greth_set_edcl_mac(uint32_t base_addr, greth_mac_t* edcl_mac)
{
    iowrite32((uint32_t) edcl_mac->mac_msb, base_addr + EDCL_MAC_MSB);
    iowrite32(           edcl_mac->mac_lsb, base_addr + EDCL_MAC_LSB);
}

void greth_set_tx_descriptor(uint32_t const base_addr, greth_descr_t volatile * const tx_descr)
{
#ifdef ETH_DEBUG
    rumboot_printf("TRANSMIT_DESCR_PTR: 0x%x\n", (uint32_t)tx_descr);
    rumboot_printf("MEM[0x%x]: 0x%x\n", (uint32_t)tx_descr, ioread32((uint32_t) tx_descr));
    rumboot_printf("MEM[0x%x]: 0x%x\n", (uint32_t)tx_descr+4, ioread32((uint32_t) tx_descr+4));
#endif
    iowrite32(rumboot_virt_to_dma(tx_descr), base_addr + TRANSMIT_DESCR_PTR);
}

void greth_set_rx_descriptor(uint32_t const base_addr, greth_descr_t volatile * const rx_descr)
{
#ifdef ETH_DEBUG
    rumboot_printf("RECEIVER_DESCR_PTR: 0x%x\n", (uint32_t)rx_descr);
    rumboot_printf("MEM[0x%x]: 0x%x\n", (uint32_t)rx_descr,   ioread32((uint32_t) rx_descr));
    rumboot_printf("MEM[0x%x]: 0x%x\n", (uint32_t)rx_descr+4, ioread32((uint32_t) rx_descr+4));
#endif
    iowrite32(rumboot_virt_to_dma(rx_descr), base_addr + RECEIVER_DESCR_PTR);
}

bool greth_reset(uint32_t base_addr)
{
    uint32_t buf;
    uint32_t t = 0;

    buf = ioread32(base_addr);
    SET_BIT(buf, GRETH_CTRL_RS);
    iowrite32(buf, base_addr + CTRL);

    while((ioread32(base_addr + CTRL) & (1 << GRETH_CTRL_RS)) && (t++ < GRETH_TIMEOUT)){}

    if (t==GRETH_TIMEOUT)
        return false;
    else
        return true;
}

static void new_greth_ctrl_struct(greth_ctrl_struct_t* ctrl_struct)
{
    ctrl_struct->mdio_int_enable            = false;
    ctrl_struct->edcl_disable               = false;
    ctrl_struct->ram_debug_enable           = false;
    ctrl_struct->disable_duplex_detection   = false;
    ctrl_struct->multicast_enable           = false;
    ctrl_struct->phy_int_enable             = false;
    ctrl_struct->speed                      = GRETH_SPEED_100MB;
    ctrl_struct->promiscuous_mode           = false;
    ctrl_struct->fullduplex_enable          = false;
    ctrl_struct->receiver_int_enable        = false;
    ctrl_struct->transmitter_int_enable     = false;
    ctrl_struct->receiver_enable            = false;
    ctrl_struct->transmitter_enable         = false;
}

static bool set_greth_ctrl(uint32_t base_addr, greth_ctrl_struct_t* ctrl_struct)
{
    uint32_t tmp;

    tmp = ioread32(base_addr + CTRL);
    BITS_CONTROL_BY_FEATURE(tmp, ctrl_struct->mdio_int_enable,          (1 << GRETH_CTRL_ME));
    BITS_CONTROL_BY_FEATURE(tmp, ctrl_struct->edcl_disable,             (1 << GRETH_CTRL_ED));
    BITS_CONTROL_BY_FEATURE(tmp, ctrl_struct->ram_debug_enable,         (1 << GRETH_CTRL_RD));
    BITS_CONTROL_BY_FEATURE(tmp, ctrl_struct->disable_duplex_detection, (1 << GRETH_CTRL_DD));
    BITS_CONTROL_BY_FEATURE(tmp, ctrl_struct->multicast_enable,         (1 << GRETH_CTRL_MCE));
    BITS_CONTROL_BY_FEATURE(tmp, ctrl_struct->phy_int_enable,           (1 << GRETH_CTRL_PI));
    BITS_CONTROL_BY_FEATURE(tmp, ctrl_struct->fullduplex_enable,        (1 << GRETH_CTRL_FD));
    BITS_CONTROL_BY_FEATURE(tmp, ctrl_struct->promiscuous_mode,         (1 << GRETH_CTRL_PM));
    BITS_CONTROL_BY_FEATURE(tmp, ctrl_struct->receiver_int_enable,      (1 << GRETH_CTRL_RI));
    BITS_CONTROL_BY_FEATURE(tmp, ctrl_struct->transmitter_int_enable,   (1 << GRETH_CTRL_TI));
    BITS_CONTROL_BY_FEATURE(tmp, ctrl_struct->receiver_enable,          (1 << GRETH_CTRL_RE));
    BITS_CONTROL_BY_FEATURE(tmp, ctrl_struct->transmitter_enable,       (1 << GRETH_CTRL_TE));

    switch (ctrl_struct->speed)
    {
        case GRETH_SPEED_100MB    :
        {
            SET_BIT(tmp, GRETH_CTRL_SP);
            break;
        }
        case GRETH_SPEED_10MB    :
        {
            CLEAR_BITS_BY_MASK(tmp, (1 << GRETH_CTRL_SP));
            break;
        }
        default :
        {
            SET_BIT(tmp, GRETH_CTRL_SP);
            break;
        }
    }

#ifdef ETH_DEBUG
    rumboot_printf("Write to CTRL 0x%x\n", tmp);
#endif
    iowrite32(tmp, base_addr + CTRL);

    return true;
}

void greth_clear_status_bits(uint32_t base_addr, uint32_t mask)
{
    iowrite32(mask, base_addr + STATUS);
}

uint32_t greth_get_status(uint32_t base_addr)
{
    return ioread32(base_addr + STATUS);
}

bool greth_wait_transmit(uint32_t base_addr)
{
    uint32_t t = 0;
    uint32_t cur_status;
    uint32_t mask;

    rumboot_printf("Waiting transmit..\n");

    mask = ((1 << GRETH_STATUS_IA) |
            (1 << GRETH_STATUS_TS) |
            (1 << GRETH_STATUS_TA) |
            (1 << GRETH_STATUS_RA) |
            (1 << GRETH_STATUS_TI) |
            (1 << GRETH_STATUS_TE) |
            (1 << GRETH_STATUS_RE));

    cur_status = ioread32(base_addr + STATUS);
    while(!(cur_status & mask) && (t++ < GRETH_TIMEOUT))
    {
        cur_status = ioread32(base_addr + STATUS);
    }
    if ((t==GRETH_TIMEOUT) || !(cur_status & (1 << GRETH_STATUS_TI)))
    {
        rumboot_printf("Transmit is timed out\n");
        greth_clear_status_bits(base_addr, mask);
        return false;
    }
    else
    {
        rumboot_printf("Transmit is OK\n");
        greth_clear_status_bits(base_addr, (1 << GRETH_STATUS_TI));
        return true;
    }
}

bool greth_wait_receive(uint32_t base_addr)
{
    uint32_t t = 0;
    uint32_t cur_status;
    uint32_t mask;
    cur_status = ioread32(base_addr + STATUS);

//    rumboot_printf("Waiting receive by GRETH%d..\n", base_addr==GRETH_0_BASE? 0 : 1);
    mask = ((1 << GRETH_STATUS_IA) |
            (1 << GRETH_STATUS_TS) |
            (1 << GRETH_STATUS_TA) |
            (1 << GRETH_STATUS_RA) |
            (1 << GRETH_STATUS_RI) |
            (1 << GRETH_STATUS_TE) |
            (1 << GRETH_STATUS_RE));

    while(!(cur_status & mask) && (t++ < GRETH_TIMEOUT*100))
    {
        cur_status = ioread32(base_addr + STATUS);
    }
    if ((t==GRETH_TIMEOUT) || !(cur_status & (1 << GRETH_STATUS_RI)))
    {
        rumboot_printf("Receive is timed out\n");
        greth_clear_status_bits(base_addr, mask);
        return false;
    }
    else
    {
        rumboot_printf("Receive is OK\n");
        greth_clear_status_bits(base_addr, (1 << GRETH_STATUS_RI) );
        return true;
    }
}

static greth_descr_t volatile * tx_descriptor_data;
static bool is_tx_descriptor_data_set __attribute__((section(".data"))) = false;
static greth_descr_t volatile * rx_descriptor_data;
static bool is_rx_descriptor_data_set __attribute__((section(".data"))) = false;

/* GRETH DMA copy functions*/

static void prepare_tx_descriptors(uint32_t src_addr, uint32_t length, uint32_t const base_addr)
{
    uint32_t i = 0;

    while(length > GRETH_MAX_DESC_DATA_LENGTH)
    {
#ifdef ETH_DEBUG
        rumboot_printf("tx_descr[%d]: 0x%x / 0x%x\n", i, tx_descriptor_data[i].options, tx_descriptor_data[i].start_addr);
#endif
        tx_descriptor_data[i].options = (1 << GRETH_TX_DESCR_EN) | GRETH_MAX_DESC_DATA_LENGTH;
        tx_descriptor_data[i].start_addr = src_addr;
        src_addr += GRETH_MAX_DESC_DATA_LENGTH;
        length -= GRETH_MAX_DESC_DATA_LENGTH;
        i++;
    }
    //wrap descriptor. enable interrupt
    tx_descriptor_data[i].options = (1 << GRETH_TX_DESCR_WR)| (1 << GRETH_TX_DESCR_EN) | (1 << GRETH_TX_DESCR_IE) | length;
    tx_descriptor_data[i].start_addr = src_addr;
#ifdef ETH_DEBUG
    rumboot_printf("tx_descr[%d]: 0x%x / 0x%x\n", i, tx_descriptor_data[i].options, tx_descriptor_data[i].start_addr);
#endif
    greth_set_tx_descriptor(base_addr, tx_descriptor_data);
}

static void prepare_rx_descriptors(uint32_t src_addr, uint32_t length, uint32_t const base_addr)
{
    uint32_t i = 0;

    while(length > GRETH_MAX_DESC_DATA_LENGTH)
    {
#ifdef ETH_DEBUG
        rumboot_printf("rx_descr[%d]: 0x%x / 0x%x\n", i, rx_descriptor_data[i].options, rx_descriptor_data[i].start_addr);
#endif
        rx_descriptor_data[i].options = (1 << GRETH_RX_DESCR_EN) | GRETH_MAX_DESC_DATA_LENGTH;
        rx_descriptor_data[i].start_addr = (src_addr);
        src_addr += GRETH_MAX_DESC_DATA_LENGTH;
        length -= GRETH_MAX_DESC_DATA_LENGTH;
        i++;
    }
    //wrap descriptor. enable interrupt
    rx_descriptor_data[i].options = (1 << GRETH_RX_DESCR_WR) | (1 << GRETH_RX_DESCR_EN) | (1 << GRETH_RX_DESCR_IE) | length;
    rx_descriptor_data[i].start_addr = (src_addr);
#ifdef ETH_DEBUG
    rumboot_printf("rx_descr[%d]: 0x%x / 0x%x\n", i, rx_descriptor_data[i].options, rx_descriptor_data[i].start_addr);
#endif
    greth_set_rx_descriptor(base_addr, rx_descriptor_data);
}

bool greth_configure_for_receive( uint32_t base_addr, void volatile * const dst, uint32_t length, greth_descr_t* rx_descriptor_data_, greth_mac_t* gr_mac)
{

    greth_mac_t greth_mac;
//    uint32_t greth_idx = base_addr==GRETH_0_BASE ? 0 : 1;

//    rumboot_printf("Start receiving %d bytes by GRETH%d(0x%x) to 0x%x\n", length, greth_idx, base_addr, (uint32_t)dst);

    //clear status register by writing '1's (TA,RA,TI,RI,TE,RE)
    iowrite32( (1 << GRETH_STATUS_RE) |
               (1 << GRETH_STATUS_TE) |
               (1 << GRETH_STATUS_RI) |
               (1 << GRETH_STATUS_TI) |
               (1 << GRETH_STATUS_RA) |
               (1 << GRETH_STATUS_TA), base_addr + STATUS);

    if( !is_rx_descriptor_data_set ) {
        rx_descriptor_data = rx_descriptor_data_;
    }
    greth_mac.mac_msb = gr_mac->mac_msb;
    greth_mac.mac_lsb = gr_mac->mac_lsb;
    greth_set_mac(base_addr, &greth_mac);

    prepare_rx_descriptors((uint32_t)dst, length, base_addr);

    return true;
}

void greth_start_receive(uint32_t base_addr, bool rcv_int_ena)
{
    greth_ctrl_struct_t greth_info;
    rumboot_printf("Start receive data (0x%X)\n", base_addr);

    new_greth_ctrl_struct(&greth_info);
    greth_info.promiscuous_mode       = true;
    greth_info.edcl_disable           = false;
    greth_info.multicast_enable       = false;
    greth_info.speed                  = GRETH_SPEED_100MB;
    greth_info.fullduplex_enable      = false;
    greth_info.transmitter_int_enable = false;
    greth_info.receiver_int_enable    = rcv_int_ena;
    greth_info.transmitter_enable     = false;
    greth_info.receiver_enable        = true;
    greth_info.ram_debug_enable       = false;
    set_greth_ctrl(base_addr, &greth_info);
}

void greth_start_receive_transmit(uint32_t base_addr, bool rcv_int_ena)
{
    greth_ctrl_struct_t greth_info;
    rumboot_printf("Start receive data (0x%X)\n", base_addr);

    new_greth_ctrl_struct(&greth_info);
    greth_info.promiscuous_mode       = true;
    greth_info.edcl_disable           = false;
    greth_info.multicast_enable       = false;
    greth_info.speed                  = GRETH_SPEED_100MB;
    greth_info.fullduplex_enable      = true;
    greth_info.transmitter_int_enable = false;
    greth_info.receiver_int_enable    = rcv_int_ena;
    greth_info.transmitter_enable     = true;
    greth_info.receiver_enable        = true;
    greth_info.ram_debug_enable       = false;
    set_greth_ctrl(base_addr, &greth_info);
}


bool greth_wait_receive_irq(uint32_t base_addr, uint32_t volatile* eth_irq_handled_flag)
{
    uint32_t t = 0;
    uint32_t irq_handled = *eth_irq_handled_flag;
    rumboot_printf("Waiting receive IRQ\n");
    while(((irq_handled)==0) && (t++ < GRETH_TIMEOUT)){
#ifdef ETH_DEBUG
        rumboot_printf("%d: (*eth_irq_handled_flag) = 0x%X\n", t, *eth_irq_handled_flag);
#endif
        irq_handled = *eth_irq_handled_flag;
    }

    if ((t==GRETH_TIMEOUT) || ((*eth_irq_handled_flag)!=1))
    {
        rumboot_printf("RI IRQ is timed out\n");
        *eth_irq_handled_flag = 0;
        return false;
    }
    else
    {
        *eth_irq_handled_flag = 0;
        rumboot_printf("RI IRQ is OK\n");
        return true;
    }
}

bool greth_wait_receive_err_irq(uint32_t base_addr, uint32_t volatile* eth_irq_handled_flag)
{
    uint32_t t = 0;
    uint32_t irq_handled = *eth_irq_handled_flag;
    rumboot_printf("Waiting RE (receive error) IRQ\n");
    while(((irq_handled)==0) && (t++ < GRETH_TIMEOUT)){
#ifdef ETH_DEBUG
        rumboot_printf("%d: (*eth_irq_handled_flag) = 0x%X\n", t, *eth_irq_handled_flag);
#endif
        irq_handled = *eth_irq_handled_flag;
    }

    if ((t==GRETH_TIMEOUT) || ((*eth_irq_handled_flag)!=3))
    {
        rumboot_printf("RE IRQ is timed out\n");
        *eth_irq_handled_flag = 0;
        return false;
    }
    else
    {
        *eth_irq_handled_flag = 0;
        rumboot_printf("RE IRQ is OK\n");
        return true;
    }
}


bool greth_configure_for_transmit( uint32_t base_addr, void volatile * const src, uint32_t length, greth_descr_t* tx_descriptor_data_, greth_mac_t* gr_mac)
{
    greth_mac_t greth_mac;
//    uint32_t greth_idx = base_addr==GRETH_0_BASE ? 0 : 1;

//    rumboot_printf("Sending %d bytes by GRETH%d(0x%x) from 0x%x \n", length, greth_idx, base_addr, (uint32_t)src);

    //clear status register by writing '1's (TA,RA,TI,RI,TE,RE)
    iowrite32( (1 << GRETH_STATUS_RE) |
               (1 << GRETH_STATUS_TE) |
               (1 << GRETH_STATUS_RI) |
               (1 << GRETH_STATUS_TI) |
               (1 << GRETH_STATUS_RA) |
               (1 << GRETH_STATUS_TA), base_addr + STATUS);

    if( !is_tx_descriptor_data_set ) {
        tx_descriptor_data = tx_descriptor_data_;
    }

    greth_mac.mac_msb = gr_mac->mac_msb;
    greth_mac.mac_lsb = gr_mac->mac_lsb;
    greth_set_mac(base_addr,&greth_mac);

    prepare_tx_descriptors((uint32_t)src, length, base_addr);

    return true;
}

void greth_start_transmit(uint32_t base_addr)
{
    greth_ctrl_struct_t greth_info;
    rumboot_printf("Start transmit data (0x%X)\n", base_addr);
    new_greth_ctrl_struct(&greth_info);
    greth_info.promiscuous_mode       = true;
    greth_info.edcl_disable           = false;
    greth_info.multicast_enable       = false;
    greth_info.speed                  = GRETH_SPEED_100MB;
    greth_info.fullduplex_enable      = false;
    greth_info.transmitter_int_enable = false;
    greth_info.receiver_int_enable    = false;
    greth_info.transmitter_enable     = true;
    greth_info.receiver_enable        = false;
    greth_info.ram_debug_enable       = false;
    set_greth_ctrl(base_addr, &greth_info);
}

void greth_start_edcl_transfer(uint32_t base_addr)
{
    greth_ctrl_struct_t greth_info;
    rumboot_printf("Start EDCL transaction (0x%X)\n", base_addr);

    new_greth_ctrl_struct(&greth_info);
    greth_info.promiscuous_mode       = true;
    greth_info.edcl_disable           = false;
    greth_info.multicast_enable       = false;
    greth_info.speed                  = GRETH_SPEED_100MB;
    greth_info.fullduplex_enable      = true;
    greth_info.transmitter_int_enable = false;
    greth_info.receiver_int_enable    = true;
    greth_info.transmitter_enable     = true;
    greth_info.receiver_enable        = true;
    greth_info.ram_debug_enable       = false;
    set_greth_ctrl(base_addr, &greth_info);
}

static bool wait_mdio_not_busy_and_no_link_fail(uint32_t base_addr)
{
    uint32_t buf;
    uint32_t timeout = 0;

    //wait clearing MDIO_CTRL[BU] (busy=0, MDIO transaction finished)
    buf = ioread32(base_addr + MDIO_CTRL);
    while ((buf & (1 << GRETH_MDIO_BU)) && (timeout++ < GRETH_TIMEOUT))
    {
        buf = ioread32(base_addr + MDIO_CTRL);
    }
    if (timeout==GRETH_TIMEOUT)
    {
        rumboot_printf("Failed to waiting MDIO_CTRL[BU]=0");
        return false;
    }

    //check link fail
    if (buf & (1 << GRETH_MDIO_LF))
    {
        rumboot_printf("Link fail detected at MDIO transaction");
        return false;
    }

    return true;
}

static bool wait_mdio_started(uint32_t base_addr)
{
    uint32_t buf;
    uint32_t timeout = 0;

    //wait asserting MDIO_CTRL[BU] (busy=1, MDIO transaction started)
    buf = ioread32(base_addr + MDIO_CTRL);
    while (( !(buf & (1 << GRETH_MDIO_BU))) && (timeout++ < GRETH_TIMEOUT))
    {
        buf = ioread32(base_addr + MDIO_CTRL);
    }
    if (timeout==GRETH_TIMEOUT)
    {
        rumboot_printf("Failed to waiting MDIO_CTRL[BU]=1");
        return false;
    }

    return true;
}

static void run_mdio_write(uint32_t base_addr, uint8_t phy_addr, uint8_t reg_addr, uint16_t data)
{
    uint32_t buf;
    //read MDIO control/status and clear data and addr bits;
    buf = ioread32(base_addr + MDIO_CTRL);
    CLEAR_BITS_BY_MASK(buf, GRETH_MDIO_DATA_MASK | GRETH_MDIO_PHYADDR_MASK | GRETH_MDIO_REGADDR_MASK);
    buf = ( (uint32_t)data     << GRETH_MDIO_DATA    ) |
          ( (uint32_t)phy_addr << GRETH_MDIO_PHYADDR ) |
          ( (uint32_t)reg_addr << GRETH_MDIO_REGADDR ) |
          (                  1 << GRETH_MDIO_WR      );
    iowrite32(buf, base_addr + MDIO_CTRL);
}

static void run_mdio_read(uint32_t base_addr, uint8_t phy_addr, uint8_t reg_addr)
{
    uint32_t buf;
    //read MDIO control/status and clear data and addr bits;
    buf = ioread32(base_addr + MDIO_CTRL);
    CLEAR_BITS_BY_MASK(buf, GRETH_MDIO_DATA_MASK | GRETH_MDIO_PHYADDR_MASK | GRETH_MDIO_REGADDR_MASK);
    buf = ( (uint32_t)phy_addr << GRETH_MDIO_PHYADDR ) |
          ( (uint32_t)reg_addr << GRETH_MDIO_REGADDR ) |
          (                  1 << GRETH_MDIO_RD      );
    iowrite32(buf, base_addr + MDIO_CTRL);
}

bool greth_mdio_write( uint32_t base_addr, uint8_t phy_addr, uint8_t reg_addr, uint16_t data)
{
    if ( !wait_mdio_not_busy_and_no_link_fail(base_addr) ) return false;
    run_mdio_write(base_addr, phy_addr, reg_addr, data);
    if (!wait_mdio_started(base_addr))                   return false;
    if (!wait_mdio_not_busy_and_no_link_fail(base_addr)) return false;
    return true;
}

uint16_t greth_mdio_read( uint32_t base_addr, uint8_t phy_addr, uint8_t reg_addr)
{
    if ( !wait_mdio_not_busy_and_no_link_fail(base_addr) ) return false;
    run_mdio_read(base_addr, phy_addr, reg_addr);
    if (!wait_mdio_started(base_addr))                   return false;
    if (!wait_mdio_not_busy_and_no_link_fail(base_addr)) return false;
    return ( (ioread32(base_addr + MDIO_CTRL) & GRETH_MDIO_DATA_MASK) >> GRETH_MDIO_DATA );
}

