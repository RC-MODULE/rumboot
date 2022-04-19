#ifndef VL_API_H
#define VL_API_H

#include <stdint.h>

#define VL_API_VERSION 1

#define VL_OP_READ				1 //
#define VL_OP_WRITE				2 //
#define VL_OP_WAIT				3 //
#define VL_OP_UPTIME			4 //

#define VL_OP_ACCEPTED			0x20
#define VL_OP_STARTED			0x40
#define VL_OP_FINISHED			0x80

#define VL_OP_DISCONNECT		14
#define VL_OP_SHMEM_LIST		15
#define VL_OP_IRQ_LIST			16
#define VL_OP_IRQ_CONFIGURE		17
#define VL_OP_IRQ_STATUS		18
#define VL_OP_IRQ_ACK     		19


#define VL_MAX_DATA_LENGTH		256

struct vl_message {
	uint64_t addr;
	uint64_t value;
	uint8_t op;
	uint8_t  data_len;
	uint16_t sync;
	union{
		char data [VL_MAX_DATA_LENGTH];
		int  intdata[VL_MAX_DATA_LENGTH/32];
	};
	
};

struct vl_shmem{
	uint32_t 	id;
	char 	 	name[32];
	char 		ipc_key[128];
	uint64_t	sys_addr;
	uint64_t	size;
};

struct vl_irq{
	int dummy;
};

struct vl_instance {
	int sockfd;
	struct vl_shmem* shared_mem_list;
	struct vl_irq* irq_mem_list;
};

//std::ostream& operator << (std::ostream& out, const struct vl_message& msg);

//-------------- low-level communication verilator api --------------------
int vl_send(struct vl_instance *vl, const struct vl_message* message);
int vl_recv(struct vl_instance *vl, struct vl_message* message);
int vl_transaction(struct vl_instance *vl, const struct vl_message* send_message, struct vl_message* recv_message);

//--------------- main verilator clint-api -------------------------------------
struct vl_instance* vl_create(const char *host, uint16_t port); /* 0.0.0.0, localhost, /tmp/my.sock */
void 				vl_disconnect(struct vl_instance *vl);
struct vl_shmem* 	vl_shmem_list(struct vl_instance *vl);
void *				vl_shmem_map(struct vl_shmem* shmem);
int 				vl_shmem_unmap(void* ptr, uint64_t size);
void 				vl_iowrite32(struct vl_instance *vl, uint32_t const value, uint32_t const base_addr);
uint32_t 			vl_ioread32 (struct vl_instance *vl, uint32_t const base_addr);
int 				vl_destroy(struct vl_instance *vl);
struct vl_irq*   	vl_irq_list(struct vl_instance *vl);
int 				vl_irq_enable(struct vl_instance *vl, int irq, uint32_t flags);
int 				vl_irq_disable(struct vl_instance *vl, int irq);
int 				vl_irq_get_pending(struct vl_instance *vl);
int 				vl_irq_ack(struct vl_instance *vl, int irq);
uint32_t 			vl_wait(struct vl_instance *vl, uint32_t irq_mask, const uint64_t clocks);
uint64_t 			vl_get_uptime(struct vl_instance *vl);

//-------------------------------------------------------
void *vl_shmem_create(struct vl_shmem* shmem);




void vl_set_async_irq_callback(struct vl_instance *vl, 
	void (*vl_async_irq_handler)(struct vl_instance *vl, int irq, void *arg), 
	void *arg);

//-------------- test api --------------------
//static inline void 		iowrite32(uint32_t const value, uint32_t const base_addr);
//static inline uint32_t 	ioread32(uint32_t const base_addr);
//uint32_t rumboot_platform_request_file_ex(const char *plusarg, uint32_t addr, uint32_t bufsize);
//void 	 rumboot_platform_request_file(const char *plusarg, uint32_t addr);
//int64_t  rumboot_virt_to_phys(volatile void *addr);

#endif // !VL_API_H
