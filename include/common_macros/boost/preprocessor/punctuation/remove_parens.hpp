# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2014.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
#ifndef BOOST_PREPROCESSOR_REMOVE_PARENS_HPP
#define BOOST_PREPROCESSOR_REMOVE_PARENS_HPP

#include <common_macros/boost/preprocessor/config/config.hpp>

#if BOOST_PP_VARIADICS

#include <common_macros/boost/preprocessor/control/iif.hpp>
#include <common_macros/boost/preprocessor/facilities/identity.hpp>
#include <common_macros/boost/preprocessor/punctuation/is_begin_parens.hpp>
#include <common_macros/boost/preprocessor/tuple/enum.hpp>

#define BOOST_PP_REMOVE_PARENS(param) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_IS_BEGIN_PARENS(param), \
      BOOST_PP_REMOVE_PARENS_DO, \
      BOOST_PP_IDENTITY \
      ) \
    (param)() \
/**/

#define BOOST_PP_REMOVE_PARENS_DO(param) \
  BOOST_PP_IDENTITY(BOOST_PP_TUPLE_ENUM(param)) \
/**/

#endif /* BOOST_PP_VARIADICS */
#endif /* BOOST_PREPROCESSOR_REMOVE_PARENS_HPP */
