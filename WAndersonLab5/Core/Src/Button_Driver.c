#include "Button_Driver.h"

void buttonInit(){
	GPIO_InitTypeDef pinConfig;
    pinConfig.Pin = USER_BUTTON_PIN_NUM;
    pinConfig.Mode = GPIO_MODE_INPUT;
    pinConfig.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOA, &pinConfig);
}

uint8_t buttonPressed(){
    uint8_t pinVal = HAL_GPIO_ReadPin(GPIOA, USER_BUTTON_PIN_NUM);
    if (pinVal == USER_BUTTON_PRESSED)
        return ACTIVE;
    else
        return INACTIVE;
}

void I_Mode_Init(){
    GPIO_InitTypeDef pinConfig;

    __HAL_RCC_GPIOA_CLK_ENABLE();

    pinConfig.Pin = USER_BUTTON_PIN_NUM; // MIGHT CAUSE ERRORS???
    pinConfig.Mode = GPIO_MODE_INPUT;
    pinConfig.Speed = GPIO_SPEED_FREQ_HIGH;
    pinConfig.Pull = GPIO_NOPULL;
    pinConfig.Alternate = GPIO_MODE_IT_RISING_FALLING;

    HAL_GPIO_Init(GPIOA, &pinConfig);

    HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}
