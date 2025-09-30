# CMix双向DCDC控制器 - 反向设计文档

**基于代码分析生成的系统设计规格**  
**生成时间**: 2025年9月17日  
**分析范围**: CMix_*.c/h 全部源码文件  

---

## 📋 系统概述

### 产品定义
- **产品名称**: CMix双向DCDC控制器
- **版本**: V1.0.0 
- **目标平台**: PT32x (PTM280x) Cortex-M0+
- **系统时钟**: 48MHz HSI内部RC振荡器
- **主要功能**: 双向DC-DC电源变换控制

### 核心特性
- **双向变换**: BUCK (降压) + BOOST (升压) 模式
- **智能切换**: 基于电压检测的自动模式选择
- **安全保护**: 硬件BKIN + 软件CMP双重保护
- **通信协议**: Modbus-RTU over UART
- **实时监控**: 完整的状态监控和调试系统

---

## 🏗️ 系统架构

### 软件架构
```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   CMix_main.c   │    │ CMix_protocol.c │    │  CMix_dcdc.c    │
│   主控制器       │◄──►│    通信协议      │    │   DCDC控制      │
│   任务调度       │    │   Modbus-RTU    │    │   PWM生成       │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                        │                        │
         ▼                        ▼                        ▼
┌─────────────────────────────────────────────────────────────────┐
│                    CMix_hardware.c                              │
│              硬件抽象层 (HAL) - PT32x驱动                        │
│           ADC | PWM | UART | GPIO | CMP | OPA                  │
└─────────────────────────────────────────────────────────────────┘
```

### 硬件接口映射
| 功能模块 | 引脚配置 | 说明 |
|----------|----------|------|
| **ADC采样** | PB12(ADC0_IN0)=Vin, PA2(ADC0_IN2)=Vout | 12位ADC，5V基准 |
| **PWM输出** | PA5(TIM1_CH1), PA7(TIM1_CH1N) | 100kHz互补PWM |
| **硬件保护** | PA8(CMP1), PB4(CMP0) → BKIN | 立即断电保护 |
| **OPA缓冲** | PA3(输入) → 内部连接ADC | 电压跟随器 |
| **UART通信** | PA15(TX), PB2(RX) | 115200bps + CRC16 |
| **状态指示** | PA4(运行LED), PA5(故障LED) | 状态可视化 |

---

## ⚙️ 功能规格

### 1. 双向DCDC变换器控制

#### 工作模式定义
```c
typedef enum {
    CMIX_MODE_IDLE = 0,           // 空闲模式
    CMIX_MODE_BUCK = 1,           // BUCK降压模式 (充电)
    CMIX_MODE_BOOST = 2,          // BOOST升压模式 (放电)
    CMIX_MODE_BIDIRECTIONAL = 3,  // 双向模式
    CMIX_MODE_STANDBY = 4,        // 待机模式
    CMIX_MODE_AUTO = 5            // 自动切换模式
} CMix_Working_Mode_t;
```

#### 模式切换逻辑
- **BUCK模式**: 当Vin > 设定阈值(60V)时自动进入
- **BOOST模式**: 当Vout < 设定阈值(55V)时自动进入  
- **AUTO模式**: 根据输入输出电压自动选择最佳模式
- **切换保护**: 模式切换前强制关闭PWM，防止直通

#### PI控制器参数
```c
// 电压环PI参数
#define CMIX_VOLTAGE_PI_KP  0.5f    // 比例系数
#define CMIX_VOLTAGE_PI_KI  0.01f   // 积分系数

// 电流环PI参数  
#define CMIX_CURRENT_PI_KP  1.0f    // 比例系数
#define CMIX_CURRENT_PI_KI  0.02f   // 积分系数
```

### 2. PWM波形生成

#### 时序参数
- **PWM频率**: 100kHz (ARR=479, 48MHz时钟)
- **死区时间**: ~200ns (可调100-500ns)
- **占空比范围**: 5%-95% (50-950)
- **初始状态**: 上电默认关闭，需软件使能

#### 硬件配置
```c
#define CMIX_PWM_FREQUENCY_HZ   100000  // 100kHz
#define CMIX_PWM_PERIOD         480     // 周期计数
#define CMIX_PWM_DEADTIME       10      // 死区时间
```

### 3. 安全保护系统

#### 硬件保护机制
- **BKIN硬件联锁**: CMP0/CMP1输出直连TIM1_BKIN
- **触发方式**: 低电平立即触发，硬件自动关闭PWM
- **响应时间**: < 1μs 硬件响应
- **保护优先级**: 最高优先级，无法被软件屏蔽

#### 软件保护算法
```c
// 保护阈值定义
#define CMIX_MAX_INPUT_VOLTAGE   100000  // 100V
#define CMIX_MAX_OUTPUT_VOLTAGE  100000  // 100V
#define CMIX_MAX_INPUT_CURRENT   10000   // 10A
#define CMIX_MAX_OUTPUT_CURRENT  20000   // 20A
#define CMIX_FAULT_THRESHOLD     5       // 故障确认次数
```

#### 故障类型分类
```c
typedef enum {
    CMIX_ERROR_NONE = 0,              // 无故障
    CMIX_ERROR_OVERVOLTAGE = 1,       // 过压
    CMIX_ERROR_OVERCURRENT = 2,       // 过流
    CMIX_ERROR_OVERPOWER = 3,         // 过功率
    CMIX_ERROR_OVERTEMPERATURE = 4,   // 过温
    CMIX_ERROR_COMMUNICATION = 5,     // 通信错误
    CMIX_ERROR_HARDWARE = 6           // 硬件错误
} CMix_Error_t;
```

### 4. 通信协议栈

#### Modbus-RTU协议
- **物理层**: UART 115200bps, 8N1
- **数据校验**: CRC16-Modbus标准
- **帧格式**: [帧头0xAA][命令][长度][数据][CRC16]
- **超时处理**: 接收超时和错误重传机制

#### 支持命令集
```c
typedef enum {
    CMIX_CMD_SET_INPUT_VOLTAGE      = 0x01,  // 设置输入电压阈值
    CMIX_CMD_SET_OUTPUT_VOLTAGE     = 0x02,  // 设置输出电压阈值
    CMIX_CMD_SET_MAX_INPUT_CURRENT  = 0x03,  // 设置最大输入电流
    CMIX_CMD_SET_MAX_OUTPUT_CURRENT = 0x04,  // 设置最大输出电流
    CMIX_CMD_SET_MAX_OUTPUT_POWER   = 0x05,  // 设置最大输出功率
    CMIX_CMD_QUERY_STATUS          = 0x06,  // 查询实时状态
    CMIX_CMD_STATUS_REPORT         = 0x07,  // 实时状态上报
    CMIX_CMD_MODE_SWITCH           = 0x08,  // 模式切换
    CMIX_CMD_ACK_ERROR             = 0x09   // ACK/错误码
} CMix_Protocol_Command_t;
```

### 5. 任务调度系统

#### 多任务时序
```c
// 任务调度时序
1ms任务:  DCDC控制环、安全检查、PWM更新
10ms任务: LED控制、系统监控  
100ms任务: 状态上报、性能监控、调试输出
1000ms任务: 运行时统计、长期监控
```

#### 系统状态机
```c
typedef enum {
    CMIX_STATE_INIT = 0,         // 初始化状态
    CMIX_STATE_IDLE = 1,         // 空闲状态  
    CMIX_STATE_RUNNING = 2,      // 运行状态
    CMIX_STATE_FAULT = 3,        // 故障状态
    CMIX_STATE_SOFT_START = 4,   // 软启动状态
    CMIX_STATE_EMERGENCY_STOP = 5, // 紧急停机
    CMIX_STATE_SHUTDOWN = 6      // 关机状态
} CMix_State_t;
```

---

## 🔧 硬件规格

### 模拟前端设计

#### ADC配置
- **分辨率**: 12位 (0-4095)
- **基准电压**: 5.0V  
- **采样触发**: TIM1 TRGO自动触发
- **转换时间**: < 10μs
- **输入通道**: 
  - `Vin → PB12 (ADC0_IN0, Channel_0)`
  - `Vout → PA2 (ADC0_IN2, Channel_2)`

#### OPA缓冲器
- **型号**: OPA0电压跟随器
- **输入**: PA3 (Vin分压信号)
- **输出**: 内部连接至ADC (不使用外部GPIO)
- **用途**: 高阻抗信号缓冲，提高ADC采样精度

#### 比较器保护
```c
// CMP1过压保护 (Vin)
#define CMIX_CMP1_UNIT           CMP1
#define CMIX_CMP1_INPUT_PIN      GPIO_Pin_8    // PA8
#define CMIX_CMP1_LDAC_THRESHOLD 3000          // 3.0V→60V

// CMP0过压保护 (Vout)  
#define CMIX_CMP0_UNIT           CMP0
#define CMIX_CMP0_INPUT_PIN      GPIO_Pin_4    // PB4
#define CMIX_CMP0_LDAC_THRESHOLD 2750          // 2.75V→55V
```

### 功率级接口

#### PWM驱动
- **主开关**: TIM1_CH1 (PA5) - 高边开关
- **同步开关**: TIM1_CH1N (PA7) - 低边开关  
- **死区设置**: 200ns防止直通
- **PWM极性**: 高电平有效

#### 保护输入
- **BKIN输入**: PA8 (来自CMP1输出)
- **触发极性**: 低电平触发保护
- **响应机制**: 硬件立即关闭PWM输出
- **恢复方式**: 故障清除后软件重启

---

## 📊 性能指标

### 控制性能
| 参数 | 规格 | 实现方式 |
|------|------|----------|
| **开关频率** | 100kHz | TIM1硬件PWM |
| **电压精度** | ±1% | PI闭环控制 |
| **电流限制** | 可编程 | 软件+硬件双重限制 |
| **效率** | >90% | 优化控制算法 |
| **响应时间** | <10ms | 1ms控制周期 |

### 保护性能
| 保护类型 | 响应时间 | 实现方式 |
|----------|----------|----------|
| **硬件过压** | <1μs | CMP→BKIN硬件 |
| **软件过流** | <5ms | ADC检测+软件 |
| **通信超时** | 100ms | UART协议层 |
| **看门狗** | 1s | IWDG硬件 |

### 通信性能
| 参数 | 规格 | 说明 |
|------|------|------|
| **波特率** | 115200bps | UART0硬件 |
| **协议** | Modbus-RTU | 标准工业协议 |
| **校验** | CRC16 | 查表法实现 |
| **延迟** | <10ms | 实时响应 |

---

## 🔍 代码架构分析

### 模块化设计
```
CMix_config.h    : 系统配置和硬件定义 (298行)
CMix_main.c      : 主控制和任务调度 (572行)  
CMix_hardware.c  : 硬件抽象层驱动 (767行)
CMix_dcdc.c      : DCDC控制算法 (582行)
CMix_protocol.c  : 通信协议栈 (527行)
```

### 关键函数接口
```c
// DCDC控制核心
void CMix_DCDC_Init(void);
void CMix_DCDC_Control_Task(void);
void CMix_DCDC_Safety_Check(void);
void CMix_DCDC_Emergency_Stop(uint8_t fault_code);

// 硬件驱动接口
void CMix_Hardware_Init(void);
void CMix_Hardware_Set_PWM_Duty(uint8_t channel, uint16_t duty);
uint16_t CMix_Hardware_ADC_Read(uint8_t channel);

// 协议通信
void CMix_Protocol_Init(void);
void CMix_Protocol_Process_Received_Data(void);
void CMix_Protocol_Send_Status_Report(void);
```

### 数据结构设计
```c
// DCDC状态结构
typedef struct {
    CMix_Mode_t mode;              // 当前工作模式
    CMix_State_t state;            // 系统状态  
    uint32_t input_voltage;        // 输入电压 (mV)
    uint32_t output_voltage;       // 输出电压 (mV)
    uint32_t input_current;        // 输入电流 (mA)
    uint32_t output_current;       // 输出电流 (mA) 
    uint32_t output_power;         // 输出功率 (mW)
    uint16_t pwm_duty_buck;        // BUCK占空比
    uint16_t pwm_duty_boost;       // BOOST占空比
    uint8_t efficiency;            // 效率百分比
} CMix_DCDC_Status_t;
```

---

## 🎯 设计亮点

### 1. 安全性设计
- **双重保护**: 硬件BKIN + 软件CMP检测
- **快速响应**: 硬件保护<1μs响应时间
- **故障隔离**: 多级故障检测和分类处理
- **自恢复机制**: 故障清除后自动恢复运行

### 2. 可靠性设计  
- **看门狗保护**: IWDG硬件看门狗防止死机
- **软启动**: PWM渐进式上升避免冲击
- **防抖机制**: 模式切换前ADC二次确认
- **错误计数**: 连续故障计数防止误触发

### 3. 可维护性设计
- **模块化架构**: 清晰的层次结构和接口
- **完整日志**: printf重定向到UART调试输出  
- **状态可视**: LED指示和实时状态监控
- **标准协议**: Modbus-RTU工业标准通信

### 4. 扩展性设计
- **参数可配**: 大量宏定义支持定制化
- **模式丰富**: 支持多种工作模式和自动切换
- **接口标准**: 标准化的硬件和软件接口
- **版本管理**: 完整的版本信息和兼容性

---

## 📋 技术规格总结

| 类别 | 项目 | 规格 |
|------|------|------|
| **处理器** | MCU | PT32x (PTM280x) Cortex-M0+ |
|  | 时钟 | 48MHz HSI |
|  | Flash | 程序存储 |
| **电源** | 输入电压 | 10V-100V |
|  | 输出电压 | 5V-100V |
|  | 最大电流 | 20A |
|  | 最大功率 | 500W |
| **控制** | PWM频率 | 100kHz |
|  | 控制周期 | 1ms |
|  | 保护响应 | <1μs (硬件) |
| **通信** | 接口 | UART |
|  | 波特率 | 115200bps |
|  | 协议 | Modbus-RTU + CRC16 |
| **保护** | 过压保护 | CMP硬件 + ADC软件 |
|  | 过流保护 | ADC软件检测 |
|  | 硬件联锁 | TIM1_BKIN |
|  | 看门狗 | IWDG |

---

*此文档基于CMix系统源码反向分析生成*  
*生成时间: 2025-09-17*  
*分析工具: GitHub Copilot*