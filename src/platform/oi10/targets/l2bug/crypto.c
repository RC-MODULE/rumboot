
#include "crypto.h"

// inline unsigned S( unsigned short x )
unsigned S( unsigned short x )
{
    static const unsigned char s[4][16] =
    {
        {0x4, 0xA, 0x9, 0x2, 0xD, 0x8, 0x0, 0xE, 0x6, 0xB, 0x1, 0xC, 0x7, 0xF, 0x5, 0x3},
        {0xE, 0xB, 0x4, 0xC, 0x6, 0xD, 0xF, 0xA, 0x2, 0x3, 0x8, 0x1, 0x0, 0x7, 0x5, 0x9},
        {0x5, 0x8, 0x1, 0xD, 0xA, 0x3, 0x4, 0x2, 0xE, 0xF, 0xC, 0x7, 0x6, 0x0, 0x9, 0xB},
        {0x7, 0xD, 0xA, 0x1, 0x0, 0x8, 0x9, 0xF, 0xE, 0x4, 0x6, 0xC, 0xB, 0x2, 0x5, 0x3}
    };
    union
    {
        unsigned short data;
        struct
        {
            unsigned short a: 4, b: 4, c: 4, d: 4;
        };
    } y;
    y.data = x;
    y.a = s[0][y.a];
    y.b = s[1][y.b];
    y.c = s[2][y.c];
    y.d = s[3][y.d];
    return y.data;
}


unsigned prand( unsigned x, unsigned key, unsigned len )
{
    unsigned ln = len / 8 * 4, l, r, c;

    unsigned mask = 0xF, fmask = 0xFFFFFFFF >> ( 32 - len );

    if ( len >= 16 )
    {
        mask = 0xFF;
    }

    if ( len >= 24 )
    {
        mask = 0xFFF;
    }

    if ( len >= 32 )
    {
        mask = 0xFFFF;
    }

    key &= fmask;

    for ( unsigned u = 7; u; u-- )
    {
        l = x & mask;
        r = ( x >> ( len - ln ) )&mask;
        c = x & ~( mask | ( mask << ( len - ln ) ) );
        l = S( l )&mask;
        x = ( l ^ r ) << ( len - ln ) | r | c;
        x ^= key;
        x &= fmask;
        x = ( x << 5 ) | ( x >> ( len - 5 ) );
        x &= fmask;
    }

    return x;
}
