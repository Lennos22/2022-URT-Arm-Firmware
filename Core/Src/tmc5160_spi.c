/*
 * tmc5160_spi.c
 *
 *  Created on: 20 Feb. 2023
 *      Author: Nelson Cardona
 */

/* Includes ------------------------------------------------------------------*/
#include "tmc5160_spi.h"

/* Private defines -----------------------------------------------------------*/
#define SPI_TIMEOUT HAL_MAX_DELAY

/* Private function prototypes -----------------------------------------------*/
void TMC5160_write_datagram(SPI_HandleTypeDef* hspi, uint8_t addr, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);
void TMC5160_read_write_array(SPI_HandleTypeDef* hspi, uint8_t* buf, size_t size);

/* Exported functions ---------------------------------------------------------*/
void TMC5160_write_value(SPI_HandleTypeDef* hspi, uint8_t addr, uint32_t data) {
	TMC5160_write_datagram(
			hspi,
			addr,
			DATA_BYTE(data, 1),
			DATA_BYTE(data, 2),
			DATA_BYTE(data, 3),
			DATA_BYTE(data, 4)
			);
}

uint32_t TMC5160_read_reg(SPI_HandleTypeDef* hspi, uint8_t addr) {
	uint8_t buf[TMC5160_SPI_DATAGRAM_SIZE] = {addr};
	TMC5160_read_write_array(hspi, buf, TMC5160_SPI_DATAGRAM_SIZE);

	// TODO: Error handling w/ buf[0]...

	buf[0]= addr; // Does this technically need to be any particular value?
	TMC5160_read_write_array(hspi, buf, TMC5160_SPI_DATAGRAM_SIZE);

	// TODO: Error handling w/ buf[0]...

	return DATA_UNSIGNED(buf[1], 1) |
			DATA_UNSIGNED(buf[2], 2) |
			DATA_UNSIGNED(buf[3], 3) |
			DATA_UNSIGNED(buf[4], 4);
}

// TODO: Generate SPI Pipeline Architecture...

/* Private functions ----------------------------------------------------------*/
/**
 * @brief	Sends a write operation datagram to a TMC5160 device
 *
 * @param 	hspi hspi pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.
 * @param 	addr 7-bit register address.
 * @param	data The 4-byte data array (MSB).
 */
void TMC5160_write_datagram(SPI_HandleTypeDef* hspi, uint8_t addr, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4) {
	uint8_t datagram[TMC5160_SPI_DATAGRAM_SIZE] = {addr | TMC5160_WRITE_BIT, b1, b2, b3, b4};
	TMC5160_read_write_array(hspi, datagram, TMC5160_SPI_DATAGRAM_SIZE);

	// TODO: Error handling w/ datagram[0]...
}

void TMC5160_read_write_array(SPI_HandleTypeDef* hspi, uint8_t* buf, size_t size) {
	HAL_SPI_TransmitReceive(hspi, buf, buf, size, SPI_TIMEOUT);
}
