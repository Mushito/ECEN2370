/*
 * Timer_Driver.h
 *
 *  Created on: Feb 27, 2025
 *      Author: willa
 */

#ifndef TIMER_DRIVER_H_
#define TIMER_DRIVER_H_

void TimerInit(GPTIMR_RegDef_t *TIMRptr, GPTimer_Config_t);

void TimerClkCtrl(GPTIMR_RegDef_t *TIMRptr, uint8_t onOff);

void TimerStart(GPTIMR_RegDef_t *TIMRptr);

void TimerStop(GPTIMR_RegDef_t *TIMRptr);

void TimerRst(GPTIMR_RegDef_t *TIMRptr);

uint32_t getTimerVal(GPTIMR_RegDef_t *TIMRptr);

void TimerInterrupt(GPTIMR_RegDef_t *TIMRptr);

void getTimerARV(GPTIMR_RegDef_t *TIMRptr);

#endif /* TIMER_DRIVER_H_ */
