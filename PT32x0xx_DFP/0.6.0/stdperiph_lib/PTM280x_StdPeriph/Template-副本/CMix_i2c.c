
#include <stdint.h>
#include "PT32x0xx.h"
#include "PT32x0xx_i2c.h"
#include "CMix_i2c.h"

// 设置PWM占空比
uint8_t CMix_I2C_SetPWMDuty(uint16_t duty, uint16_t slave_addr)
{
    uint8_t buf[3] = {0x01, (duty >> 8) & 0xFF, duty & 0xFF};
    CMix_I2C_Master_Write(buf, 0x00, slave_addr, 3);
    uint8_t response = 0;
    CMix_I2C_Master_Read(&response, 0x00, slave_addr, 1);
    return response;
}

// 设置PWM频率
uint8_t CMix_I2C_SetPWMFreq(uint16_t freq, uint16_t slave_addr)
{
    uint8_t buf[3] = {0x02, (freq >> 8) & 0xFF, freq & 0xFF};
    CMix_I2C_Master_Write(buf, 0x00, slave_addr, 3);
    uint8_t response = 0;
    CMix_I2C_Master_Read(&response, 0x00, slave_addr, 1);
    return response;
}

// 启动PWM
uint8_t CMix_I2C_StartPWM(uint16_t slave_addr)
{
    uint8_t cmd = 0x03;
    CMix_I2C_Master_Write(&cmd, 0x00, slave_addr, 1);
    uint8_t response = 0;
    CMix_I2C_Master_Read(&response, 0x00, slave_addr, 1);
    return response;
}

// 停止PWM
uint8_t CMix_I2C_StopPWM(uint16_t slave_addr)
{
    uint8_t cmd = 0x04;
    CMix_I2C_Master_Write(&cmd, 0x00, slave_addr, 1);
    uint8_t response = 0;
    CMix_I2C_Master_Read(&response, 0x00, slave_addr, 1);
    return response;
}

// 查询状态
uint8_t CMix_I2C_QueryStatus(uint16_t slave_addr, uint8_t* status_buf)
{
    uint8_t cmd = 0x05;
    CMix_I2C_Master_Write(&cmd, 0x00, slave_addr, 1);
    CMix_I2C_Master_Read(status_buf, 0x00, slave_addr, 6);
    return status_buf[0]; // 返回0xAA表示成功
}


int CMix_I2C_Master_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t DeviceAddr, uint16_t data_size)
{
    int i;
    // 发送起始信号
    I2C_GenerateEvent(I2C0, I2C_Event_Start, DISABLE);
    I2C0->CCR |= I2C_CCR_SI | I2C_CCR_ACK;
    I2C_Cmd(I2C0, DISABLE);
    I2C_Cmd(I2C0, ENABLE);
    I2C_GenerateEvent(I2C0, I2C_Event_Start, ENABLE);
    while (I2C_GetFlagStatus(I2C0, I2C_FLAG_StartOk) != SET);
    // 发送器件地址，读
    I2C_SendAddress(I2C0, DeviceAddr | 0x01); // 读操作
    while (I2C_GetFlagStatus(I2C0, I2C_FLAG_MASGetAckR) != SET);
    // 可选：发送要读的字地址
    if (ReadAddr != 0xFF) {
        I2C_SendData(I2C0, ReadAddr);
        while (I2C_GetFlagStatus(I2C0, I2C_FLAG_MDSGetAck) != SET);
    }
    // 读取数据
    for (i = 0; i < data_size; i++) {
        pBuffer[i] = I2C_ReceiveData(I2C0);
        if (i < data_size - 1)
            I2C0->CCR |= I2C_CCR_ACK; // 继续ACK
        else
            I2C0->CCR &= ~I2C_CCR_ACK; // 最后一个字节NACK
        while (I2C_GetFlagStatus(I2C0, I2C_FLAG_MDSGetAck) != SET);
    }
    // 发送停止信号
    I2C_GenerateEvent(I2C0, I2C_Event_Stop, ENABLE);
    return data_size;
}

static uint8_t i2c_rx_buffer[CMIX_I2C_BUFFER_SIZE];
static uint8_t i2c_tx_buffer[CMIX_I2C_BUFFER_SIZE];
static uint8_t i2c_rx_count = 0;
static uint8_t i2c_tx_count = 0;

void CMix_I2C_Init(void)
{
    // 初始化I2C硬件，按PT32x0xx库实际结构体成员
    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Prescaler = 200; // 速率分频，具体值可根据主频调整
    I2C_InitStructure.I2C_Broadcast = I2C_Broadcast_Disable;
    I2C_InitStructure.I2C_OwnAddress = CMIX_I2C_SLAVE_ADDR;
    I2C_InitStructure.I2C_Acknowledge = I2C_Acknowledge_Enable;
    I2C_Init(I2C0, &I2C_InitStructure);
    I2C_Cmd(I2C0, ENABLE);
}

void CMix_I2C_Master_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t DeviceAddr, uint16_t data_size)
{
    int i;
    I2C_GenerateEvent(I2C0, I2C_Event_Start, DISABLE);
    I2C0->CCR |= I2C_CCR_SI | I2C_CCR_ACK;
    I2C_Cmd(I2C0, DISABLE);
    I2C_Cmd(I2C0, ENABLE);
    I2C_GenerateEvent(I2C0, I2C_Event_Start, ENABLE);
    while (I2C_GetFlagStatus(I2C0, I2C_FLAG_StartOk) != SET);
    I2C_SendAddress(I2C0, DeviceAddr);
    while (I2C_GetFlagStatus(I2C0, I2C_FLAG_MASGetAckW) != SET);
    I2C_SendData(I2C0, WriteAddr);
    while (I2C_GetFlagStatus(I2C0, I2C_FLAG_MDSGetAck) != SET);
    for (i = 0; i < data_size; i++) {
        I2C_SendData(I2C0, *pBuffer);
        while (I2C_GetFlagStatus(I2C0, I2C_FLAG_MDSGetAck) != SET);
        pBuffer++;
    }
    I2C_GenerateEvent(I2C0, I2C_Event_Stop, ENABLE);
}

void CMix_I2C_Slave_Proc(void)
{
    // 参考Template\i2c协议实现I2C从机读写
    // 这里只做协议框架，具体命令和数据处理可根据实际需求扩展
    // 例如：轮询或在中断服务函数中处理I2C_GetFlagStatus和I2C_ReceiveData
    // 用户可根据实际协议补充
}
