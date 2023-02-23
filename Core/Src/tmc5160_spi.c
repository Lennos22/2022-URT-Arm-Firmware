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
/**
 * @brief	Write 32-bit value to register.
 * @param	hspi hspi pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.
 * @param	addr Register address.
 * @param	new_val Value to write to address.
 * @retval	None
 */
void TMC5160_write_value(SPI_HandleTypeDef* hspi, uint8_t addr, uint32_t new_val) {
	TMC5160_write_datagram(
			hspi,
			addr,
			DATA_BYTE(new_val, 1),
			DATA_BYTE(new_val, 2),
			DATA_BYTE(new_val, 3),
			DATA_BYTE(new_val, 4)
			);
}

/**
 * @brief	Read value from register.
 * @param	hspi hspi pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.
 * @param	addr Register address.
 * @retval	Value read from register
 */
uint32_t TMC5160_read_reg(SPI_HandleTypeDef* hspi, uint8_t addr) {
	uint8_t buf[TMC5160_SPI_DATAGRAM_SIZE] = {addr};
	TMC5160_read_write_array(hspi, buf, TMC5160_SPI_DATAGRAM_SIZE);

	// TODO: Error handling w/ buf[0]...

	buf[0] = addr; // Does this technically need to be any particular value?
	TMC5160_read_write_array(hspi, buf, TMC5160_SPI_DATAGRAM_SIZE);

	// TODO: Error handling w/ buf[0]...

	return DATA_CONV(buf[1], 1) |
			DATA_CONV(buf[2], 2) |
			DATA_CONV(buf[3], 3) |
			DATA_CONV(buf[4], 4);
}

// TODO: Generate SPI Pipeline Architecture...

/* Private functions ----------------------------------------------------------*/
/**
 * @brief	Sends a write operation datagram to a TMC5160 device.
 * @param 	hspi hspi pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.
 * @param 	addr 7-bit register address.
 * @param	b1 First byte (MSB).
 * @param	b2 Second byte.
 * @param	b3 Third byte.
 * @param	b4 Fourth byte.
 * @retval	None
 */
void TMC5160_write_datagram(SPI_HandleTypeDef* hspi, uint8_t addr, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4) {
	uint8_t datagram[TMC5160_SPI_DATAGRAM_SIZE] = {addr | TMC5160_WRITE_BIT, b1, b2, b3, b4};
	TMC5160_read_write_array(hspi, datagram, TMC5160_SPI_DATAGRAM_SIZE);

	// TODO: Error handling w/ datagram[0]...
}

/**
 * @brief	Transmit and then receive data array across SPI line, overwriting
 * 			same array with data received.
 * @param	hspi pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.
 * @param	buf Data array.
 * @param	size Amount of data to be sent and received.
 * @retval	None
 */
void TMC5160_read_write_array(SPI_HandleTypeDef* hspi, uint8_t* buf, size_t size) {
	HAL_SPI_TransmitReceive(hspi, buf, buf, size, SPI_TIMEOUT);
}
