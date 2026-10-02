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

constexpr uint16_t REMOTE_CONNECT_TIMEOUT = 500u; 
// Constructor 构造函数
Remote::Remote(UART_HandleTypeDef *huart): huart_(huart), connect_(REMOTE_CONNECT_TIMEOUT){
    switch_.l = RCSwitchState_e::DOWN;
    switch_.r = RCSwitchState_e::DOWN;
}

// Start UART(SBUS) receive. 打开UART接收
void Remote::init() {
    // Your code here 
}

// Reset RC data. 重置遥控器数据
void Remote::reset() {
    // Your code here 
}

// Check for uart correspondence. 检查串口是否匹配
void Remote::rxMsgCheck(UART_HandleTypeDef *huart) const {
    // Your code here 

}

// Update connect status, restart UART(SBUS) receive.
// 更新连接状态，重新打开UART(SBUS)接收
void Remote::rxMsgCallback(uint8_t* rx_data_){
    // Your code here 

}

// Unpack data. 数据解包
void Remote::handle() {
    // Your code here 

}
