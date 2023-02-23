/*
 * tmc5160_spi_def.h
 *
 *  Created on: 21 Feb. 2023
 *      Author: nelson
 */

#ifndef INC_TMC5160_SPI_DEF_H_
#define INC_TMC5160_SPI_DEF_H_

/* Includes ------------------------------------------------------------------*/
#include <tmc5160.h>

/* Exported constants --------------------------------------------------------*/
// Important defines
#define TMC5160_SPI_DATAGRAM_SIZE		5		//!<In bytes
#define TMC5160_WRITE_BIT				0x80U

/* Exported macro ------------------------------------------------------------*/
#define DATA_BYTE(data, index)			((uint8_t) ((data) >> (4 - (index))*8))
#define DATA_CONV(data, index)			((uint32_t) data << (4 - (index))*8)

#endif /* INC_TMC5160_SPI_DEF_H_ */
