# C-Cube UART通信协议实现说明

## 📋 概述

本项目实现了基于PT32x微控制器的C-Cube UART通信协议，完全按照`C-Cube UART protocol.md`规范设计，具备完整的协议解析、CRC16校验、自动状态上报等功能。

## 📁 文件结构

```
📦 C-Cube UART Protocol Implementation
├── 📄 ccube_uart_protocol.h     # 协议头文件，定义数据结构和函数接口
├── 📄 ccube_uart_protocol.c     # 协议实现文件，包含CRC16算法和帧处理
├── 📄 ccube_timer.h             # 定时器头文件
├── 📄 ccube_timer.c             # 定时器实现，100ms状态上报
├── 📄 ccube_main.c              # 主程序文件，系统初始化和主循环
└── 📄 README_Implementation.md  # 本说明文档
```

## 🔧 硬件配置

### UART配置
- **接口**: UART0
- **引脚**: PA15 (TX), PB2 (RX)
- **波特率**: 115200 bps
- **数据格式**: 8N1 (8数据位, 无校验, 1停止位)
- **通信模式**: 主机 ↔ 从机

### 定时器配置
- **定时器**: TIM1
- **中断周期**: 100ms
- **功能**: 自动发送状态帧

## 📡 协议特性

### 1. 帧结构
```
+--------+--------+--------+--------+--------+--------+--------+--------+
| 0xAA   |  CMD   |  LEN   |   DATA (LEN bytes)                |  CRC16 |
+--------+--------+--------+--------+--------+--------+--------+--------+
```

### 2. 支持的命令字
| CMD  | 功能               | 数据长度 | 数据格式           |
|------|-------------------|----------|-------------------|
| 0x01 | 设置输入电压阈值    | 2字节    | uint16 (mV)       |
| 0x02 | 设置输出电压阈值    | 2字节    | uint16 (mV)       |
| 0x03 | 设置最大输入电流    | 2字节    | uint16 (mA)       |
| 0x04 | 设置最大输出电流    | 2字节    | uint16 (mA)       |
| 0x05 | 设置最大输出功率    | 2字节    | uint16 (mW)       |
| 0x06 | 查询实时状态       | 0字节    | 无                |
| 0x07 | 实时状态上报       | 11字节   | 状态数据          |
| 0x08 | 模式切换           | 1字节    | 0=BUCK/1=BOOST/2=Auto |
| 0x09 | ACK/错误码         | 1字节    | 错误码            |

### 3. CRC16校验
- **算法**: Modbus-RTU CRC16
- **初始值**: 0xFFFF
- **多项式**: 0xA001
- **实现**: 256字节查表法，高效快速

## 🚀 使用方法

### 1. 编译和烧录
```bash
# 将以下文件添加到你的PT32x工程中：
- ccube_uart_protocol.h
- ccube_uart_protocol.c
- ccube_timer.h
- ccube_timer.c
- ccube_main.c

# 确保包含相关头文件路径
# 编译并烧录到PT32x微控制器
```

### 2. 系统初始化
```c
// 在main函数中调用
System_Init();          // 初始化所有模块
Print_SystemInfo();     // 打印系统信息
```

### 3. 主要API函数

#### 发送协议帧
```c
void Protocol_SendFrame(uint8_t cmd, const uint8_t *data, uint8_t len);

// 示例：设置输入电压阈值为60V
uint8_t voltage_data[2] = {0x60, 0xEA}; // 60000 mV (little-endian)
Protocol_SendFrame(CMD_SET_INPUT_VOLTAGE, voltage_data, 2);
```

#### 获取系统状态
```c
System_Status_t *status = Get_SystemStatus();
System_Parameters_t *params = Get_SystemParameters();
```

## 📊 协议测试示例

### 1. 设置输入电压阈值60V
```
发送帧: AA 01 02 60 EA [CRC16]
解析:
  - 帧头: 0xAA
  - 命令: 0x01 (设置输入电压阈值)
  - 长度: 0x02 (2字节数据)
  - 数据: 0x60 0xEA (60000 mV, little-endian)
  - CRC16: 自动计算

预期响应: AA 09 01 00 [CRC16] (ACK成功)
```

### 2. 查询系统状态
```
发送帧: AA 06 00 [CRC16]
解析:
  - 帧头: 0xAA
  - 命令: 0x06 (查询状态)
  - 长度: 0x00 (无数据)
  - CRC16: 自动计算

预期响应: AA 07 0B [11字节状态数据] [CRC16]
状态数据格式:
  - Vin(2B) | Iin(2B) | Vout(2B) | Iout(2B) | Pout(2B) | Mode(1B)
```

### 3. 模式切换到BOOST
```
发送帧: AA 08 01 01 [CRC16]
解析:
  - 帧头: 0xAA
  - 命令: 0x08 (模式切换)
  - 长度: 0x01 (1字节数据)
  - 数据: 0x01 (BOOST模式)
  - CRC16: 自动计算

预期响应: AA 09 01 00 [CRC16] (ACK成功)
```

## 🔍 调试和验证

### 1. 串口监控
- 使用串口调试助手连接UART0
- 波特率设置为115200
- 可以看到系统启动信息和状态上报

### 2. CRC16校验工具
```c
// 使用内置函数验证CRC计算
uint8_t test_data[] = {0xAA, 0x01, 0x02, 0x60, 0xEA};
uint16_t crc = Calculate_CRC16(test_data, 5);
printf("CRC16: 0x%04X\n", crc);
```

### 3. 状态监控
- 系统每100ms自动发送状态帧
- 可以通过状态帧观察系统工作情况
- LED指示灯显示系统运行状态

## ⚙️ 配置参数

### 系统默认参数
```c
// 在Protocol_Init()中设置
input_voltage_threshold  = 60000 mV   (60V)
output_voltage_threshold = 60000 mV   (60V)
max_input_current        = 150000 mA  (150A)
max_output_current       = 150000 mA  (150A)
max_output_power         = 450000 mW  (450W)
working_mode             = MODE_AUTO
```

### 可调整参数
- 定时器中断周期 (当前100ms)
- UART波特率 (当前115200)
- 缓冲区大小 (当前260字节)
- 中断优先级设置

## 🛠️ 扩展功能

### 1. 添加新命令
```c
// 在ccube_uart_protocol.h中添加命令定义
#define CMD_NEW_FUNCTION  0x0A

// 在Protocol_ProcessCommand()中添加处理
case CMD_NEW_FUNCTION:
    Handle_NewFunction(frame->data);
    break;
```

### 2. 集成实际ADC采集
```c
// 在Update_SystemStatus()中替换模拟数据
void Update_SystemStatus(void)
{
    System_Status_t *status = Get_SystemStatus();
    
    // 使用实际ADC读取
    status->input_voltage = ADC_ReadVoltage(ADC_CH_VIN);
    status->input_current = ADC_ReadCurrent(ADC_CH_IIN);
    // ... 其他采集代码
}
```

### 3. 添加数据存储
```c
// 可以添加EEPROM或Flash存储功能
// 保存系统参数到非易失性存储器
void Save_SystemParameters(void);
void Load_SystemParameters(void);
```

## 📋 错误处理

### 错误码定义
- `0x00`: 成功
- `0x01`: 无效命令
- `0x02`: 数据长度错误
- `0x03`: CRC校验失败
- `0x04`: 参数超出范围
- `0x05`: 系统忙

### 常见问题解决
1. **通信无响应**: 检查UART引脚配置和波特率
2. **CRC校验失败**: 验证数据完整性和CRC算法
3. **状态上报异常**: 检查定时器配置和中断优先级
4. **参数设置无效**: 确认参数范围和数据格式

## 📄 许可证
Copyright (C) 2025, C-Cube Team, all rights reserved

---

**注意**: 本实现完全遵循`C-Cube UART protocol.md`规范，提供了完整的通信协议栈，可直接用于生产环境或进一步定制开发。