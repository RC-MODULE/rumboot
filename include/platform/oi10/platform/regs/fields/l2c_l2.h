/*
 * l2c_l2.h
 *
 *  Created on: Feb 11, 2019
 *      Author: a.gurov
 */

#ifndef L2C_L2_H_
#define L2C_L2_H_



BEGIN_ENUM( L2C_L2ARRACCCTL_FIELD )
    /* Request field [9] */
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_REQUEST_e,          9     )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_REQUEST_n,          1     )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_REQUEST_i,          IBM_BIT_INDEX(32, L2C_L2ARRACCCTL_REQUEST_e) )
    /* ReadRequestComp field [10] (r/o) */
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_READREQUESTCOMP_e,  10    )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_READREQUESTCOMP_n,  1     )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_READREQUESTCOMP_i,  IBM_BIT_INDEX(32, L2C_L2ARRACCCTL_READREQUESTCOMP_e) )
    /* WrtRequestComp field [11] (r/o) */
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_WRTREQUESTCOMP_e,   11    )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_WRTREQUESTCOMP_n,   1     )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_WRTREQUESTCOMP_i,   IBM_BIT_INDEX(32, L2C_L2ARRACCCTL_WRTREQUESTCOMP_e) )
    /* BufferID (entire) field [13:15] */
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_BUFFERID_e,         15    )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_BUFFERID_n,         3     )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_BUFFERID_i,         IBM_BIT_INDEX(32, L2C_L2ARRACCCTL_BUFFERID_e) )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_BUFFERID_mask,      FIELD_MASK32(L2C_L2ARRACCCTL_BUFFERID_i, L2C_L2ARRACCCTL_BUFFERID_n) )
    /* ReqType field [16:19] */
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_REQTYPE_e,          19    )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_REQTYPE_n,          4     )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_REQTYPE_i,          IBM_BIT_INDEX(32, L2C_L2ARRACCCTL_REQTYPE_e) )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_REQTYPE_mask,       FIELD_MASK32(L2C_L2ARRACCCTL_REQTYPE_i, L2C_L2ARRACCCTL_REQTYPE_n) )
    /* L2Way field [22:23] */
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_L2WAY_e,            23    )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_L2WAY_n,            2     )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_L2WAY_i,            IBM_BIT_INDEX(32, L2C_L2ARRACCCTL_L2WAY_e) )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_L2WAY_mask,         FIELD_MASK32(L2C_L2ARRACCCTL_L2WAY_i, L2C_L2ARRACCCTL_L2WAY_n) )
    /* MaskDW field [24:31] */
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_MASKDW_e,           31    )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_MASKDW_n,           8     )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_MASKDW_i,           IBM_BIT_INDEX(32, L2C_L2ARRACCCTL_MASKDW_e) )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_MASKDW_mask,        FIELD_MASK32(L2C_L2ARRACCCTL_MASKDW_i, L2C_L2ARRACCCTL_MASKDW_n) )
END_ENUM( L2C_L2ARRACCCTL_FIELD )

BEGIN_ENUM( L2C_L2ARRACCCTL_BUFFERID )
    /* BufferID_LRU sub-field [13] */
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_BUFFERID_LRU_e,     13    )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_BUFFERID_LRU_n,     1     )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_BUFFERID_LRU_i,     IBM_BIT_INDEX(32, L2C_L2ARRACCCTL_BUFFERID_LRU_e) )
    /* BufferID_TAG sub-field [14] */
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_BUFFERID_TAG_e,     14    )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_BUFFERID_TAG_n,     1     )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_BUFFERID_TAG_i,     IBM_BIT_INDEX(32, L2C_L2ARRACCCTL_BUFFERID_TAG_e) )
    /* BufferID_DATA sub-field [15] */
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_BUFFERID_DATA_e,    15    )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_BUFFERID_DATA_n,    1     )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_BUFFERID_DATA_i,    IBM_BIT_INDEX(32, L2C_L2ARRACCCTL_BUFFERID_DATA_e) )
END_ENUM( L2C_L2ARRACCCTL_BUFFERID )

BEGIN_ENUM( L2C_L2ARRACCCTL_REQTYPE )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_REQTYPE_RD8WOECC,   0x0F  )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_REQTYPE_WR8WTECC,   0x06  )
    DECLARE_ENUM_VAL(   L2C_L2ARRACCCTL_REQTYPE_WR8WOECC,   0x07  )
END_ENUM( L2C_L2ARRACCCTL_REQTYPE )


#endif /* L2C_L2_H_ */
