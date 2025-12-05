# Audio Client API 开发指南

> **版本**: v1.0 | **日期**: 2025-10-10 | **状态**: ✅ 生产就绪

---

## 📖 目录

- [快速开始](#快速开始)
- [API参考](#api参考)
- [完整示例](#完整示例)
- [故障排查](#故障排查)
- [命令行工具](#命令行工具)

---

## 🚀 快速开始

### 功能简介

控制音频播放，支持PCM文件播放和TTS文本转语音。

### 编译客户端程序

#### 链接SDK库

```bash
# 先编译SDK库
cd ai_glass_sdk
make

# 编译自己的程序
arm-rockchip831-linux-uclibcgnueabihf-gcc \
    -o my_audio_app \
    my_audio_app.c \
    -I/path/to/ai_glass_sdk/include \
    -L/path/to/ai_glass_sdk/lib \
    -lai_glass_sdk
```

### 运行示例程序

```bash
# 编译SDK示例程序
cd ai_glass_sdk/examples/audio_play_client
make

# 推送并运行SDK示例程序
adb push ./audio_play_client /path/you/like
./audio_play_client -f /path/to/audio.pcm -v 80
```

### 最小客户端代码

```c
#include "ai_audio.h"
#include <stdio.h>

int main() {
    // 1. 初始化客户端
    ai_audio_t *client = ai_audio_init(NULL);  // 使用默认路径
    if (!client) {
        printf("初始化失败\n");
        return -1;
    }

    // 2. 播放音频（使用默认参数）
    int result = ai_audio_play_simple(client, "/oem/usr/bin/resources/beep.pcm");
    if (result != AI_AUDIO_SUCCESS) {
        printf("播放失败: %s\n", ai_audio_get_error_string(result));
    }

    // 3. 清理资源
    ai_audio_cleanup(client);
    return 0;
}
```

### 高级播放示例

```c
#include "ai_audio.h"

int main() {
    ai_audio_t *client = ai_audio_init(NULL);

    // 创建播放参数
    ai_audio_params_t params = {
        .file_path = "/path/to/audio.pcm",
        .volume = 80,              // 音量80%
        .force = 1,                // 强制播放（打断当前）
        .sample_rate = 48000,      // 48kHz采样率
        .channels = 2,             // 双声道
        .bit_width = 16            // 16位
    };

    // 播放
    ai_audio_play(client, &params);

    // 清理
    ai_audio_cleanup(client);
    return 0;
}
```

### 主要特性

- ✅ 简单易用的C语言API
- ✅ 支持PCM文件播放和TTS文本转语音
- ✅ 多种音频参数配置（采样率、声道、位宽）
- ✅ 强制播放模式（打断当前播放）
- ✅ 排队播放模式
- ✅ 音量控制 (0-100)
- ✅ 停止当前播放
- ✅ MD5智能缓存（TTS功能）

---

## 📋 API参考

### 数据结构

#### ai_audio_t

音频客户端句柄（不透明类型），通过 `ai_audio_init()` 创建。

#### ai_audio_params_t

音频播放参数结构：

```c
typedef struct {
    const char *file_path;    // PCM文件路径（必填，播放PCM时使用）
    int volume;               // 音量 (0-100)，-1表示使用默认值
    int force;                // 强制播放标志 (0=排队, 1=打断当前播放)
    int sample_rate;          // 采样率 (8000-96000)，-1表示使用默认值
    int channels;             // 声道数 (1-8)，-1表示使用默认值
    int bit_width;            // 位宽 (8/16/24/32)，-1表示使用默认值
} ai_audio_params_t;
```

#### ai_audio_tts_params_t

TTS文本转语音参数结构：

```c
typedef struct {
    const char *text;         // 要转换的文本（必填）
    int volume;               // 音量 (0-100)，-1表示使用默认值
    int force;                // 强制播放标志 (0=排队, 1=打断当前播放)
    int use_cache;            // 是否使用缓存 (0=不使用, 1=使用，推荐)
} ai_audio_tts_params_t;
```

**字段说明**：
- **file_path**: PCM文件的完整路径（仅PCM播放时使用）
- **text**: 要转换的文本内容（仅TTS播放时使用）
- **volume**: 音量百分比，0=静音，100=最大
- **force**: 0=排队播放，1=立即打断当前播放
- **use_cache**: TTS缓存开关，推荐启用以提高响应速度
- **sample_rate**: 音频采样率（Hz）
- **channels**: 声道数量
- **bit_width**: 每个采样点的位数

#### 错误码

```c
#define AI_AUDIO_SUCCESS           0    // 成功
#define AI_AUDIO_ERROR_INIT       -1    // 初始化失败
#define AI_AUDIO_ERROR_CONNECT    -2    // 连接失败
#define AI_AUDIO_ERROR_SEND       -3    // 发送失败
#define AI_AUDIO_ERROR_PARAM      -4    // 参数错误
#define AI_AUDIO_ERROR_RESPONSE   -5    // 服务端响应错误
```

### 核心API

#### ai_audio_init()

初始化音频客户端。

```c
ai_audio_t* ai_audio_init(const char *socket_path);
```

**参数**：
- `socket_path` - 服务端连接路径，传NULL使用默认路径

**返回值**：
- 成功：客户端句柄指针
- 失败：NULL

**说明**：
- 创建客户端实例，不建立实际连接
- 连接在每次发送命令时动态建立

---

#### ai_audio_play()

播放音频文件。

```c
int ai_audio_play(ai_audio_t *client, const ai_audio_params_t *params);
```

**参数**：
- `client` - 客户端句柄
- `params` - 播放参数

**返回值**：
- `AI_AUDIO_SUCCESS` (0) - 成功
- `AI_AUDIO_ERROR_CONNECT` (-2) - 连接服务端失败
- `AI_AUDIO_ERROR_SEND` (-3) - 发送命令失败
- `AI_AUDIO_ERROR_PARAM` (-4) - 参数错误
- `AI_AUDIO_ERROR_RESPONSE` (-5) - 服务端返回错误

**说明**：
- `force=0`: 将音频添加到播放队列末尾
- `force=1`: 立即停止当前播放，播放新音频
- 未指定的参数（值为-1）将使用服务端默认值

---

#### ai_audio_stop()

停止当前播放。

```c
int ai_audio_stop(ai_audio_t *client);
```

**参数**：
- `client` - 客户端句柄

**返回值**：
- `AI_AUDIO_SUCCESS` (0) - 成功
- 负数 - 错误码

**说明**：
- 立即停止当前正在播放的音频
- 清空播放队列

---

#### ai_audio_cleanup()

清理客户端资源。

```c
void ai_audio_cleanup(ai_audio_t *client);
```

**参数**：
- `client` - 客户端句柄

**说明**：
- 释放客户端内存
- 不影响服务端的播放状态

---

#### ai_audio_get_error_string()

获取错误码对应的错误信息。

```c
const char* ai_audio_get_error_string(int error_code);
```

**参数**：
- `error_code` - 错误码

**返回值**：
- 错误信息字符串

---

### 辅助API

#### ai_audio_create_default_params()

创建默认播放参数。

```c
ai_audio_params_t ai_audio_create_default_params(const char *file_path);
```

**参数**：
- `file_path` - PCM文件路径

**返回值**：
- 参数结构体，所有可选参数设置为-1

**示例**：
```c
ai_audio_params_t params = ai_audio_create_default_params("/path/to/audio.pcm");
params.volume = 80;  // 只设置音量
params.force = 1;    // 设置强制播放
ai_audio_play(client, &params);
```

---

#### ai_audio_play_tts()

播放TTS文本转语音。

```c
int ai_audio_play_tts(ai_audio_t *client, const ai_audio_tts_params_t *params);
```

**参数**：
- `client` - 客户端句柄
- `params` - TTS播放参数

**返回值**：
- `AI_AUDIO_SUCCESS` (0) - 成功
- `AI_AUDIO_ERROR_CONNECT` (-2) - 连接服务端失败
- `AI_AUDIO_ERROR_SEND` (-3) - 发送命令失败
- `AI_AUDIO_ERROR_PARAM` (-4) - 参数错误
- `AI_AUDIO_ERROR_RESPONSE` (-5) - 服务端返回错误

**说明**：
- 将文本转换为语音并播放
- 支持智能缓存，相同文本会复用缓存
- `use_cache=0`: 强制重新生成，适用于动态内容

---

#### ai_audio_play_tts_simple()

简化的TTS播放函数。

```c
int ai_audio_play_tts_simple(ai_audio_t *client, const char *text);
```

**参数**：
- `client` - 客户端句柄
- `text` - 要转换的文本

**返回值**：
- 错误码

**说明**：
- 使用默认参数播放TTS
- 等价于使用默认TTS参数调用 `ai_audio_play_tts()`

---

#### ai_audio_play_simple()

简化的PCM播放函数。

```c
int ai_audio_play_simple(ai_audio_t *client, const char *file_path);
```

**参数**：
- `client` - 客户端句柄
- `file_path` - PCM文件路径

**返回值**：
- 错误码

**说明**：
- 使用默认参数播放PCM文件
- 等价于 `ai_audio_play(client, &ai_audio_create_default_params(file_path))`

---

## 💡 完整示例

### 示例1：基本播放

```c
#include "ai_audio.h"
#include <stdio.h>

int main() {
    ai_audio_t *client = ai_audio_init(NULL);
    if (!client) {
        printf("Failed to initialize client\n");
        return -1;
    }

    int result = ai_audio_play_simple(client, "/oem/usr/bin/resources/beep.pcm");
    if (result != AI_AUDIO_SUCCESS) {
        printf("Play failed: %s\n", ai_audio_get_error_string(result));
    } else {
        printf("Playing audio...\n");
    }

    ai_audio_cleanup(client);
    return 0;
}
```

### 示例2：自定义参数播放

```c
#include "ai_audio.h"
#include <stdio.h>

int main() {
    ai_audio_t *client = ai_audio_init(NULL);

    // 配置播放参数
    ai_audio_params_t params = {
        .file_path = "/data/alert.pcm",
        .volume = 90,
        .force = 1,           // 强制播放
        .sample_rate = 48000,
        .channels = 2,
        .bit_width = 16
    };

    printf("Playing audio with custom parameters...\n");
    int result = ai_audio_play(client, &params);
    if (result != AI_AUDIO_SUCCESS) {
        printf("Error: %s\n", ai_audio_get_error_string(result));
    }

    ai_audio_cleanup(client);
    return 0;
}
```

### 示例3：播放多个音频文件

```c
#include "ai_audio.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    ai_audio_t *client = ai_audio_init(NULL);

    const char *files[] = {
        "/data/sound1.pcm",
        "/data/sound2.pcm",
        "/data/sound3.pcm"
    };

    for (int i = 0; i < 3; i++) {
        printf("Playing %s...\n", files[i]);

        ai_audio_params_t params = ai_audio_create_default_params(files[i]);
        params.force = 0;  // 排队播放，不打断

        int result = ai_audio_play(client, &params);
        if (result != AI_AUDIO_SUCCESS) {
            printf("Failed to play %s: %s\n", files[i],
                   ai_audio_get_error_string(result));
        }

        sleep(1);  // 等待1秒再添加下一个
    }

    ai_audio_cleanup(client);
    return 0;
}
```

### 示例4：TTS文本转语音

```c
#include "ai_audio.h"
#include <stdio.h>

int main() {
    ai_audio_t *client = ai_audio_init(NULL);

    // 简单TTS播放
    printf("Playing TTS...\n");
    int result = ai_audio_play_tts_simple(client, "你好，欢迎使用AI语音助手");
    if (result != AI_AUDIO_SUCCESS) {
        printf("TTS failed: %s\n", ai_audio_get_error_string(result));
    }

    // 带参数的TTS播放
    ai_audio_tts_params_t tts_params = {
        .text = "这是一条重要通知",
        .volume = 90,
        .force = 0,      // 排队播放
        .use_cache = 1   // 使用缓存
    };

    ai_audio_play_tts(client, &tts_params);

    // 动态内容（禁用缓存）
    ai_audio_tts_params_t dynamic_params = {
        .text = "当前时间：2025年10月16日",
        .volume = 80,
        .force = 0,
        .use_cache = 0   // 禁用缓存，因为内容是动态的
    };

    ai_audio_play_tts(client, &dynamic_params);

    ai_audio_cleanup(client);
    return 0;
}
```

### 示例5：紧急播放（打断当前）

```c
#include "ai_audio.h"
#include <stdio.h>

void play_alert(ai_audio_t *client) {
    // 使用PCM文件播放警报音
    ai_audio_params_t params = {
        .file_path = "/oem/usr/bin/resources/alert.pcm",
        .volume = 100,        // 最大音量
        .force = 1,           // 立即打断当前播放
        .sample_rate = 16000,
        .channels = 1,
        .bit_width = 16
    };

    ai_audio_play(client, &params);
}

void play_alert_tts(ai_audio_t *client) {
    // 使用TTS播放紧急通知
    ai_audio_tts_params_t tts_params = {
        .text = "警告！检测到异常情况",
        .volume = 100,
        .force = 1,           // 立即打断当前播放
        .use_cache = 1
    };

    ai_audio_play_tts(client, &tts_params);
}

int main() {
    ai_audio_t *client = ai_audio_init(NULL);

    // 正常播放背景音乐
    ai_audio_play_simple(client, "/data/background.pcm");

    // 模拟紧急情况
    sleep(2);
    printf("Alert! Playing emergency sound...\n");
    play_alert(client);  // 会打断背景音乐

    sleep(3);
    printf("Alert TTS! Playing emergency message...\n");
    play_alert_tts(client);  // 使用TTS播放紧急消息

    ai_audio_cleanup(client);
    return 0;
}
```

---

## 🔧 故障排查

### 1. 客户端初始化失败

**错误**：`ai_audio_init()` 返回 NULL

**原因**：
- 内存分配失败

**解决**：
- 检查系统可用内存
- 确认没有内存泄漏

---

### 2. 连接服务端失败

**错误**：API函数返回 `AI_AUDIO_ERROR_CONNECT`

**原因**：
- 服务端未启动
- 连接路径错误

**解决**：
```bash
# 检查服务端是否运行
ps aux | grep ai-core
```

---

### 3. TTS播放失败

**错误**：TTS函数返回非零值

**原因**：
- TTS服务器不可达
- 网络连接问题
- 文本编码问题

**解决**：
- 检查网络连接和TTS服务器配置
- 确保文本使用UTF-8编码
- 详细TTS配置参见 [TTS客户端API](TTS_Client_API.md)

---

### 4. 参数错误

**错误**：函数返回 `AI_AUDIO_ERROR_PARAM`

**原因**：
- 参数值超出范围
- 必填参数为空

**解决**：
- 确认参数在有效范围内：
  - volume: 0-100
  - sample_rate: 8000-96000
  - channels: 1-8
  - bit_width: 8/16/24/32
- 确保必填参数（file_path或text）不为空

---

## ⚙️ 系统要求

### 前提条件

服务端（ai-core）必须已启动。

### 音频文件要求

- **格式**: PCM（Raw audio）
- **编码**: 无压缩
- **采样率**: 8000-96000 Hz
- **声道数**: 1-8
- **位宽**: 8/16/24/32 bit

### 编译依赖

- **交叉编译工具链**: arm-rockchip831-linux-uclibcgnueabihf-gcc
- **系统库**: 标准C库

---

## 📊 性能特性

| 特性 | 说明 |
|------|------|
| **连接模式** | 短连接（每次命令建立新连接） |
| **响应延迟** | < 50ms |
| **并发支持** | 支持多个客户端 |
| **播放模式** | 排队播放 / 强制播放 |
| **音频格式** | PCM（未压缩） |
| **采样率范围** | 8kHz - 96kHz |

---

---

## 🛠️ 命令行工具

除了编程接口，SDK还提供了方便的命令行工具：

### 快速使用

```bash
# PCM文件播放
./audio_play_client -f /path/to/audio.pcm -v 80

# TTS文本播放
./audio_play_client -t "你好世界" -v 90

# 强制播放（打断当前）
./audio_play_client -f /tmp/urgent.pcm -F

# 停止播放
./audio_play_client -S
```

### 详细说明

完整的命令行工具使用说明，请参见：
**📖 [Audio Play Client 使用指南](../examples/audio_play_client/README.md)**

---

## 🔗 相关文档

- **示例程序**: `ai_glass_sdk/examples/audio_play_client/`
- **头文件**: `ai_glass_sdk/include/ai_audio.h`
- **TTS功能**: [TTS客户端API](TTS_Client_API.md)
- **SDK README**: `ai_glass_sdk/README.md`

---

**版本**: v1.0
**日期**: 2025-10-10
**状态**: ✅ 生产就绪
