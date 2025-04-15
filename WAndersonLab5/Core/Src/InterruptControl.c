/*
 * InterruptControl.c
 *
 *  Created on: Feb 19, 2025
 *      Author: willa
 */
#include <InterruptControl.h>

void enableIRQ(uint8_t IRQnum){
    if (IRQnum < 32)
        NVIC_EnableIRQ(IRQnum);
}

void disableIRQ(uint8_t IRQnum){
    if (IRQnum < 32)
        NVIC_EnableIRQ(IRQnum);
}

void clrPendingIRQ(uint8_t IRQnum){
    if (IRQnum < 32)
        NVIC_EnableIRQ(IRQnum);
}

void clrPendingBit(uint8_t pinNum){
	if(pinNum <32)
		EXTI->PR = (1 << pinNum);
}
