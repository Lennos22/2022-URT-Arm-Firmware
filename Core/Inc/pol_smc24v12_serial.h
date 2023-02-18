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

/* Exported functions prototypes ---------------------------------------------*/
/**
 * @brief	Sets the baud rate of the SMC drivers on the line to the MCU's
 * @param 	huart Pointer to a UART_HandleTypeDef structure that contains
 * 		  		  the configuration information for the specified UART module.
 * @retval 	None
 */
void pol_set_baud_rate(UART_HandleTypeDef* huart);
/**
 * @brief	Sets the speed of a DC Motor
 * @param	huart Pointer to a UART_HandleTypeDef structure that contains
 * 		  		  the configuration information for the specified UART module.
 * @param	dev_num The Device Number configured in the Simple Motor Control
 * 					Center.
 * @param	speed The speed to set the motor. (between -3200 and 3200, inc.)
 * @retval	None
 */
void pol_SMC_set_speed(UART_HandleTypeDef* huart, uint8_t dev_num, int16_t speed);


#endif /* INC_POL_SMC24V12_SERIAL_H_ */
