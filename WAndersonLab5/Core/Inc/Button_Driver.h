/*
* Button_Driver.h
*
*  Created on: Feb 13, 2025
*      Author: willa
*/

#ifndef BUTTON_DRIVER_H_
#define BUTTON_DRIVER_H_

#include <stm32f4xx.h>

#define USER_BUTTON_PORT A 
#define USER_BUTTON_PIN_NUM 0
#define USER_BUTTON_PRESSED 1
#define USER_BUTTON_UNPRESSED 0

#define ACTIVE   1
#define INACTIVE 0

void buttonInit();

uint8_t buttonPressed();

void I_Mode_Init();

#endif /* BUTTON_DRIVER_H_ */
