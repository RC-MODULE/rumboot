#include <stdlib.h>
#include <stdint.h>
#include <devices/greth_edcl.h>
#include <rumboot/boot.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>


#define EDCL_IP                             0x1C
#define EDCL_MAC_MSB                        0x28
#define EDCL_MAC_LSB                        0x2C


union u64 {
        uint8_t bytes[8];
        uint64_t dword;
        uint32_t words[2];
};

union u32 {
    uint8_t bytes[4];
    uint32_t word;
};

#define SWAP32(x) ((((x) & 0xff) << 24) | (((x) & 0xff00) << 8) | (((x) & 0xff0000) >> 8) | (((x) >> 24) & 0xff))

void greth_edcl_configure(uintptr_t base, int enable)
{
        uint32_t tmp = ioread32(base + 0);
        if (enable) {
                tmp &= ~(1<<14);
        } else {
                tmp |= (1<<14);            
        }
        iowrite32(tmp, base + 0);
}

void greth_dump_edcl_params(int i, uintptr_t base)
{
        union u64 macaddr;
        union u32 ip;
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        macaddr.dword = ioread64(base + EDCL_MAC_MSB);
        ip.word = ioread32(base + EDCL_IP);
#else
    macaddr.words[0] = SWAP32(ioread32(base + EDCL_MAC_MSB));
    macaddr.words[1] = SWAP32(ioread32(base + EDCL_MAC_LSB));
    ip.word = SWAP32(ioread32(base + EDCL_IP));
#endif
        dbg_boot(NULL, "GRETH%d EDCL MAC: %x:%x:%x:%x:%x:%x IP: %d.%d.%d.%d",
                i,
                macaddr.bytes[2],
                macaddr.bytes[3],
                macaddr.bytes[4],
                macaddr.bytes[5],
                macaddr.bytes[6],
                macaddr.bytes[7],
                ip.bytes[0],
                ip.bytes[1],
                ip.bytes[2],
                ip.bytes[3]
        );
}
