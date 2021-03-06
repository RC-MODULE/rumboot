/**
 * PLB6PLB4 bridge implementation
 */

#include <platform/devices/p64.h>

void p64_get_configuration( uint32_t const base_addr, p64_cfg * cfg ) {
    uint32_t reg_value;
    reg_value = p64_dcr_read_P64_P64CR( base_addr );
    cfg->pri = ( reg_value >> P64CR_PRI ) & P64CR_PRI_MASK;
    cfg->spen = ( reg_value >> P64CR_SPEN ) & P64CR_SPEN_MASK;
    cfg->owen = ( reg_value >> P64CR_OWEN ) & P64CR_OWEN_MASK;
    cfg->ipc = ( reg_value >> P64CR_IPC ) & P64CR_IPC_MASK;
    cfg->ifmid0 = ( reg_value >> P64CR_IFMID0 ) & P64CR_IFMIDi_MASK;
    cfg->ifmid1 = ( reg_value >> P64CR_IFMID1 ) & P64CR_IFMIDi_MASK;
    cfg->ife0 = ( reg_value >> P64CR_IFE0 ) & P64CR_IFEi_MASK;
    cfg->ife1 = ( reg_value >> P64CR_IFE1 ) & P64CR_IFEi_MASK;
}

void p64_set_configuration( uint32_t const base_addr, p64_cfg * cfg ) {
    uint32_t reg_value;
    reg_value = 0;
    reg_value |= ( cfg->pri << P64CR_PRI );
    reg_value |= ( cfg->spen << P64CR_SPEN );
    reg_value |= ( cfg->owen << P64CR_OWEN );
    reg_value |= ( cfg->ipc << P64CR_IPC );
    reg_value |= ( cfg->ifmid0 << P64CR_IFMID0 );
    reg_value |= ( cfg->ifmid1 << P64CR_IFMID1 );
    reg_value |= ( cfg->ife0 << P64CR_IFE0 );
    reg_value |= ( cfg->ife1 << P64CR_IFE1 );
    p64_dcr_write_P64_P64CR( base_addr, reg_value );
    /*    TEST_ASSERT(p64_dcr_read_P64_P64CR(base_addr)==reg_value, "Error at setting plb6plb4 bridge configuration");*/
}

/* TODO remove this function */
void __attribute__((section(".text.rom"),optimize("omit-frame-pointer"))) p64_init_simple() {
    #include <platform/oi10/platform/devices.h>
    /*enable requests to boot rom mirror (0x00000010_00000000 - 0x0000001F_FFFFFFFF)*/
    p64_set_split_range_address(DCR_PLB6PLB4_0_BASE,0x00000010,0x0000001F);
}
