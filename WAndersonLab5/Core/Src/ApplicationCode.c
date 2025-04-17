/*
 * ApplicationCode.c
 *
 *  Created on: Jan 30, 2025
 *      Author: willa
 */


#include "ApplicationCode.h"

char nameArray[NAME_LENGTH] = {'W','I','L','L'};
char destinationArray[NAME_LENGTH];

void appDelay(uint32_t delay){
	int i;
	int j;
	for (i = 0; i <= delay; i++){
		for (j = 0; j <= NAME_LENGTH; j++){
			destinationArray[j] = nameArray[j];
		}
	}
}

void applicationInit(){
    addSchedulerEvent(DELAY_EVENT);

    #if USE_INTERRUPT_FOR_BUTTON
        I_ModeButtonInit();

    #else
        buttonInit();
        addSchedulerEvent(POLLING_EVENT);
    #endif
}

void appButtonInit(){
    buttonInit();
}

void EXTI0_IRQHandler(){
	HAL_NVIC_DisableIRQ(EXTI0_IRQn);
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void I_ModeButtonInit(){
	GPIO_InitTypeDef pinConfig;
    
    pinConfig.Pin       = USER_BUTTON_PIN_NUM;
    pinConfig.Mode         = GPIO_MODE_INPUT;
    pinConfig.Pull          = GPIO_NOPULL;
    pinConfig.Speed        = GPIO_SPEED_FREQ_HIGH;
    pinConfig.Alternate= GPIO_MODE_IT_RISING_FALLING;               
    
    HAL_GPIO_Init(GPIOA, &pinConfig);

    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}
