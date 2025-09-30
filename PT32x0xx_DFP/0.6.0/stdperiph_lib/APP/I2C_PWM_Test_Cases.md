# I2C PWM 协议测试用例 - 校验和参数验证

## 测试概述
本文档包含用于验证I2C PWM控制协议中校验和验证和参数范围检查功能的测试用例。

## 测试环境
- 微控制器: AD16H02
- I2C地址: 0x50 (基地址，实际地址由PB6/PB7配置)
- 测试工具: Arduino或其他I2C主机设备

## 测试用例

### 1. 校验和验证测试

#### 测试1.1: 正确校验和 - 设置PWM占空比
```
命令: 设置占空比为512 (0x0200)
发送数据: [0x01, 0x02, 0x00, 0xFD]
计算过程: sum = 0x01 + 0x02 + 0x00 = 0x03, checksum = (~0x03) + 1 = 0xFD
期望响应: [0xAA] (PWM_RESPONSE_OK)
```

#### 测试1.2: 错误校验和 - 设置PWM占空比
```
命令: 设置占空比为512 (故意错误的校验码)
发送数据: [0x01, 0x02, 0x00, 0xFC] (正确应为0xFD)
期望响应: [0xEE] (PWM_RESPONSE_CHECKSUM_ERR)
```

#### 测试1.3: 正确校验和 - PWM启动命令
```
命令: 启动PWM
发送数据: [0x03, 0xFD]  
计算过程: sum = 0x03, checksum = (~0x03) + 1 = 0xFD
期望响应: [0xAA] (PWM_RESPONSE_OK)
```

#### 测试1.4: 错误校验和 - PWM启动命令  
```
命令: 启动PWM (故意错误的校验码)
发送数据: [0x03, 0xFE] (正确应为0xFD)
期望响应: [0xEE] (PWM_RESPONSE_CHECKSUM_ERR)
```

### 2. 参数范围验证测试

#### 测试2.1: PWM占空比 - 有效范围内
```
测试值: 0 (最小值)
发送数据: [0x01, 0x00, 0x00, 0xFF]
计算过程: sum = 0x01 + 0x00 + 0x00 = 0x01, checksum = (~0x01) + 1 = 0xFF
期望响应: [0xAA] (PWM_RESPONSE_OK)

测试值: 1023 (最大值)  
发送数据: [0x01, 0x03, 0xFF, 0xFD]
计算过程: sum = 0x01 + 0x03 + 0xFF = 0x103 & 0xFF = 0x03, checksum = (~0x03) + 1 = 0xFD
期望响应: [0xAA] (PWM_RESPONSE_OK)

测试值: 512 (中间值)
发送数据: [0x01, 0x02, 0x00, 0xFD]  
期望响应: [0xAA] (PWM_RESPONSE_OK)
```

#### 测试2.2: PWM占空比 - 超出范围
```
测试值: 1024 (超出最大值)
发送数据: [0x01, 0x04, 0x00, 0xFB]
计算过程: sum = 0x01 + 0x04 + 0x00 = 0x05, checksum = (~0x05) + 1 = 0xFB
期望响应: [0xDD] (PWM_RESPONSE_RANGE_ERR)

测试值: 65535 (远超最大值)
发送数据: [0x01, 0xFF, 0xFF, 0x02]
计算过程: sum = 0x01 + 0xFF + 0xFF = 0x1FF & 0xFF = 0xFF, checksum = (~0xFF) + 1 = 0x01
实际校验码应为: 0x02 (因为0x01溢出)
期望响应: [0xDD] (PWM_RESPONSE_RANGE_ERR)
```

#### 测试2.3: PWM频率 - 有效范围内
```
测试值: 100 Hz (最小值)
发送数据: [0x02, 0x00, 0x64, 0x9A]
计算过程: sum = 0x02 + 0x00 + 0x64 = 0x66, checksum = (~0x66) + 1 = 0x9A
期望响应: [0xAA] (PWM_RESPONSE_OK)

测试值: 10000 Hz (最大值)
发送数据: [0x02, 0x27, 0x10, 0xC7] 
计算过程: sum = 0x02 + 0x27 + 0x10 = 0x39, checksum = (~0x39) + 1 = 0xC7
期望响应: [0xAA] (PWM_RESPONSE_OK)

测试值: 1000 Hz (中间值)
发送数据: [0x02, 0x03, 0xE8, 0x13]
期望响应: [0xAA] (PWM_RESPONSE_OK)
```

#### 测试2.4: PWM频率 - 超出范围  
```
测试值: 99 Hz (低于最小值)
发送数据: [0x02, 0x00, 0x63, 0x9B]
计算过程: sum = 0x02 + 0x00 + 0x63 = 0x65, checksum = (~0x65) + 1 = 0x9B
期望响应: [0xDD] (PWM_RESPONSE_RANGE_ERR)

测试值: 10001 Hz (超出最大值)
发送数据: [0x02, 0x27, 0x11, 0xC6]
计算过程: sum = 0x02 + 0x27 + 0x11 = 0x3A, checksum = (~0x3A) + 1 = 0xC6
期望响应: [0xDD] (PWM_RESPONSE_RANGE_ERR)
```

### 3. 命令长度验证测试

#### 测试3.1: 命令长度不足
```
测试: 设置占空比命令缺少数据字节
发送数据: [0x01, 0x02] (缺少低字节和校验码)
期望响应: [0xCC] (PWM_RESPONSE_INVALID_CMD)

测试: 启动命令缺少校验码
发送数据: [0x03] (缺少校验码)  
期望响应: [0xCC] (PWM_RESPONSE_INVALID_CMD)
```

### 4. 状态查询测试（带校验码响应）

#### 测试4.1: 正确的状态查询
```
发送数据: [0x05, 0xFB]
计算过程: sum = 0x05, checksum = (~0x05) + 1 = 0xFB
期望响应: [0xAA, 状态, 占空比高, 占空比低, 频率高, 频率低, 响应校验码]
响应长度: 7字节

验证响应校验码:
- 计算前6字节的和
- 验证第7字节是否为正确的二进制补码校验码
```

### 5. Arduino测试代码示例

```cpp
#include <Wire.h>

#define I2C_ADDR 0x50

// 校验码计算函数
uint8_t calculateChecksum(uint8_t* data, uint8_t length) {
    uint8_t sum = 0;
    for (int i = 0; i < length; i++) {
        sum += data[i];
    }
    return (~sum) + 1;
}

// 测试函数
void testChecksumValidation() {
    Serial.println("=== 校验码验证测试 ===");
    
    // 测试1: 正确校验码
    Serial.println("测试1: 设置占空比512，正确校验码");
    uint8_t cmd1[] = {0x01, 0x02, 0x00, 0xFD};
    sendCommand(cmd1, 4);
    uint8_t response = readResponse();
    Serial.print("响应: 0x"); Serial.println(response, HEX);
    Serial.println(response == 0xAA ? "通过" : "失败");
    
    // 测试2: 错误校验码
    Serial.println("测试2: 设置占空比512，错误校验码");
    uint8_t cmd2[] = {0x01, 0x02, 0x00, 0xFC}; // 故意错误
    sendCommand(cmd2, 4);
    response = readResponse();
    Serial.print("响应: 0x"); Serial.println(response, HEX);
    Serial.println(response == 0xEE ? "通过" : "失败");
}

void testParameterRange() {
    Serial.println("=== 参数范围验证测试 ===");
    
    // 测试占空比超出范围
    Serial.println("测试: 占空比1024（超出最大值1023）");
    uint8_t cmd[] = {0x01, 0x04, 0x00};
    cmd[3] = calculateChecksum(cmd, 3);
    sendCommand(cmd, 4);
    uint8_t response = readResponse();
    Serial.print("响应: 0x"); Serial.println(response, HEX);
    Serial.println(response == 0xDD ? "通过" : "失败");
    
    // 测试频率超出范围
    Serial.println("测试: 频率99Hz（低于最小值100Hz）");
    uint8_t cmd2[] = {0x02, 0x00, 0x63};
    cmd2[3] = calculateChecksum(cmd2, 3);
    sendCommand(cmd2, 4);
    response = readResponse();
    Serial.print("响应: 0x"); Serial.println(response, HEX);
    Serial.println(response == 0xDD ? "通过" : "失败");
}

void sendCommand(uint8_t* data, uint8_t length) {
    Wire.beginTransmission(I2C_ADDR);
    for (int i = 0; i < length; i++) {
        Wire.write(data[i]);
    }
    Wire.endTransmission();
    delay(10); // 等待处理
}

uint8_t readResponse() {
    Wire.requestFrom(I2C_ADDR, 1);
    if (Wire.available()) {
        return Wire.read();
    }
    return 0x00; // 无响应
}

void setup() {
    Serial.begin(9600);
    Wire.begin();
    delay(1000);
    
    Serial.println("开始I2C PWM协议测试");
    testChecksumValidation();
    delay(1000);
    testParameterRange();
}

void loop() {
    // 空循环
}
```

## 测试结果记录

### 预期测试结果汇总
| 测试用例 | 输入数据 | 期望响应 | 测试重点 |
|----------|----------|----------|----------|
| 正确校验-占空比 | [0x01,0x02,0x00,0xFD] | 0xAA | 校验码验证 |
| 错误校验-占空比 | [0x01,0x02,0x00,0xFC] | 0xEE | 校验码错误处理 |
| 占空比范围内 | [0x01,0x02,0x00,0xFD] | 0xAA | 参数有效性 |
| 占空比超范围 | [0x01,0x04,0x00,0xFB] | 0xDD | 参数范围检查 |
| 频率范围内 | [0x02,0x03,0xE8,0x13] | 0xAA | 频率参数验证 |
| 频率超范围 | [0x02,0x00,0x63,0x9B] | 0xDD | 频率范围检查 |
| 命令长度不足 | [0x01,0x02] | 0xCC | 命令格式验证 |

## 测试注意事项
1. 确保I2C地址配置正确（通过PB6/PB7引脚设置）
2. 测试前确认微控制器已正常初始化
3. 校验码计算使用8位运算，注意溢出处理
4. 所有多字节数据采用大端序（高字节在前）
5. 建议逐个测试用例执行，避免状态干扰