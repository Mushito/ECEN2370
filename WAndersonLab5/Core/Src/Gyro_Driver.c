/*
 * Gyro_Driver.c
 *
 *  Created on: Apr 15, 2025
 *      Author: willa
 */

#include "Gyro_Driver.h"
#include "ErrorHandler.h"
#include <stdio.h>

extern SPI_HandleTypeDef hspi5;

static HAL_StatusTypeDef gyro_status;

void Gyro_Init(void) {
    Gyro_DisableSlave();
    Gyro_Configure();
    Gyro_PowerOn();
}

void Gyro_EnableSlave(void) {
    HAL_GPIO_WritePin(GYRO_CS_PORT, GYRO_CS_PIN, GPIO_PIN_RESET);
}

void Gyro_DisableSlave(void) {
    HAL_GPIO_WritePin(GYRO_CS_PORT, GYRO_CS_PIN, ACTIVE); // CHIP SELECT ACTIVE LOW
}

void Gyro_VerifyStatus(HAL_StatusTypeDef status) {
    APPLICATION_ASSERT(status == HAL_OK);
}

uint8_t Gyro_ReadRegister(uint8_t reg_addr) {
    uint8_t tx = reg_addr | GYRO_READ_BIT;
    uint8_t rx = 0;

    Gyro_EnableSlave();
    gyro_status = HAL_SPI_Transmit(&hspi5, &tx, 1, HAL_MAX_DELAY);
    Gyro_VerifyStatus(gyro_status);

    gyro_status = HAL_SPI_Receive(&hspi5, &rx, 1, HAL_MAX_DELAY);
    Gyro_VerifyStatus(gyro_status);
    Gyro_DisableSlave();

    return rx;
}

void Gyro_WriteRegister(uint8_t reg_addr, uint8_t value) {
    uint8_t tx[2] = {reg_addr & GYRO_WRITE_BIT, value};

    Gyro_EnableSlave();
    gyro_status = HAL_SPI_Transmit(&hspi5, tx, 2, HAL_MAX_DELAY);
    Gyro_VerifyStatus(gyro_status);
    Gyro_DisableSlave();
}

void Gyro_PowerOn(void) {
    Gyro_WriteRegister(GYRO_CTRL_REG1, 0x0F);
}

void Gyro_Configure(void) {
    Gyro_WriteRegister(GYRO_CTRL_REG5, 0x80);
}

void Gyro_GetDeviceID(void) {
    uint8_t id = Gyro_ReadRegister(GYRO_WHO_AM_I_REG);
    printf("Gyro Device ID: 0x%02X\r\n", id); // should be 0xD3?
}

void Gyro_ReadTemp(void) {
    int8_t temp = (int8_t)Gyro_ReadRegister(GYRO_OUT_TEMP);
    printf("Gyro Temp: %d°C\r\n", temp);
}


