#ifndef PPC_C_H
#define PPC_C_H

#include <platform/oi10/platform/ppc_476fp_lib_c.h>
#include <platform/oi10/platform/ppc_476fp_config.h>
#include <platform/oi10/platform/boost/preprocessor/repetition/repeat_from_to.hpp>
#include <platform/oi10/platform/boost/preprocessor/cat.hpp>


void usleep(uint32_t usec);

void  ppc_noncr_int_enable();
void  ppc_cr_int_enable();


inline static void dcr_write( uint32_t const addr, uint32_t const wval )
{
    mtdcrx(addr, wval);
}
inline static uint32_t dcr_read( uint32_t const addr )
{
    return mfdcrx(addr);
}

inline static void msr_write( uint32_t const wval )
{
    mtmsr(wval);
}
inline static uint32_t msr_read()
{
    return mfmsr();
}


//SPR access
#define REG_READ_SPR(REG)		                    \
inline static uint32_t REG##_read()					\
{													\
    uint32_t rval=0;                                \
    asm volatile   (                                \
        "mfspr %0, %1 \n\t"                         \
        :"=r"(rval)                                 \
        :"i"(REG)                                   \
        :                                           \
    );                                              \
    return rval;                                    \
}													\

#define REG_WRITE_SPR(REG)                      	\
inline static void REG##_write(uint32_t const value)\
{													\
    asm volatile   (                                \
        "mtspr %1, %0 \n\t"                         \
        ::"r"(value), "i"(REG)                      \
        :                                           \
    );                                              \
}													\

REG_WRITE_SPR(SPR_CTR)
REG_READ_SPR(SPR_CTR)

REG_WRITE_SPR(SPR_LR)
REG_READ_SPR(SPR_LR)

REG_WRITE_SPR(SPR_DCRIPR)
REG_READ_SPR(SPR_DCRIPR)

REG_READ_SPR(SPR_DCDBTRH)

REG_READ_SPR(SPR_DCDBTRL)

REG_WRITE_SPR(SPR_DCESR)
REG_READ_SPR(SPR_DCESR)

#define SPR_ICDBDRx_ACCESS( z, x, dummy )\
REG_READ_SPR(SPR_ICDBDR##x)
BOOST_PP_REPEAT_FROM_TO( 0, 2, SPR_ICDBDRx_ACCESS, dummy )

REG_READ_SPR(SPR_ICDBTRH)

REG_READ_SPR(SPR_ICDBTRL)

REG_WRITE_SPR(SPR_ICESR)
REG_READ_SPR(SPR_ICESR)

REG_WRITE_SPR(SPR_IOCCR)
REG_READ_SPR(SPR_IOCCR)

#define SPR_IOCRx_ACCESS( z, x, dummy )\
REG_WRITE_SPR(SPR_IOCR##x)\
REG_READ_SPR(SPR_IOCR##x)
BOOST_PP_REPEAT_FROM_TO( 1, 3, SPR_IOCRx_ACCESS, dummy )

#define SPR_DACx_ACCESS( z, x, dummy )\
REG_WRITE_SPR(SPR_DAC##x)\
REG_READ_SPR(SPR_DAC##x)
BOOST_PP_REPEAT_FROM_TO( 1, 3, SPR_DACx_ACCESS, dummy )

#define SPR_DVCx_ACCESS( z, x, dummy )\
REG_WRITE_SPR(SPR_DVC##x)\
REG_READ_SPR(SPR_DVC##x)
BOOST_PP_REPEAT_FROM_TO( 1, 3, SPR_DVCx_ACCESS, dummy )

#define SPR_DBCRx_ACCESS( z, x, dummy )\
REG_WRITE_SPR(SPR_DBCR##x)\
REG_READ_SPR(SPR_DBCR##x)
BOOST_PP_REPEAT_FROM_TO( 0, 3, SPR_DBCRx_ACCESS, dummy )

REG_WRITE_SPR(SPR_DBDR)
REG_READ_SPR(SPR_DBDR)

REG_WRITE_SPR(SPR_DBSR_W)
REG_READ_SPR(SPR_DBSR_RC)
REG_WRITE_SPR(SPR_DBSR_RC)

#define SPR_IACx_ACCESS( z, x, dummy )\
REG_WRITE_SPR(SPR_IAC##x)\
REG_READ_SPR(SPR_IAC##x)
BOOST_PP_REPEAT_FROM_TO( 1, 5, SPR_IACx_ACCESS, dummy )

REG_WRITE_SPR(SPR_XER)
REG_READ_SPR(SPR_XER)

REG_WRITE_SPR(SPR_CSRR0)
REG_READ_SPR(SPR_CSRR0)

REG_WRITE_SPR(SPR_CSRR1)
REG_READ_SPR(SPR_CSRR1)

REG_WRITE_SPR(SPR_DEAR)
REG_READ_SPR(SPR_DEAR)

REG_WRITE_SPR(SPR_ESR)
REG_READ_SPR(SPR_ESR)

#define SPR_IVORx_ACCESS( z, x, dummy )\
REG_WRITE_SPR(SPR_IVOR##x)\
REG_READ_SPR(SPR_IVOR##x)
BOOST_PP_REPEAT_FROM_TO( 0, 16, SPR_IVORx_ACCESS, dummy )

REG_WRITE_SPR(SPR_IVPR)
REG_READ_SPR(SPR_IVPR)

#define SPR_MCSRRx_ACCESS( z, x, dummy )\
REG_WRITE_SPR(SPR_MCSRR##x)\
REG_READ_SPR(SPR_MCSRR##x)
BOOST_PP_REPEAT_FROM_TO( 0, 2, SPR_MCSRRx_ACCESS, dummy )

REG_WRITE_SPR(SPR_MCSR_RW)
REG_READ_SPR(SPR_MCSR_RW)
REG_WRITE_SPR(SPR_MCSR_C)

#define SPR_SRRx_ACCESS( z, x, dummy )\
REG_WRITE_SPR(SPR_SRR##x)\
REG_READ_SPR(SPR_SRR##x)
BOOST_PP_REPEAT_FROM_TO( 0, 2, SPR_SRRx_ACCESS, dummy )

REG_WRITE_SPR(SPR_PMUCC0)
REG_READ_SPR(SPR_PMUCC0)

REG_READ_SPR(SPR_PMUCC0_UR)

REG_WRITE_SPR(SPR_ISPCR)
REG_READ_SPR(SPR_ISPCR)

#define SPR_MMUBEx_ACCESS( z, x, dummy )\
REG_WRITE_SPR(SPR_MMUBE##x) /*need to know access*/\
REG_READ_SPR(SPR_MMUBE##x)
BOOST_PP_REPEAT_FROM_TO( 0, 2, SPR_MMUBEx_ACCESS, dummy )

REG_WRITE_SPR(SPR_MMUCR)
REG_READ_SPR(SPR_MMUCR)

REG_WRITE_SPR(SPR_PID)
REG_READ_SPR(SPR_PID)

REG_WRITE_SPR(SPR_RMPD)
REG_READ_SPR(SPR_RMPD)

REG_READ_SPR(SPR_RSTCFG)

REG_WRITE_SPR(SPR_SSPCR)
REG_READ_SPR(SPR_SSPCR)

REG_WRITE_SPR(SPR_USPCR)
REG_READ_SPR(SPR_USPCR)

#define SPR_CCRx_ACCESS( z, x, dummy )\
REG_WRITE_SPR(SPR_CCR##x)\
REG_READ_SPR(SPR_CCR##x)
BOOST_PP_REPEAT_FROM_TO( 0, 3, SPR_CCRx_ACCESS, dummy )

REG_READ_SPR(SPR_PIR)

REG_READ_SPR(SPR_PVR)

#define SPR_SPRGx_ACCESS( z, x, dummy )\
REG_WRITE_SPR(SPR_SPRG##x)\
REG_READ_SPR(SPR_SPRG##x)
BOOST_PP_REPEAT_FROM_TO( 0, 9, SPR_SPRGx_ACCESS, dummy )

REG_WRITE_SPR(SPR_USPGR0)
REG_READ_SPR(SPR_USPGR0)

REG_WRITE_SPR(SPR_DECAR)
REG_READ_SPR(SPR_DECAR)

REG_WRITE_SPR(SPR_DEC)
REG_READ_SPR(SPR_DEC)

REG_WRITE_SPR(SPR_TBL_W)
REG_READ_SPR(SPR_TBL_R)

REG_WRITE_SPR(SPR_TBU_W)
REG_READ_SPR(SPR_TBU_R)

REG_WRITE_SPR(SPR_TCR)
REG_READ_SPR(SPR_TCR)

REG_WRITE_SPR(SPR_TSR_W)
REG_READ_SPR(SPR_TSR_RC)
REG_WRITE_SPR(SPR_TSR_RC)

#endif  /* PPC_C_H */
