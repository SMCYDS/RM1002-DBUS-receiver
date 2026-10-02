/**
******************************************************************************
 * @file    remote.cpp/h
 * @brief   Remote control. 遥控器
 ******************************************************************************
 * Copyright (c) 2026 Team JiaoLong-SJTU
 * All rights reserved.
 ******************************************************************************
 */

#include "remote.h"
#include "usart.h"

#include <stdio.h>
#include <string.h>

constexpr uint16_t REMOTE_CONNECT_TIMEOUT = 500u;

//调试输出
uint8_t init_msg[] = "Remote Init\n";
uint8_t reset_msg[] = "Remote Reset\n";

// Constructor 构造函数
Remote::Remote(UART_HandleTypeDef *huart): huart_(huart), connect_(REMOTE_CONNECT_TIMEOUT){
    switch_.l = RCSwitchState_e::DOWN;
    switch_.r = RCSwitchState_e::DOWN;
}

// Start UART(SBUS) receive. 打开UART接收
void Remote::init()
{
    HAL_UART_Transmit(&huart1,init_msg,sizeof(init_msg)-1,1000);
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1,(uint8_t*)rx_data_,RC_FRAME_LEN);//继续中断接收
}
// Reset RC data. 重置遥控器数据
void Remote::reset() {
    // Your code here
    channel_.l_row = 1024; //11bits
    channel_.l_col = 1024; //11bits
    channel_.r_row = 1024; //11bits
    channel_.r_col = 1024; //11bits
    switch_.l = RCSwitchState_e::DOWN;
    switch_.r = RCSwitchState_e::DOWN;
    HAL_UART_Transmit(&huart1,reset_msg,sizeof(reset_msg)-1,1000);
}


// Check for uart correspondence. 检查串口是否匹配
void Remote::rxMsgCheck(UART_HandleTypeDef *huart) {
    // Your code here
    if (huart == &huart3 || huart == &huart1)
    {
        HAL_GPIO_TogglePin(LED_B_GPIO_Port, LED_B_Pin);
        rxMsgCallback((uint8_t*)rx_data_);

        HAL_UARTEx_ReceiveToIdle_DMA(&huart1,(uint8_t*)rx_data_,RC_FRAME_LEN);//继续中断接收
    }
}

void Remote::sendbackInfo() //数据回送
{
    char callback_msg[64];

    int len = snprintf(callback_msg, sizeof(callback_msg),
                       "%s %uHz|%u,%u|%u,%u|%u,%u|\n",
                       connect_.check()?"T":"F",(unsigned int)connect_.getfreq(),(unsigned int)channel_.l_row,(unsigned int)channel_.l_col,(unsigned int)channel_.r_row,(unsigned int)channel_.r_col,(unsigned int)switch_.l,(unsigned int)switch_.r);

    // 发送数据到 USART1
    if (len > 0 && len < sizeof(callback_msg)) {
        HAL_UART_Transmit(&huart1, (uint8_t *)callback_msg, strlen(callback_msg), 1000);
    }
}

// Update connect status, restart UART(SBUS) receive.
// 更新连接状态，重新打开UART(SBUS)接收
void Remote::rxMsgCallback(uint8_t* rx_data_ ){
    connect_.refresh();
    handle();
}

// Unpack data. 数据解包
void Remote::handle() {
    const uint8_t* RxData = rx_data_;
    channel_.l_row = (RxData[0] | RxData[1]<<8) & 0x07FF; //11bits
    channel_.l_col = (RxData[1]>>3 | RxData[2]<<5) & 0x07FF; //11bits
    channel_.r_row = (RxData[2]>>6 | RxData[3]<<2 | (uint16_t)RxData[4]<<10) & 0x07FF; //11bits
    channel_.r_col = (RxData[4]>>1 | RxData[5]<<7) & 0x07FF; //11bits
    switch_.l = static_cast<RCSwitchState_e>(RxData[5]>>4 & 0x03);//2bits
    switch_.r = static_cast<RCSwitchState_e>(RxData[5]>>6 & 0x03);//2bits
}

Remote remote(&huart3);


extern "C" {
void Remote_Init(void) {
    remote.init();
}
void Remote_Reset(void){remote.reset();}
void Remote_Handle(void){remote.handle();};
}
