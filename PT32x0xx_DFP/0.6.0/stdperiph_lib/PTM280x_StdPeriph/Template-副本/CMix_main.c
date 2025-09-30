#include <stdbool.h>

#include "PT32x0xx.h"
#include "CMix_board.h"
#include "CMix_control.h"
// 新增I2C头文件
#include "CMix_i2c.h"

static void CMix_MainLoop(CMix_ControlContext *ctx);
/**
 * @brief
 * @param ms:
 * @retval None
 */
void CMix_Hardware_Delay_ms(uint32_t ms)
{
    uint32_t i, j;

    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 8000; j++)
        {
            __NOP();
        }
    }
}
void I2C_Master_Write(u8 *pBuffer, u32 WriteAddr, u16 DeviceAddr, u16 data_size)
{
    int i;
}
// I2C_Master_Write implementation removed
// New I2C interface will be used instead
int main(void)
{
    CMix_ControlContext control_ctx;

    CMix_SystemInit();
    GPIO_SetBits(GPIOA, GPIO_Pin_3);
    //    GPIO_ResetBits(GPIOA, GPIO_Pin_3);
    //    CMix_ControlInit(&control_ctx);
    while (1)
    {
 //       GPIO_ReverseBits(GPIOA, GPIO_Pin_3);
        
        // 可选：如需支持I2C从机协议处理，可在主循环中调用
         CMix_I2C_Proc();
        CMix_Hardware_Delay_ms(500);
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
