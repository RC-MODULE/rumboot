#ifndef DEVICES_PLB6MCIF2
#define DEVICES_PLB6MCIF2

/**
 * @brief Does some basic initialisation of PLB6MCIF2
 * 
 * The rest of the software interface is hidded because code
 * is ugly, untested pile of legacy crap
 * 
 * @param base_addr base address of the bridge
 * @param puaba puaba value
 */
void plb6mcif2_simple_init( uint32_t base_addr, const uint32_t puaba );



#endif // !DEVICES_PLB6MCIF2