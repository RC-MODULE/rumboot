
//-----------------------------------------------------------------------------
//  This program is for PCIe outbound Address Translator testing
//    
//    
//    Test includes:
//    - turn PCIe On
//    - turn DDR0 On
//    - turn DDR1 On
//    - setting outbound Address Translator (all regions)
//    - setting inbound Address Translator (all regions)
//    - repeat for few regions combinations
//        - write data (ARM -> outb AT -> PCIe -> inb AT -> DDR0)
//    - repeat for same few regions combinations
//        - read and check data (ARM <- outb AT <- PCIe <- inb AT <- DDR0)
//        - direct read and check data (ARM <- DDR0) - its difficult, TBD?
//    
//    Test duration (RTL, short_test_for_sim): < TODO
//    Test duration (RTL): < 
//-----------------------------------------------------------------------------

#include <rumboot/pcie_test_lib.h>
#include <rumboot/ddr_test_lib.h>
#include <rumboot/io.h>

#if short_test_for_sim == 1
#define check_amount 0x80
#else
#define check_amount 0x1000
#endif

#define random_seed_1 300
#define random_seed_2 400
#define random_seed_3 500
    

#define TAUSWORTHE(s,a,b,c,d) ((s&c)<<d) ^ (((s <<a) ^ s)>>b)

void at_slv_mst_config ()
{
    
    rgADDR_TRANS_SLV_ctrl = 0;
    rgADDR_TRANS_MST_ctrl = 0;
    
    for (volatile uint32_t i = 0; i < 128; i++)
    {
        iowrite32 ((0x40000000 + 0x800000 * i) | 0x1   , ADDR_TRANS_SLV_BASE + ((i * 3 + 1) << 2)) ;
        iowrite32 (0x407FFFFC + 0x800000 * i           , ADDR_TRANS_SLV_BASE + ((i * 3 + 2) << 2)) ;
        iowrite32 ((~(0x800000 * (2 * i + 1))) + 1     , ADDR_TRANS_SLV_BASE + ((i * 3 + 3) << 2)) ;
    }
    for (volatile uint32_t i = 1; i < 9; i++)
    {
        iowrite32 ((0x00000000 + 0x8000000 * (i - 1)) | 0x1  , ADDR_TRANS_MST_BASE + ((i * 3 + 1) << 2)) ;
        iowrite32 ((0x07FFFFFC + 0x8000000 * (i - 1))        , ADDR_TRANS_MST_BASE + ((i * 3 + 2) << 2)) ;
        iowrite32 ((~(0x8000000 * (3 * (i - 1) + 2))) + 1    , ADDR_TRANS_MST_BASE + ((i * 3 + 3) << 2)) ;
    }
    iowrite32 (0x00000000 | 0x1  , ADDR_TRANS_MST_BASE + ((0 * 3 + 1) << 2)) ;
    iowrite32 (0x0003FFFC        , ADDR_TRANS_MST_BASE + ((0 * 3 + 2) << 2)) ;
    iowrite32 (0x00040000        , ADDR_TRANS_MST_BASE + ((0 * 3 + 3) << 2)) ;
}

void data_write ()
{
    uint32_t s1;
    uint32_t s2;
    uint32_t s3;
    
    uint32_t waddr;
    
    s1 = random_seed_1;
    s2 = random_seed_2;
    s3 = random_seed_3;

    for (volatile uint32_t i = 0; i < check_amount; i++)
    {
        s1 = TAUSWORTHE (s1,13, 19, 4294967294UL, 12);
        s2 = TAUSWORTHE (s2, 2, 25, 4294967288UL,  4);
        s3 = TAUSWORTHE (s3, 3, 11, 4294967280UL, 17);
        waddr = ((s1 ^ s2 ^ s3) & 0x7FFFFFFC) | 0x40000000;
        
        iowrite32 (waddr, 0xC0000000 ) ;
        iowrite32 (0,     waddr - 0x4) ;
        iowrite32 (0,     waddr + 0x4) ;
        iowrite32 (waddr, waddr      ) ;
    }
}
    
uint32_t data_read_check ()
{
    uint32_t s1;
    uint32_t s2;
    uint32_t s3;
    
    uint32_t waddr;
    uint32_t direct_waddr;
    
    s1 = random_seed_1;
    s2 = random_seed_2;
    s3 = random_seed_3;
    
    for (volatile uint32_t i = 0; i < check_amount; i++)
    {
        s1 = TAUSWORTHE (s1,13, 19, 4294967294UL, 12);
        s2 = TAUSWORTHE (s2, 2, 25, 4294967288UL,  4);
        s3 = TAUSWORTHE (s3, 3, 11, 4294967280UL, 17);
        waddr = ((s1 ^ s2 ^ s3) & 0x7FFFFFFC) | 0x40000000;
        
        if (ioread32 (waddr) != waddr)
            return -1;
        
        direct_waddr = waddr - 0x00800000 - 2 * ((waddr - 0x40000000) & 0xFF800000);
        if (direct_waddr < 0x00040000)
            direct_waddr += 0x00040000;
        else
            direct_waddr = direct_waddr - 0x10000000 - 3 * (direct_waddr & 0xF8000000);
            
        // iowrite32(direct_waddr, 0xC0000000) ;
        if (ioread32 (direct_waddr) != waddr)
            return -1;
    }
    return 0;
}

uint32_t main ()
{
    crg_ddr_init (0x63 ,0x0);
    if (ddr0_ddr1_init () != 0)
        return -1;
    if (pcie_turn_on_with_options_ep (1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) != 0)
        return -2;
    
    at_slv_mst_config    ();
    data_write           ();
    if (data_read_check () != 0)
        return -3;
    
    return 0;
}
