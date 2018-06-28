
//-----------------------------------------------------------------------------
//  This program is for testing ext_irq_gen registers
//    Test includes:
//    - checking registers reset values
//    - checking access to some registers
//    - checking bandwidth with some registers
//    - checking independence of mapping and vector registers
//    
//    
//    Test duration (RTL): < 600us
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdlib.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>

#include <platform/devices.h>
#include <regs/regs_pcie.h>

#define reset_EXT_IRQ_GEN_Ctrl                     0x00000021
// #define reset_EXT_IRQ_GEN_Global_IRQ_Status_l      0x00000000
//  We have interruption from debug system immediately after start
#define reset_EXT_IRQ_GEN_Global_IRQ_Status_l      0x00000002
#define reset_EXT_IRQ_GEN_Global_IRQ_Status_h      0x00000000
#define reset_EXT_IRQ_GEN_Global_IRQ_Mask_l        0xFFFFFFFF
#define reset_EXT_IRQ_GEN_Global_IRQ_Mask_h        0xFFFFFFFF
#define reset_EXT_IRQ_GEN_Global_IRQ_Map_0         ((3  << 18) | (2  << 12) | (1  << 6) | (0  << 0))
#define reset_EXT_IRQ_GEN_Global_IRQ_Map_1         ((7  << 18) | (6  << 12) | (5  << 6) | (4  << 0))
#define reset_EXT_IRQ_GEN_Global_IRQ_Map_2         ((11 << 18) | (10 << 12) | (9  << 6) | (8  << 0))
#define reset_EXT_IRQ_GEN_Global_IRQ_Map_3         ((15 << 18) | (14 << 12) | (13 << 6) | (12 << 0))
#define reset_EXT_IRQ_GEN_Global_IRQ_Map_4         ((19 << 18) | (18 << 12) | (17 << 6) | (16 << 0))
#define reset_EXT_IRQ_GEN_Global_IRQ_Map_5         ((23 << 18) | (22 << 12) | (21 << 6) | (20 << 0))
#define reset_EXT_IRQ_GEN_Global_IRQ_Map_6         ((27 << 18) | (26 << 12) | (25 << 6) | (24 << 0))
#define reset_EXT_IRQ_GEN_Global_IRQ_Map_7         ((31 << 18) | (30 << 12) | (29 << 6) | (28 << 0))
#define reset_EXT_IRQ_GEN_Global_IRQ_Map_8         ((35 << 18) | (34 << 12) | (33 << 6) | (32 << 0))
#define reset_EXT_IRQ_GEN_Global_IRQ_Map_9         ((39 << 18) | (38 << 12) | (37 << 6) | (36 << 0))
#define reset_EXT_IRQ_GEN_Global_IRQ_Map_10        ((43 << 18) | (42 << 12) | (41 << 6) | (40 << 0))
#define reset_EXT_IRQ_GEN_Global_IRQ_Map_11        ((47 << 18) | (46 << 12) | (45 << 6) | (44 << 0))
#define reset_EXT_IRQ_GEN_Global_IRQ_Map_12        ((51 << 18) | (50 << 12) | (49 << 6) | (48 << 0))
#define reset_EXT_IRQ_GEN_Global_IRQ_Map_13        ((55 << 18) | (54 << 12) | (53 << 6) | (52 << 0))
#define reset_EXT_IRQ_GEN_Global_IRQ_Map_14        ((59 << 18) | (58 << 12) | (57 << 6) | (56 << 0))
#define reset_EXT_IRQ_GEN_Global_IRQ_Map_15        ((63 << 18) | (62 << 12) | (61 << 6) | (60 << 0))
// #define reset_EXT_IRQ_GEN_v0_Message_Address       0x00000000
#define reset_EXT_IRQ_GEN_v0_Message_Upper_Address 0x00000000
// #define reset_EXT_IRQ_GEN_v0_Message_Data          0x0000_0000
#define reset_EXT_IRQ_GEN_v0_Vector_Control        0x00000001
#define reset_EXT_IRQ_GEN_Global_PBA_Entries_l     0x00000000
#define reset_EXT_IRQ_GEN_Global_PBA_Entries_h     0x00000000
#define reset_EXT_IRQ_GEN_Global_AXI_param         (1 << 6)


#define reset_default           0x00000000

//---------------------------------------------------------------------
//  Check reset values of all registers
//---------------------------------------------------------------------
uint32_t reset_values_test ()
{
    // uint32_t prdata;

    // prdata = ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msix_ctrl);
    // iowrite32 (prdata & (~0xC0000000), PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msix_ctrl);
    
    for (uint32_t i = 0; i <= 3; i++)
    {
        // if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Ctrl                     ) != reset_EXT_IRQ_GEN_Ctrl                     ) return -1;
        // if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_l      ) != reset_EXT_IRQ_GEN_Global_IRQ_Status_l      ) return -1;
        // if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_h      ) != reset_EXT_IRQ_GEN_Global_IRQ_Status_h      ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_l        ) != reset_EXT_IRQ_GEN_Global_IRQ_Mask_l        ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_h        ) != reset_EXT_IRQ_GEN_Global_IRQ_Mask_h        ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_0         ) != reset_EXT_IRQ_GEN_Global_IRQ_Map_0         ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_1         ) != reset_EXT_IRQ_GEN_Global_IRQ_Map_1         ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_2         ) != reset_EXT_IRQ_GEN_Global_IRQ_Map_2         ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_3         ) != reset_EXT_IRQ_GEN_Global_IRQ_Map_3         ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_4         ) != reset_EXT_IRQ_GEN_Global_IRQ_Map_4         ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_5         ) != reset_EXT_IRQ_GEN_Global_IRQ_Map_5         ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_6         ) != reset_EXT_IRQ_GEN_Global_IRQ_Map_6         ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_7         ) != reset_EXT_IRQ_GEN_Global_IRQ_Map_7         ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_8         ) != reset_EXT_IRQ_GEN_Global_IRQ_Map_8         ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_9         ) != reset_EXT_IRQ_GEN_Global_IRQ_Map_9         ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_10        ) != reset_EXT_IRQ_GEN_Global_IRQ_Map_10        ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_11        ) != reset_EXT_IRQ_GEN_Global_IRQ_Map_11        ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_12        ) != reset_EXT_IRQ_GEN_Global_IRQ_Map_12        ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_13        ) != reset_EXT_IRQ_GEN_Global_IRQ_Map_13        ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_14        ) != reset_EXT_IRQ_GEN_Global_IRQ_Map_14        ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_15        ) != reset_EXT_IRQ_GEN_Global_IRQ_Map_15        ) return -1;
        for (uint32_t j = 0; j <= 63; j++)
        {
            // if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Address       + (j << 4)) != reset_EXT_IRQ_GEN_v0_Message_Address       ) return -1;  //  This has X by default
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Upper_Address + (j << 4)) != reset_EXT_IRQ_GEN_v0_Message_Upper_Address ) return -1;
            // if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Data          + (j << 4)) != reset_EXT_IRQ_GEN_v0_Message_Data          ) return -1;  //  This has X by default
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Vector_Control        + (j << 4)) != reset_EXT_IRQ_GEN_v0_Vector_Control        ) return -1;
        }
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_PBA_Entries_l     ) != reset_EXT_IRQ_GEN_Global_PBA_Entries_l     ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_PBA_Entries_h     ) != reset_EXT_IRQ_GEN_Global_PBA_Entries_h     ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_AXI_param         ) != reset_EXT_IRQ_GEN_Global_AXI_param         ) return -1;
    }
    
    return 0;
}

//---------------------------------------------------------------------
//  Check writing and reading from all registers
//---------------------------------------------------------------------
uint32_t access_test ()
{
    for (uint32_t i = 0; i <= 3; i++)
    {
        //-----------------------------------------------------------------
        //  Check write all 1
        //-----------------------------------------------------------------
        // iowrite32 (0xFFFFFFFF, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Ctrl);
        // if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Ctrl) != 0x39) return -1;
        
        // iowrite32 (0xFFFFFFFF, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_l);
        // if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_l) != 0x00000002) return -1;
        
        // iowrite32 (0xFFFFFFFF, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_h);
        // if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_h) != 0x00000000) return -1;
        
        iowrite32 (0xFFFFFFFF, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_l);
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_l) != 0xFFFFFFFF) return -1;
        
        iowrite32 (0xFFFFFFFF, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_h);
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_h) != 0xFFFFFFFF) return -1;
        
        for (uint32_t j = 0; j <= 15; j++)
        {
            iowrite32 (0xFFFFFFFF, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_0 + (j << 2));
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_0 + (j << 2)) != 0x00FFFFFF) return -1;
        }
        for (uint32_t j = 0; j <= 63; j++)
        {
            iowrite32 (0xFFFFFFFF, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Address       + (j << 4));
            iowrite32 (0xFFFFFFFF, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Upper_Address + (j << 4));
            iowrite32 (0xFFFFFFFF, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Data          + (j << 4));
            iowrite32 (0xFFFFFFFF, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Vector_Control        + (j << 4));
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Address       + (j << 4)) != 0xFFFFFFFC ) return -1;
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Upper_Address + (j << 4)) != 0x00000000 ) return -1;
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Data          + (j << 4)) != 0xFFFFFFFF ) return -1;
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Vector_Control        + (j << 4)) != 0x00000001 ) return -1;
        }
        
        iowrite32 (0xFFFFFFFF, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_PBA_Entries_l);
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_PBA_Entries_l) != 0x00000000) return -1;
        
        iowrite32 (0xFFFFFFFF, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_PBA_Entries_h);
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_PBA_Entries_h) != 0x00000000) return -1;
        
        iowrite32 (0xFFFFFFFF, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_AXI_param);
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_AXI_param) != 0x000001FF) return -1;
        
        //-----------------------------------------------------------------
        //  Check write all 0
        //-----------------------------------------------------------------
        // iowrite32 (0x00000000, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Ctrl);
        // if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Ctrl) != 0x20) return -1;
        
        // iowrite32 (0x00000000, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_l);
        // //  We have interruption from debug system immediately after start
        // //  It wasnt elaborated, so set after all interruptions clearing
        // if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_l) != 0x00000002) return -1;
        
        // iowrite32 (0x00000000, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_h);
        // if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_h) != 0x00000000) return -1;
        
        iowrite32 (0x00000000, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_l);
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_l) != 0x00000000) return -1;
        
        iowrite32 (0x00000000, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_h);
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_h) != 0x00000000) return -1;
        
        for (uint32_t j = 0; j <= 15; j++)
        {
            iowrite32 (0x00000000, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_0 + (j << 2));
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_0 + (j << 2)) != 0x00000000) return -1;
        }
        for (uint32_t j = 0; j <= 63; j++)
        {
            iowrite32 (0x00000000, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Address       + (j << 4));
            iowrite32 (0x00000000, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Upper_Address + (j << 4));
            iowrite32 (0x00000000, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Data          + (j << 4));
            iowrite32 (0x00000000, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Vector_Control        + (j << 4));
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Address       + (j << 4)) != 0x00000000 ) return -1;
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Upper_Address + (j << 4)) != 0x00000000 ) return -1;
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Data          + (j << 4)) != 0x00000000 ) return -1;
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Vector_Control        + (j << 4)) != 0x00000000 ) return -1;
        }
        
        iowrite32 (0x00000000, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_PBA_Entries_l);
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_PBA_Entries_l) != 0x00000000) return -1;
        
        iowrite32 (0x00000000, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_PBA_Entries_h);
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_PBA_Entries_h) != 0x00000000) return -1;
        
        iowrite32 (0x00000000, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_AXI_param);
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_AXI_param) != 0x00000000) return -1;
    }
    
    
    return 0;
}

//---------------------------------------------------------------------
//  Check writes, close to each other.
//    Close reads cant be checked with assembler inline
//---------------------------------------------------------------------
uint32_t bandwidth_test ()
{
    //-----------------------------------------------------------------
    //  Bandwidth with mapping registers
    //-----------------------------------------------------------------
    for (uint32_t i = 0; i <= 3; i++)
    {
        asm (
            "PUSH {R0-R7}\n\t"
            
            "MOV R0, %0\n\t"
            "LDR R1, =0xFFFFFFFF\n\t"
            "LDR R2, =0xFFFFFFFF\n\t"
            "LDR R3, =0xFFFFFFFF\n\t"
            "LDR R4, =0xFFFFFFFF\n\t"
            "STMIA R0!, {R1, R2, R3, R4}\n\t"
            "STMIA R0!, {R1, R2, R3, R4}\n\t"
            "STMIA R0!, {R1, R2, R3, R4}\n\t"
            "STMIA R0!, {R1, R2, R3, R4}\n\t"
            
            "POP {R0-R7}\n\t"
            :
            : "r" (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_0)
            :
        );
        for (uint32_t j = 0; j <= 15; j++)
        {
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_0 + (j << 2)) != 0x00FFFFFF) return -1;
        }
        
        asm (
            "PUSH {R0-R7}\n\t"
            
            "MOV R0, %0\n\t"
            "LDR R1, =0xAAAAAAAA\n\t"
            "LDR R2, =0xAAAAAAAA\n\t"
            "LDR R3, =0xAAAAAAAA\n\t"
            "LDR R4, =0xAAAAAAAA\n\t"
            "STMIA R0!, {R1, R2, R3, R4}\n\t"
            "STMIA R0!, {R1, R2, R3, R4}\n\t"
            "STMIA R0!, {R1, R2, R3, R4}\n\t"
            "STMIA R0!, {R1, R2, R3, R4}\n\t"
            
            "POP {R0-R7}\n\t"
            :
            : "r" (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_0)
            :
        );
        for (uint32_t j = 0; j <= 15; j++)
        {
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_0 + (j << 2)) != 0x00AAAAAA) return -1;
        }
    }
    
    //-----------------------------------------------------------------
    //  Bandwidth with MSIX vectors
    //-----------------------------------------------------------------
    for (uint32_t i = 0; i <= 3; i++)
    {
        asm (
            "PUSH {R0-R7}\n\t"
            
            "MOV R0, %0\n\t"
            "LDR R1, =0xFFFFFFFF\n\t"
            "LDR R2, =0xFFFFFFFF\n\t"
            "LDR R3, =0xFFFFFFFF\n\t"
            "LDR R4, =0xFFFFFFFF\n\t"
            "STMIA R0!, {R1, R2, R3, R4}\n\t"
            "STMIA R0!, {R1, R2, R3, R4}\n\t"
            "STMIA R0!, {R1, R2, R3, R4}\n\t"
            "STMIA R0!, {R1, R2, R3, R4}\n\t"
            
            "POP {R0-R7}\n\t"
            :
            : "r" (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Address)
            :
        );
        for (uint32_t j = 0; j <= 3; j++)
        {
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Address       + (j << 4)) != 0xFFFFFFFC ) return -1;
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Upper_Address + (j << 4)) != 0x00000000 ) return -1;
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Data          + (j << 4)) != 0xFFFFFFFF ) return -1;
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Vector_Control        + (j << 4)) != 0x00000001 ) return -1;
        }
        
        asm (
            "PUSH {R0-R7}\n\t"
            
            "MOV R0, %0\n\t"
            "LDR R1, =0x00000000\n\t"
            "LDR R2, =0x00000000\n\t"
            "LDR R3, =0x00000000\n\t"
            "LDR R4, =0x00000000\n\t"
            "STMIA R0!, {R1, R2, R3, R4}\n\t"
            "STMIA R0!, {R1, R2, R3, R4}\n\t"
            "STMIA R0!, {R1, R2, R3, R4}\n\t"
            "STMIA R0!, {R1, R2, R3, R4}\n\t"
            
            "POP {R0-R7}\n\t"
            :
            : "r" (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Address)
            :
        );
        for (uint32_t j = 0; j <= 3; j++)
        {
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Address       + (j << 4)) != 0x00000000 ) return -1;
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Upper_Address + (j << 4)) != 0x00000000 ) return -1;
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Data          + (j << 4)) != 0x00000000 ) return -1;
            if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Vector_Control        + (j << 4)) != 0x00000000 ) return -1;
        }
    }
    
    return 0;
}

//---------------------------------------------------------------------
//  Check, that "generated" registers and memory has correct access
//---------------------------------------------------------------------
uint32_t independence_test ()
{
    for (uint32_t j = 0; j <= 15; j++)
    {
        iowrite32 (j, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_0 + (j << 2));
    }
    for (uint32_t j = 0; j <= 15; j++)
    {
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Map_0 + (j << 2)) != j) return -1;
    }
    
    for (uint32_t j = 0; j <= 63; j++)
    {
        iowrite32 ((j << 4) + 0x1000, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Address       + (j << 4));
        iowrite32 ((j << 4) + 0x2000, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Upper_Address + (j << 4));
        iowrite32 ((j << 4) + 0x3000, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Data          + (j << 4));
        iowrite32 ((j << 4) + 0x4000, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Vector_Control        + (j << 4));
    }
    for (uint32_t j = 0; j <= 63; j++)
    {
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Address       + (j << 4)) != ((j << 4) + 0x1000)            ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Upper_Address + (j << 4)) !=   0x00000000                   ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Data          + (j << 4)) != ((j << 4) + 0x3000)            ) return -1;
        if (ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Vector_Control        + (j << 4)) != (((j << 4) + 0x3000) & 0x1)    ) return -1;
    }
        
    return 0;
}

//-------------------------------------------------------------------------
//  Test sequence
//-------------------------------------------------------------------------
uint32_t main ()
{
    if (reset_values_test () != 0)
        return -1;

    if (access_test () != 0)
        return -2;

    if (bandwidth_test () != 0)
        return -3;
    
    if (independence_test () != 0)
        return -4;

    return 0;
}

