#include <stdint.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <stdlib.h>
#include <rumboot/io.h>
#include <regs/rcm_cp.h>
#include <devices/rcm_cp.h>
#include <assert.h>
#include <rumboot/rumboot.h>

#define DEBUG

static void cp_check_buffer(const void *bufptr, size_t len)
{
    uintptr_t buf = (uintptr_t) bufptr;
    if (buf % 8 != 0) {
        rumboot_platform_panic("FATAL: cp buffer is not aligned");
    }

    if (len % 8 != 0) {
        rumboot_platform_panic("FATAL: cp buffer size is not aligned");
    }
}

void cp_instance_init(struct rcm_cp_instance *inst, uintptr_t base, uint32_t base_freq_khz)
{
    inst->base = base;
    inst->two_dimentional = 0;
    inst->base_freq_khz = base_freq_khz;
    cp_detach_buffers(inst);
}

int32_t cp_set_speed(struct rcm_cp_instance *inst, int32_t target_freq_khz)
{
    int best_div = 0;
    int32_t best_freq = 0;
    int32_t best_freq_delta = inst->base_freq_khz;

    for (int fdiv = 0; fdiv < 16; fdiv++) {
        int32_t freq = inst->base_freq_khz / (fdiv + 3);
        int32_t delta = abs(target_freq_khz - freq);
        if (delta < best_freq_delta) {
            best_div = fdiv; 
            best_freq = freq;
            best_freq_delta = delta;
        }
    }

    uint32_t spdreg = ioread32(inst->base + RCM_CP_PHYCONFIG) & 0xfffffff0;
    spdreg |= best_div;
    iowrite32(spdreg, inst->base + RCM_CP_PHYCONFIG);

    #ifdef DEBUG
    rumboot_printf("CP: div %d actual speed %d kHz requested %d kHz\n", best_div, best_freq, target_freq_khz);
    #endif
    return best_freq;
}

void cp_attach_buffers(struct rcm_cp_instance *inst, void *rxbuf, void *txbuf, size_t buflen)
{
    cp_check_buffer(rxbuf, buflen);
    cp_check_buffer(txbuf, buflen);
    inst->rxbuf = rxbuf;
    inst->txbuf = txbuf;
    inst->buflen = buflen;
}

void cp_detach_buffers(struct rcm_cp_instance *inst)
{
    inst->txbuf = NULL;
    inst->rxbuf = NULL;
    inst->buflen = 0;
}

void cp_attach_buffers_from_heap(struct rcm_cp_instance *inst, const char *heapname, size_t buflen)
{
    void *rx = rumboot_malloc_from_named_heap_aligned(heapname, buflen, 8);
    void *tx = rumboot_malloc_from_named_heap_aligned(heapname, buflen, 8);
    cp_attach_buffers(inst, rx, tx, buflen);
}

void cp_start_tx(struct rcm_cp_instance *inst, const void *buf, size_t len)
{
    if (inst->txbuf) {
        if (inst->buflen < len) {
            rumboot_platform_panic("BUG: Please increase cp dma buffer\n");
        }
        memcpy(inst->txbuf, buf, len);
        buf = inst->txbuf;
    } else {
        cp_check_buffer(buf, len);
    }
	iowrite32( len >> 3,  inst->base + RCM_CP_MAINCOUNTER_TR ); //set dma total data 512 byte
	iowrite32( rumboot_virt_to_dma(buf), inst->base + RCM_CP_ADDRESS_TR ); //dma destination atart address
	iowrite32( 0x0,  inst->base + RCM_CP_BIAS_TR );
	iowrite32( 0x0,  inst->base + RCM_CP_ROWCOUNTER_TR );
    iowrite32(inst->two_dimentional, inst->base + RCM_CP_ADDRESSMODE_TR);
    #ifdef __PPC__
    asm("msync");
    #endif
    /* Go! */
    iowrite32(0x1,   inst->base + RCM_CP_CSR_TR);
}

enum cp_status cp_tx_status(struct rcm_cp_instance *inst)
{
    uint32_t status = ioread32(inst->base + RCM_CP_CSR_TR);
#ifdef DEBUG
    rumboot_printf("[%x] tx status: %x\n", inst->base, status);
#endif
    if (status & (1<<1)) { /* cpl */
        iowrite32(status & (~(1<<1)), inst->base + RCM_CP_CSR_TR);
    }
    return status;
}

void cp_start_rx(struct rcm_cp_instance *inst, void *buf, size_t len)
{
    assert((len % 8) == 0);
    if (inst->rxbuf) {
        if (inst->buflen < len) {
            rumboot_platform_panic("BUG: Please increase cp dma buffer");
        }
        inst->rxtargetbuf = buf;
        buf = inst->rxbuf;
    } else {
        cp_check_buffer(buf, len);
    }

	iowrite32( len >> 3,  inst->base + RCM_CP_MAINCOUNTER_RCV ); //set dma total data 512 byte
	iowrite32( rumboot_virt_to_dma(buf), inst->base +  RCM_CP_ADDRESS_RCV ); //dma destination atart address
	iowrite32( 0x0,  inst->base + RCM_CP_BIAS_RCV );
	iowrite32( 0x0,  inst->base + RCM_CP_ROWCOUNTER_RCV );
	iowrite32( 0x0,  inst->base + RCM_CP_ADDRESSMODE_RCV );	
    iowrite32(inst->two_dimentional, inst->base + RCM_CP_ADDRESSMODE_RCV);

    /* Go! */
    iowrite32(0x1,   inst->base + RCM_CP_CSR_RCV);
}

enum cp_status cp_rx_status(struct rcm_cp_instance *inst)
{
    uint32_t status = ioread32(inst->base + RCM_CP_CSR_RCV);
#ifdef DEBUG
    rumboot_printf("[%x] rx status: %x\n", inst->base, status);
#endif
    if (status & (1<<1)) { /* cpl */
        iowrite32(status & (~(1<<1)), inst->base + RCM_CP_CSR_RCV);
        #ifdef __PPC__
        asm("msync");
        #endif
    }
    return status;
}

int cp_wait(struct rcm_cp_instance *inst, bool rx, bool tx, uint32_t timeout_us)
{
    uint32_t start = rumboot_platform_get_uptime();
    while (rumboot_platform_get_uptime() - start < timeout_us) {
        if (rx && (cp_rx_status(inst) != 0))
            continue;
        if (tx && (cp_tx_status(inst) != 0))
            continue;
        return 0;
    }
    return -1;
}
