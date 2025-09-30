

# 📄 chatgpt\_notes.md

## 项目名称

PTM280x 双向 DCDC （C-Cube 双向 Buck/Boost 控制）

---

## 硬件环境

* **MCU**: PTM280x （LQFP32 封装, VDDA=5V）
* **功率驱动**: 半桥驱动芯片 SA2612
* **拓扑**: 双向 Buck/Boost
* **MOS/电感**: 每组独立，主控芯片实现同步控制

---

## 工作模式

1. **BUCK 模式 (充电)**

   * 条件：输入电压 Vin > 60V
   * 控制：TIM1 输出互补 PWM，经 SA2612 驱动 MOS

2. **BOOST 模式 (放电/升压充电)**

   * 条件：输出电压 Vout < 55V
   * 控制：TIM1 输出互补 PWM，经 SA2612 驱动 MOS

3. **模式切换**

   * 通过 ADC + CMP 判断
   * CMP 使用 LDAC 参考电压，触发中断后切换 BUCK / BOOST

---

## 外设规划

### GPIO

* PA5 → TIM1\_CH1 (PWM 高侧输出)
* PA7 → TIM1\_CH1N (PWM 低侧输出)
* PB12 → TIM1\_BKIN (紧急关断输入)
* PA15 → UART0\_TX
* PB2 → UART0\_RX
* PA3/PA4 → OPA 输入
* PA5 → OPA 输出/ADC 输入

---

### TIM1 (PWM 定时器)

* **频率**: 50\~100kHz
* **输出**: CH1/CH1N 互补输出
* **死区时间**: 根据 MOS/SA2612 驱动需求配置 (\~200ns 示例)
* **触发**: TRGO 触发 ADC 转换
* **保护**: BKIN 输入紧急关断

---

### ADC0

* **通道**: IN0 (Vin), IN2 (Vout)
* **触发源**: TIM1 TRGO
* **分辨率**: 12bit
* **采样方式**: 定时触发 + 中断读取

---

### OPA

* **OPA0**: 跟随器模式，用于 Vin/Vout 分压缓冲，输出送 ADC/CMP
* 配置：正端=P0，负端=OPA\_OUT，自偏置，输出使能

---

### CMP + LDAC

* **CMP1**: Vin 比较，阈值=60V

  * 正端=OPA 输出 (Vin 分压)
  * 负端=LDAC 电压 (对应 60V 分压电平)
  * 输出到 EXTI
* **CMP0**: Vout 比较，阈值=55V

  * 正端=OPA 输出 (Vout 分压)
  * 负端=LDAC 电压 (对应 55V 分压电平)
  * 输出到 EXTI
* **LDAC**: 5bit DAC，输出参考电压

---

### UART0

* **引脚**: PA15 (TX), PB2 (RX)
* **波特率**: 115200bps (可扩展到 1Mbps)
* **用途**: 通信调试 / 上位机监控

---

## 软件逻辑

### 初始化流程

1. SystemClock\_Config → 配置系统时钟 (PLL = 48MHz)
2. GPIO\_Config → 配置 PWM / UART / OPA / BKIN 引脚
3. OPA\_Config → 启用 OPA 跟随器模式
4. ADC\_Config → 配置 ADC (TIM1 触发)
5. CMP\_Config → 配置 CMP (Vin=60V, Vout=55V 阈值)
6. TIM1\_PWM\_Config → 配置互补 PWM, 死区, TRGO
7. UART0\_Config → 配置串口调试

### 运行逻辑

* **ADC 中断**：周期采样 Vin/Vout
* **CMP 中断**：根据阈值切换 BUCK/BOOST 模式
* **TIM1**：根据模式调整占空比
* **UART**：发送调试信息 / 接收上位机指令

---

## Copilot 指令建议

> * 在 `// TODO:` 注释处补全寄存器级配置
> * 参考本文件中的 **外设映射 / 引脚 / 阈值 / 时钟 / 模式**
> * 所有外设初始化要基于寄存器操作，而不是 HAL 库
> * 优化 PWM 占空比调节逻辑，实现 BUCK/BOOST 模式切换
> * 增加 UART 协议帧解析函数

---
