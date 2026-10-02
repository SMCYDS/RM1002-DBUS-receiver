//
// Created by Thomas on 2026/10/2.
//
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "tim.h"

extern uint8_t rx_msg[10];

struct DBUS_Frame parseDBUSFrame(const uint8_t *RxData)
{
    struct DBUS_Frame frame;
    frame.CHN0 = ((uint16_t)RxData[0] | (uint16_t)RxData[1]<<8) & 0x07FF; //11bits
    frame.CHN1 = ((uint16_t)RxData[1]>>3 | (uint16_t)RxData[2]<<5) & 0x07FF; //11bits



    return frame;
}
uint8_t getEventType(UART_HandleTypeDef *huar){return HAL_UARTEx_GetRxEventType(huar) == HAL_UART_RXEVENT_IDLE || HAL_UARTEx_GetRxEventType(huar) == HAL_UART_RXEVENT_TC;}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart == &huart1 && getEventType(huart))
    {
        HAL_UART_Transmit(&huart1,rx_msg,Size,1000);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1,rx_msg,10);//继续中断接收

    }else if (huart == &huart3)
    {
        HAL_GPIO_TogglePin(LED_B_GPIO_Port, LED_B_Pin);

        HAL_UART_Transmit(&huart1,rx_msg,10,1000);
        HAL_UART_Receive_DMA(&huart1,rx_msg,10);//继续中断接收
    }
}

