# 📘 PTM280x 双向 DCDC 控制开发说明书（Template 专用）

本文档总结硬件连接建议、外设引脚映射、ADC/OPA/CMP/PWM 配置要点与软件流程，结合 `Template/` 目录中的 DCDC 控制框架代码（`main.c`, `adc_monitor.[ch]`, `dcdc_control.[ch]`, `fault_detect.[ch]`, `comm.[ch]`）。

---

## 1. 总体设计思路（关键点）

1. 高压测量不要直接上 ADC：用高阻分压把高压降到安全范围（为了降低功耗分压阻值可做大），再用芯片内部 OPA 做缓冲/放大（把高阻源驱动到 ADC 要求的低阻）。手册提醒 ADC 对外部源阻抗有限制（见 RAIN 的最大公式/表），所以必须用 OPA 缓冲或把分压等效阻抗降至允许范围。
2. 比较器（CMP）用于快速硬件阈值判断：把经 OPA 缓冲后的电压送给 CMP 的正输入，用内部 LDAC 或 BG1v0 给 CMP 负端设定阈值（LDAC 可编程 5bit），CMP 输出可触发中断或（在设计允许时）与 PWM/定时器联动以尽快切换模式。手册说明 CMP 支持多个 P 输入和 N0/LDAC/BG1v0 作为负端。
3. PWM 用 TIM1/TIM8：TIM1/TIM8 是高级定时器，支持互补输出、死区、BKIN 紧急制动、并能产生 ADC 触发（TRGO）用于同步采样。把主 PWM 由 TIM1（或 TIM8）产生，给从机同步信号（外部线或 TRGO），并用 BKIN 做硬件断开（过流/急停）。
4. UART 做主从通信与配置下发（波特率 1~3 Mbps 可选），建议用 UART0。

---

## 2. 建议的引脚 & 外设映射（以常见 LQFP32/QFN32 为例；按实际封装核对）

- 测量（ADC）通道：
  - Vin（高压分压 + OPA）：`ADC0_IN0` = PB12
  - Vout（分压 + OPA）：`ADC0_IN1` = PB13 或 `ADC0_IN2` = PA2
  - 其他电流/备用：`ADC0_IN4/5/6`（PA4/PA5/PA6）等
- 运算放大器 OPA（缓冲/PGA）：
  - OPA0 输出：`OPA0_OUT0` -> PA5（与 TIM1_CH1 复用，注意管脚模式）
  - OPA0 输入：`OPA0_P0` = PA3，`OPA0_N0` = PA4
- 比较器 CMP（快速阈值）：
  - CMP0_OUT：PB4 或 PA9；CMP1_OUT：PA8 或 PB7
  - 负端阈值：内部 `LDAC` 或 `BG1v0`（推荐 LDAC）
- 高级定时器 / PWM：
  - 主 PWM 用 TIM1：TIM1_CH1 = PA5，TIM1_CH1N = PA7（示例），BKIN = PB12 或 PA6
  - 可用 TIM8 作为副 PWM/同步
- UART（主从通信）：
  - UART0_TX = PA15（或 PA14/PA9/PA8），UART0_RX = PB2

> 请以数据手册引脚复用表最终核对并固定 PIN。

---

## 3. ADC + 分压 + OPA（含计算与注意事项）

- 参考电压：VDDA 建议 5.0 V（ADC 参考 = VDDA）。
- Vin 测量示例：Rtop≈950 kΩ，Rbot≈50 kΩ，总约 1 MΩ，分压比 ≈ 0.05；Vin=60 V → 3.0 V；Vin=100 V → 5.0 V。高阻分压需 OPA 缓冲。
- OPA 配置：跟随器（×1）或 PGA（×2/×5/×10）。OPA 输出连 ADC 或 CMP。
- 采样时间：选择足够采样时间以满足 RAIN 限制；或使用 OPA 缓冲降低源阻。
- 推荐：ADC 周期采样用于闭环与记录；阈值判定由 CMP 硬件快速响应。

---

## 4. CMP（比较器）配置与阈值管理

- 功能分配建议：
  - CMP1：检测 Vin > 60 V（进入 BUCK）
  - CMP0：检测 Vout < 55 V（进入 BOOST）
- 阈值设定：用 5-bit `LDAC` 设定等效阈值电压；启动时校准。
- 响应路径：
  - 硬件快速：CMP → EXTI/联动定时器/BKIN 迅速动作
  - 软件稳态：ADC 复核 + 防抖去抖，再切模式
- CMP 与 BKIN：CMP 输出可接入 TIM BKIN，实现硬件级紧急停止。

---

## 5. PWM（TIM1/TIM8）配置建议

- TIM1 作为主 PWM：互补输出、死区、BKIN；中心/边沿对齐按拓扑选择；死区 100~500 ns 量级。
- TRGO 触发 ADC 同步采样，降低采样相位误差。
- 多从模块同步：
  - 方案 A：主发 PWM/同步脉冲至各从模块，硬件同步好
  - 方案 B：主发占空比/模式命令，从模块本地锁相生成 PWM（模块化）
- 安全：BKIN 用于过流/急停，立即关断 PWM。

---

## 6. UART 配置（主从通信）

- 外设：UART0；引脚：TX=PA15, RX=PB2；波特率：115200~1/3 Mbps。
- 协议建议：`[Header][Cmd][Data][CRC]`；支持模式切换、占空比下发、状态回报、心跳。

---

## 7. 软件工作流程建议（主控）

1) 初始化：ADC、OPA、CMP、PWM、UART、命令接口。
2) 常规循环：
   - CMP 触发 → 立即硬件动作（或软切）
   - ADC 平滑/平均 → 二次确认与阈值去抖
   - 按模式更新占空比（带 ramp）并下发命令
3) 保护：过流/过温触发 BKIN/关闭 PWM/报警。
4) 同步：TIM1 TRGO 或外部 GPIO 做多模块同步。

---

## 8. 示例与计算

- 假设 VDDA=5.0 V；Vin 分压 950k/50k → 100 V → 5.0 V，60 V → 3.0 V；OPA 跟随输出到 ADC。
- CMP 阈值：LDAC≈3.0 V 对应 Vin=60 V 分压水平；比较 OPA 输出与 LDAC 判断。
- 分压节点建议 RC 滤波 + TVS/肖特基防护。

---

## 9. Checklist（设计/验证要点）

- [ ] 确定封装与引脚复用；
- [ ] 确定 VDDA（3.3V/5.0V）并调整分压与阈值；
- [ ] 高压分压点的浪涌保护与滤波；
- [ ] 高阻分压后务必用 OPA 缓冲（或降低等效阻抗）；
- [ ] 依据手册采样时间表满足 RAIN 限制；
- [ ] 设计 PWM 死区、软启动、验证 BKIN；
- [ ] CMP 中断去抖与 ADC 冗余确认；
- [ ] 定义从模块通信协议。

---

## 10. 与代码框架的映射

- 控制循环与模式管理：`dcdc_control.[ch]`
  - 模式：IDLE/BUCK/BOOST/FAULT
  - 双环 PI：外环电压（给出电流参考），内环电流（给出占空比）
  - BOOST 比较器 + BKIN：消抖/保持时间（`BOOST_CMP_DEBOUNCE_TICKS` / `BOOST_CMP_HOLD_TICKS`）
  - 板级 Hook（需用户实现或重写弱函数）：
    - `fun_DCDC_ApplyBuckDuty(float duty)`
    - `fun_DCDC_ApplyBoostDuty(float duty)`
    - `fun_DCDC_BoostCmpInit(float vthr_V)` / `fun_DCDC_BoostCmpTrip(void)`
    - `fun_DCDC_ApplyBrake(int enable)`
- 采样与换算：`adc_monitor.[ch]`
  - 多通道：`fun_ADC_GetVin/Vout/Iin/Iout`
  - 校准宏：`ADC_VREF`, `ADC_RES`, `VOLT_DIV_K`（默认 200k/5.1k），`CURR_SENSE_V_PER_A`
  - 板级 Hook（弱函数，可重写）：`fun_ADC_Read*Raw`
- 故障检测：`fault_detect.[ch]`（OV/UV/OC 阈值宏，可扩展）
- 通信：`comm.[ch]`（命令：MODE/SET/GET/FAULT/CLRFAULT/SETCMP）
- 主循环：`main.c`（顺序：ADC→Fault→Mode→Control→Comm）

---

## 11. 快速上手与调参

- 先实现 BSP Hook：PWM、比较器、BKIN、ADC 原始采样；
- 设置目标：
  - BUCK 充电：`MODE BUCK`，`SET 6.4 2.0`
  - BOOST 放电：`MODE BOOST`，`SET 52.0 5.0`
  - 设置 BOOST 比较器阈值：`SETCMP 55.0`（单位 V，注意分压关系）
- 监控：`GET` 打印 `MODE Vt It Vm Im Db Ds PI F`（Buck/Boost duty、PI 积分、电压/电流测量、故障码）
- 调参：
  - 外环：`DCDC_V_KP`, `DCDC_V_KI`；内环：`DCDC_I_KP`, `DCDC_I_KI`；周期：`DCDC_DT`
  - 比较器去抖/保持：`BOOST_CMP_DEBOUNCE_TICKS`, `BOOST_CMP_HOLD_TICKS`

> VS Code 的 IntelliSense includePath 警告不影响 Keil/MDK 编译，可后续单独配置。

---

## 12. 参考

- PTM280x 数据手册：引脚复用、TIM1/TIM8、ADC/OPA/CMP/LDAC、BKIN 相关章节
- Template 代码中的注释与 README（本文件）
