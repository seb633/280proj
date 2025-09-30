
#ifndef __CMIX_I2C_H__
#define __CMIX_I2C_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

// I2C Slave地址定义，可根据实际硬件修改
#define CMIX_I2C_SLAVE_ADDR 0x50

// I2C缓冲区大小
#define CMIX_I2C_BUFFER_SIZE 8

    // I2C初始化
    void CMix_I2C_Init(void);

    // I2C主机写（兼容原有接口）
    void CMix_I2C_Master_Write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t DeviceAddr, uint16_t data_size);

    // I2C主机读（补充接口）
    int CMix_I2C_Master_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t DeviceAddr, uint16_t data_size);

    // I2C从机读写处理（协议实现）
    void CMix_I2C_Proc(void);

    // 高层协议函数声明


#ifdef __cplusplus
}
#endif

#endif // __CMIX_I2C_H__
