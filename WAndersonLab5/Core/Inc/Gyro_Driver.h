/*
 * Gyro_Driver.h
 *
 *  Created on: Apr 15, 2025
 *      Author: willa
 */

#ifndef INC_GYRO_DRIVER_H_
#define INC_GYRO_DRIVER_H_

#include <stm32f4xx_hal.h>
#include <stdint.h>

#define GYRO_WHO_AM_I_REG 0x0F
#define GYRO_CTRL_REG1    0x20
#define GYRO_CTRL_REG5    0x24
#define GYRO_OUT_TEMP     0x26

#define GYRO_CS_PORT GPIOE
#define GYRO_CS_PIN  GPIO_PIN_3 //DIFFERENT PORT AND PIN MAYBE??

#define ACTIVE   1
#define INACTIVE 0

#define GYRO_READ_BIT 0x80
#define GYRO_WRITE_BIT 0x7F

void Gyro_Init(void);

void Gyro_PowerOn(void);

void Gyro_Configure(void);

void Gyro_GetDeviceID(void);

void Gyro_ReadTemp(void);

uint8_t Gyro_ReadRegister(uint8_t reg_addr);

void Gyro_WriteRegister(uint8_t reg_addr, uint8_t value);

void Gyro_EnableSlave(void);

void Gyro_DisableSlave(void);

void Gyro_VerifyStatus(HAL_StatusTypeDef status);



#endif /* INC_GYRO_DRIVER_H_ */
