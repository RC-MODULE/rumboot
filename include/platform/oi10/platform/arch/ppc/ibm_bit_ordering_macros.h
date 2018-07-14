/*
 * ibm_bit_ordering_macros.h
 *
 *  Created on: Apr 2, 2018
 *      Author: v.strukov
 */

#ifndef IBM_BIT_ORDERING_MACROS_H_
#define IBM_BIT_ORDERING_MACROS_H_


#define IBM_BIT_INDEX( size, index )    ( ((size)-1) - ((index)%(size)) )

#define reg_field(field_right_bit_num_from_ppc_user_manual, value)\
    ((value) << IBM_BIT_INDEX( 32, field_right_bit_num_from_ppc_user_manual ))


#endif /* IBM_BIT_ORDERING_MACROS_H_ */
