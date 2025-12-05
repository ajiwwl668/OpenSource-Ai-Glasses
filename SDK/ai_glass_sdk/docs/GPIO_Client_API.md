# GPIO事件广播服务 - 客户端接入指南

> **版本**: v1.1 | **日期**: 2025-10-10 | **状态**: ✅ 生产就绪

---

## 📖 目录

- [快速开始](#快速开始)
- [接入指南](#二外部接入指南)
- [配置参数](#三配置参数)
- [故障排查](#四故障排查)
- [完整示例](#五完整示例)

---

## 🚀 快速开始

### 功能简介

监听GPIO按键事件，支持多进程异步回调。提供按键按下、短按、长按、释放等事件通知。

### 编译和运行

#### 1. 编译并运行客户端示例
```bash
# 编译客户端示例
cd ai_glass_sdk/examples/gpio_client
make

# 运行示例
./gpio_event_client_example
```

### 最小客户端代码

```c
#include "ai_gpio.h"

void my_callback(gpio_event_t event, int gpio, void *data) {
    if (event == GPIO_EVENT_PRESS) {
        printf("按键按下\n");
    }
}

int main() {
    gpio_event_client_t client = {0};

    // 创建并连接
    ai_gpio_event_client_create(&client);
    ai_gpio_event_client_connect(&client);

    // 订阅事件
    ai_gpio_event_client_subscribe(&client, my_callback, NULL);

    // 等待事件...
    while (1) sleep(1);

    // 清理
    ai_gpio_event_client_unsubscribe(&client);
    ai_gpio_event_client_destroy(&client);

    return 0;
}
```

### 主要特性

- ✅ 支持多个进程同时监听同一个GPIO
- ✅ 低延迟事件通知（< 13ms）
- ✅ 支持最多64个并发客户端
- ✅ 事件历史缓存（32个事件）
- ✅ 异步回调机制
- ✅ 服务端停止时客户端可正常退出

### 性能指标

| 指标 | 值 |
|------|-----|
| 事件延迟 | < 13ms |
| 内存占用 | 4KB (共享内存) |
| CPU占用 | < 1% |
| 客户端容量 | 64个 |

---

## 二、外部接入指南

### 2.1 编译客户端程序

#### 链接SDK库

```bash
# 先编译SDK库
cd ai_glass_sdk
make

# 编译自己的程序
arm-rockchip831-linux-uclibcgnueabihf-gcc \
    -o my_gpio_client \
    my_gpio_client.c \
    -I/path/to/ai_glass_sdk/include \
    -L/path/to/ai_glass_sdk/lib \
    -lai_glass_sdk \
    -lpthread -lrt
```

### 2.2 客户端API

#### 基本流程

```c
#include "ai_gpio.h"

// 1. 创建客户端
gpio_event_client_t client = {0};
ai_gpio_event_client_create(&client);

// 2. 连接到服务
ai_gpio_event_client_connect(&client);

// 3. 订阅事件
ai_gpio_event_client_subscribe(&client, my_callback, NULL);

// 4. 程序继续运行,事件会通过回调异步通知
while (running) {
    sleep(1);
}

// 5. 清理资源
ai_gpio_event_client_unsubscribe(&client);
ai_gpio_event_client_destroy(&client);
```

#### 事件回调函数

```c
void my_callback(gpio_event_t event, int gpio_num, void *user_data) {
    switch (event) {
        case GPIO_EVENT_PRESS:
            printf("GPIO%d 按键按下\n", gpio_num);
            // 处理按下事件
            break;

        case GPIO_EVENT_RELEASE:
            printf("GPIO%d 按键释放\n", gpio_num);
            // 处理释放事件
            break;

        case GPIO_EVENT_ERROR:
            printf("GPIO%d 错误\n", gpio_num);
            break;
    }
}
```

#### 核心API说明

| API | 说明 |
|-----|------|
| `ai_gpio_event_client_create()` | 创建客户端实例 |
| `ai_gpio_event_client_connect()` | 连接到GPIO事件服务 |
| `ai_gpio_event_client_subscribe()` | 订阅GPIO事件(异步回调) |
| `ai_gpio_event_client_unsubscribe()` | 取消订阅 |
| `ai_gpio_event_client_disconnect()` | 断开连接 |
| `ai_gpio_event_client_destroy()` | 销毁客户端,释放资源 |
| `ai_gpio_event_client_is_service_alive()` | 检查服务是否可用 |

---

## 三、配置参数

### 事件类型

| 事件 | 说明 |
|------|------|
| `GPIO_EVENT_PRESS` | 按键按下 |
| `GPIO_EVENT_CLICK` | 短按（≤2秒） |
| `GPIO_EVENT_LONG_CLICK` | 长按（>2秒） |
| `GPIO_EVENT_RELEASE` | 按键释放 |
| `GPIO_EVENT_ERROR` | 错误 |

### 性能指标

- 事件延迟：< 13ms
- 支持最多 64 个并发客户端
- 事件队列：32 个事件缓存

---

## 四、故障排查

### 连接失败

**问题**: `ai_gpio_event_client_connect()` 返回 -1

**原因**:
- 服务未启动或异常
- GPIO模块未初始化

**解决方案**:
- 使用 `ai_gpio_event_client_is_service_alive()` 检查服务状态
- 确认服务端正常运行

### 收不到事件

**原因**:
- 回调函数未正确设置
- GPIO硬件未触发
- 订阅失败

**解决方案**:
- 检查 `ai_gpio_event_client_subscribe()` 的返回值
- 验证回调函数是否被正确注册
- 手动触发GPIO测试

### 多个客户端

✅ 完全支持多个客户端同时监听同一个GPIO

---

## 五、完整示例

参考文件: `ai_glass_sdk/examples/gpio_client/gpio_event_client_example.c`

```c
#include "ai_gpio.h"
#include <stdio.h>
#include <signal.h>

static volatile int g_running = 1;
static int g_press_count = 0;
static int g_release_count = 0;

void signal_handler(int sig) {
    g_running = 0;
}

void my_gpio_event_callback(gpio_event_t event, int gpio_num, void *user_data) {
    switch (event) {
        case GPIO_EVENT_PRESS:
            g_press_count++;
            printf("═══════════════════════════════════════════\n");
            printf("  🔴 GPIO%d 按键按下事件\n", gpio_num);
            printf("  按下次数: %d\n", g_press_count);
            printf("═══════════════════════════════════════════\n\n");
            break;

        case GPIO_EVENT_RELEASE:
            g_release_count++;
            printf("═══════════════════════════════════════════\n");
            printf("  ⚪ GPIO%d 按键释放事件\n", gpio_num);
            printf("  释放次数: %d\n", g_release_count);
            printf("═══════════════════════════════════════════\n\n");
            break;

        case GPIO_EVENT_ERROR:
            printf("❌ GPIO%d 错误\n", gpio_num);
            break;
    }
}

int main() {
    gpio_event_client_t client = {0};

    // 注册信号处理
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    printf("═══════════════════════════════════════════\n");
    printf("  GPIO事件客户端示例\n");
    printf("═══════════════════════════════════════════\n\n");

    // 1. 创建客户端
    printf("📝 [步骤1/3] 创建GPIO事件客户端...\n");
    if (ai_gpio_event_client_create(&client) != 0) {
        printf("❌ 创建客户端失败\n");
        return -1;
    }
    printf("✅ 客户端已创建\n\n");

    // 2. 连接到服务
    printf("📝 [步骤2/3] 连接到GPIO事件广播服务...\n");
    if (ai_gpio_event_client_connect(&client) != 0) {
        printf("❌ 连接服务失败,请检查服务状态\n");
        ai_gpio_event_client_destroy(&client);
        return -1;
    }
    printf("✅ 已连接到服务\n\n");

    // 3. 订阅事件
    printf("📝 [步骤3/3] 订阅GPIO事件...\n");
    if (ai_gpio_event_client_subscribe(&client, my_gpio_event_callback, NULL) != 0) {
        printf("❌ 订阅事件失败\n");
        ai_gpio_event_client_disconnect(&client);
        ai_gpio_event_client_destroy(&client);
        return -1;
    }

    printf("\n═══════════════════════════════════════════\n");
    printf("  🎧 监听中... 请按下GPIO按键\n");
    printf("  💡 提示：按 Ctrl+C 退出程序\n");
    printf("═══════════════════════════════════════════\n\n");

    // 主循环
    while (g_running) {
        sleep(1);
    }

    // 清理
    printf("\n📝 清理资源...\n");
    ai_gpio_event_client_unsubscribe(&client);
    ai_gpio_event_client_disconnect(&client);
    ai_gpio_event_client_destroy(&client);

    printf("\n═══════════════════════════════════════════\n");
    printf("  统计信息:\n");
    printf("  按下次数: %d\n", g_press_count);
    printf("  释放次数: %d\n", g_release_count);
    printf("═══════════════════════════════════════════\n");
    printf("✅ 程序正常退出\n");

    return 0;
}
```

---

## 相关文件

- **头文件**: `ai_glass_sdk/include/ai_gpio.h`
- **示例程序**: `ai_glass_sdk/examples/gpio_client/gpio_event_client_example.c`
- **SDK README**: `ai_glass_sdk/README.md`

---

**版本**: v1.1
**日期**: 2025-10-10
**作者**: AI Media Service Team
**状态**: ✅ 生产就绪
