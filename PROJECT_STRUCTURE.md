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
├── firmware/                  # 🔧 固件开发
├── examples/                  # 📝 示例程序
├── hardware/                  # ⚙️ 硬件相关文件
├── resources/                 # 📦 资源文件
├── third_party/               # 📦 第三方库（已修改）
├── Applications/              # 💾 预编译程序
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

### 🔧 firmware/ - 固件开发

固件相关的所有内容，包括源码、脚本和工具。需要在瑞芯微RV1106B SDK环境中编译。

```
firmware/
├── src/                       # 应用程序源码
│   ├── display/              # JBD显示屏主程序
│   │   ├── main.c           # 主程序入口
│   │   ├── jbd013_api.c     # JBD显示屏驱动API
│   │   ├── hal_driver.c     # 硬件抽象层驱动
│   │   ├── ui/              # LVGL UI界面
│   │   └── Makefile         # 编译配置
│   │
│   ├── launcher/             # 启动管理器
│   │   ├── FFmLauncher/     # FFmpeg和摄像头启动管理
│   │   └── TouchPadManager/ # 触摸板GPIO管理器
│   │
│   └── media_service/        # WiFi媒体服务
│       ├── ai_media_service.c
│       └── Makefile.ai_media_service
│
├── scripts/                   # 系统脚本
│   ├── S99wifi               # WiFi启动脚本
│   ├── start_wifi.sh         # WiFi连接脚本
│   ├── expect.sh             # 自动化交互脚本
│   └── wpa_supplicant.conf   # WiFi配置
│
└── tools/                     # 辅助工具
    ├── bin/                  # 可执行工具
    └── lib/                  # 工具库
```

#### 编译说明

**display程序**：
```bash
cd firmware/src/display
make
# 生成 build/bin/display
```

**launcher程序**：
```bash
# FFmLauncher
cd firmware/src/launcher/FFmLauncher/build-arm
cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-arm.cmake
make

# TouchPadManager
cd firmware/src/launcher/TouchPadManager/build-arm
cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-arm.cmake
make
```

**media_service**：
```bash
cd firmware/src/media_service
make -f Makefile.ai_media_service
```

#### 运行说明

编译后的程序对应关系：
- `firmware/src/display/` → `Applications/display`
- `firmware/src/launcher/FFmLauncher/` → `Applications/launch`
- `firmware/src/media_service/` → `Applications/ai_media_service`

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

项目媒体资源文件。

```
resources/
└── media/                     # 媒体资源
    ├── 外观示例.png          # 产品外观图
    ├── 视频通话示例.mp4      # 演示视频
    └── [其他图片和视频]
```

> **注意**: 系统脚本和工具已移至 `firmware/scripts/` 和 `firmware/tools/`

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

### 📦 third_party/ - 第三方库

已修改的第三方开源库源码。

```
third_party/
├── README.md              # 第三方库说明文档
├── bluez-5.66/           # BlueZ蓝牙协议栈（392MB）
└── ffmpeg-rockchip/      # 瑞芯微定制FFmpeg（136MB）
```

**说明**:
- 这些库已针对RV1106B平台进行定制修改
- 详细信息请查看 [third_party/README.md](../third_party/README.md)
- 如果只进行应用开发，使用Docker镜像即可，无需编译这些库

**许可证**:
- BlueZ: GPL v2
- FFmpeg: LGPL v2.1+ / GPL v2+

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
├── firmware/                  # 🔧 Firmware development
├── examples/                  # 📝 Example programs
├── hardware/                  # ⚙️ Hardware files
├── resources/                 # 📦 Resource files
├── third_party/               # 📦 Third-party libraries (modified)
├── Applications/              # 💾 Pre-compiled binaries
```

### 📚 docs/ - Documentation

Project documentation including user guides, development docs, and API references.

### 🔧 firmware/ - Firmware Development

All firmware-related content including source code, scripts, and tools. Needs to be compiled in Rockchip RV1106B SDK environment.

**Structure**:
```
firmware/
├── src/                      # Application source code
│   ├── display/             # JBD display driver with LVGL UI
│   ├── launcher/            # FFmpeg and touchpad managers
│   └── media_service/       # WiFi media sync service
├── scripts/                  # System scripts
└── tools/                    # Utility tools
```

### 📝 examples/ - Example Programs

Reference examples for developers:
- `http/` - HTTP communication with Mongoose library
- `live_broadcast/` - RTMP live streaming
- `audio_streaming/` - Real-time audio streaming
- `video_recorder/` - H.264 video recording

### ⚙️ hardware/ - Hardware Files

Hardware design files including 3D printing models.

### 📦 resources/ - Resource Files

Project media resources:
- `media/` - Product photos and demo videos

> **Note**: System scripts and tools have been moved to `firmware/scripts/` and `firmware/tools/`

### 💾 Applications/ - Pre-compiled Programs

Pre-compiled executables (optional, can compile from source).

---

## Development Workflow

### Firmware Development

1. **Setup**: Follow [Docker Deployment Guide](docs/DOCKER_DEPLOYMENT.en.md)
2. **Modify**: Edit source code in `firmware/src/` directory
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
