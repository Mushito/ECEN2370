/*
 * ApplicationCode.h
 *
 *  Created on: Jan 30, 2025
 *      Author: willa
 */

#ifndef APPLICATIONCODE_H_
#define APPLICATIONCODE_H_

#include "Scheduler.h"
#include "Button_Driver.h"

#define NAME_LENGTH 4
#define DEFAULT_DELAY 100000
#define USE_INTERRUPT_FOR_BUTTON 1

void I_ModeButtonInit();

void applicationInit();

void appButtonInit();

void executeButtonPollingRoutine();

void greenLEDInit();

void redLEDInit();

void LEDInitAll();

void toggleGreenLED();

void toggleRedLED();

void activateGreenLED();

void activateRedLED();

void deactivateGreenLED();

void deactivateRedLED();

void appDelay(uint32_t delay);


#endif /* APPLICATIONCODE_H_ */
