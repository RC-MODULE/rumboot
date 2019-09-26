#ifndef MACROS_H
#define MACROS_H

#define ARRAY_SIZE( array )             (sizeof(array) / sizeof(array[0]))

#ifdef __ASSEMBLER__
#define DECLARE_CONST( name, value )    .equiv name, value
#else
#define DECLARE_CONST( name, value )    enum{ name = value };
#endif

#ifdef __ASSEMBLER__

.macro equiv_and_increment name, value
    .equiv \name, \value
    .set  current_enum_val___, \value+1
.endm

#define BEGIN_ENUM( enum_name )         .set  current_enum_val___, 0
#define DECLARE_ENUM_NEXT( name )       equiv_and_increment name, current_enum_val___
#define DECLARE_ENUM_VAL( name, value ) equiv_and_increment name, value
#define END_ENUM( enum_name )
#else
#define BEGIN_ENUM( enum_name )         typedef enum enum_name {
#define DECLARE_ENUM_NEXT( name )       name,
#define DECLARE_ENUM_VAL( name, value ) name = value,
#define END_ENUM( enum_name )           } enum_name;
#endif

#define __stringify_1(x...)             #x
#define __stringify(x...)               __stringify_1(x)

#define SET_ALL_BITS( val )             ((val) = -1)
#define CLEAR_ALL_BITS( val )           ((val) = 0)
#define SET_BIT( val, i )               ((val) |= (1 << (i)))
#define GET_BIT( val, i )               (((val) >> i) & 1 )
#define CLEAR_BIT( val, i )             ((val) &= ~(1 << (i)))
#define SET_BITS_BY_MASK( val, mask )   ((val) |= (mask))
#define CLEAR_BITS_BY_MASK( val, mask ) ((val) &= ~(mask))
#define GET_BITS( val, i, n )           (((val) >> (i)) & ~(((1) ? -1 : (val)) << (n)))
#define bf_(val, msb, lsb)              GET_BITS(val, lsb, msb-lsb+1)
#define b__(val, bit_index)             bf_(val, bit_index, bit_index)

#define BIT_FIELD_SIZE( begin, end )    ((end) - (begin) + 1)

#define FIELD_MASK32( i, n )            ( ~(0xFFFFFFFFu << (n)) << (i) )
#define FIELD_MASK64( i, n )            ( ~(0xFFFFFFFFFFFFFFFFull << (n)) << (i) )


#endif /* end of include guard: MACROS_H */
