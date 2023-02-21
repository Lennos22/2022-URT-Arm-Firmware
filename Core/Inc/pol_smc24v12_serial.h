/*
 * pol_smc24v12_serial.h
 *
 *  Created on: Feb 18, 2023
 *      Author: nelson
 */

#ifndef INC_POL_SMC24V12_SERIAL_H_
#define INC_POL_SMC24V12_SERIAL_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported constants --------------------------------------------------------*/
/** @defgroup SMC_var_ids Pololu SMC Variable IDs
 *  @{
 */

/** @defgroup SMC_status_flags Pololu SMC Status Flags
 *  @{
 */
#define ERROR_STATUS 			0x00
#define ERRORS_OCCURRED			0x01
#define SERIAL_ERRORS_OCC		0x02
#define LIMIT_STATUS			0x03
#define RESET_FLAGS				0x7F
/**
 *  @}
 */

/** @defgroup SMC_RC_CH_in Pololu SMC RC Channel Inputs
 *  @{
 */
#define RC1_UNLIMITED_RAW_VALUE	0x04
#define RC1_RAW_VALUE			0x05
#define RC1_SCALED_VALUE		0x06
#define RC2_UNLIMITED_RAW_VALUE	0x08
#define RC2_RAW_VALUE			0x09
#define RC2_SCALED_VALUE		0x0A
/**
 *  @}
 */

/** @defgroup SMC_AN_CH_in Pololu SMC Analog Channel Inputs
 *  @{
 */
#define AN1_UNLIMITED_RAW_VALUE	0x0C
#define AN1_RAW_VALUE			0x0D
#define AN1_SCALED_VALUE		0x0E
#define AN2_UNLIMITED_RAW_VALUE	0x10
#define AN2_RAW_VALUE			0x11
#define AN2_SCALED_VALUE		0x12
/**
 *  @}
 */

/** @defgroup SMC_diagnostic_vars Pololu SMC Diagnostic Variables
 *  @{
 */
#define TARGET_SPEED			0x14
#define	CURRENT_SPEED			0x15
#define	BRAKE_AMOUNT			0x16
#define	INPUT_VOLTAGE			0x17
#define TEMPERATURE				0x18
#define RC_PERIOD				0x1A
#define BAUD_RATE_REGISTER		0x1B
#define SYSTEM_TIME_LOW			0x1C
#define SYSTEM_TIME_HIGH		0x1D
/**
 *  @}
 */

/** @SMC_temp_motor_limits Pololu SMC Temporary Motor Limits
 *  @{
 */
#define MAX_SPEED_FORWARD		0x1E
#define MAX_ACCEL_FORWARD		0x1F
#define	MAX_DECEL_FORWARD		0x20
#define	BRAKE_DURATION_FORWARD	0x21
#define MAX_SPEED_REVERSE		0x24
#define MAX_ACCEL_REVERSE		0x25
#define	MAX_DECEL_REVERSE		0x26
#define	BRAKE_DURATION_REVERSE	0x27
/**
 *  @}
 */

/**
 *  @}
 */

/** @defgroup pol_SMC_limit_ids Pololu SMC Limit IDs
 *  @{
 */

#define SPEED_FORWARD_LIM_ID	0x04
#define ACCEL_FORWARD_LIM_ID	0x05
#define	DECEL_FORWARD_LIM_ID	0x06
#define	BRK_DURTN_FWD_LIM_ID	0x07
#define SPEED_REVERSE_LIM_ID	0x08
#define ACCEL_REVERSE_LIM_ID	0x09
#define	DECEL_REVERSE_LIM_ID	0x0A
#define	BRK_DURTN_BWD_LIM_ID	0x0B

#define SPEED_SYMMTRC_LIM_ID	0x00
#define ACCEL_SYMMTRC_LIM_ID	0x01
#define	DECEL_SYMMTRC_LIM_ID	0x02
#define	BRK_DURTN_SYM_LIM_ID	0x03

/**
 *  @}
 */

/* Exported functions ---------------------------------------------------------*/
/** @addtogroup SMC_exported_functions Pololu SMC Exported Functions
 *  @{
 */

void SMC_set_baud_rate(UART_HandleTypeDef* huart);

/** @addtogroup pol_SMC_exported_functions Pololu Protocol SMC Exported Functions
 *  @{
 */

void pol_SMC_set_speed(UART_HandleTypeDef* huart, uint8_t dev_num, int16_t speed);
void pol_SMC_set_spd_7b(UART_HandleTypeDef* huart, uint8_t dev_num, int8_t speed);
void pol_SMC_set_brake(UART_HandleTypeDef* huart, uint8_t dev_num, uint8_t brake_val);
uint16_t pol_SMC_get_variable(UART_HandleTypeDef* huart, uint8_t dev_num, uint8_t var_id);
void pol_SMC_set_motor_limit(UART_HandleTypeDef* huart, uint8_t dev_num, uint8_t limit_id, uint16_t new_limit);
void pol_SMC_get_firmware_ver(UART_HandleTypeDef* huart, uint8_t dev_num);
void pol_SMC_stop_motor(UART_HandleTypeDef* huart, uint8_t dev_num);

/**
 *  @}
 */

/** @addtogroup mini_SSC_SMC_exported_functions Mini SSC Protocol SMC Exported Functions
 *  @{
 */

void mini_SMC_set_speed(UART_HandleTypeDef* huart, uint8_t dev_num, uint8_t speed);

/**
 *  @}
 */

/**
 *  @}
 */

#endif /* INC_POL_SMC24V12_SERIAL_H_ */
