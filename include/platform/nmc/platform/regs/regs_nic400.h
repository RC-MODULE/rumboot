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

#define NIC400_PERIPHERAL_ID4                0x1FD0
#define NIC400_PERIPHERAL_ID5                0x1FD4
#define NIC400_PERIPHERAL_ID6                0x1FD8
#define NIC400_PERIPHERAL_ID7                0x1FDC
#define NIC400_PERIPHERAL_ID0                0x1FE0
#define NIC400_PERIPHERAL_ID1                0x1FE4
#define NIC400_PERIPHERAL_ID2                0x1FE8
#define NIC400_PERIPHERAL_ID3                0x1FEC

#define RQOS_OFFSET                          0x00000
#define WQOS_OFFSET                          0x00004
#define FN_MOD_OFFSET                        0x00008



/////AXI32 regs name
//#define NIC400_CPU_S                         0x42100
//#define NIC400_GSPI_1_S                      0x43100
//#define NIC400_ETH_1_S                       0x44100
//#define NIC400_SW_0_3_S                      0x45100
//#define NIC400_GSPI_0_S                      0x46100
//#define NIC400_ETH_0_S                       0x47100
//#define NIC400_MKIO_0_S                      0x48100
//#define NIC400_MKIO_1_S                      0x49100
/////AXI64 regs name
//#define NIC400_SW_0_S                        0x42100
//#define NIC400_SW_1_S                        0x43100
//#define NIC400_SW_2_1_S                      0x44100
//#define NIC400_SW_3_S                        0x45100





///AXI32 left regs name
#define NIC400_AXI64_TO_AXI32_LEFT_S         0x42100
#define NIC400_GSPI_0_S                      0x46100
#define NIC400_ETH_0_S                       0x47100
#define NIC400_MKIO_0_S                      0x48100
#define NIC400_MKIO_2_S                      0x49100

///AXI32 right regs name
#define NIC400_AXI64_TO_AXI32_RIGHT_S        0x42100
#define NIC400_GSPI_1_S                      0x46100
#define NIC400_ETH_1_S                       0x47100
#define NIC400_MKIO_1_S                      0x48100
#define NIC400_MKIO_3_S                      0x49100

///AXI64 regs name
#define NIC400_SW_0_S                        0x42100
#define NIC400_SW_1_S                        0x43100
#define NIC400_SW_2_S                        0x44100
#define NIC400_SW_3_S                        0x45100
#define NIC400_CPU_S                         0x46100
#define NIC400_SI_S                          0x47100
#define NIC400_AXI32_LEFT_TO_AXI64_S         0x48100
#define NIC400_AXI_DMA_S                     0x49100
#define NIC400_COM_0_S                       0x4A100
#define NIC400_COM_1_S                       0x4B100
#define NIC400_AXI32_RIGHT_TO_AXI64_S        0x4C100






///AXI32 left regs+offset
#define NIC400_AXI64_TO_AXI32_LEFT_S_RQOS           NIC400_AXI64_TO_AXI32_LEFT_S+RQOS_OFFSET
#define NIC400_AXI64_TO_AXI32_LEFT_S_WQOS           NIC400_AXI64_TO_AXI32_LEFT_S+WQOS_OFFSET
#define NIC400_AXI64_TO_AXI32_LEFT_S_FN_MOD         NIC400_AXI64_TO_AXI32_LEFT_S+FN_MOD_OFFSET

#define NIC400_GSPI_0_S_RQOS           NIC400_GSPI_0_S+RQOS_OFFSET
#define NIC400_GSPI_0_S_WQOS           NIC400_GSPI_0_S+WQOS_OFFSET
#define NIC400_GSPI_0_S_FN_MOD         NIC400_GSPI_0_S+FN_MOD_OFFSET

#define NIC400_ETH_0_S_RQOS           NIC400_ETH_0_S+RQOS_OFFSET
#define NIC400_ETH_0_S_WQOS           NIC400_ETH_0_S+WQOS_OFFSET
#define NIC400_ETH_0_S_FN_MOD         NIC400_ETH_0_S+FN_MOD_OFFSET

#define NIC400_MKIO_0_S_RQOS           NIC400_MKIO_0_S+RQOS_OFFSET
#define NIC400_MKIO_0_S_WQOS           NIC400_MKIO_0_S+WQOS_OFFSET
#define NIC400_MKIO_0_S_FN_MOD         NIC400_MKIO_0_S+FN_MOD_OFFSET

#define NIC400_MKIO_2_S_RQOS           NIC400_MKIO_2_S+RQOS_OFFSET
#define NIC400_MKIO_2_S_WQOS           NIC400_MKIO_2_S+WQOS_OFFSET
#define NIC400_MKIO_2_S_FN_MOD         NIC400_MKIO_2_S+FN_MOD_OFFSET

///AXI32 right regs+offset
#define NIC400_AXI64_TO_AXI32_RIGHT_S_RQOS           NIC400_AXI64_TO_AXI32_RIGHT_S+RQOS_OFFSET
#define NIC400_AXI64_TO_AXI32_RIGHT_S_WQOS           NIC400_AXI64_TO_AXI32_RIGHT_S+WQOS_OFFSET
#define NIC400_AXI64_TO_AXI32_RIGHT_S_FN_MOD         NIC400_AXI64_TO_AXI32_RIGHT_S+FN_MOD_OFFSET

#define NIC400_GSPI_1_S_RQOS           NIC400_GSPI_1_S+RQOS_OFFSET
#define NIC400_GSPI_1_S_WQOS           NIC400_GSPI_1_S+WQOS_OFFSET
#define NIC400_GSPI_1_S_FN_MOD         NIC400_GSPI_1_S+FN_MOD_OFFSET

#define NIC400_ETH_1_S_RQOS           NIC400_ETH_1_S+RQOS_OFFSET
#define NIC400_ETH_1_S_WQOS           NIC400_ETH_1_S+WQOS_OFFSET
#define NIC400_ETH_1_S_FN_MOD         NIC400_ETH_1_S+FN_MOD_OFFSET

#define NIC400_MKIO_1_S_RQOS           NIC400_MKIO_1_S+RQOS_OFFSET
#define NIC400_MKIO_1_S_WQOS           NIC400_MKIO_1_S+WQOS_OFFSET
#define NIC400_MKIO_1_S_FN_MOD         NIC400_MKIO_1_S+FN_MOD_OFFSET

#define NIC400_MKIO_3_S_RQOS           NIC400_MKIO_3_S+RQOS_OFFSET
#define NIC400_MKIO_3_S_WQOS           NIC400_MKIO_3_S+WQOS_OFFSET
#define NIC400_MKIO_3_S_FN_MOD         NIC400_MKIO_3_S+FN_MOD_OFFSET


///AXI64 regs+offset
#define NIC400_SW_0_S_RQOS                      NIC400_SW_0_S+RQOS_OFFSET
#define NIC400_SW_0_S_WQOS                      NIC400_SW_0_S+WQOS_OFFSET
#define NIC400_SW_0_S_FN_MOD                    NIC400_SW_0_S+FN_MOD_OFFSET
            
#define NIC400_SW_1_S_RQOS                      NIC400_SW_1_S+RQOS_OFFSET
#define NIC400_SW_1_S_WQOS                      NIC400_SW_1_S+WQOS_OFFSET
#define NIC400_SW_1_S_FN_MOD                    NIC400_SW_1_S+FN_MOD_OFFSET
                                                    
#define NIC400_SW_2_S_RQOS                      NIC400_SW_2_S+RQOS_OFFSET
#define NIC400_SW_2_S_WQOS                      NIC400_SW_2_S+WQOS_OFFSET
#define NIC400_SW_2_S_FN_MOD                    NIC400_SW_2_S+FN_MOD_OFFSET
                                                    
#define NIC400_SW_3_S_RQOS                      NIC400_SW_3_S+RQOS_OFFSET
#define NIC400_SW_3_S_WQOS                      NIC400_SW_3_S+WQOS_OFFSET
#define NIC400_SW_3_S_FN_MOD                    NIC400_SW_3_S+FN_MOD_OFFSET
                                   
#define NIC400_CPU_S_RQOS                       NIC400_CPU_S+RQOS_OFFSET
#define NIC400_CPU_S_WQOS                       NIC400_CPU_S+WQOS_OFFSET
#define NIC400_CPU_S_FN_MOD                     NIC400_CPU_S+FN_MOD_OFFSET
            
#define NIC400_SI_S_RQOS                        NIC400_SI_S+RQOS_OFFSET
#define NIC400_SI_S_WQOS                        NIC400_SI_S+WQOS_OFFSET
#define NIC400_SI_S_FN_MOD                      NIC400_SI_S+FN_MOD_OFFSET
    
#define NIC400_AXI32_LEFT_TO_AXI64_S_RQOS       NIC400_AXI32_LEFT_TO_AXI64_S+RQOS_OFFSET
#define NIC400_AXI32_LEFT_TO_AXI64_S_WQOS       NIC400_AXI32_LEFT_TO_AXI64_S+WQOS_OFFSET
#define NIC400_AXI32_LEFT_TO_AXI64_S_FN_MOD     NIC400_AXI32_LEFT_TO_AXI64_S+FN_MOD_OFFSET

#define NIC400_AXI_DMA_S_RQOS                   NIC400_AXI_DMA_S+RQOS_OFFSET
#define NIC400_AXI_DMA_S_WQOS                   NIC400_AXI_DMA_S+WQOS_OFFSET
#define NIC400_AXI_DMA_S_FN_MOD                 NIC400_AXI_DMA_S+FN_MOD_OFFSET

#define NIC400_COM_0_S_RQOS                     NIC400_COM_0_S+RQOS_OFFSET
#define NIC400_COM_0_S_WQOS                     NIC400_COM_0_S+WQOS_OFFSET
#define NIC400_COM_0_S_FN_MOD                   NIC400_COM_0_S+FN_MOD_OFFSET
            
#define NIC400_COM_1_S_RQOS                     NIC400_COM_1_S+RQOS_OFFSET
#define NIC400_COM_1_S_WQOS                     NIC400_COM_1_S+WQOS_OFFSET
#define NIC400_COM_1_S_FN_MOD                   NIC400_COM_1_S+FN_MOD_OFFSET

#define NIC400_AXI32_RIGHT_TO_AXI64_S_RQOS      NIC400_AXI32_RIGHT_TO_AXI64_S+RQOS_OFFSET
#define NIC400_AXI32_RIGHT_TO_AXI64_S_WQOS      NIC400_AXI32_RIGHT_TO_AXI64_S+WQOS_OFFSET
#define NIC400_AXI32_RIGHT_TO_AXI64_S_FN_MOD    NIC400_AXI32_RIGHT_TO_AXI64_S+FN_MOD_OFFSET


/**
* @}
*/

#endif /* end of include guard: DEVICES_NIC400_H */
