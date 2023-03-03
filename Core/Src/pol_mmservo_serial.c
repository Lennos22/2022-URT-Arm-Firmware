/*
 * pol_mmservo_serial.c
 *
 *  Created on: Feb 24, 2023
 *      Author: Zachary Lee, with reference to code by Nelson Cardona
 */

/* Includes ------------------------------------------------------------------*/
#include "pol_mmservo_serial.h"
#include "pol_smc24v12_serial.h"

/* Private defines -----------------------------------------------------------*/
#define SYNC_BYTE			0xAA

// Pololu Protocol Commands
#define POL_SET_TARGET			0x04
#define POL_SET_SPEED			0x07
#define POL_SET_ACCEL			0x09
#define POL_GET_POS				0x10
#define POL_GO_HOME				0x22

// Mini SSC Protocol Commands
#define CMD_BYTE			0xFF

/* Private function prototypes -----------------------------------------------*/
/** @addtogroup MM_private_functions Pololu MM Private Functions
 *  @{
 */


/**
 *  @}
 */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup MM_exported_functions Pololu MM Exported Functions
 *  @{
 */

/**
 * @brief	Sets the baud rate of all controllers on the serial line.
 * @param 	huart Pointer to a UART_HandleTypeDef structure that contains
 * 		  		  the configuration information for the specified UART module.
 * @retval 	None
 */
void MM_set_baud_rate(UART_HandleTypeDef* huart) {
	HAL_UART_Transmit(huart, (uint8_t[]){SYNC_BYTE}, 1, HAL_MAX_DELAY);
}

/** @defgroup pol_SMC_exported_functions Pololu Protocol SMC Exported Functions
 *  @{
 */

/**
 * @brief	Sets the FULL-resolution target position for the servo.
 * @param	huart Pointer to a UART_HandleTypeDef structure that contains
 * 		  		  the configuration information for the specified UART module.
 * @param	dev_num The Device Number configured in the Simple Motor Control
 * 					Center.
 * @param	chnl_num The Channel Number.
 * @param	target The target position to set the servo, in units of quarter-microseconds.
 * @retval	None
 */
void pol_MM_set_target(UART_HandleTypeDef* huart, uint8_t dev_num, uint8_t chnl_num, int16_t target) {
	// Generate the low and high data bytes of the target parameter
	uint8_t data[] = {chnl_num, target & 0x7F, (target >> 7) & 0x7F};

	pol_transmit_packet(huart, dev_num, POL_SET_TARGET, data, 3);
}

/**
 * @brief	Sets the LOW-resolution target position for the servo using Mini SSC Protocol.
 * @param	huart Pointer to a UART_HandleTypeDef structure that contains
 * 		  		  the configuration information for the specified UART module.
 * @param	chnl_addr The Channel Address, by default it's equal to the Channel Number (0, 11).
 * @param	target The target position to set the servo, unit-less 8-bit target value (0, 254).
 * @retval	None
 */
void mini_MM_set_target(UART_HandleTypeDef* huart, uint8_t chnl_addr, int8_t target) {
	uint8_t packet[] = {CMD_BYTE};

	HAL_UART_Transmit(huart, packet, 3, HAL_MAX_DELAY);
}

/**
 * @brief	Limits the speed at which the channel output values change.
 * @param	huart Pointer to a UART_HandleTypeDef structure that contains
 * 		  		  the configuration information for the specified UART module.
 * @param	dev_num The Device Number configured in the Simple Motor Control
 * 					Center.
 * @param	chnl_num The Channel Number.
 * @param	speed The speed limit to set the servo, in units of 0.25us/10ms (0, 255).
 * 					Note that a speed of 0 means no limit, and 1 is the minimum speed setting.
 * @retval	None
 */
void pol_MM_set_speed(UART_HandleTypeDef* huart, uint8_t dev_num, uint8_t chnl_num, int16_t speed) {
	// Generate the low and high data bytes of the speed parameter
	uint8_t data[] = {chnl_num, speed & 0x7F, (speed >> 7) & 0x7F};

	pol_transmit_packet(huart, dev_num, POL_SET_SPEED, data, 3);
}

/**
 * @brief	Limits the acceleration at which the channel output values change.
 * @param	huart Pointer to a UART_HandleTypeDef structure that contains
 * 		  		  the configuration information for the specified UART module.
 * @param	dev_num The Device Number configured in the Simple Motor Control
 * 					Center.
 * @param	chnl_num The Channel Number.
 * @param	accel The acceleration limit to set the servo, in units of 0.25us/10ms/80ms (0, 255).
 * 				Note that an acceleration of 0 means no limit, and 1 is the minimum acceleration setting.
 * @retval	None
 */
void pol_MM_set_accel(UART_HandleTypeDef* huart, uint8_t dev_num, uint8_t chnl_num, int16_t accel) {
	// Generate the low and high data bytes of the speed parameter
	uint8_t data[] = {chnl_num, accel & 0x7F, (accel >> 7) & 0x7F};

	pol_transmit_packet(huart, dev_num, POL_SET_ACCEL, data, 3);
}

/**
 * @brief	Retrieves the current position of a servo on a certain channel.
 * @param	huart Pointer to a UART_HandleTypeDef structure that contains
 * 		  		  the configuration information for the specified UART module.
 * @param	dev_num The Device Number configured in the Simple Motor Control
 * 					Center.
 * @param	chnl_num The Channel Number.
 * @retval	The current 2-byte pulse width of the servo on the channel.
 */
uint16_t pol_MM_get_pos(UART_HandleTypeDef* huart, uint8_t dev_num, uint8_t chnl_num) {
	// Send for data
	pol_transmit_packet(huart, dev_num, POL_GET_POS, chnl_num, 1);

	// Return received data
	return recv_raw_var(huart);
}

/**
 * @brief	Send all servos to their home position.
 * @param	huart Pointer to a UART_HandleTypeDef structure that contains
 * 		  		  the configuration information for the specified UART module.
 * @param	dev_num The Device Number configured in the Simple Motor Control
 * 					Center.
 * @retval	None
 */
void pol_MM_go_home(UART_HandleTypeDef* huart, uint8_t dev_num) {
	pol_transmit_packet(huart, dev_num, POL_GO_HOME, 0, 0);
}

/**
 *  @}
 */
