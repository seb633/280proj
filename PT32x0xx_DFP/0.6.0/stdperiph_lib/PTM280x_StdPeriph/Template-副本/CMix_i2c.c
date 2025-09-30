
#include <stdint.h>
#include "PT32x0xx.h"
#include "PT32x0xx_i2c.h"
#include "CMix_i2c.h"
#include "PT32x0xx_config.h"


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

/**
* @brief I2C写函数
* @param pBuffer：需要写入的数据
* @param WriteAddr：从机地址
* @param NumByteToWrite：需要写入的数据长度
* @retval 无
*/
//void CMix_I2C_Master_Write(u8* pBuffer, u32 WriteAddr,u16 DeviceAddr, u16 data_size)
void CMix_I2C_Master_Write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t DeviceAddr, uint16_t data_size)
{
	static  uint32_t count;
	int i;
/******************等待从机ready***************/		
	I2C_GenerateEvent(I2Cn,I2C_Event_Start,DISABLE);
	I2Cn->CCR |= I2C_CCR_SI | I2C_CCR_ACK;
	I2C_Cmd(I2Cn,DISABLE);
	I2C_Cmd(I2Cn,ENABLE);
	I2C_GenerateEvent(I2Cn,I2C_Event_Start,ENABLE);	
	while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_StartOk)!= SET);
	I2C_SendAddress(I2Cn, DeviceAddr);//器件地址，写
	while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_MASGetAckW)!=SET)
	{
        count ++;
        if(count >64000)
        {
            I2C_GenerateEvent(I2Cn,I2C_Event_Stop,ENABLE);	
            count = 0;
            return;
        }
	};
	I2C_SendData(I2Cn,*pBuffer);//发送要写的字地址
	pBuffer++;
	while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_MDSGetAck)!=SET)
	{
        count ++;
        if(count >64000)
        {
            I2C_GenerateEvent(I2Cn,I2C_Event_Stop,ENABLE);	
            count = 0;
            return;
        }
	};
	for(i=0;(i<data_size-1);i++)
	{
		I2C_SendData(I2Cn, *pBuffer);
		while(I2C_GetFlagStatus(I2Cn,I2C_FLAG_MDSGetAck) != SET)
		{
			count ++;
			if(count >640000)
			{
				I2C_GenerateEvent(I2Cn,I2C_Event_Stop,ENABLE);	
				count = 0;
				return;
			}
		};
		pBuffer++;
	}
	/******************发送停止位***************/
	I2C_GenerateEvent(I2Cn,I2C_Event_Stop,ENABLE);	
}
// void CMix_I2C_Master_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t DeviceAddr, uint16_t data_size)
// {
//     int i;
//     I2C_GenerateEvent(I2C0, I2C_Event_Start, DISABLE);
//     I2C0->CCR |= I2C_CCR_SI | I2C_CCR_ACK;
//     I2C_Cmd(I2C0, DISABLE);
//     I2C_Cmd(I2C0, ENABLE);
//     I2C_GenerateEvent(I2C0, I2C_Event_Start, ENABLE);
//     while (I2C_GetFlagStatus(I2C0, I2C_FLAG_StartOk) != SET);
//     I2C_SendAddress(I2C0, DeviceAddr);
//     while (I2C_GetFlagStatus(I2C0, I2C_FLAG_MASGetAckW) != SET);
//     I2C_SendData(I2C0, WriteAddr);
//     while (I2C_GetFlagStatus(I2C0, I2C_FLAG_MDSGetAck) != SET);
//     for (i = 0; i < data_size; i++) {
//         I2C_SendData(I2C0, *pBuffer);
//         while (I2C_GetFlagStatus(I2C0, I2C_FLAG_MDSGetAck) != SET);
//         pBuffer++;
//     }
//     I2C_GenerateEvent(I2C0, I2C_Event_Stop, ENABLE);
// }
int i=0;
u8 addr=0x0;
static uint16_t duty, prd;
static uint8_t onoff, dead;
void CMix_I2C_Proc(void)
{
    // 参考Template\i2c协议实现I2C从机读写
    // 这里只做协议框架，具体命令和数据处理可根据实际需求扩展
    // 例如：轮询或在中断服务函数中处理I2C_GetFlagStatus和I2C_ReceiveData
    // 用户可根据实际协议补充
    duty = 160;
    onoff = 1;
    prd = 320;
    dead = 10;//1-16
    i2c_tx_buffer[0] = 0x06;
    i2c_tx_buffer[1] = onoff;
    i2c_tx_buffer[2] = (duty>>8)&0xFF;
    i2c_tx_buffer[3] = (duty & 0xFF);
    i2c_tx_buffer[4] = (prd>>8)&0xFF;
    i2c_tx_buffer[5] = (prd & 0xFF);
    i2c_tx_buffer[6] = dead;
    i2c_tx_buffer[7] = 0;
    for(int i = 0; i<7;i++)
    {
        i2c_tx_buffer[7] += i2c_tx_buffer[i];
    }
    i2c_tx_buffer[7] = ~(i2c_tx_buffer[7]);
    i2c_tx_buffer[7] += 1; 
    
    CMix_I2C_Master_Write(i2c_tx_buffer, addr,0xA0, 8);
		
//			CMix_Hardware_Delay_ms(500);
//			arry_write[0] = 0x01;
//			arry_write[1] = 0x00;
//			arry_write[2] = 0x50;
//			arry_write[3] = (~(arry_write[0]+arry_write[1]+arry_write[2]))+1;

//			I2C_EE_Write(arry_write, addr,0xA0, 4);
//		
//			arry_write[0] = 0x02;
//			arry_write[1] = 0x01;
//			arry_write[2] = 0x04;
//			arry_write[3] = (~(arry_write[0]+arry_write[1]+arry_write[2]))+1;
//			CMix_Hardware_Delay_ms(500);
//			I2C_EE_Write(arry_write, addr,0xA0, 4);
//			CMix_Hardware_Delay_ms(20);
//			arry_write[0] = 0x03;
//			I2C_EE_Write(arry_write, addr,0xA0, 1);
//			I2C_EE_Read(arry_read,addr,0xA0,/*sizeof(arry_read)*/1);
//    CMix_Hardware_Delay_ms(50);
}
