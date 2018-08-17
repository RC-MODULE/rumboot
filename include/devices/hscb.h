/*
 * hscb.h
 *
 *  Created on: Jul 19, 2018
 *      Author: r.galiulin
 */

#ifndef DEVICES_HSCB_H
#define DEVICES_HSCB_H
/**
 * \defgroup devices_hscb HSCB
 * \ingroup devices
 *
 * Change endianness in 4-byte word function
 * - data_in:   input 4-byte data word
 * Function returns data word with reversed byte order

 *
 *
 * \addtogroup devices_hscb
 *
 * @{
 */


/**
 * \brief Change endianness in 4-byte word function
 * \param[in]   data_in input 4-byte data word
 * \return              Function returns data word with reversed byte order
 */

uint32_t change_endian (uint32_t data_in);

/**
 * \brief Convert two 4-byte data words to 8 1-byte words function
 * \param[in]   data_in     input 4-byte data array pointer
 * \param[out]  data_out    result 1-byte data array pointer
 * \param[in]   len         length of data to be processed in bytes
 */
void convert_to_bytes (uint32_t* data_in, uint8_t* data_out, uint32_t len);

/**
 * \brief Set descriptor data function
 * \param[in]   sys_addr    address of descriptor
 * \param[in]   len         length of data array
 * \param[in]   desc_addr   address of data array (address in descriptor)
 * \param[in]   data_in     byte-data array for write to memory pointer
 * \param[in]   act0        descriptors flag pointer
 * \param[in]   interrupt   descriptors flag pointer
 * \param[in]   end         descriptors flag pointer
 * \param[in]   valid       descriptors flag pointer
 * \return                  Function returns address of next descriptor
 */
uint32_t set_desc (uint32_t sys_addr, uint32_t len, uint32_t desc_addr, uint8_t* data_in, bool act0, bool interrupt, bool valid, bool endian);

/**
 * \brief Get descriptor data function
 * \param[in]   sys_addr    address of descriptor
 * \param[in]   data_out    accepted byte-data array pointer
 * \param[in]   len         length of data array pointer
 * \param[in]   act0        descriptors flag pointer
 * \param[in]   interrupt   descriptors flag pointer
 * \param[in]   end         descriptors flag pointer
 * \param[in]   valid       descriptors flag pointer
 * \return                  In case COMPL descriptor
 *
 *      function returns address of next descriptor
 *
 *      function returns 0xffffffff as impossible address
 */
uint32_t get_desc (uint32_t sys_addr, uint8_t* data_out, uint32_t* len,  bool* act0, bool* interrupt, bool* end, bool* valid, bool endian);


/**
 * @}
 */

#endif /* DEVICES_HSCB_H */
