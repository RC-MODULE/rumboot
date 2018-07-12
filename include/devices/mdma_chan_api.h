#ifndef __MDMA_CHAN_H__
#define __MDMA_CHAN_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <algo/list.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>

#define DEBUG_MDMA_CHAN

#ifdef DEBUG_MDMA_CHAN
#define chan_print_dbg(x, format, ...) \
	do { \
		if (x) \
			rumboot_printf("%s#%d: " format "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
		else \
			rumboot_printf("\t" format "\n", ##__VA_ARGS__); \
	} while (0)
#else
#define chan_print_dbg(x, format, ...);
#endif

static inline void __attribute__((no_instrument_function)) mdma_chan_write32(uint32_t _value, uint32_t _reg)
{
	chan_print_dbg(0, "write [0x%x]<=0x%x", _reg, _value);

	iowrite32(_value, _reg);

	return;
}

static inline uint32_t __attribute__((no_instrument_function)) mdma_chan_read32(uint32_t _reg)
{
	uint32_t _value = ioread32(_reg);

	chan_print_dbg(0, "read [0x%x]==0x%x", _reg, _value);

	return _value;
}

/*
 * TODO:
 * 	Define format of commentaries to doxygen
 */
 
/*
 * Descriptors of group must be mapped in physically adjacent
 * blocks of memory.
 *
 *     Group1              Group2
 * +-----------+       +-----------+
 * | data desc |  +--->| data desc |
 * +-----------+  |    +-----------+
 * | data desc |  |    |    ...    |
 * +-----------+  |    +-----------+
 * |    ...    |  |    | data desc |
 * +-----------+  |    | (StopBit) |
 * | link desc |--+    +-----------+
 * +-----------+
 *
 * Descriptor doesn't cross 4KB boundary. Descriptor address
 * must be aligned on 8 or 16 bytes boundary (descriptor size).
 *
 * Burst - a number of bytes which can be read or write by
 *      one bus access. The burst size is "bus access width" *
 *      "bus access length". The burst size must be aligned
 *      on the boundary of 4 or 8 bytes (burst width).
 * Segment - a contiguous collection of bursts.
 *	The segment address must be aligned on the burst width.
 *      The segment size is fully defined by one descriptor, and
 *      must be less than 16KB/64MB (add info) or 64KB * "number
 *      of string" (~1GB/4GB (add info)). 
 * Transfer - a collection of segments (be it contiguous
 *      or not). MDMA uses groups of descriptors to
 *      perform transfer.
 *
 *        |-width-|     Segment
 *      - +-------+    +-------+   +-------+
 *      | |       |    |       |   |       |
 * length | Burst |    | Burst |   | Burst |
 *      | |       |    |       |   |       |
 *      - +-------+    +-------+   +-------+
 *        |       |    |       | . |       |
 *        |  ...  |    |  ...  | . |  ...  |
 *        |       |    |       | . |       |
 *        +-------+    +-------+   +-------+
 *        |       |    |       |   |       |
 *        | Burst |    | Burst |   | Burst |
 *        |       |    |       |   |       |
 *        +-------+    +-------+   +-------+
 *        <------------ Transfer ---------->
 *
 */
 
/* MDMA Descriptor flags_length field */
#define MDMA_DESC_OWNERSHIP		(1 << 31)
#define MDMA_DESC_LINK			(1 << 30)
enum mdma_desc_type {
	DESC_DATA	= 0,
	DESC_LINK	= 1
};
#define MDMA_DESC_INTERRUPT		(1 << 29)
#define MDMA_DESC_STOP			(1 << 28)
#define MDMA_DESC_INCREMENT		(1 << 27)
#define MDMA_DESC_ERROR			(1 << 26)
#define MDMA_DESC_CUSTOM_W(x)		(((x) & 0xFFF) << 14)
#define MDMA_DESC_CUSTOM_R(x)		(((x) >> 14) & 0xFFF)
#define MDMA_DESC_LENGTH(x)		((x) & 0x3FFF)
#define MDMA_DESC_LENGTH_EXT(x)		((x) & 0x3FFFFFF)

#define MDMA_LENGTH_LIMIT(ext)		((ext) ? 0x4000000 : 0x4000)

struct __normal_desc {
	uint32_t memptr;
	uint32_t flags_length;
} __attribute__ ((packed, aligned(8)));

/* MDMA Descriptor usrdata field */
#define MDMA_DESC_USER_DATA_L(x)	((uint32_t)(x))
#define MDMA_DESC_USER_DATA_H(x)	((uint32_t)(((uint64_t)(x)) >> 32))

struct __long_desc {
	uint64_t usrdata;
	uint32_t memptr;
	uint32_t flags_length;
} __attribute__ ((packed, aligned(16)));

/* MDMA Descriptor pitch_strlen field */
#define MDMA_DESC_PITCH(x)		(((x) & 0xFFFF) << 16)
#define MDMA_DESC_STRLEN(x)		((x) & 0xFFFF)

#define MDMA_STRLEN_LIMIT		0x10000
#define MDMA_PITCH_LIMIT		0x10000

struct __pitch_desc {
	uint32_t usrdata;
	uint32_t pitch_strlen;
	uint32_t memptr;
	uint32_t flags_length;
} __attribute__ ((packed, aligned(16)));

/* mdma enable register bits */
#define MDMA_ENABLE			(1 << 0)

/* mdma suspend register bits */
#define MDMA_SUSPEND			(1 << 0)

/* mdma cancel register bits */
#define MDMA_CANCEL			(1 << 0)

/* mdma settings register bits */
#define MDMA_SETTINGS_DESC_KIND(x)	((x) & 0x3)
enum mdma_desc_kind {
	NORMAL_DESC	= 0,
	LONG_DESC	= 2,
	PITCH_DESC	= 3
};
#define MDMA_SETTINGS_ADD_INFO		(1 << 4)
#define MDMA_SETTINGS_DESC_GAP(x)	(((x) & 0xFFFF) << 16)

/* mdma irq_mask_register bits */
#define MDMA_IRQ_MASK_SUSPEND_DONE	(1 << 0)
#define MDMA_IRQ_MASK_CANCEL_DONE	(1 << 1)
#define MDMA_IRQ_MASK_INT_DESC		(1 << 2)
#define MDMA_IRQ_MASK_BAD_DESC		(1 << 3)
#define MDMA_IRQ_MASK_STOP_DESC		(1 << 4)
#define MDMA_IRQ_MASK_DISCARD_DESC	(1 << 5)
#define MDMA_IRQ_MASK_WAXI_ERR		(1 << 6)
#define MDMA_IRQ_MASK_AXI_ERR		(1 << 7)
#define MDMA_IRQ_MASK_START_EVENT	(1 << 8)
#define MDMA_IRQ_MASK_IGNORE_EVENT	(1 << 9)

/* mdma status register bits */
#define MDMA_STATUS_SUSPEND_DONE	(1 << 0)
#define MDMA_STATUS_CANCEL_DONE		(1 << 1)
#define MDMA_STATUS_INT_DESC		(1 << 2)
#define MDMA_STATUS_BAD_DESC		(1 << 3)
#define MDMA_STATUS_STOP_DESC		(1 << 4)
#define MDMA_STATUS_DISCARD_DESC	(1 << 5)
#define MDMA_STATUS_WAXI_ERR		(1 << 6)
#define MDMA_STATUS_AXI_ERR		(1 << 7)
#define MDMA_STATUS_START_EVENT		(1 << 8)
#define MDMA_STATUS_IGNORE_EVENT	(1 << 9)

/* mdma state register bits */
#define MDMA_STATE_CANCEL		(1 << 21)
#define MDMA_STATE_SUSPEND		(1 << 22)

/* mdma event_prior register bits */
#define MDMA_GET_PRIOR(i, x)		((((uint64_t)(x)) >> (((i) & 0xF) * 4)) & 0xF)
#define MDMA_SET_PRIOR(i, x)		(((uint64_t)((x) & 0xF)) << (((i) & 0xF) * 4))

/* mdma synch_events register bits */
#define MDMA_SYNC_EVENT_SLAVE		(1 << 0)
#define MDMA_SYNC_EVENT_MASTER		(1 << 8)

struct mdma_chan_regs {
	uint32_t enable;
	uint32_t suspend;
	uint32_t cancel;
	uint32_t reserved;
	uint32_t settings;
	uint32_t irq_mask;
	uint32_t status;
	uint32_t reserved1;
	uint32_t desc_addr;
	uint32_t reserved2;
	uint32_t cur_desc_addr;
	uint32_t cur_addr;
	uint32_t dma_state;
	uint32_t reserved3[0x3];
	uint32_t desc_axlen;
	uint32_t desc_axcache;
	uint32_t desc_axprot;
	uint32_t desc_axlock;
	uint32_t desc_rresp;
	uint32_t desc_raxi_err_addr;
	uint32_t desc_bresp;
	uint32_t desc_waxi_err_addr;
	uint32_t desc_permut;
	uint32_t reserved4[0x7];
	uint32_t max_trans;
	uint32_t axlen;
	uint32_t axcache;
	uint32_t axprot;
	uint32_t axlock;
	uint32_t xresp;
	uint32_t axi_err_addr;
	uint32_t reserved5;
	uint32_t axi_state;
	uint32_t available_space;
	uint32_t permutation;
	uint32_t reserved6[0x5];
	uint32_t sense_list;
	uint32_t signal_time;
	uint32_t events_prior_l;
	uint32_t events_prior_h;
	uint32_t activ_events;
	uint32_t ignore_events;
	uint32_t synch_events;
	uint32_t reserved7;
	uint32_t event_desc_addr[0x8];
} __attribute__ ((packed));

struct mdma_node {
	struct mdma_group	*group;

	enum mdma_desc_type	desc_type;
	int			desc_indx;

	void			*mem_addr;
	size_t			mem_size;

	bool			is_stop;	
	bool			is_last;

	int			custom_flags;
};

struct mdma_group {
	struct mdma_trans	*trans;
	struct list_head	group_node;

	struct mdma_node	*node_group;
	int			node_nmb;

	int			cur_indx;

	void			*desc_pool_addr;
	size_t			desc_pool_size;
	int			gap_size;

	bool			group_done;
};

static inline struct mdma_group* __attribute__((no_instrument_function)) head_to_mdma_group(
									struct list_head *_ptr)
{
	struct mdma_group *_group = container_of(_ptr, struct mdma_group, group_node);

	return _group;
}

enum mdma_trans_type {
	MDMA_TRANS_QUEUE,
	MDMA_TRANS_RING
};

struct mdma_trans {
	struct mdma_chan	*chan;
	struct list_head	group_list;
	int			group_nmb;

	enum mdma_trans_type	trans_type;

	struct mdma_group	*first_group;
	struct mdma_group	*cur_group;
	struct mdma_group	*next_group;
	struct mdma_group	*last_group;

	int			group_wait;

	bool			ignore_event;
};

enum mdma_chan_mode {
	MDMA_CHAN_POLLING,
	MDMA_CHAN_INTERRUPT
};

enum mdma_burst_width {
	MDMA_BURST_WIDTH4	= 4,
	MDMA_BURST_WIDTH8	= 8

};

enum mdma_sync_mode {
	MDMA_SYNC_NONE,
	MDMA_SYNC_SLAVE,
	MDMA_SYNC_MASTER
};

#define MDMA_EVENT_MAX_NUM	16
#define MDMA_PRIOR_MAX_NUM	8

struct mdma_cfg {
	enum mdma_chan_mode	mode;
	enum mdma_desc_kind	desc_kind;
	
	int			heap_id;
	enum mdma_burst_width	burst_width;

	uint32_t		pitch_width;
	uint32_t		str_length;

	int			event_indx;
	uint32_t		event_prior;
	int			signal_time;
	enum mdma_sync_mode     sync_mode;

	bool			add_info;
	bool			addr_inc;
};

enum mdma_chan_state {
	MDMA_CHAN_DUMMY,
	MDMA_CHAN_READY,
	MDMA_CHAN_WAITING,
	MDMA_CHAN_RUNNING,
	MDMA_CHAN_SUSPENDED,
	MDMA_CHAN_CANCELED,
	MDMA_CHAN_PENDING,
	MDMA_CHAN_ERROR
};

enum mdma_chan_irq {
	IRQ_CHAN_NONE	= 0,
	IRQ_CHAN_GROUP	= 1,
	IRQ_CHAN_EVENT	= 2,
	IRQ_CHAN_ERROR	= 4,
	IRQ_CHAN_DONE	= 8
};

enum mdma_chan_error {
	CHAN_NONE_ERR,
	CHAN_RDESC_ERR,
	CHAN_WDESC_ERR,
	CHAN_DATA_ERR
};

#define MDMA_CHAN_TIMEOUT	1000

struct mdma_chan {
	struct mdma_chan_regs	*regs;
	enum mdma_chan_state	state;
	enum mdma_chan_error	error;

	struct mdma_cfg		cfg;
	struct mdma_trans	*trans;

	struct mdma_chan	*slave;
	struct mdma_chan	*master;
};

struct mdma_chan *mdma_chan_create(void *base_addr, struct mdma_cfg *cfg);
int mdma_chan_attach(struct mdma_chan *master, struct mdma_chan *slave);
void mdma_chan_destroy(struct mdma_chan *chan);

int mdma_trans_create(struct mdma_chan *chan, enum mdma_trans_type trans_type);
void mdma_trans_destroy(struct mdma_chan *chan);

int mdma_trans_add_group(struct mdma_chan *chan, void *mem_addr, size_t mem_size,
			int custom_flags, bool is_last);
void mdma_trans_free_groups(struct mdma_chan *chan);

int mdma_chan_config(struct mdma_chan *chan);
int mdma_chan_reset(struct mdma_chan *chan);

int mdma_chan_start(struct mdma_chan *chan);
int mdma_chan_wait_event(struct mdma_chan *chan, int timeout);

int mdma_chan_pause(struct mdma_chan *chan, bool suspend);
int mdma_chan_resume(struct mdma_chan *chan);

int mdma_chan_get_state(struct mdma_chan *chan);
int mdma_chan_irq_cb(struct mdma_chan *chan);

int mdma_chan_wait_trans_end(struct mdma_chan *chan, int timeout);
int mdma_chan_wait_group_end(struct mdma_chan *chan, int timeout);

struct mdma_trans *mdma_chan_get_trans(struct mdma_chan *chan);
int mdma_trans_ignore_event(struct mdma_trans *trans);

struct mdma_group *mdma_trans_get_group(struct mdma_trans *trans);
int mdma_trans_rest_group(struct mdma_group *group);

struct mdma_node *mdma_group_get_node(struct mdma_group *group);
int mdma_node_rebase(struct mdma_node *node, struct mdma_cfg *cfg,
			void *mem_addr, size_t mem_size);

void *mdma_node_get_desc(struct mdma_node *node, int *size);
uint32_t mdma_desc_flags_length(void *desc, int size);
uint64_t mdma_desc_user_data(void *desc);

#endif // __MDMA_CHAN_H__
