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
///AXI32 regs name
#define NIC400_CPU_S                         0x42100
#define NIC400_GSPI_1_S                      0x43100
#define NIC400_ETH_1_S                       0x44100
#define NIC400_SW_0_3_S                      0x45100
#define NIC400_GSPI_0_S                      0x46100
#define NIC400_ETH_0_S                       0x47100
///AXI64 regs name
#define NIC400_SW_0_S                        0x42100
#define NIC400_SW_1_S                        0x43100
#define NIC400_SW_2_1_S                      0x44100
#define NIC400_SW_3_S                        0x45100

///AXI32 regs+offset
#define NIC400_CPU_S_RQOS           NIC400_CPU_S+RQOS_OFFSET
#define NIC400_CPU_S_WQOS           NIC400_CPU_S+WQOS_OFFSET
#define NIC400_CPU_S_FN_MOD         NIC400_CPU_S+FN_MOD_OFFSET

#define NIC400_GSPI_1_S_RQOS        NIC400_GSPI_1_S+RQOS_OFFSET
#define NIC400_GSPI_1_S_WQOS        NIC400_GSPI_1_S+WQOS_OFFSET
#define NIC400_GSPI_1_S_FN_MOD      NIC400_GSPI_1_S+FN_MOD_OFFSET

#define NIC400_ETH_1_S_RQOS         NIC400_ETH_1_S+RQOS_OFFSET
#define NIC400_ETH_1_S_WQOS         NIC400_ETH_1_S+WQOS_OFFSET
#define NIC400_ETH_1_S_FN_MOD       NIC400_ETH_1_S+FN_MOD_OFFSET

#define NIC400_SW_0_3_S_RQOS        NIC400_SW_0_3_S+RQOS_OFFSET
#define NIC400_SW_0_3_S_WQOS        NIC400_SW_0_3_S+WQOS_OFFSET
#define NIC400_SW_0_3_S_FN_MOD      NIC400_SW_0_3_S+FN_MOD_OFFSET

#define NIC400_GSPI_0_S_RQOS        NIC400_GSPI_0_S+RQOS_OFFSET
#define NIC400_GSPI_0_S_WQOS        NIC400_GSPI_0_S+WQOS_OFFSET
#define NIC400_GSPI_0_S_FN_MOD      NIC400_GSPI_0_S+FN_MOD_OFFSET

#define NIC400_ETH_0_S_RQOS         NIC400_ETH_0_S+RQOS_OFFSET
#define NIC400_ETH_0_S_WQOS         NIC400_ETH_0_S+WQOS_OFFSET
#define NIC400_ETH_0_S_FN_MOD       NIC400_ETH_0_S+FN_MOD_OFFSET

///AXI64 regs+offset
#define NIC400_SW_0_S_RQOS          NIC400_SW_0_S+RQOS_OFFSET
#define NIC400_SW_0_S_WQOS          NIC400_SW_0_S+WQOS_OFFSET
#define NIC400_SW_0_S_FN_MOD        NIC400_SW_0_S+FN_MOD_OFFSET

#define NIC400_SW_1_S_RQOS          NIC400_SW_1_S+RQOS_OFFSET
#define NIC400_SW_1_S_WQOS          NIC400_SW_1_S+WQOS_OFFSET
#define NIC400_SW_1_S_FN_MOD        NIC400_SW_1_S+FN_MOD_OFFSET
                                           
#define NIC400_SW_2_1_S_RQOS        NIC400_SW_2_1_S+RQOS_OFFSET
#define NIC400_SW_2_1_S_WQOS        NIC400_SW_2_1_S+WQOS_OFFSET
#define NIC400_SW_2_1_S_FN_MOD      NIC400_SW_2_1_S+FN_MOD_OFFSET
                                           
#define NIC400_SW_3_S_RQOS          NIC400_SW_3_S+RQOS_OFFSET
#define NIC400_SW_3_S_WQOS          NIC400_SW_3_S+WQOS_OFFSET
#define NIC400_SW_3_S_FN_MOD        NIC400_SW_3_S+FN_MOD_OFFSET
                                           

/**
* @}
*/

#endif /* end of include guard: DEVICES_NIC400_H */
