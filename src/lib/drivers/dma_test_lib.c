
//-----------------------------------------------------------------------------
//  This file contain DMA related functions, used for testing.
//  It includes next functions:
//    - rmace_set_desc
//    - change_endian
//    -
//
//-----------------------------------------------------------------------------
#include <stdint.h>

//-----------------------------------------------------------------------------
//  This function is for reversing byte order in one 32bit word
//  Results value is returned by function
//
//  Arguments:
//    - data_in
//
//-----------------------------------------------------------------------------
uint32_t change_endian (uint32_t data_in)
{
    uint32_t data_out = 0;
    // trace_hex(data_in);

    data_out = data_in << 24 & 0xff000000;
    data_out = data_out | (data_in << 8  & 0x00ff0000);
    data_out = data_out | (data_in >> 8  & 0x0000ff00);
    data_out = data_out | (data_in >> 24 & 0x000000ff);

    return data_out;
}

//-----------------------------------------------------------------------------
//  This function is for creating descriptor, used by RMACE DMA block.
//  There are 3 types of neccessary descriptors:
//    configuration descriptor
//    data descriptor
//    end descriptor
//  All of them must present for successful data transmition.
//
//  Arguments:
//    - sys_addr - address, where descriptor will be placed.
//        (Usually - one of memories - internal, DDR, ...)
//    - len - length of data, that must be trancsieved.
//        If 0, descriptor will be incorrect = end descriptor.
//    - desc_addr - address of data array, that must be trancsieved.
//    - act0
//    - interrupt
//    - valid
//    - endian
//
//-----------------------------------------------------------------------------
uint32_t rmace_set_desc
(
    uint32_t sys_addr     ,
    uint32_t len          ,
    uint32_t desc_addr    ,
    uint8_t  act0         ,
    uint8_t  interrupt    ,
    uint8_t  valid        ,
    uint8_t  endian
)
{
    // int i = 0;
    // uint32_t tmp = 0;
    uint32_t desc_data = 0;
    if (len == 0)
    {
        *(uint32_t*)(sys_addr) = 0x01000000; // "Stop" descriptor
    }
    else
    {
        desc_data = len << 6; // Set length
        desc_data = desc_data | 0x00000020;
        if (act0) desc_data = desc_data | 0x00000008;
        if (interrupt) desc_data = desc_data | 0x00000004;
        if (valid) desc_data = desc_data | 0x00000001;
        if (endian)
            *(uint32_t*)(sys_addr) = change_endian(desc_data);
        else
            *(uint32_t*)(sys_addr) = desc_data;
    }

    sys_addr += 4;

    if (len == 0)
    {
        *(uint32_t*)(sys_addr) = 0x00000000; // "Stop" descriptor
        *(uint32_t*)(sys_addr+4) = 0x00000000; // "Stop" descriptor
        sys_addr += 8;
        return sys_addr;
    }
    else
    {
        if (endian)
            *(uint32_t*)(sys_addr) = change_endian(desc_addr);
        else
            *(uint32_t*)(sys_addr) = desc_addr;
    }
    sys_addr += 4;

    return sys_addr;
}
