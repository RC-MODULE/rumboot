#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <devices/mdio.h>
#include <devices/gpio.h>
#include <rumboot/io.h>
#include <rumboot/timer.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>
#include <platform/devices.h>
#include <platform/interrupts.h>

#define TIMEOUT_MDIO_ETH_PHY 12


static void handler_mdio(int irq, void *arg)
{
    volatile uint32_t *done = arg;
    rumboot_printf("IRQ from MDIO arrived, arg %x, count %d\n", arg, (*done));
    ioread32(MDIO0_BASE + MDIO_STATUS);
    ioread32(MDIO1_BASE + MDIO_STATUS);
    ioread32(MDIO2_BASE + MDIO_STATUS);
    ioread32(MDIO3_BASE + MDIO_STATUS);
    *done = *done + 1;  
}

bool mdio_phy_intrp_test(uint32_t num_mdio)
{
    //Switch MGPIO PADS to MDIO
    iowrite32(0x00000000, MGPIO0_BASE + GPIO_SWITCH_SOURCE);
    iowrite32(0x00000000, MGPIO1_BASE + GPIO_SWITCH_SOURCE);
    
    //Read status reg for MDIO_STATUS reset
    ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_STATUS);
    
    //non-active rst for PHY
    iowrite32(1 << ETH_RST_N, MDIO0_BASE + 0x1000*num_mdio + MDIO_ETH_RST_N);
    
    //mdc enable
    iowrite32(1 << MDC_EN, MDIO0_BASE + 0x1000*num_mdio + MDIO_EN);
    
       
    //Wait intrp from PHY
    uint32_t read_data=ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_STATUS);
    uint32_t uptime = rumboot_platform_get_uptime();
    rumboot_printf("uptime = %x\n", uptime);
    while (!(read_data & 1 << PHY_IRQ))
    {
        if (rumboot_platform_get_uptime() - uptime > TIMEOUT_MDIO_ETH_PHY) {
			rumboot_printf("Timeout! MDIO hasn't detected intrp from PHY! \n");
			return 0;
        }
        read_data=ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_STATUS);
    }
    rumboot_printf("MDIO has detected intrp from PHY!\n");
    
    return 1;
    
}


bool mdio_test(uint32_t num_mdio)
{
    volatile uint32_t done = 0;
    // Disable all interrupts
    rumboot_irq_cli(); 
    
    // Create an IRQ table
    struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);

    // Set handler
    int num_intrp = MDIO0_INT + num_mdio;
    rumboot_irq_set_handler(tbl, num_intrp, 0, handler_mdio, (void*)&done);
        
    // Activate the table
    rumboot_irq_table_activate(tbl);
    rumboot_irq_enable(num_intrp);
    
    // Allow interrupt handling
    rumboot_irq_sei();
    
        
    //Switch MGPIO PADS to MDIO
    iowrite32(0x00000000, MGPIO0_BASE + GPIO_SWITCH_SOURCE);
    iowrite32(0x00000000, MGPIO1_BASE + GPIO_SWITCH_SOURCE);
    
    //Read status reg for MDIO_STATUS reset
    ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_STATUS);
    
    //Read rst value from MDIO registers
    if (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_ID) != MDIO_ID_RESET){
        rumboot_printf("MDIO_ID reset value is wrong!\n");
        return 0;
    }
    if (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_VER) != MDIO_VER_RESET){
        rumboot_printf("MDIO_VER reset value is wrong!\n");
        return 0;
    }
    if (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_STATUS) != MDIO_STATUS_RESET){
        rumboot_printf("MDIO_STATUS reset value is wrong!\n");
        return 0;
    }
    if (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_IRQ_MASK) != MDIO_IRQ_MASK_RESET){
        rumboot_printf("MDIO_IRQ_MASK reset value is wrong!\n");
        return 0;
    }
    if (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_PHY_IRQ_STATE) != MDIO_PHY_IRQ_STATE_RESET){
        rumboot_printf("MDIO_PHY_IRQ_STATE reset value is wrong!\n");
        return 0;
    }
    if (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_CONTROL) != MDIO_CONTROL_RESET){
        rumboot_printf("MDIO_PHY_IRQ_STATE reset value is wrong!\n");
        return 0;
    }
    if (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_ETH_RST_N) != MDIO_ETH_RST_N_RESET){
        rumboot_printf("MDIO_ETH_RST_N reset value is wrong!\n");
        return 0;
    }
    if (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_FREQ_DIVIDER) != MDIO_FREQ_DIVIDER_RESET){
        rumboot_printf("MDIO_FREQ_DIVIDER reset value is wrong!\n");
        return 0;
    }
    if (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_EN) != MDIO_EN_RESET){
    //if (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_EN) != 0x12345678){
        rumboot_printf("MDIO_EN reset value is wrong!\n");
        return 0;
    }
    //End of rst value reading from MDIO registers
    
    //non-active rst for PHY
    iowrite32(1 << ETH_RST_N, MDIO0_BASE + 0x1000*num_mdio + MDIO_ETH_RST_N);
    
    //mdc enable
    iowrite32(1 << MDC_EN, MDIO0_BASE + 0x1000*num_mdio + MDIO_EN);
    
    //irq_mask
    iowrite32(0 << WR_IRQ | 1 << RD_IRQ | 0 << PHY_IRQ, MDIO0_BASE + 0x1000*num_mdio + MDIO_IRQ_MASK);
    
    //Read from PHY
    iowrite32(2 << ADDR_REG | 7 << ADDR_PHY | 1 << START_RD, MDIO0_BASE + 0x1000*num_mdio + MDIO_CONTROL);
    
    bool rd_wr_rslt = (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_CONTROL)&(1 << BUSY));
    while (rd_wr_rslt){
        rd_wr_rslt = (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_CONTROL)&(1 << BUSY));
    }
    rumboot_printf("Read operation is finished!\n");
    int rd_addr = (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_CONTROL) >>  ADDR_REG)&0x0000001F;
    int rd_data = (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_CONTROL) >> CTRL_DATA)&0x0000FFFF;
    rumboot_printf ("Read from ADDR_REG = %x, Data = %x \n", rd_addr, rd_data);
    if (rd_data != ETH_PHY_ID)
    {
        rumboot_printf("Not Valid PHY ID!\n");
        return 0;
    }
    //End of reading from PHY
    
    
    
    
    // Deinit
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
    
    rumboot_printf("done = %d\n", done);    
    
    if (done==1)
        return 1;
    else
        return 0;
    

}


