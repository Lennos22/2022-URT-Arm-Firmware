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
void pol_set_baud_rate(UART_HandleTypeDef* huart);
void pol_SMC_set_speed(UART_HandleTypeDef* huart, uint8_t dev, int16_t speed);


#endif /* INC_POL_SMC24V12_SERIAL_H_ */
