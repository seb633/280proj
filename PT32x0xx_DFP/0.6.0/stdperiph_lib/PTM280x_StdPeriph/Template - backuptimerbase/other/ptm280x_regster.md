

# 📑 PTM280x 寄存器速查表

## 1. RCC (时钟控制)

* 基地址：`0x4002 1000`
* 常用寄存器：

  * `RCC_AHBENR`   – AHB 外设时钟使能
  * `RCC_APB1ENR` – APB1 外设时钟使能
  * `RCC_APB2ENR` – APB2 外设时钟使能

    * `TIM1EN`  (bit 11) → TIM1 时钟
    * `USART0EN`(bit 14) → UART0 时钟
    * `ADC0EN`  (bit 9)  → ADC0 时钟
    * `COMPEN`  (bit 31) → CMP/OPA 时钟

---

## 2. GPIO (通用 I/O)

* 基地址：`0x4800 0000` (GPIOA), `0x4800 0400` (GPIOB)
* 常用寄存器：

  * `GPIOx_MODER` – 模式寄存器 (00=输入, 01=输出, 10=复用, 11=模拟)
  * `GPIOx_AFRL/AFRH` – 复用功能选择
  * `GPIOx_OTYPER` – 输出类型 (0=推挽, 1=开漏)
  * `GPIOx_OSPEEDR` – 输出速度
  * `GPIOx_PUPDR` – 上下拉

**例**：配置 PA5 为 TIM1\_CH1：

```c
GPIOA->MODER   &= ~(3U<<(5*2));
GPIOA->MODER   |=  (2U<<(5*2));  // 复用功能
GPIOA->AFR[0]  |=  (AF2<<(5*4)); // AF2: TIM1_CH1
```

---

## 3. TIM1 (高级定时器)

* 基地址：`0x4001 2C00`
* 常用寄存器：

  * `TIM1_CR1`   – 控制寄存器1 (CEN, CMS, DIR)
  * `TIM1_CR2`   – 控制寄存器2 (TRGO 触发输出选择)
  * `TIM1_SMCR`  – 从模式控制 (同步触发)
  * `TIM1_DIER`  – 中断/事件使能
  * `TIM1_SR`    – 状态寄存器
  * `TIM1_EGR`   – 事件发生寄存器
  * `TIM1_CCMR1/2` – 捕获比较模式 (PWM模式设置)
  * `TIM1_CCER`  – 输出使能 (CHxE/CHxNE)
  * `TIM1_BDTR`  – 死区时间 & 主输出使能 (MOE, BKIN)
  * `TIM1_PSC`   – 预分频器
  * `TIM1_ARR`   – 自动重装值 (设定PWM周期)
  * `TIM1_CCRx`  – 通道占空比比较值 (设定PWM占空比)

---

## 4. ADC0

* 基地址：`0x4001 2400`
* 常用寄存器：

  * `ADC_CR`    – 控制寄存器 (ADEN, ADSTART)
  * `ADC_CFGR1` – 配置1 (触发源选择, 分辨率)
  * `ADC_CFGR2` – 配置2 (采样时间)
  * `ADC_CHSELR` – 通道选择
  * `ADC_DR`    – 数据寄存器 (读取转换结果)
  * `ADC_ISR`   – 状态 (EOC, EOS)
  * `ADC_IER`   – 中断使能

**关键位**：

* `EXTEN` (触发边沿) / `EXTSEL` (外部触发源 = TIM1\_TRGO)
* `RES[1:0]` = 00 → 12 位

---

## 5. CMP (比较器) + LDAC

* 基地址：`0x4001 3C00`
* 常用寄存器：

  * `CMPx_CSR` – 配置寄存器

    * `CMPEN`   → 启用比较器
    * `INPSEL`  → 选择正端输入 (ADC通道/OPA输出)
    * `INNSEL`  → 选择负端输入 (LDAC, BG1V0, 外部引脚)
    * `POL`     → 输出极性
    * `OUTSEL`  → 输出到 EXTI / TIM1\_BKIN
  * `LDAC_CR` – 内部 5 位 DAC 配置 (输出 0\~VDDA\*31/32)

---

## 6. UART0

* 基地址：`0x4001 3800`
* 常用寄存器：

  * `USART_BRR`  – 波特率寄存器
  * `USART_CR1`  – 控制1 (UE, TE, RE, RXNEIE)
  * `USART_CR2`  – 控制2 (停止位)
  * `USART_CR3`  – 控制3 (DMA, 硬件流控)
  * `USART_ISR`  – 状态 (TXE, RXNE, TC)
  * `USART_TDR`  – 发送数据寄存器
  * `USART_RDR`  – 接收数据寄存器

**关键位**：

* `UE` = 1 → 使能
* `TE` = 1 → 发送使能
* `RE` = 1 → 接收使能

---

## 7. OPA (运算放大器)

* 基地址：`0x4001 4000`
* 常用寄存器：

  * `OPA_CSR` – 控制寄存器

    * `OPAEN` → 使能
    * `MODE`  → 跟随器/放大倍数 (×1, ×2, ×5, ×10)
    * `PSEL`  → 正端选择
    * `NSEL`  → 负端选择
    * `OUTEN` → 输出使能 (OPA\_OUTx 引脚/连到ADC)

---



> “这里是 PTM280x 的外设寄存器表，请在我写好的初始化框架代码中，用寄存器级配置填充 `// TODO:` 部分。例如启用时钟时写 RCC->APB2ENR |= (1<<11) 这样的语句，配置 TIM1 互补PWM、ADC触发、CMP比较器、UART0 通信。”

---

