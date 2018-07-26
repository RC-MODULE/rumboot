#ifndef __MGETH__H__
#define __MGETH__H__

#define DBG

#ifdef DBG
#define DBG_print(fmt, ...) rumboot_printf("%s:%d: "fmt"\n", __FUNCTION__, __LINE__, ##__VA_ARGS__);
#else
#define DBG_print(fmt, ...);
#endif // DBG

/**
 * \defgroup devices_mgeth MGETH
 * \ingroup devices
 * \brief MGETH function library
 * \addtogroup devices_mgeth
 * @{
 */

enum mgeth_SPEED
{
	SPEED_10   = 0b00,
	SPEED_100  = 0b01,
	SPEED_1000 = 0b10,
};

struct mgeth_conf
{
	bool is_full_duplex;
	enum mgeth_SPEED speed;
};

/**
 * \brief SGMII initialization
 * \param sgmii_base_addr Base address of SGMII
 * \param sctl_base_addr Base address of SCTL
 */
void mgeth_init_sgmii(uint32_t sgmii_base_addr, uint32_t sctl_base_addr);

/**
 * \brief MGETH reset
 * \param base_addr Base address of MGETH
 */
void mgeth_reset(uint32_t base_addr);

/**
 * \brief MGETH initialization
 * \param base Base address of MGETH
 * \param conf Configuration structure
 */
void mgeth_init(const uint32_t base, const struct mgeth_conf *conf);

/**
 * \brief Read data throught MDMA channel
 * \param base Base address of MGETH
 * \param data Pointer to read data
 * \param len Size of transmitted data
 * \return If OK - pointer on MDMA read channel, else - NULL
 */
struct mdma_chan *mgeth_transmit(uint32_t base, void *data, size_t len);

/**
 * \brief Write data throught MDMA channel
 * \param base Base address of MGETH
 * \param data Pointer to written data
 * \param len Size of transmitted data
 * \return If OK - pointer on MDMA write channel, else - NULL
 */
struct mdma_chan *mgeth_receive(uint32_t base, void *data, size_t len);

/**
 * \brief Waiting transfer complete
 * \param chan Pointer on MDMA channel
 * \param timeout Timeout in microseconds (-1 for infinite timeout)
 * \return If OK - 0, else - not 0
 */
int mgeth_wait_transfer_complete(struct mdma_chan *chan, int timeout);

/**
 * \brief Finish work with channel
 * \param chan Pointer on MDMA channel
 * \return If OK - 0, else - not 0
 */
int mgeth_finish_transfer(struct mdma_chan *chan);

/**
 * @}
 */

#endif // __MGETH__H__
