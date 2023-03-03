/*
 * pol_mmservo_serial.h
 *
 *  Created on: Feb 24, 2023
 *      Author: Zachary Lee, with reference to code by Nelson Cardona
 */

#ifndef INC_POL_MMSERVO_SERIAL_H_
#define INC_POL_MMSERVO_SERIAL_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported functions prototypes ---------------------------------------------*/
/** @addtogroup MM_exported_functions Pololu MM Exported Functions
 *  @{
 */
void MM_set_baud_rate(UART_HandleTypeDef* huart);

/** @addtogroup pol_MM_exported_functions Pololu Protocol MM Exported Functions
 *  @{
 */
void pol_MM_set_target(UART_HandleTypeDef* huart, uint8_t dev_num, uint8_t chnl_num, int16_t target);
void pol_MM_set_speed(UART_HandleTypeDef* huart, uint8_t dev_num, uint8_t chnl_num, int16_t speed);
void pol_MM_set_accel(UART_HandleTypeDef* huart, uint8_t dev_num, uint8_t chnl_num, int16_t accel);
uint16_t pol_MM_get_pos(UART_HandleTypeDef* huart, uint8_t dev_num, uint8_t chnl_num);
void pol_MM_go_home(UART_HandleTypeDef* huart, uint8_t dev_num);

/** @addtogroup mini_SSC_MM_exported_functions Mini SSC Protocol MM Exported Functions
 *  @{
 */
void mini_MM_set_target(UART_HandleTypeDef* huart, uint8_t chnl_addr, int8_t target);

/**
 *  @}
 */

#endif /* INC_POL_MMSERVO_SERIAL_H_ */
