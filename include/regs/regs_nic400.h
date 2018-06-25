#ifndef DEVICES_NIC400_H
#define DEVICES_NIC400_H

/**
 * \defgroup devices_nic400_regs Registers
 * \ingroup devices_nic400
 *
 * \addtogroup devices_nic400_regs
 *
 * @{
 */


#define NIC400_PIDR4                0x1FD0
#define NIC400_PIDR5                0x1FD4
#define NIC400_PIDR6                0x1FD8
#define NIC400_PIDR7                0x1FDC
#define NIC400_PIDR0                0x1FE0
#define NIC400_PIDR1                0x1FE4
#define NIC400_PIDR2                0x1FE8
#define NIC400_PIDR3                0x1FEC
#define NIC400_CIDR0                0x1FF0
#define NIC400_CIDR1                0x1FF4
#define NIC400_CIDR2                0x1FF8
#define NIC400_CIDR3                0x1FFC


#define RQOS_OFFSET                 0x00000
#define WQOS_OFFSET                 0x00004
#define FN_MOD_OFFSET               0x00008


#define NIC400_ARM_S                0x42100
#define NIC400_PCIE_S               0x43100
#define NIC400_DMA0_S               0x44100
#define NIC400_DMA1_S               0x45100
#define NIC400_DMA2_S               0x46100
#define NIC400_DMA3_S               0x47100
#define NIC400_ETH0_S               0x48100
#define NIC400_ETH1_S               0x49100
#define NIC400_ETH2_S               0x4A100
#define NIC400_ETH3_S               0x4B100
#define NIC400_ETR_S                0x4C100



#define NIC400_ARM_S_RQOS           NIC400_ARM_S+RQOS_OFFSET
#define NIC400_ARM_S_WQOS           NIC400_ARM_S+WQOS_OFFSET
#define NIC400_ARM_S_FN_MOD         NIC400_ARM_S+FN_MOD_OFFSET

#define NIC400_PCIE_S_RQOS          NIC400_PCIE_S+RQOS_OFFSET
#define NIC400_PCIE_S_WQOS          NIC400_PCIE_S+WQOS_OFFSET
#define NIC400_PCIE_S_FN_MOD        NIC400_PCIE_S+FN_MOD_OFFSET
                                           
#define NIC400_DMA0_S_RQOS          NIC400_DMA0_S+RQOS_OFFSET
#define NIC400_DMA0_S_WQOS          NIC400_DMA0_S+WQOS_OFFSET
#define NIC400_DMA0_S_FN_MOD        NIC400_DMA0_S+FN_MOD_OFFSET
                                           
#define NIC400_DMA1_S_RQOS          NIC400_DMA1_S+RQOS_OFFSET
#define NIC400_DMA1_S_WQOS          NIC400_DMA1_S+WQOS_OFFSET
#define NIC400_DMA1_S_FN_MOD        NIC400_DMA1_S+FN_MOD_OFFSET
                                           
#define NIC400_DMA2_S_RQOS          NIC400_DMA2_S+RQOS_OFFSET
#define NIC400_DMA2_S_WQOS          NIC400_DMA2_S+WQOS_OFFSET
#define NIC400_DMA2_S_FN_MOD        NIC400_DMA2_S+FN_MOD_OFFSET
                                           
#define NIC400_DMA3_S_RQOS          NIC400_DMA3_S+RQOS_OFFSET
#define NIC400_DMA3_S_WQOS          NIC400_DMA3_S+WQOS_OFFSET
#define NIC400_DMA3_S_FN_MOD        NIC400_DMA3_S+FN_MOD_OFFSET
                                           
#define NIC400_ETH0_S_RQOS          NIC400_ETH0_S+RQOS_OFFSET
#define NIC400_ETH0_S_WQOS          NIC400_ETH0_S+WQOS_OFFSET
#define NIC400_ETH0_S_FN_MOD        NIC400_ETH0_S+FN_MOD_OFFSET
                                           
#define NIC400_ETH1_S_RQOS          NIC400_ETH1_S+RQOS_OFFSET
#define NIC400_ETH1_S_WQOS          NIC400_ETH1_S+WQOS_OFFSET
#define NIC400_ETH1_S_FN_MOD        NIC400_ETH1_S+FN_MOD_OFFSET
                                           
#define NIC400_ETH2_S_RQOS          NIC400_ETH2_S+RQOS_OFFSET
#define NIC400_ETH2_S_WQOS          NIC400_ETH2_S+WQOS_OFFSET
#define NIC400_ETH2_S_FN_MOD        NIC400_ETH2_S+FN_MOD_OFFSET
                                           
#define NIC400_ETH3_S_RQOS          NIC400_ETH3_S+RQOS_OFFSET
#define NIC400_ETH3_S_WQOS          NIC400_ETH3_S+WQOS_OFFSET
#define NIC400_ETH3_S_FN_MOD        NIC400_ETH3_S+FN_MOD_OFFSET

#define NIC400_ETR_S_RQOS           NIC400_ETR_S+RQOS_OFFSET
#define NIC400_ETR_S_WQOS           NIC400_ETR_S+WQOS_OFFSET
#define NIC400_ETR_S_FN_MOD         NIC400_ETR_S+FN_MOD_OFFSET



/**
* @}
*/

#endif /* end of include guard: DEVICES_NIC400_H */