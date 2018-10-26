#include <stdint.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <arch/irq_macros.h>
#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>


__attribute__((no_instrument_function)) void rumboot_platform_event_raise(
        enum rumboot_simulation_event const event,
        uint32_t const * const data, uint32_t const len
) {
    uint32_t const max_event_params_n = 8;
    if( len > max_event_params_n ) rumboot_platform_panic( "Too many event params. Max: %d, set: %d", max_event_params_n, len );

    RUMBOOT_ATOMIC_BLOCK() {
        switch( len ) {
        case    8: spr_write( SPR_USPRG0,   data[7] );
        case    7: spr_write( SPR_SPRG8,    data[6] );
        case    6: spr_write( SPR_SPRG7,    data[5] );
        case    5: spr_write( SPR_SPRG6,    data[4] );
        case    4: spr_write( SPR_SPRG5,    data[3] );
        case    3: spr_write( SPR_SPRG4,    data[2] );
        case    2: spr_write( SPR_SPRG3,    data[1] );
        case    1: spr_write( SPR_SPRG2,    data[0] );
        break;
        }
        spr_write( SPR_SPRG1, event );
    }
}

 __attribute__((no_instrument_function)) enum rumboot_simulation_event rumboot_platform_event_get( volatile uint32_t **data ) {
    while(!rumboot_platform_runtime_info->in.opcode) {}

    *data = rumboot_platform_runtime_info->in.data;
    return rumboot_platform_runtime_info->in.opcode;
}

__attribute__((no_instrument_function)) void rumboot_platform_event_clear()
{
    rumboot_platform_runtime_info->in.opcode = 0;
}
