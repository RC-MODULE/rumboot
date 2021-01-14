#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <arch/ppc_476fp_mmu.h>

struct addr_map {
    uint32_t    virt;
    MEM_WINDOW  window;
    int64_t     phy;
};

int main() {
    struct addr_map addr[ 10 ];
    rumboot_printf( "Printing physical addresses!\n" );

    addr[ 0 ].virt = 0xFFFF0000; addr[ 0 ].window = MEM_WINDOW_0;
    addr[ 1 ].virt = 0x80000001; addr[ 1 ].window = MEM_WINDOW_0;
    addr[ 2 ].virt = 0x80020002; addr[ 2 ].window = MEM_WINDOW_0;
    addr[ 3 ].virt = 0xC0000003; addr[ 3 ].window = MEM_WINDOW_1;
    addr[ 4 ].virt = 0xC0020004; addr[ 4 ].window = MEM_WINDOW_0;
    addr[ 5 ].virt = 0xC0100005; addr[ 5 ].window = MEM_WINDOW_0;
    addr[ 6 ].virt = 0xC0300006; addr[ 6 ].window = MEM_WINDOW_0;
    addr[ 7 ].virt = 0x10000007; addr[ 7 ].window = MEM_WINDOW_0;
    addr[ 8 ].virt = 0x20000008; addr[ 8 ].window = MEM_WINDOW_2;
    addr[ 9 ].virt = 0x10000009; addr[ 9 ].window = MEM_WINDOW_10;
    for( int i = 0; i < ARRAY_SIZE( addr ); i++ ) {
        set_mem_window( addr[ i ].window );
        addr[ i ].phy = get_physical_addr( addr[ i ].virt, 0 );
    }

    for( int i = 0; i < ARRAY_SIZE( addr ); i++ ) {
        rumboot_printf( "addr[ %d ]: virt = 0x%x, window = %x, phy = 0x%x_%x\n",
                               i,             addr[ i ].virt,
                                                           addr[ i ].window,
                                                                       ( uint32_t )( addr[ i ].phy >> 32 ),
                                                                          ( uint32_t )( addr[ i ].phy & 0xFFFFFFFF ) );
    }

    return 0;
}
