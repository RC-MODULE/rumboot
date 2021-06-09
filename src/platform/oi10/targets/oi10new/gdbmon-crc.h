
#ifndef  __CRC_H__
#define  __CRC_H__

/*!
@addtogroup GDB_OTHER Расчёт CRC
@{
@brief Заголовочный файл функций расчета контрольных сумм CRC.
@author Леонтьев С.Н.
*/

#include  <stdint.h>

/*!
@brief Начальное значение CRC.

Значение, которое необходимо передавать в функцию @link
gdbmon_crc32_next @endlink при начале расчёта.
*/
#define GDBMON_CRC32_START   0xFFFFFFFF

/*!
@brief Расчёт CRC следующего блока.

Рассчитывает CRC указанного блока данных. В некоторых случаях, вся
последовательность данных от которой необходимо посчитать CRC не доступна в
адресном пространстве одновременно, в этом случае необходимо рассчитывать CRC
частями, при этом на вход следующего блока передаётся CRC предыдущего и далее
по цепочке.
@warning В случае расчёта первого блока в цепочки на вход передаётся
значение @link GDBMON_CRC32_START @endlink.
@param[in]  data        Адрес блока данных в памяти
@param[in]  len         Длина блока данных
@param[in]  init_value  CRC предыдущего блока
*/
uint32_t gdbmon_crc32_next ( const void *data, uint32_t len, uint32_t init_value );

/*!
@}
*/

#endif // __CRC_H__
