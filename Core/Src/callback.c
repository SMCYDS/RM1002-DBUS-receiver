//
// Created by Thomas on 2026/10/2.
//
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "tim.h"

extern uint8_t rx_msg[20];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1)
    {
        if (rx_msg[0] == 'R')HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin,RESET);
        if (rx_msg[0]=='M')HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin,SET);
        HAL_UART_Transmit(&huart1,rx_msg,10,1000);
        HAL_UART_Receive_DMA(&huart1,rx_msg,10);//继续中断接收

    }
}

