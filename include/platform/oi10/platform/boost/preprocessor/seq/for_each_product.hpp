# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_SEQ_FOR_EACH_PRODUCT_HPP
# define BOOST_PREPROCESSOR_SEQ_FOR_EACH_PRODUCT_HPP
#
# include <platform/oi10/platform/boost/preprocessor/arithmetic/dec.hpp>
# include <platform/oi10/platform/boost/preprocessor/config/config.hpp>
# include <platform/oi10/platform/boost/preprocessor/control/if.hpp>
# include <platform/oi10/platform/boost/preprocessor/repetition/for.hpp>
# include <platform/oi10/platform/boost/preprocessor/seq/seq.hpp>
# include <platform/oi10/platform/boost/preprocessor/seq/size.hpp>
# include <platform/oi10/platform/boost/preprocessor/tuple/elem.hpp>
# include <platform/oi10/platform/boost/preprocessor/tuple/rem.hpp>
#
# /* BOOST_PP_SEQ_FOR_EACH_PRODUCT */
#
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT(macro, sets) BOOST_PP_SEQ_FOR_EACH_PRODUCT_E(BOOST_PP_FOR, macro, sets)
#
# /* BOOST_PP_SEQ_FOR_EACH_PRODUCT_R */
#
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_R(r, macro, sets) BOOST_PP_SEQ_FOR_EACH_PRODUCT_E(BOOST_PP_FOR_ ## r, macro, sets)
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_EDG()
#    define BOOST_PP_SEQ_FOR_EACH_PRODUCT_E(impl, macro, sets) impl((BOOST_PP_SEQ_HEAD(sets)(nil), BOOST_PP_SEQ_TAIL(sets)(nil), (nil), macro), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_0)
# else
#    define BOOST_PP_SEQ_FOR_EACH_PRODUCT_E(impl, macro, sets) BOOST_PP_SEQ_FOR_EACH_PRODUCT_E_I(impl, macro, sets)
#    define BOOST_PP_SEQ_FOR_EACH_PRODUCT_E_I(impl, macro, sets) impl((BOOST_PP_SEQ_HEAD(sets)(nil), BOOST_PP_SEQ_TAIL(sets)(nil), (nil), macro), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_0)
# endif
#
# if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_STRICT()
#    define BOOST_PP_SEQ_FOR_EACH_PRODUCT_P(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_P_I data
#    define BOOST_PP_SEQ_FOR_EACH_PRODUCT_P_I(cset, rset, res, macro) BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(cset))
# else
#    define BOOST_PP_SEQ_FOR_EACH_PRODUCT_P(r, data) BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(BOOST_PP_TUPLE_ELEM(4, 0, data)))
# endif
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC()
#    define BOOST_PP_SEQ_FOR_EACH_PRODUCT_O(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_O_I data
#    define BOOST_PP_SEQ_FOR_EACH_PRODUCT_O_I(cset, rset, res, macro) (BOOST_PP_SEQ_TAIL(cset), rset, res, macro)
# else
#    define BOOST_PP_SEQ_FOR_EACH_PRODUCT_O(r, data) (BOOST_PP_SEQ_TAIL(BOOST_PP_TUPLE_ELEM(4, 0, data)), BOOST_PP_TUPLE_ELEM(4, 1, data), BOOST_PP_TUPLE_ELEM(4, 2, data), BOOST_PP_TUPLE_ELEM(4, 3, data))
# endif
#
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, i) BOOST_PP_IF(BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(BOOST_PP_TUPLE_ELEM(4, 1, data))), BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_ ## i, BOOST_PP_SEQ_FOR_EACH_PRODUCT_I)
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_EDG()
#    define BOOST_PP_SEQ_FOR_EACH_PRODUCT_I(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_I_I(r, BOOST_PP_TUPLE_ELEM(4, 0, data), BOOST_PP_TUPLE_ELEM(4, 1, data), BOOST_PP_TUPLE_ELEM(4, 2, data), BOOST_PP_TUPLE_ELEM(4, 3, data))
# else
#    define BOOST_PP_SEQ_FOR_EACH_PRODUCT_I(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_I_IM(r, BOOST_PP_TUPLE_REM_4 data)
#    define BOOST_PP_SEQ_FOR_EACH_PRODUCT_I_IM(r, im) BOOST_PP_SEQ_FOR_EACH_PRODUCT_I_I(r, im)
# endif
#
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_I_I(r, cset, rset, res, macro) macro(r, BOOST_PP_SEQ_TAIL(res (BOOST_PP_SEQ_HEAD(cset))))
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC()
#    define BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_H_I data
# else
#    define BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_H_I(BOOST_PP_TUPLE_ELEM(4, 0, data), BOOST_PP_TUPLE_ELEM(4, 1, data), BOOST_PP_TUPLE_ELEM(4, 2, data), BOOST_PP_TUPLE_ELEM(4, 3, data))
# endif
#
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_H_I(cset, rset, res, macro) (BOOST_PP_SEQ_HEAD(rset)(nil), BOOST_PP_SEQ_TAIL(rset), res (BOOST_PP_SEQ_HEAD(cset)), macro)
#
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_0(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 0)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_1(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 1)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_2(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 2)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_3(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 3)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_4(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 4)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_5(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 5)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_6(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 6)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_7(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 7)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_8(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 8)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_9(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 9)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_10(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 10)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_11(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 11)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_12(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 12)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_13(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 13)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_14(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 14)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_15(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 15)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_16(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 16)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_17(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 17)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_18(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 18)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_19(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 19)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_20(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 20)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_21(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 21)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_22(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 22)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_23(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 23)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_24(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 24)(r, data)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_25(r, data) BOOST_PP_SEQ_FOR_EACH_PRODUCT_C(data, 25)(r, data)
#
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_0(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_1)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_1(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_2)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_2(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_3)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_3(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_4)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_4(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_5)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_5(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_6)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_6(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_7)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_7(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_8)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_8(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_9)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_9(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_10)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_10(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_11)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_11(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_12)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_12(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_13)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_13(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_14)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_14(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_15)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_15(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_16)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_16(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_17)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_17(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_18)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_18(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_19)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_19(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_20)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_20(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_21)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_21(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_22)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_22(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_23)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_23(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_24)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_24(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_25)
# define BOOST_PP_SEQ_FOR_EACH_PRODUCT_N_25(r, data) BOOST_PP_FOR_ ## r(BOOST_PP_SEQ_FOR_EACH_PRODUCT_H(data), BOOST_PP_SEQ_FOR_EACH_PRODUCT_P, BOOST_PP_SEQ_FOR_EACH_PRODUCT_O, BOOST_PP_SEQ_FOR_EACH_PRODUCT_M_26)
#
# endif
