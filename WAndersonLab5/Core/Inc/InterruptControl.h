/*
 * InterruptControl.h
 *
 *  Created on: Feb 19, 2025
 *      Author: willa
 */

#ifndef INTERRUPTCONTROL_H_
#define INTERRUPTCONTROL_H_

#include <STM32F4xx_hal.h>

#define EXTI0_IRQ_NUMBER 6

void enableIRQ(uint8_t IRQnum);

void disableIRQ(uint8_t IRQnum);

void clrPendingIRQ(uint8_t IRQnum);

void setPendingIRQ(uint8_t IRQnum);

void clrPendingBit(uint8_t pinNum);

#endif /* INTERRUPTCONTROL_H_ */
