# 项目目录结构说明

[English](#project-structure-english) | [中文](#项目目录结构说明-中文)

---

## 项目目录结构说明 (中文)

本文档说明项目的目录组织结构和各目录的用途。

### 📁 顶层目录结构

```
OpenSource-Ai-Glasses/
├── .gitignore                 # Git忽略文件配置
├── LICENSE                    # Apache 2.0开源许可证
├── README.md                  # 项目说明（英文）
├── README.zh.md               # 项目说明（中文）
├── CONTRIBUTING.md            # 贡献指南
├── PROJECT_STRUCTURE.md       # 本文档
│
├── docs/                      # 📚 项目文档
├── firmware/                  # 🔧 固件源码
├── examples/                  # 📝 示例程序
├── hardware/                  # ⚙️ 硬件相关文件
├── resources/                 # 📦 资源文件
├── Applications/              # 💾 预编译程序
├── bluez-5.66/               # 📡 BlueZ蓝牙协议栈源码
└── ffmpeg-rockchip/          # 🎬 FFmpeg瑞芯微版本源码
```

---

### 📚 docs/ - 项目文档

存放所有项目文档，包括用户指南、开发文档、API参考等。

```
docs/
├── README.md                          # 文档索引
├── DOCKER_DEPLOYMENT.md              # Docker部署指南（中文）
├── DOCKER_DEPLOYMENT.en.md           # Docker部署指南（英文）
├── APPLICATION_DEVELOPMENT.md         # 应用开发指南（中文）
├── APPLICATION_DEVELOPMENT.en.md      # 应用开发指南（英文）
├── FIRMWARE_FLASHING.md              # 固件烧录指南（中文）
├── FIRMWARE_FLASHING.en.md           # 固件烧录指南（英文）
└── templates/                         # 文档模板
    ├── README-template.md
    ├── firmware-doc-template.md
    └── hardware-spec-template.md
```

**主要文档**：
- **Docker部署指南**: 如何使用Docker搭建开发环境
- **应用开发指南**: 如何开发用户级应用程序
- **固件烧录指南**: 如何编译和烧录固件

---

### 🔧 firmware/ - 固件源码

核心固件程序源代码，需要在瑞芯微RV1106B SDK环境中编译。

```
firmware/
├── display/                   # JBD显示屏主程序
│   ├── main.c                # 主程序入口
│   ├── jbd013_api.c          # JBD显示屏驱动API
│   ├── hal_driver.c          # 硬件抽象层驱动
│   ├── ui/                   # LVGL UI界面
│   ├── lvgl/                 # LVGL图形库
│   └── Makefile              # 编译配置
│
├── launcher/                  # 启动管理器
│   ├── FFmLauncher/          # FFmpeg和摄像头启动管理
│   │   ├── launch.cpp        # 摄像头和视频处理启动器
│   │   └── CMakeLists.txt    # CMake配置
│   │
│   └── TouchPadManager/       # 触摸板GPIO管理器
│       ├── launch.cpp         # 触摸板事件处理
│       └── CMakeLists.txt     # CMake配置
│
└── media_service/             # WiFi媒体服务
    ├── ai_media_service.c     # WiFi图传和相册同步服务
    ├── Makefile.ai_media_service
    └── How to USE.txt         # 使用说明
```

#### 编译说明

**display程序**：
```bash
cd firmware/display
make
# 生成 build/bin/display
```

**launcher程序**：
```bash
# FFmLauncher
cd firmware/launcher/FFmLauncher/build-arm
cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-arm.cmake
make

# TouchPadManager
cd firmware/launcher/TouchPadManager/build-arm
cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-arm.cmake
make
```

**media_service**：
```bash
cd firmware/media_service
make -f Makefile.ai_media_service
```

#### 运行说明

编译后的程序对应关系：
- `firmware/display/` → `Applications/display`
- `firmware/launcher/FFmLauncher/` → `Applications/launch`
- `firmware/media_service/` → `Applications/ai_media_service`

---

### 📝 examples/ - 示例程序

各种功能示例代码，供开发者参考学习。

```
examples/
├── http/                      # HTTP通信示例
│   ├── mongoose.c/.h         # Mongoose HTTP库
│   ├── post_example.c        # HTTP POST示例
│   └── compile_cmd.txt       # 编译命令
│
├── live_broadcast/            # RTMP直播推流
│   ├── simple_vi_bind_venc.c # 视频编码和推流
│   ├── simple_adec_bind_ao.c # 音频解码播放
│   ├── video_recorder/       # 录像程序
│   └── Readme.txt            # 使用说明
│
├── audio_streaming/           # 音频串流播放
│   ├── audio_server.c        # 音频服务端（C）
│   ├── audio_client.py       # 音频客户端（Python）
│   └── readme.txt            # 使用说明
│
└── video_recorder/            # H.264视频录制
    ├── simple_vi_bind_venc.c
    └── simple_vi_bind_venc_with_A
```

#### 示例说明

**HTTP示例** (`examples/http/`):
- 演示如何使用Mongoose库进行HTTP通信
- 包含POST请求示例

**直播示例** (`examples/live_broadcast/`):
- RTMP协议视频直播推流
- 音频解码和播放
- 需要配合SRS等流媒体服务器使用

**音频串流** (`examples/audio_streaming/`):
- 实时音频流传输
- C语言服务端 + Python客户端

**视频录制** (`examples/video_recorder/`):
- H.264视频编码
- 本地录像保存

---

### ⚙️ hardware/ - 硬件相关文件

硬件设计文件，包括3D打印模型等。

```
hardware/
└── 3d_models/                 # 3D打印模型文件
    ├── 主板板框.dxf          # 主板DXF文件
    ├── 眼镜框.step           # 眼镜框架STEP模型
    ├── 镜腿内壳.step         # 镜腿内壳STEP模型
    └── 镜腿外壳.step         # 镜腿外壳STEP模型
```

---

### 📦 resources/ - 资源文件

项目资源文件，包括媒体文件、脚本、工具等。

```
resources/
├── media/                     # 媒体资源
│   ├── 外观示例.png          # 产品外观图
│   ├── 视频通话示例.mp4      # 演示视频
│   └── [其他图片和视频]
│
├── scripts/                   # 系统脚本
│   ├── S99wifi               # WiFi启动脚本
│   ├── start_wifi.sh         # WiFi连接脚本
│   ├── expect.sh             # 自动化交互脚本
│   ├── wpa_supplicant.conf   # WiFi配置文件
│   └── adb push.txt          # ADB部署说明
│
└── tools/                     # 辅助工具
    └── [expect工具及依赖]    # 自动化交互工具
```

---

### 💾 Applications/ - 预编译程序

预编译的可执行程序（可选，也可以自行编译）。

```
Applications/
├── display                    # 显示程序（来自firmware/display）
├── launch                     # 启动器（来自firmware/launcher）
├── ai_media_service          # 媒体服务（来自firmware/media_service）
└── ai_client_socket          # AI客户端
```

---

### 📡 bluez-5.66/ - 蓝牙协议栈

BlueZ蓝牙协议栈完整源码（392MB）。

> **注意**: 这是第三方依赖，体积较大。如果只进行应用开发，可以使用Docker镜像，无需下载此目录。

---

### 🎬 ffmpeg-rockchip/ - FFmpeg源码

瑞芯微定制的FFmpeg源码（136MB）。

> **注意**: 这是第三方依赖，体积较大。Docker镜像中已包含编译好的版本。

---

## 开发工作流

### 固件开发流程

1. **搭建环境**: 按照[Docker部署指南](docs/DOCKER_DEPLOYMENT.md)搭建开发环境
2. **修改代码**: 在`firmware/`目录下修改源码
3. **编译固件**: 在Docker环境中编译
4. **烧录测试**: 参考[固件烧录指南](docs/FIRMWARE_FLASHING.md)烧录到设备

### 应用开发流程

1. **搭建环境**: 使用Docker环境或交叉编译工具链
2. **编写代码**: 参考`examples/`中的示例
3. **交叉编译**: 使用ARM交叉编译工具链
4. **部署运行**: 通过ADB推送到设备并运行

### 参考示例

需要实现某个功能时，可以参考`examples/`目录中的相关示例：
- HTTP通信 → `examples/http/`
- 视频直播 → `examples/live_broadcast/`
- 音频处理 → `examples/audio_streaming/`

---

## 相关文档

- [项目README](README.md) / [中文版](README.zh.md)
- [贡献指南](CONTRIBUTING.md)
- [Docker部署指南](docs/DOCKER_DEPLOYMENT.md)
- [应用开发指南](docs/APPLICATION_DEVELOPMENT.md)
- [固件烧录指南](docs/FIRMWARE_FLASHING.md)

---

## Project Structure (English)

### 📁 Top-Level Structure

```
OpenSource-Ai-Glasses/
├── .gitignore                 # Git ignore configuration
├── LICENSE                    # Apache 2.0 license
├── README.md                  # Project description (English)
├── README.zh.md               # Project description (Chinese)
├── CONTRIBUTING.md            # Contributing guidelines
├── PROJECT_STRUCTURE.md       # This document
│
├── docs/                      # 📚 Documentation
├── firmware/                  # 🔧 Firmware source code
├── examples/                  # 📝 Example programs
├── hardware/                  # ⚙️ Hardware files
├── resources/                 # 📦 Resource files
├── Applications/              # 💾 Pre-compiled binaries
├── bluez-5.66/               # 📡 BlueZ Bluetooth stack
└── ffmpeg-rockchip/          # 🎬 FFmpeg Rockchip version
```

### 📚 docs/ - Documentation

Project documentation including user guides, development docs, and API references.

### 🔧 firmware/ - Firmware Source Code

Core firmware programs that need to be compiled in Rockchip RV1106B SDK environment.

**Structure**:
- `display/` - JBD display driver with LVGL UI
- `launcher/` - FFmpeg and touchpad managers
- `media_service/` - WiFi media sync service

### 📝 examples/ - Example Programs

Reference examples for developers:
- `http/` - HTTP communication with Mongoose library
- `live_broadcast/` - RTMP live streaming
- `audio_streaming/` - Real-time audio streaming
- `video_recorder/` - H.264 video recording

### ⚙️ hardware/ - Hardware Files

Hardware design files including 3D printing models.

### 📦 resources/ - Resource Files

Project resources:
- `media/` - Product photos and demo videos
- `scripts/` - System scripts (WiFi, startup, etc.)
- `tools/` - Utility tools

### 💾 Applications/ - Pre-compiled Programs

Pre-compiled executables (optional, can compile from source).

---

## Development Workflow

### Firmware Development

1. **Setup**: Follow [Docker Deployment Guide](docs/DOCKER_DEPLOYMENT.en.md)
2. **Modify**: Edit source code in `firmware/` directory
3. **Compile**: Build in Docker environment
4. **Flash**: Refer to [Firmware Flashing Guide](docs/FIRMWARE_FLASHING.en.md)

### Application Development

1. **Setup**: Use Docker environment or cross-compilation toolchain
2. **Code**: Reference examples in `examples/` directory
3. **Compile**: Use ARM cross-compilation toolchain
4. **Deploy**: Push to device via ADB and run

---

## Related Documentation

- [Project README](README.md) / [Chinese Version](README.zh.md)
- [Contributing Guide](CONTRIBUTING.md)
- [Docker Deployment Guide](docs/DOCKER_DEPLOYMENT.en.md)
- [Application Development Guide](docs/APPLICATION_DEVELOPMENT.en.md)
- [Firmware Flashing Guide](docs/FIRMWARE_FLASHING.en.md)
