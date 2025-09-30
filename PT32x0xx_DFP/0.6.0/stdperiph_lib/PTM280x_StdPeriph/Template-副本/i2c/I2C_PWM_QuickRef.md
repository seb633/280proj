# I2C PWM控制协议 - 快速参考

## 基本配置
- **从机地址**: 0x50-0x53 (通过PB6/PB7配置)
- **引脚**: SDA=PB4, SCL=PB5
- **数据格式**: 大端序 (高字节在前)

## 命令列表

| 命令 | 功能 | 发送格式 | 响应 |
|------|------|----------|------|
| 0x01 | 设置占空比 | [0x01][高字节][低字节] | 0xAA/0xFF |
| 0x02 | 设置频率 | [0x02][高字节][低字节] | 0xAA/0xFF |
| 0x03 | 启动PWM | [0x03] | 0xAA/0xFF |
| 0x04 | 停止PWM | [0x04] | 0xAA/0xFF |
| 0x05 | 查询状态 | [0x05] | 6字节状态数据 |

## 状态查询响应格式
```
字节1: 0xAA (成功标志)
字节2: 0x00(停止)/0x01(运行)
字节3-4: 当前占空比 (高字节在前)
字节5-6: 当前频率 (高字节在前)
```

## 典型参数范围
- **占空比**: 80-320 (推荐194)
- **频率**: 240-320 (推荐320)

## Arduino示例
```cpp
#include <Wire.h>

#define PWM_ADDR 0x50

void setup() {
  Wire.begin();
  Serial.begin(9600);
  
  // 设置占空比194
  setPWMDuty(194);
  
  // 设置频率320
  setPWMFreq(320);
  
  // 启动PWM
  startPWM();
}

void setPWMDuty(uint16_t duty) {
  Wire.beginTransmission(PWM_ADDR);
  Wire.write(0x01);           // 命令
  Wire.write(duty >> 8);      // 高字节
  Wire.write(duty & 0xFF);    // 低字节
  Wire.endTransmission();
  
  // 读取响应
  Wire.requestFrom(PWM_ADDR, 1);
  if (Wire.available()) {
    uint8_t response = Wire.read();
    Serial.println(response == 0xAA ? "占空比设置成功" : "设置失败");
  }
}

void setPWMFreq(uint16_t freq) {
  Wire.beginTransmission(PWM_ADDR);
  Wire.write(0x02);           // 命令
  Wire.write(freq >> 8);      // 高字节  
  Wire.write(freq & 0xFF);    // 低字节
  Wire.endTransmission();
  
  // 读取响应
  Wire.requestFrom(PWM_ADDR, 1);
  if (Wire.available()) {
    uint8_t response = Wire.read();
    Serial.println(response == 0xAA ? "频率设置成功" : "设置失败");
  }
}

void startPWM() {
  Wire.beginTransmission(PWM_ADDR);
  Wire.write(0x03);           // 启动命令
  Wire.endTransmission();
  
  // 读取响应
  Wire.requestFrom(PWM_ADDR, 1);
  if (Wire.available()) {
    uint8_t response = Wire.read();
    Serial.println(response == 0xAA ? "PWM启动成功" : "启动失败");
  }
}

void stopPWM() {
  Wire.beginTransmission(PWM_ADDR);
  Wire.write(0x04);           // 停止命令
  Wire.endTransmission();
  
  // 读取响应
  Wire.requestFrom(PWM_ADDR, 1);
  if (Wire.available()) {
    uint8_t response = Wire.read();
    Serial.println(response == 0xAA ? "PWM停止成功" : "停止失败");
  }
}

void getPWMStatus() {
  Wire.beginTransmission(PWM_ADDR);
  Wire.write(0x05);           // 状态查询命令
  Wire.endTransmission();
  
  // 读取6字节状态
  Wire.requestFrom(PWM_ADDR, 6);
  if (Wire.available() >= 6) {
    uint8_t ok = Wire.read();
    uint8_t running = Wire.read();
    uint16_t duty = (Wire.read() << 8) | Wire.read();
    uint16_t freq = (Wire.read() << 8) | Wire.read();
    
    if (ok == 0xAA) {
      Serial.print("PWM状态: ");
      Serial.println(running ? "运行中" : "已停止");
      Serial.print("占空比: ");
      Serial.println(duty);
      Serial.print("频率: ");
      Serial.println(freq);
    }
  }
}

void loop() {
  // 每5秒查询一次状态
  getPWMStatus();
  delay(5000);
}
```

## 树莓派Python示例
```python
import smbus
import time

bus = smbus.SMBus(1)  # I2C总线1
PWM_ADDR = 0x50

def set_duty(duty):
    bus.write_i2c_block_data(PWM_ADDR, 0x01, [duty >> 8, duty & 0xFF])
    return bus.read_byte(PWM_ADDR) == 0xAA

def set_freq(freq):
    bus.write_i2c_block_data(PWM_ADDR, 0x02, [freq >> 8, freq & 0xFF])
    return bus.read_byte(PWM_ADDR) == 0xAA

def start_pwm():
    bus.write_byte(PWM_ADDR, 0x03)
    return bus.read_byte(PWM_ADDR) == 0xAA

def stop_pwm():
    bus.write_byte(PWM_ADDR, 0x04)
    return bus.read_byte(PWM_ADDR) == 0xAA

def get_status():
    bus.write_byte(PWM_ADDR, 0x05)
    data = bus.read_i2c_block_data(PWM_ADDR, 0, 6)
    if data[0] == 0xAA:
        return {
            'running': data[1] == 1,
            'duty': (data[2] << 8) | data[3],
            'freq': (data[4] << 8) | data[5]
        }
    return None

# 使用示例
if set_duty(194) and set_freq(320) and start_pwm():
    print("PWM启动成功")
    status = get_status()
    if status:
        print(f"状态: {status}")
```