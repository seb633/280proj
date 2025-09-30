# I2C PWM控制协议文档

## 基本信息

### 硬件配置
- **芯片型号**: AD16H02 (B02)
- **I2C从机地址**: 可配置 (0x50-0x53)
- **引脚定义**:
  - SDA: PB4 (数据线)
  - SCL: PB5 (时钟线)
  - 地址配置: PB6/PB7 (通过上下拉电阻配置)

### 地址配置表
| PB7 | PB6 | I2C地址 |
|-----|-----|---------|
| 0   | 0   | 0x50    |
| 1   | 0   | 0x51    |
| 0   | 1   | 0x52    |
| 1   | 1   | 0x53    |

### 通信参数
- **I2C速度**: 标准模式 (100kHz) 或快速模式 (400kHz)
- **数据格式**: 8位数据，无奇偶校验
- **字节序**: 大端序 (高字节在前)
- **最大数据长度**: 8字节

---

## 命令协议

### 1. 设置PWM占空比 (0x01)

**主机发送:**
```
起始位 + 从机地址(W) + ACK + 0x01 + ACK + 占空比高字节 + ACK + 占空比低字节 + ACK + 停止位
```

**数据格式:**
- 字节1: 0x01 (命令码)
- 字节2: 占空比值高字节
- 字节3: 占空比值低字节

**占空比范围:** 0-65535 (推荐范围: 80-320)

**从机响应:**
- 成功: 0xAA
- 失败: 0xFF

**示例:**
```c
// 设置占空比为194 (0x00C2)
uint8_t cmd[] = {0x01, 0x00, 0xC2};
i2c_write(0x50, cmd, 3);
uint8_t response = i2c_read_byte(0x50);
```

---

### 2. 设置PWM频率 (0x02)

**主机发送:**
```
起始位 + 从机地址(W) + ACK + 0x02 + ACK + 频率高字节 + ACK + 频率低字节 + ACK + 停止位
```

**数据格式:**
- 字节1: 0x02 (命令码)
- 字节2: 频率值高字节
- 字节3: 频率值低字节

**频率范围:** 0-65535 (推荐范围: 240-320)

**从机响应:**
- 成功: 0xAA
- 失败: 0xFF

**示例:**
```c
// 设置频率为320 (0x0140)
uint8_t cmd[] = {0x02, 0x01, 0x40};
i2c_write(0x50, cmd, 3);
uint8_t response = i2c_read_byte(0x50);
```

---

### 3. 启动PWM (0x03)

**主机发送:**
```
起始位 + 从机地址(W) + ACK + 0x03 + ACK + 停止位
```

**数据格式:**
- 字节1: 0x03 (命令码)

**从机响应:**
- 成功: 0xAA
- 失败: 0xFF

**示例:**
```c
// 启动PWM
uint8_t cmd[] = {0x03};
i2c_write(0x50, cmd, 1);
uint8_t response = i2c_read_byte(0x50);
```

---

### 4. 停止PWM (0x04)

**主机发送:**
```
起始位 + 从机地址(W) + ACK + 0x04 + ACK + 停止位
```

**数据格式:**
- 字节1: 0x04 (命令码)

**从机响应:**
- 成功: 0xAA
- 失败: 0xFF

**示例:**
```c
// 停止PWM
uint8_t cmd[] = {0x04};
i2c_write(0x50, cmd, 1);
uint8_t response = i2c_read_byte(0x50);
```

---

### 5. 查询PWM状态 (0x05)

**主机发送:**
```
起始位 + 从机地址(W) + ACK + 0x05 + ACK + 停止位
```

**主机读取:**
```
起始位 + 从机地址(R) + ACK + [6字节数据] + 停止位
```

**数据格式:**
- 字节1: 0xAA (响应成功标志)
- 字节2: PWM状态 (0x00=停止, 0x01=运行)
- 字节3: 当前占空比高字节
- 字节4: 当前占空比低字节
- 字节5: 当前频率高字节
- 字节6: 当前频率低字节

**示例:**
```c
// 查询PWM状态
uint8_t cmd[] = {0x05};
i2c_write(0x50, cmd, 1);
uint8_t status[6];
i2c_read(0x50, status, 6);

if (status[0] == 0xAA) {
    uint8_t pwm_running = status[1];
    uint16_t pwm_duty = (status[2] << 8) | status[3];
    uint16_t pwm_freq = (status[4] << 8) | status[5];
    printf("PWM状态: %s\n", pwm_running ? "运行" : "停止");
    printf("占空比: %d\n", pwm_duty);
    printf("频率: %d\n", pwm_freq);
}
```

---

## 错误处理

### 错误码定义
- **0xAA**: 命令执行成功
- **0xFF**: 命令执行失败

### 常见错误原因
1. **地址不匹配**: 检查I2C从机地址配置
2. **数据长度错误**: 确保发送正确的数据字节数
3. **命令码无效**: 使用规定的命令码 (0x01-0x05)
4. **参数超出范围**: 占空比和频率值应在合理范围内

---

## 使用示例

### 完整的PWM控制流程 (C语言)

```c
#include <stdio.h>
#include <stdint.h>

#define I2C_SLAVE_ADDR  0x50
#define PWM_CMD_SET_DUTY    0x01
#define PWM_CMD_SET_FREQ    0x02
#define PWM_CMD_START       0x03
#define PWM_CMD_STOP        0x04
#define PWM_CMD_STATUS      0x05
#define PWM_RESPONSE_OK     0xAA
#define PWM_RESPONSE_ERROR  0xFF

// 设置PWM参数并启动
int pwm_setup_and_start(uint16_t duty, uint16_t freq)
{
    uint8_t cmd[3];
    uint8_t response;
    
    // 1. 设置占空比
    cmd[0] = PWM_CMD_SET_DUTY;
    cmd[1] = (duty >> 8) & 0xFF;  // 高字节
    cmd[2] = duty & 0xFF;         // 低字节
    
    if (i2c_write(I2C_SLAVE_ADDR, cmd, 3) != 0) {
        printf("设置占空比失败\n");
        return -1;
    }
    
    response = i2c_read_byte(I2C_SLAVE_ADDR);
    if (response != PWM_RESPONSE_OK) {
        printf("占空比设置响应错误: 0x%02X\n", response);
        return -1;
    }
    
    // 2. 设置频率
    cmd[0] = PWM_CMD_SET_FREQ;
    cmd[1] = (freq >> 8) & 0xFF;  // 高字节
    cmd[2] = freq & 0xFF;         // 低字节
    
    if (i2c_write(I2C_SLAVE_ADDR, cmd, 3) != 0) {
        printf("设置频率失败\n");
        return -1;
    }
    
    response = i2c_read_byte(I2C_SLAVE_ADDR);
    if (response != PWM_RESPONSE_OK) {
        printf("频率设置响应错误: 0x%02X\n", response);
        return -1;
    }
    
    // 3. 启动PWM
    cmd[0] = PWM_CMD_START;
    
    if (i2c_write(I2C_SLAVE_ADDR, cmd, 1) != 0) {
        printf("启动PWM失败\n");
        return -1;
    }
    
    response = i2c_read_byte(I2C_SLAVE_ADDR);
    if (response != PWM_RESPONSE_OK) {
        printf("PWM启动响应错误: 0x%02X\n", response);
        return -1;
    }
    
    printf("PWM启动成功 - 占空比: %d, 频率: %d\n", duty, freq);
    return 0;
}

// 查询PWM状态
int pwm_get_status(void)
{
    uint8_t cmd[1];
    uint8_t status[6];
    
    // 发送状态查询命令
    cmd[0] = PWM_CMD_STATUS;
    
    if (i2c_write(I2C_SLAVE_ADDR, cmd, 1) != 0) {
        printf("发送状态查询命令失败\n");
        return -1;
    }
    
    // 读取状态数据
    if (i2c_read(I2C_SLAVE_ADDR, status, 6) != 0) {
        printf("读取状态数据失败\n");
        return -1;
    }
    
    if (status[0] != PWM_RESPONSE_OK) {
        printf("状态查询响应错误: 0x%02X\n", status[0]);
        return -1;
    }
    
    uint8_t pwm_running = status[1];
    uint16_t pwm_duty = (status[2] << 8) | status[3];
    uint16_t pwm_freq = (status[4] << 8) | status[5];
    
    printf("PWM状态: %s\n", pwm_running ? "运行中" : "已停止");
    printf("当前占空比: %d\n", pwm_duty);
    printf("当前频率: %d\n", pwm_freq);
    
    return 0;
}

// 停止PWM
int pwm_stop(void)
{
    uint8_t cmd[1];
    uint8_t response;
    
    cmd[0] = PWM_CMD_STOP;
    
    if (i2c_write(I2C_SLAVE_ADDR, cmd, 1) != 0) {
        printf("发送停止命令失败\n");
        return -1;
    }
    
    response = i2c_read_byte(I2C_SLAVE_ADDR);
    if (response != PWM_RESPONSE_OK) {
        printf("PWM停止响应错误: 0x%02X\n", response);
        return -1;
    }
    
    printf("PWM已停止\n");
    return 0;
}

// 主函数示例
int main(void)
{
    // 初始化I2C
    i2c_init();
    
    // 设置PWM参数并启动 (占空比194, 频率320)
    if (pwm_setup_and_start(194, 320) == 0) {
        
        // 查询状态
        pwm_get_status();
        
        // 运行一段时间后停止
        delay_ms(5000);
        pwm_stop();
    }
    
    return 0;
}
```

### Python示例

```python
import smbus
import time

class PWMController:
    def __init__(self, bus_num=1, slave_addr=0x50):
        self.bus = smbus.SMBus(bus_num)
        self.slave_addr = slave_addr
        
        # 命令定义
        self.PWM_CMD_SET_DUTY = 0x01
        self.PWM_CMD_SET_FREQ = 0x02
        self.PWM_CMD_START = 0x03
        self.PWM_CMD_STOP = 0x04
        self.PWM_CMD_STATUS = 0x05
        self.PWM_RESPONSE_OK = 0xAA
        self.PWM_RESPONSE_ERROR = 0xFF
    
    def set_duty(self, duty):
        """设置PWM占空比"""
        cmd = [self.PWM_CMD_SET_DUTY, (duty >> 8) & 0xFF, duty & 0xFF]
        self.bus.write_i2c_block_data(self.slave_addr, cmd[0], cmd[1:])
        response = self.bus.read_byte(self.slave_addr)
        return response == self.PWM_RESPONSE_OK
    
    def set_frequency(self, freq):
        """设置PWM频率"""
        cmd = [self.PWM_CMD_SET_FREQ, (freq >> 8) & 0xFF, freq & 0xFF]
        self.bus.write_i2c_block_data(self.slave_addr, cmd[0], cmd[1:])
        response = self.bus.read_byte(self.slave_addr)
        return response == self.PWM_RESPONSE_OK
    
    def start(self):
        """启动PWM"""
        self.bus.write_byte(self.slave_addr, self.PWM_CMD_START)
        response = self.bus.read_byte(self.slave_addr)
        return response == self.PWM_RESPONSE_OK
    
    def stop(self):
        """停止PWM"""
        self.bus.write_byte(self.slave_addr, self.PWM_CMD_STOP)
        response = self.bus.read_byte(self.slave_addr)
        return response == self.PWM_RESPONSE_OK
    
    def get_status(self):
        """获取PWM状态"""
        self.bus.write_byte(self.slave_addr, self.PWM_CMD_STATUS)
        data = self.bus.read_i2c_block_data(self.slave_addr, 0, 6)
        
        if data[0] == self.PWM_RESPONSE_OK:
            status = {
                'running': data[1] == 1,
                'duty': (data[2] << 8) | data[3],
                'frequency': (data[4] << 8) | data[5]
            }
            return status
        return None

# 使用示例
if __name__ == "__main__":
    pwm = PWMController(bus_num=1, slave_addr=0x50)
    
    # 设置参数
    if pwm.set_duty(194):
        print("占空比设置成功")
    
    if pwm.set_frequency(320):
        print("频率设置成功")
    
    # 启动PWM
    if pwm.start():
        print("PWM启动成功")
        
        # 查询状态
        status = pwm.get_status()
        if status:
            print(f"PWM状态: {'运行中' if status['running'] else '已停止'}")
            print(f"占空比: {status['duty']}")
            print(f"频率: {status['frequency']}")
        
        # 等待5秒后停止
        time.sleep(5)
        
        if pwm.stop():
            print("PWM停止成功")
```

---

## 注意事项

1. **时序要求**: 确保I2C时序符合标准，特别是建立时间和保持时间
2. **地址配置**: 上电前确保PB6/PB7的上下拉电阻配置正确
3. **参数范围**: 占空比和频率参数应在合理范围内，避免硬件损坏
4. **错误重试**: 建议在通信失败时实现重试机制
5. **状态检查**: 在关键操作后及时检查响应状态
6. **电源稳定**: 确保供电稳定，避免通信中断

---

## 更新日志

- **V1.0** (2025-09-28): 初始版本，支持基本PWM控制功能
  - 占空比设置
  - 频率设置  
  - 启动/停止控制
  - 状态查询