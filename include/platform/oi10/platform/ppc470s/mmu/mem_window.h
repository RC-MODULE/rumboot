#ifndef __PPC470_MMU_MEM_WINDOW_H__
#define __PPC470_MMU_MEM_WINDOW_H__


#include <platform/oi10/platform/mivem_macro.h>
#include <platform/oi10/platform/boost/preprocessor/iteration/local.hpp>
#include <platform/oi10/platform/boost/preprocessor/cat.hpp>


BEGIN_ENUM( MEM_WINDOW )
DECLARE_ENUM_VAL(               MEM_WINDOW_SHARED,  0x0000 )

#define BOOST_PP_LOCAL_MACRO( n )\
DECLARE_ENUM_VAL( BOOST_PP_CAT( MEM_WINDOW_,n ),    (0x8000 | n) )
#define BOOST_PP_LOCAL_LIMITS (0, 15)
#include BOOST_PP_LOCAL_ITERATE()
END_ENUM( MEM_WINDOW )


#endif // __PPC470_MMU_MEM_WINDOW_H__
