#ifndef __PLB6BC_H__
#define __PLB6BC_H__


#include <stdint.h>
#include <stdbool.h>
#include <platform/reg_access_plb6bc.h>


typedef struct plb6bc_cfg
{
    bool                            enabled;
    struct
    {
        bool                        mult_slaves_resp;
        bool                        snoopable_req;
    } mask_err;
    PLB6BC_PAAM_WIN_EXT_N           comb_resp_timeout:                  PLB6BC_PAAM_WIN_EXT_n;
    uint32_t                        hang_count_pulse_period:            PLB6BC_HCPP_n;
    struct
    {
        #define PLB6BC_MASTERi_PRIORITY( i, z )\
        PLB6BC_MiP                  CAT( master,i ):                    PLB6BC_PRI_MiP_n;
        REPEAT( PLB6BC_MASTER_N, PLB6BC_MASTERi_PRIORITY, z )
    } priority;
    #define PLB6BC_SEGMENTi_ADDR( i, z )\
    uint32_t                        CAT(CAT( segment,INC(i)),_addr ):   PLB6BC_SGDi_n;
    REPEAT( DEC(PLB6BC_SEGMENT_N), PLB6BC_SEGMENTi_ADDR, z )
} plb6bc_cfg;


typedef enum PLB6BC_ERR_TYPE
{
    PLB6BC_ERR_no_slave_resp    = (1 << 0),
    PLB6BC_ERR_mult_slaves_resp = (1 << 1),
    PLB6BC_ERR_snoopable_req    = (1 << 2),
    PLB6BC_ERR_all              = PLB6BC_ERR_no_slave_resp | PLB6BC_ERR_mult_slaves_resp | PLB6BC_ERR_snoopable_req,
} PLB6BC_ERR_TYPE;

typedef struct plb6bc_status
{
    /*struct TODO speed up and use
    {
        bool                        hang_detected;
        bool                        quiesce_detected;
        bool                        sys_hung_detected;
    } master[PLB6BC_MASTER_N];*/
    uint32_t                        hang_detected:          PLB6BC_HD_n;
    uint32_t                        quiesce_detected:       PLB6BC_QD_n;
    uint32_t                        sys_hung_detected:      PLB6BC_SHD_n;
    /*struct TODO speed up and use
    {
        struct
        {
            bool                    responded;
        } slave[PLB6BC_SEGMENT_SLAVE_N];
    } segment[PLB6BC_SEGMENT_N];*/
    uint32_t                        segment_slave_responded;
    struct
    {
    	PLB6BC_ERR_TYPE             type;
        uint32_t                    master_id:              PLB6BC_ERR_BC_SREMID_n;
        uint32_t                    mtag:                   PLB6BC_ERR_BC_SREMT_n;
        PLB6BC_SRERD                transaction_type:       PLB6BC_ERR_BC_SRERD_n;
        uint32_t                    segment_id:             PLB6BC_ERR_BC_SEGID_n;
        /*struct TODO speed up and use
        {
            bool                    responded;
        } slave[PLB6BC_SEGMENT_SLAVE_N];*/
        uint32_t                    slave_responded:        PLB6BC_ERR_BC_SPVAL_n;
    } err;
} plb6bc_status;


void plb6bc_init( uint32_t base_addr,  plb6bc_cfg const * cfg );


void plb6bc_enable( uint32_t base_addr );


#endif // __PLB6BC_H__
