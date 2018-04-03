#ifndef __PLB6BC_REG_ACCESS_H__
#define __PLB6BC_REG_ACCESS_H__


#include <platform/oi10/platform/plb6bc/reg.h>
#include <platform/oi10/platform/reg_access/macros.h>
#include <platform/oi10/platform/boost/preprocessor/repetition/repeat_from_to.hpp>


REG_READ_DCR( plb6bc, PLB6BC_CR0, 32 )
REG_WRITE_DCR( plb6bc, PLB6BC_CR0, 32 )

REG_READ_DCR( plb6bc, PLB6BC_PRI, 32 )
REG_WRITE_DCR( plb6bc, PLB6BC_PRI, 32 )

REG_READ_DCR( plb6bc, PLB6BC_TSNOOP, 32 )

REG_READ_DCR( plb6bc, PLB6BC_PAAM_WIN_EXT, 32 )
REG_WRITE_DCR( plb6bc, PLB6BC_PAAM_WIN_EXT, 32 )

#define PLB6BC_SGDx_ACCESS(z, x, dummy) \
REG_READ_DCR(plb6bc, PLB6BC_SGD##x, 32)\
REG_WRITE_DCR( plb6bc, PLB6BC_SGD##x, 32 )
BOOST_PP_REPEAT_FROM_TO( 1, PLB6BC_SEGMENT_N, PLB6BC_SGDx_ACCESS, dummy )

REG_READ_DCR( plb6bc, PLB6BC_ERR, 32 )
REG_WRITE_DCR( plb6bc, PLB6BC_ERR, 32 )

REG_READ_DCR( plb6bc, PLB6BC_MSRSP, 32 )
REG_WRITE_DCR( plb6bc, PLB6BC_MSRSP, 32 )

REG_READ_DCR( plb6bc, PLB6BC_HCPP, 32 )
REG_WRITE_DCR( plb6bc, PLB6BC_HCPP, 32 )

REG_READ_DCR( plb6bc, PLB6BC_HD, 32 )
REG_WRITE_DCR( plb6bc, PLB6BC_HD, 32 )

REG_READ_DCR( plb6bc, PLB6BC_QD, 32 )
REG_WRITE_DCR( plb6bc, PLB6BC_QD, 32 )

REG_READ_DCR( plb6bc, PLB6BC_SHD, 32 )
REG_WRITE_DCR( plb6bc, PLB6BC_SHD, 32 )

REG_READ_DCR( plb6bc, PLB6BC_REVID, 32 )


#endif // __PLB6BC_REG_ACCESS_H__
