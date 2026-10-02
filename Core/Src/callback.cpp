//
// Created by Thomas on 2026/10/2.
//
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "tim.h"
#include "remote.h"



uint8_t getEventType(UART_HandleTypeDef *huar){return HAL_UARTEx_GetRxEventType(huar) == HAL_UART_RXEVENT_IDLE || HAL_UARTEx_GetRxEventType(huar) == HAL_UART_RXEVENT_TC;}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(getEventType(huart))remote.rxMsgCheck(huart);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim==&htim1)
    {
        remote.sendbackInfo();
        if (remote.connect_.check())//遥控器还活着
        {

        }else//遥控器似了
        {
            remote.reset();
        }
    }
}