/*
 * tmc5160_spi.h
 *
 *  Created on: 20 Feb. 2023
 *      Author: nelson
 */

#ifndef INC_TMC5160_SPI_H_
#define INC_TMC5160_SPI_H_

/* Includes ------------------------------------------------------------------*/
#include "tmc5160_def.h"
#include "stm32f4xx_hal.h"

/* Exported constants --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void TMC5160_write_value(SPI_HandleTypeDef* hspi, uint8_t addr, uint32_t data);
uint32_t TMC5160_read_reg(SPI_HandleTypeDef* hspi, uint8_t addr);

#endif /* INC_TMC5160_SPI_H_ */
