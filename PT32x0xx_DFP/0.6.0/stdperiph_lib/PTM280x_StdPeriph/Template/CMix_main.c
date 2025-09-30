#include <stdbool.h>

#include "PT32x0xx.h"
#include "CMix_board.h"
#include "CMix_control.h"

static void CMix_MainLoop(CMix_ControlContext *ctx);
/**
 * @brief CMix延时函数(毫秒)
 * @param ms: 延时毫秒数
 * @retval None
 */
void CMix_Hardware_Delay_ms(uint32_t ms)
{
    uint32_t i, j;
    /* 简单延时实现，实际项目中建议使用SysTick */
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 8000; j++) {
            __NOP();
        }
    }
}
void I2C_Master_Write(u8* pBuffer, u32 WriteAddr,u16 DeviceAddr, u16 data_size)
{
	int i;
/******************等待从机ready***************/		
	I2C_GenerateEvent(I2C0,I2C_Event_Start,DISABLE);
	I2C0->CCR |= I2C_CCR_SI | I2C_CCR_ACK;
	I2C_Cmd(I2C0,DISABLE);
	I2C_Cmd(I2C0,ENABLE);
	I2C_GenerateEvent(I2C0,I2C_Event_Start,ENABLE);	
	while(I2C_GetFlagStatus(I2C0,I2C_FLAG_StartOk)!= SET);
	I2C_SendAddress(I2C0, DeviceAddr);//器件地址，写
	while(I2C_GetFlagStatus(I2C0,I2C_FLAG_MASGetAckW)!=SET);
	I2C_SendData(I2C0,WriteAddr);//发送要写的字地址
	while(I2C_GetFlagStatus(I2C0,I2C_FLAG_MDSGetAck)!=SET);
	for(i=0;i<data_size;i++)
	{
		I2C_SendData(I2C0, *pBuffer);
		while(I2C_GetFlagStatus(I2C0,I2C_FLAG_MDSGetAck) != SET);
		pBuffer++;
	}
	/******************发送停止位***************/
	I2C_GenerateEvent(I2C0,I2C_Event_Stop,ENABLE);	
}
int main(void)
{
    CMix_ControlContext control_ctx;

    CMix_SystemInit();
    GPIO_SetBits(GPIOA, GPIO_Pin_3);
//    GPIO_ResetBits(GPIOA, GPIO_Pin_3);
//    CMix_ControlInit(&control_ctx);
    while(1)
    {
        GPIO_ReverseBits(GPIOA, GPIO_Pin_3);
        I2C_Master_Write((u8*)"Hello World!\n", 0x00, 0x50, 14);
        CMix_Hardware_Delay_ms(1000);
    }
//    CMix_MainLoop(&control_ctx);

    return 0;
}

static void CMix_MainLoop(CMix_ControlContext *ctx)
{
    bool run = true;

    while (run)
    {
        CMix_ControlUpdate(ctx);
        __NOP();
    }
}
