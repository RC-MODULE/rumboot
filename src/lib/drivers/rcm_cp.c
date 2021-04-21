#include <stdint.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <stdlib.h>
#include <rumboot/io.h>
#include <regs/rcm_cp.h>
#include <devices/rcm_cp.h>
#include <rumboot/rumboot.h>


void cp_instance_init(struct rcm_cp_instance *inst, uintptr_t base)
{
    inst->base = base;
}

void cp_attach_buffers(struct rcm_cp_instance *inst, void *rxbuf, void *txbuf, size_t buflen)
{
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
    void *rx = rumboot_malloc_from_named_heap(heapname, buflen);
    void *tx = rumboot_malloc_from_named_heap(heapname, buflen);
    cp_attach_buffers(inst, rx, tx, buflen);
}

void cp_start_sending(struct rcm_cp_instance *inst, const void *buf, size_t len)
{
    if (inst->txbuf) {
        if (inst->buflen < len) {
            rumboot_platform_panic("BUG: Please increase cp dma buffer");
        }
        memcpy(inst->txbuf, buf, len);
        buf = inst->txbuf;
    }
	iowrite32( len,  inst->base + RCM_CP_MAINCOUNTER_TR ); //set dma total data 512 byte
	iowrite32( rumboot_virt_to_dma(buf), inst->base + RCM_CP_ADDRESS_TR ); //dma destination atart address
	iowrite32( 0x0,  inst->base + RCM_CP_BIAS_TR );
	iowrite32( 0x0,  inst->base + RCM_CP_ROWCOUNTER_TR );
    /* Go! */
    iowrite32(0x1,   inst->base + RCM_CP_CSR_TR);
}


void cp_start_receiving(struct rcm_cp_instance *inst, void *buf, size_t len)
{
    if (inst->rxbuf) {
        if (inst->buflen < len) {
            rumboot_platform_panic("BUG: Please increase cp dma buffer");
        }
        inst->rxtargetbuf = buf;
        buf = inst->rxbuf;
    }

	iowrite32( len,  inst->base + RCM_CP_MAINCOUNTER_RCV ); //set dma total data 512 byte
	iowrite32( rumboot_virt_to_dma(buf), inst->base +  RCM_CP_ADDRESS_RCV ); //dma destination atart address
	iowrite32( 0x0,  inst->base + RCM_CP_BIAS_RCV );
	iowrite32( 0x0,  inst->base + RCM_CP_ROWCOUNTER_RCV );
	iowrite32( 0x0,  inst->base + RCM_CP_ADDRESSMODE_RCV );	
    /* Go! */
    iowrite32(0x1,   inst->base + RCM_CP_CSR_RCV);
}