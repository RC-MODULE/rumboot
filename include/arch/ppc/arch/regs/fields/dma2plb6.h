#ifndef REG_DMA2PLB6_FIELDS_H__
#define REG_DMA2PLB6_FIELDS_H__

//reg fields

//CRn
#define CE      0
#define TCIE    1
#define EIE     2
#define CIM     4
#define PW      7
#define DSS     10
#define DSD     11
#define TCE     12
#define CP      14
#define PF      17
#define SSC     18
#define DSC     19

//SR
#define CS      3
#define RI0     4
#define RS0     7
#define RI1     8
#define RS1     11
#define RI2     12
#define RS2     15
#define RI3     16
#define RS3     19
#define CB      23
#define SG      27

#define get_addr(channel,reg_offset,base)   (base + (CHANNEL_ADDR_OFF*channel)+ reg_offset)

BEGIN_ENUM( DMA_OPTIONS_FIELD )
    DECLARE_ENUM_VAL( DMA_OPTIONS_ATTRIB_e,                                                        0 )
    DECLARE_ENUM_VAL( DMA_OPTIONS_ATTRIB_n,                                                        8 )
    DECLARE_ENUM_VAL( DMA_OPTIONS_ATTRIB_i,     IBM_BIT_INDEX( 32, DMA_OPTIONS_ATTRIB_e            ) )

    DECLARE_ENUM_VAL( DMA_OPTIONS_RPR_e,                                                           8 )
    DECLARE_ENUM_VAL( DMA_OPTIONS_RPR_n,                                                           1 )
    DECLARE_ENUM_VAL( DMA_OPTIONS_RPR_i,        IBM_BIT_INDEX( 32, DMA_OPTIONS_RPR_e               ) )

    DECLARE_ENUM_VAL( DMA_OPTIONS_RPW_e,                                                           9 )
    DECLARE_ENUM_VAL( DMA_OPTIONS_RPW_n,                                                           1 )
    DECLARE_ENUM_VAL( DMA_OPTIONS_RPW_i,        IBM_BIT_INDEX( 32, DMA_OPTIONS_RPW_e               ) )

    DECLARE_ENUM_VAL( DMA_OPTIONS_WRP_e,                                                          10 )
    DECLARE_ENUM_VAL( DMA_OPTIONS_WRP_n,                                                          1 )
    DECLARE_ENUM_VAL( DMA_OPTIONS_WRP_i,        IBM_BIT_INDEX( 32, DMA_OPTIONS_WRP_e              ) )

    DECLARE_ENUM_VAL( DMA_OPTIONS_SL_PE_e,                                                       11 )
    DECLARE_ENUM_VAL( DMA_OPTIONS_SL_PE_n,                                                        1 )
    DECLARE_ENUM_VAL( DMA_OPTIONS_SL_PE_i,      IBM_BIT_INDEX( 32, DMA_OPTIONS_SL_PE_e            ) )

    DECLARE_ENUM_VAL( DMA_OPTIONS_M_ADPE_e,                                                       12 )
    DECLARE_ENUM_VAL( DMA_OPTIONS_M_ADPE_n,                                                        1 )
    DECLARE_ENUM_VAL( DMA_OPTIONS_M_ADPE_i,     IBM_BIT_INDEX( 32, DMA_OPTIONS_M_ADPE_e            ) )

    DECLARE_ENUM_VAL( DMA_OPTIONS_M_BEPE_e,                                                       13 )
    DECLARE_ENUM_VAL( DMA_OPTIONS_M_BEPE_n,                                                        1 )
    DECLARE_ENUM_VAL( DMA_OPTIONS_M_BEPE_i,     IBM_BIT_INDEX( 32, DMA_OPTIONS_M_BEPE_e            ) )

    DECLARE_ENUM_VAL( DMA_OPTIONS_M_WDPE_e,                                                       14 )
    DECLARE_ENUM_VAL( DMA_OPTIONS_M_WDPE_n,                                                        1 )
    DECLARE_ENUM_VAL( DMA_OPTIONS_M_WDPE_i,     IBM_BIT_INDEX( 32, DMA_OPTIONS_M_WDPE_e            ) )

    DECLARE_ENUM_VAL( DMA_OPTIONS_SL_INT_e,                                                       15 )
    DECLARE_ENUM_VAL( DMA_OPTIONS_SL_INT_n,                                                        1 )
    DECLARE_ENUM_VAL( DMA_OPTIONS_SL_INT_i,     IBM_BIT_INDEX( 32, DMA_OPTIONS_SL_INT_e            ) )

    DECLARE_ENUM_VAL( DMA_OPTIONS_SL_PEE_e,                                                      16 )
    DECLARE_ENUM_VAL( DMA_OPTIONS_SL_PEE_n,                                                       1 )
    DECLARE_ENUM_VAL( DMA_OPTIONS_SL_PEE_i,     IBM_BIT_INDEX( 32, DMA_OPTIONS_SL_PE_e            ) )

END_ENUM( DMA_OPTIONS_FIELD )

BEGIN_ENUM( DMA_PSE_FIELD )

    DECLARE_ENUM_VAL( DMA_PSE_RE_e,                                                0 )
    DECLARE_ENUM_VAL( DMA_PSE_RE_n,                                                1 )
    DECLARE_ENUM_VAL( DMA_PSE_RE_i,     IBM_BIT_INDEX( 32, DMA_PSE_RE_e            ) )

    DECLARE_ENUM_VAL( DMA_PSE_RS_e,                                                4 )
    DECLARE_ENUM_VAL( DMA_PSE_RS_n,                                                4 )
    DECLARE_ENUM_VAL( DMA_PSE_RS_i,     IBM_BIT_INDEX( 32, DMA_PSE_RS_e            ) )

    DECLARE_ENUM_VAL( DMA_PSE_WE_e,                                                5 )
    DECLARE_ENUM_VAL( DMA_PSE_WE_n,                                                1 )
    DECLARE_ENUM_VAL( DMA_PSE_WE_i,     IBM_BIT_INDEX( 32, DMA_PSE_WE_e            ) )

    DECLARE_ENUM_VAL( DMA_PSE_PE_e,                                                6 )
    DECLARE_ENUM_VAL( DMA_PSE_PE_n,                                                1 )
    DECLARE_ENUM_VAL( DMA_PSE_PE_i,     IBM_BIT_INDEX( 32, DMA_PSE_PE_e            ) )

    DECLARE_ENUM_VAL( DMA_PSE_MID_e,                                              12 )
    DECLARE_ENUM_VAL( DMA_PSE_MID_n,                                               6 )
    DECLARE_ENUM_VAL( DMA_PSE_MID_i,    IBM_BIT_INDEX( 32, DMA_PSE_MID_e           ) )

    DECLARE_ENUM_VAL( DMA_PSE_RTG_e,                                              17 )
    DECLARE_ENUM_VAL( DMA_PSE_RTG_n,                                               5 )
    DECLARE_ENUM_VAL( DMA_PSE_RTG_i,    IBM_BIT_INDEX( 32, DMA_PSE_RTG_e           ) )

END_ENUM( DMA_PSE_FIELD )


#endif // REG_DMA2PLB6_FIELDS_H__
