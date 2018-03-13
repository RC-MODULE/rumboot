#include <stdint.h>
#include <stdbool.h>

#include <devices/sctl.h>
#include <devices/mdio.h>
#include <devices/gpio.h>

#include <rumboot/io.h>
#include <rumboot/timer.h>
#include <rumboot/printf.h>

#include <platform/devices.h>




bool mdio_test(uint32_t num_mdio)
{
    //Switch MGPIO PADS to MDIO
    iowrite32(0x00000000, MGPIO0_BASE + GPIO_SWITCH_SOURCE);
    iowrite32(0x00000000, MGPIO1_BASE + GPIO_SWITCH_SOURCE);
    
    //Read status reg for MDIO_STATUS reset
    ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_STATUS);
    
    //Read rst value from MDIO registers
    if (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_ID) != MDIO_ID_RESET){
        rumboot_printf("MDIO_ID reset value is wrong!\n");
        return true;
    }
    if (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_VER) != MDIO_VER_RESET){
        rumboot_printf("MDIO_VER reset value is wrong!\n");
        return true;
    }
    if (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_STATUS) != MDIO_STATUS_RESET){
        rumboot_printf("MDIO_STATUS reset value is wrong!\n");
        return true;
    }
    if (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_IRQ_MASK) != MDIO_IRQ_MASK_RESET){
        rumboot_printf("MDIO_IRQ_MASK reset value is wrong!\n");
        return true;
    }
    if (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_PHY_IRQ_STATE) != MDIO_PHY_IRQ_STATE_RESET){
        rumboot_printf("MDIO_PHY_IRQ_STATE reset value is wrong!\n");
        return true;
    }
    if (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_CONTROL) != MDIO_CONTROL_RESET){
        rumboot_printf("MDIO_PHY_IRQ_STATE reset value is wrong!\n");
        return true;
    }
    if (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_ETH_RST_N) != MDIO_ETH_RST_N_RESET){
        rumboot_printf("MDIO_ETH_RST_N reset value is wrong!\n");
        return true;
    }
    if (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_FREQ_DIVIDER) != MDIO_FREQ_DIVIDER_RESET){
        rumboot_printf("MDIO_FREQ_DIVIDER reset value is wrong!\n");
        return true;
    }
    if (ioread32(MDIO0_BASE + 0x1000*num_mdio + MDIO_EN) != MDIO_EN_RESET){
        rumboot_printf("MDIO_EN reset value is wrong!\n");
        return true;
    }
    //End of rst value reading from MDIO registers
    
    //non-active rst for PHY
    iowrite32(0x1, MDIO0_BASE + 0x1000*num_mdio + MDIO_ETH_RST_N);
    
    //mdc enable
    iowrite32(0x1, MDIO0_BASE + 0x1000*num_mdio + MDIO_EN);
    
    //irq_mask
    iowrite32(0x7, MDIO0_BASE + 0x1000*num_mdio + MDIO_IRQ_MASK);
    
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
        rumboot_printf("Not Valid PHY ID\n");
        return true;
    }
    
    
    //    
    

    
    return false;
    
}


