/*
 * pol_smc24v12_serial.c
 *
 *  Created on: Feb 18, 2023
 *      Author: nelson
 */

/* Includes ------------------------------------------------------------------*/
#include "pol_smc24v12_serial.h"

/* Private defines -----------------------------------------------------------*/
#define HEADER_BYTE             0xAA

// Pololu Protocol Commands
#define POL_EXIT_SAFESTART		0x03
#define POL_MOTOR_FORWARD    	0X05
#define POL_MOTOR_REVERSE    	0X06
#define POL_MOTOR_BRAKE      	0x12

#define MAX_MOTOR_SPEED         3200

/* Private function prototypes -----------------------------------------------*/
void pol_transmit_packet(UART_HandleTypeDef* huart, uint8_t dev, uint8_t cmd, uint8_t* data, size_t data_size);

void pol_SMC_set_speed(UART_HandleTypeDef* huart, uint8_t dev, int16_t speed) {
	uint8_t motor_dir = POL_MOTOR_FORWARD;

	if (speed < 0) {
		speed = -speed;
		motor_dir = POL_MOTOR_REVERSE;
	}

	// The speed should not exceed the maximum motor speed
	if (speed > MAX_MOTOR_SPEED) {
		return;
	}

	// Generate the low and high data bytes of the speed parameter
	uint8_t data[2] = {speed & 0x1F, speed >> 5};

	pol_transmit_packet(huart, dev, motor_dir, data, 2);
}

void pol_transmit_packet(UART_HandleTypeDef* huart, uint8_t dev, uint8_t cmd, uint8_t* data, size_t data_size) {
	uint8_t packet[] = {HEADER_BYTE, dev, cmd};

	HAL_UART_Transmit(huart, packet, 3, HAL_MAX_DELAY);

	if (data)
		HAL_UART_Transmit(huart, data, data_size, HAL_MAX_DELAY);
}
