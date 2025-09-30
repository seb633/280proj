# I2C PWM 控制协议使用说明 (带校验和参数验证)

## 概述
本文档描述了AD16H02微控制器的I2C PWM控制协议，包含校验码验证和参数范围检查功能。

## I2C地址配置
- 基地址: 0x50
- 地址范围: 0x50-0x53 (由PB6/PB7引脚状态决定)

## 命令格式
所有命令都包含校验码，采用二进制补码校验方式。

### 校验码计算方法
1. 计算除校验码外所有字节的和
2. 对和值取反加1 (二进制补码)
3. 将校验码作为最后一个字节发送

### 命令列表

#### 1. 设置PWM占空比 (PWM_CMD_SET_DUTY = 0x01)
```
发送: [0x01, 占空比高字节, 占空比低字节, 校验码]
响应: [响应码]
```
- 占空比范围: 0-1023 (PWM_DUTY_MIN=0, PWM_DUTY_MAX=1023)
- 示例: 设置占空比为512
  - 数据: [0x01, 0x02, 0x00]
  - 校验码计算: sum = 0x01 + 0x02 + 0x00 = 0x03, 校验码 = (~0x03) + 1 = 0xFD
  - 完整命令: [0x01, 0x02, 0x00, 0xFD]

#### 2. 设置PWM频率 (PWM_CMD_SET_FREQ = 0x02)  
```
发送: [0x02, 频率高字节, 频率低字节, 校验码]
响应: [响应码]
```
- 频率范围: 100-10000 Hz (PWM_FREQ_MIN=100, PWM_FREQ_MAX=10000)
- 示例: 设置频率为1000Hz
  - 数据: [0x02, 0x03, 0xE8] 
  - 校验码计算: sum = 0x02 + 0x03 + 0xE8 = 0xED, 校验码 = (~0xED) + 1 = 0x13
  - 完整命令: [0x02, 0x03, 0xE8, 0x13]

#### 3. 启动PWM (PWM_CMD_START = 0x03)
```
发送: [0x03, 校验码]
响应: [响应码]
```
- 示例:
  - 数据: [0x03]
  - 校验码计算: sum = 0x03, 校验码 = (~0x03) + 1 = 0xFD
  - 完整命令: [0x03, 0xFD]

#### 4. 停止PWM (PWM_CMD_STOP = 0x04)
```
发送: [0x04, 校验码] 
响应: [响应码]
```
- 示例:
  - 数据: [0x04]
  - 校验码计算: sum = 0x04, 校验码 = (~0x04) + 1 = 0xFC
  - 完整命令: [0x04, 0xFC]

#### 5. 查询PWM状态 (PWM_CMD_STATUS = 0x05)
```
发送: [0x05, 校验码]
响应: [响应码, PWM状态, 占空比高字节, 占空比低字节, 频率高字节, 频率低字节, 响应校验码]
```
- 示例:
  - 发送数据: [0x05]
  - 校验码计算: sum = 0x05, 校验码 = (~0x05) + 1 = 0xFB
  - 完整命令: [0x05, 0xFB]
  - 响应会包含7字节（包含响应校验码）

## 响应码定义
- `0xAA` (PWM_RESPONSE_OK): 命令执行成功
- `0xFF` (PWM_RESPONSE_ERROR): 命令执行失败
- `0xEE` (PWM_RESPONSE_CHECKSUM_ERR): 校验码错误
- `0xDD` (PWM_RESPONSE_RANGE_ERR): 参数超出范围
- `0xCC` (PWM_RESPONSE_INVALID_CMD): 无效命令

## 参数范围
- PWM占空比: 0-1023
- PWM频率: 100-10000 Hz

## 错误处理
1. **校验码错误**: 如果接收到的校验码与计算值不匹配，返回PWM_RESPONSE_CHECKSUM_ERR
2. **参数范围错误**: 如果占空比或频率超出有效范围，返回PWM_RESPONSE_RANGE_ERR  
3. **命令长度错误**: 如果命令字节数不足，返回PWM_RESPONSE_INVALID_CMD
4. **未知命令**: 如果命令码无效，返回PWM_RESPONSE_INVALID_CMD

## Arduino/C++ 示例代码

```cpp
#include <Wire.h>

#define I2C_ADDR 0x50
#define PWM_CMD_SET_DUTY 0x01
#define PWM_CMD_SET_FREQ 0x02  
#define PWM_CMD_START 0x03
#define PWM_CMD_STOP 0x04
#define PWM_CMD_STATUS 0x05

// 计算校验码
uint8_t calculateChecksum(uint8_t* data, uint8_t length) {
    uint8_t sum = 0;
    for (int i = 0; i < length; i++) {
        sum += data[i];
    }
    return (~sum) + 1;  // 二进制补码
}

// 发送PWM命令
void sendPWMCommand(uint8_t cmd, uint16_t value = 0) {
    uint8_t buffer[4];
    uint8_t len;
    
    buffer[0] = cmd;
    
    if (cmd == PWM_CMD_SET_DUTY || cmd == PWM_CMD_SET_FREQ) {
        buffer[1] = (value >> 8) & 0xFF;  // 高字节
        buffer[2] = value & 0xFF;         // 低字节  
        buffer[3] = calculateChecksum(buffer, 3);
        len = 4;
    } else {
        buffer[1] = calculateChecksum(buffer, 1);
        len = 2;
    }
    
    Wire.beginTransmission(I2C_ADDR);
    for (int i = 0; i < len; i++) {
        Wire.write(buffer[i]);
    }
    Wire.endTransmission();
}

void setup() {
    Wire.begin();
    Serial.begin(9600);
    
    // 设置PWM参数并启动
    sendPWMCommand(PWM_CMD_SET_DUTY, 512);  // 设置占空比50%
    delay(10);
    sendPWMCommand(PWM_CMD_SET_FREQ, 1000); // 设置频率1kHz
    delay(10);
    sendPWMCommand(PWM_CMD_START);          // 启动PWM
}

void loop() {
    // 查询PWM状态
    sendPWMCommand(PWM_CMD_STATUS);
    delay(1000);
}
```

## 注意事项
1. 所有多字节数值采用大端序（高字节在前）
2. 每个命令都必须包含正确的校验码
3. 参数值必须在规定范围内，否则会返回范围错误
4. 建议在发送命令后检查响应码，确认命令执行状态
5. PWM输出引脚为互补输出模式，适用于H桥驱动电路