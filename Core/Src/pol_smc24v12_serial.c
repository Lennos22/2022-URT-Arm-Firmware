/*
 * pol_smc24v12_serial.c
 *
 *  Created on: Feb 18, 2023
 *      Author: Nelson Cardona
 */

/* Includes ------------------------------------------------------------------*/
#include "pol_smc24v12_serial.h"

/* Private defines -----------------------------------------------------------*/
#define SYNC_BYTE			0xAA

// Pololu Protocol Commands
#define POL_EXIT_SAFESTART		0x03
#define POL_MOTOR_FORWARD		0X05
#define POL_MOTOR_REVERSE		0X06
#define POL_MOTOR_FWD_7BIT		0x09
#define POL_MOTOR_BWD_7BIT		0x0A
#define POL_MOTOR_BRAKE			0x12
#define POL_GET_VAR				0x21
#define POL_SET_MOTOR_LIMIT		0x22
#define	POL_GET_FIRMWARE		0x42
#define POL_STOP_MOTOR			0x60

// Mini SSC Protocol Commands
#define MINI_SSC_SET_SPEED		0xFF

// Maximum temp motor limit vals
#define MAX_SPEED_LIMIT			3200
#define MAX_ACCEL_LIMIT			3200
#define MAX_DECEL_LIMIT			3200
#define MAX_BRK_DURATION_LIMIT	16384	//!<Units are 4ms instead of 1ms, so UINT16_MAX / 4

#define MAX_SPEED_7BIT			INT8_MAX
#define BRAKE_LIMIT				32

/* Private function prototypes -----------------------------------------------*/
/** @addtogroup SMC_private_functions Pololu SMC Private Functions
 *  @{
 */

void pol_transmit_packet(UART_HandleTypeDef* huart, uint8_t dev_num, uint8_t cmd, const uint8_t* data, size_t data_size);
uint16_t recv_raw_var(UART_HandleTypeDef* huart);

/**
 *  @}
 */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup SMC_exported_functions Pololu SMC Exported Functions
 *  @{
 */

/**
 * @brief	Sets the baud rate of all controllers on the serial line.
 * @param 	huart Pointer to a UART_HandleTypeDef structure that contains
 * 		  		  the configuration information for the specified UART module.
 * @retval 	None
 */
void SMC_set_baud_rate(UART_HandleTypeDef* huart) {
	HAL_UART_Transmit(huart, (uint8_t[]){SYNC_BYTE}, 1, HAL_MAX_DELAY);
}

/** @defgroup pol_SMC_exported_functions Pololu Protocol SMC Exported Functions
 *  @{
 */

/**
 * @brief	Sets the the FULL-resolution target speed of the controller.
 * @param	huart Pointer to a UART_HandleTypeDef structure that contains
 * 		  		  the configuration information for the specified UART module.
 * @param	dev_num The Device Number configured in the Simple Motor Control
 * 					Center.
 * @param	speed The speed to set the motor (-3200 to 3200).
 * @retval	None
 */
void pol_SMC_set_speed(UART_HandleTypeDef* huart, uint8_t dev_num, int16_t speed) {
	uint8_t motor_dir = POL_MOTOR_FORWARD;

	if (speed < 0) {
		speed = -speed;
		motor_dir = POL_MOTOR_REVERSE;
	}

	// The speed should not exceed the maximum motor speed
	if (speed > MAX_SPEED_LIMIT) {
		// Todo: Error Handling...
		return;
	}

	// Generate the low and high data bytes of the speed parameter
	uint8_t data[] = {speed & 0x1F, speed >> 5};

	pol_transmit_packet(huart, dev_num, motor_dir, data, 2);
}

/**
 * @brief	Sets the target speed of the controller based on 7-bit low-
 * 			resolution Speed byte.
 * @param	huart Pointer to a UART_HandleTypeDef structure that contains
 * 		  		  the configuration information for the specified UART module.
 * @param	dev_num The Device Number configured in the Simple Motor Control
 * 					Center.
 * @param	speed The low-resolution speed bit (-127 to 127).
 * @retval	None
 */
void pol_SMC_set_spd_7b(UART_HandleTypeDef* huart, uint8_t dev_num, int8_t speed) {
	uint8_t motor_dir = POL_MOTOR_FWD_7BIT;

	if (speed < 0) {
		speed = -speed;
		motor_dir = POL_MOTOR_BWD_7BIT;
	}

	if (speed > MAX_SPEED_7BIT) {
		// TODO: Error Handling...
		return;
	}

	pol_transmit_packet(huart, dev_num, motor_dir, (uint8_t[]){speed}, 1);
}

/**
 * @brief	Sets the motor brake value of the controller.
 * @param	huart Pointer to a UART_HandleTypeDef structure that contains
 * 		  		  the configuration information for the specified UART module.
 * @param	dev_num The Device Number configured in the Simple Motor Control
 * 					Center.
 * @param	brake_val The value to set the brakes.
 * @retval	None
 */
void pol_SMC_set_brake(UART_HandleTypeDef* huart, uint8_t dev_num, uint8_t brake_val) {
	if (brake_val > BRAKE_LIMIT) {
		// TODO: Error Handling...
		return;
	}

	pol_transmit_packet(huart, dev_num, POL_MOTOR_BRAKE, (uint8_t[]){brake_val}, 1);
}

/**
 * @brief	Reads a variable from a controller
 *
 * @param	huart Pointer to a UART_HandleTypeDef structure that contains
 * @param	dev_num The Device Number configured in the Simple Motor Control
 * 					Center.
 * @param	var_id Variable ID.
 * @retval	The unsigned 16-bit value of the requested variable.
 */
uint16_t pol_SMC_get_variable(UART_HandleTypeDef* huart, uint8_t dev_num, uint8_t var_id) {
	pol_transmit_packet(huart, dev_num, POL_GET_VAR, (uint8_t[]){var_id}, 1);

	return recv_raw_var(huart);
}

/**
 * @brief	Sets the value of a controller's temporary motor limit variable.
 * @note	The ID of the limit is specified by the first data byte, and the
 * 			value of the limit is specified by the next two data bytes, the
 * 			first of which (limit byte 1) contains the low seven bits and the
 * 			second (limit byte 2) contains the high seven bits.
 * @param	huart Pointer to a UART_HandleTypeDef structure that contains
 * 		  		  the configuration information for the specified UART module.
 * @param	dev_num The Device Number configured in the Simple Motor Control
 * 					Center.
 * @param	limit_id  The limit ID of the temporary motor limit variable.
 * @param	new_limit The new limit with which to set the variable
 * @retval	None
 */
void pol_SMC_set_motor_limit(UART_HandleTypeDef* huart, uint8_t dev_num, uint8_t limit_id, uint16_t new_limit) {
	// There are multiple possible bounds for a limit variable.
	switch (limit_id) {
		case SPEED_FORWARD_LIM_ID:
		case SPEED_REVERSE_LIM_ID:
		case SPEED_SYMMTRC_LIM_ID:
			if (new_limit > MAX_SPEED_LIMIT) {
				// TODO: Error Handling...
				return;
			}
			break;
		case ACCEL_FORWARD_LIM_ID:
		case ACCEL_REVERSE_LIM_ID:
		case ACCEL_SYMMTRC_LIM_ID:
			if (new_limit > MAX_ACCEL_LIMIT) {
				// TODO: Error Handling...
				return;
			}
			break;
		case DECEL_FORWARD_LIM_ID:
		case DECEL_REVERSE_LIM_ID:
		case DECEL_SYMMTRC_LIM_ID:
			if (new_limit > MAX_DECEL_LIMIT) {
				// TODO: Error Handling...
				return;
			}
			break;
		case BRK_DURTN_FWD_LIM_ID:
		case BRK_DURTN_BWD_LIM_ID:
		case BRK_DURTN_SYM_LIM_ID:
			if (new_limit > MAX_BRK_DURATION_LIMIT) {
				// TODO: Error Handling...
				return;
			}
			break;
		default:
			// TODO: Error Handling...
			return;
	}

	// Compute the values for the 2 data bytes
	uint8_t data[] = {limit_id, new_limit & 0x7F, new_limit >> 7};

	pol_transmit_packet(huart, dev_num, POL_SET_MOTOR_LIMIT, data, 3);
}

void pol_SMC_get_firmware_ver(UART_HandleTypeDef* huart, uint8_t dev_num) {
	// TODO
}

/**
 * @brief	Sets the controller's motor target speed to zero.
 * @note	WARNING: Will set the controller to Safe-Start if enabled.
 * @param	huart Pointer to a UART_HandleTypeDef structure that contains
 * 		  		  the configuration information for the specified UART module.
 * @param	dev_num The Device Number configured in the Simple Motor Control
 * 					Center.
 * @retval	None
 */
void pol_SMC_stop_motor(UART_HandleTypeDef* huart, uint8_t dev_num) {
	pol_transmit_packet(huart, dev_num, POL_STOP_MOTOR, NULL, 0);
}

/**
 *  @}
 */

/** @defgroup comp_SMC_exported_functions Compact Protocol SMC Exported Functions
 *  @{
 */

// TODO

/**
 *  @}
 */

/** @defgroup mini_SSC_SMC_exported_functions Mini SSC Protocol SMC Exported Functions
 *  @{
 */

/**
 * @brief	Sets the target speed of the controller using the Mini SSC Protocol.
 * @note	Motor speed ranges from -127 to 127 mapped to unsigned 8-bit, so
 * 			speed byte of 0 maps to -127 (full-speed reverse), speed byte 127
 * 			maps to 0 (full stop), and speed byte 254 as 127 (full-speed forward).
 * @param	huart Pointer to a UART_HandleTypeDef structure that contains
 * 		  		  the configuration information for the specified UART module.
 * @param	dev_num The Device Number configured in the Simple Motor Control
 * 					Center.
 * @param	speed Speed byte.
 * @retval	None
 */
void mini_SMC_set_speed(UART_HandleTypeDef* huart, uint8_t dev_num, uint8_t speed) {
	pol_transmit_packet(huart, dev_num, MINI_SSC_SET_SPEED, (uint8_t[]){speed}, 1);
}

/**
 *  @}
 */

/**
 *  @}
 */

/** @defgroup pol_SMC_private_functions Pololu SMC Private Functions
 *  @{
 */

/**
 * @brief	Pololu Protocol packet transmission.
 *
 * @param 	huart Pointer to a UART_HandleTypeDef structure that contains
 * 		  		  the configuration information for the specified UART module.
 * @param 	dev_num The Device Number configured in the Simple Motor Control
 * 					Center.
 * @param 	cmd Command byte
 * @param 	data Pointer to data buffer
 * @param 	data_size Amount of bytes in data buffer to be sent.
 * @retval	None
 */
void pol_transmit_packet(UART_HandleTypeDef* huart, uint8_t dev_num, uint8_t cmd, const uint8_t* data, size_t data_size) {
	uint8_t packet[] = {SYNC_BYTE, dev_num, cmd};

	HAL_UART_Transmit(huart, packet, 3, HAL_MAX_DELAY);

	if (data)
		HAL_UART_Transmit(huart, data, data_size, HAL_MAX_DELAY);
}

// TODO: Compact Protocol packet transmit

/**
 * @brief	Receives the requested variable as unsigned 16-bit.
 *
 * @param	huart Pointer to a UART_HandleTypeDef structure that contains
 * 		  		  the configuration information for the specified UART module.
 * @retval	The unsigned 16-bit value of the requested variable.
 */
uint16_t recv_raw_var(UART_HandleTypeDef* huart) {
	uint8_t buf[2] = {0}; // Idk how I feel about zero init'ing this

	HAL_UART_Receive(huart, buf, 2, HAL_MAX_DELAY);

	uint16_t raw_result = buf[1] + (buf[2] << 8);

	// TODO: Add functionality for SIGNED variables?

	return raw_result;
}

/**
 *  @}
 */
