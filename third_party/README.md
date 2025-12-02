# Third-Party Libraries | 第三方库

[English](#third-party-libraries-english) | [中文](#第三方库说明-中文)

---

## 第三方库说明 (中文)

本目录包含项目依赖的第三方开源库。这些库已针对RV1106B平台进行了定制修改。

### 📚 包含的库

#### 1. BlueZ 5.66 - 蓝牙协议栈

**目录**: `bluez-5.66/`

**原始来源**: [BlueZ Project](http://www.bluez.org/)
- 官方版本: 5.66
- 许可证: GPL v2
- 原始仓库: https://github.com/bluez/bluez

**修改说明**:
- ✅ 已针对uClibc环境进行适配
- ✅ 针对RV1106B平台优化
- ✅ [在此补充你的具体修改内容]

**编译方法**:
```bash
cd third_party/bluez-5.66
# [补充编译命令]
```

**使用说明**:
本项目的蓝牙功能依赖此版本的BlueZ。

---

#### 2. FFmpeg-Rockchip - 视频编解码库

**目录**: `ffmpeg-rockchip/`

**原始来源**: 瑞芯微定制版FFmpeg
- 基于版本: FFmpeg [版本号]
- 许可证: LGPL v2.1+ / GPL v2+
- 原始仓库: [补充原始仓库地址]

**修改说明**:
- ✅ 集成瑞芯微硬件加速
- ✅ 针对RV1106B平台优化
- ✅ [在此补充你的具体修改内容]

**编译方法**:
```bash
cd third_party/ffmpeg-rockchip
# [补充编译命令]
```

**使用说明**:
本项目的视频录制、直播推流等功能依赖此版本的FFmpeg。

---

### 📝 修改记录与维护

#### 为什么保留完整源码？

1. **定制修改**: 针对RV1106B平台进行了深度定制
2. **依赖固定**: 确保编译环境的一致性
3. **离线开发**: 支持完全离线的开发环境

#### 如何追踪修改？

如果你需要查看相对于原始版本的修改：

**对于BlueZ**:
```bash
cd third_party/bluez-5.66
# 下载官方5.66版本进行对比
wget http://www.kernel.org/pub/linux/bluetooth/bluez-5.66.tar.xz
tar -xf bluez-5.66.tar.xz
diff -ur bluez-5.66-original/ bluez-5.66/ > ../bluez-modifications.patch
```

**对于FFmpeg**:
```bash
cd third_party/ffmpeg-rockchip
# 与瑞芯微官方版本对比
# [补充对比方法]
```

#### 更新建议

⚠️ **注意**: 这些库已针对项目定制，不建议直接升级到新版本。如需升级：

1. 备份当前修改
2. 记录所有定制内容
3. 在新版本上重新应用修改
4. 完整测试所有功能

---

### 🔧 编译依赖

这些第三方库在以下场景需要编译：

#### 在Docker环境中
```bash
# 完整镜像已包含编译好的版本，无需重新编译
docker run -it aiglasses/rk-rv1106b:ready bash -l
```

#### 在SDK环境中
```bash
# 在瑞芯微SDK中编译
cd /path/to/rv1106b_rv1103b_linux_ipc_v1.0.0_20241016
# 执行SDK的构建流程会自动编译这些库
./build.sh
```

---

### 📖 许可证信息

请注意各库的许可证要求：

| 库名 | 许可证 | 说明 |
|------|--------|------|
| BlueZ | GPL v2 | 使用时需遵守GPL协议 |
| FFmpeg | LGPL v2.1+ / GPL v2+ | 根据编译选项可能需要遵守GPL |

**重要提示**:
- 本项目使用Apache 2.0许可证
- 第三方库保持其原有许可证
- 分发时请注意许可证兼容性

---

### 🤝 贡献修改

如果你对这些第三方库有改进：

1. **优先考虑**: 能否提交到上游（BlueZ/FFmpeg官方）？
2. **记录修改**: 在本文档中详细说明修改内容
3. **测试验证**: 确保修改不影响现有功能
4. **提交PR**: 按照[贡献指南](../CONTRIBUTING.md)提交

---

### 📞 问题反馈

如果遇到第三方库相关问题：
- 查看库的官方文档
- 在项目Issues中报告
- 联系: iam5stilllearning@foxmail.com

---

## Third-Party Libraries (English)

This directory contains third-party open-source libraries required by the project. These libraries have been customized for the RV1106B platform.

### 📚 Included Libraries

#### 1. BlueZ 5.66 - Bluetooth Protocol Stack

**Directory**: `bluez-5.66/`

**Original Source**: [BlueZ Project](http://www.bluez.org/)
- Official Version: 5.66
- License: GPL v2
- Repository: https://github.com/bluez/bluez

**Modifications**:
- ✅ Adapted for uClibc environment
- ✅ Optimized for RV1106B platform
- ✅ [Add your specific modifications here]

**Build Instructions**:
```bash
cd third_party/bluez-5.66
# [Add build commands]
```

---

#### 2. FFmpeg-Rockchip - Video Codec Library

**Directory**: `ffmpeg-rockchip/`

**Original Source**: Rockchip customized FFmpeg
- Based on: FFmpeg [version]
- License: LGPL v2.1+ / GPL v2+
- Repository: [Add original repository]

**Modifications**:
- ✅ Integrated Rockchip hardware acceleration
- ✅ Optimized for RV1106B platform
- ✅ [Add your specific modifications here]

**Build Instructions**:
```bash
cd third_party/ffmpeg-rockchip
# [Add build commands]
```

---

### 📝 Modification Tracking

#### Why Keep Full Source Code?

1. **Custom Modifications**: Deep customization for RV1106B platform
2. **Dependency Locking**: Ensure build environment consistency
3. **Offline Development**: Support fully offline development

#### License Information

Please note each library's license requirements:

| Library | License | Notes |
|---------|---------|-------|
| BlueZ | GPL v2 | Must comply with GPL when using |
| FFmpeg | LGPL v2.1+ / GPL v2+ | May require GPL compliance based on build options |

**Important**:
- This project uses Apache 2.0 license
- Third-party libraries retain their original licenses
- Be aware of license compatibility when distributing

---

### 🤝 Contributing Modifications

If you have improvements to these libraries:

1. **Consider Upstream**: Can it be submitted to official BlueZ/FFmpeg?
2. **Document Changes**: Detail modifications in this document
3. **Test Thoroughly**: Ensure no impact on existing functionality
4. **Submit PR**: Follow [Contributing Guide](../CONTRIBUTING.md)

---

### 📞 Support

For third-party library issues:
- Check official library documentation
- Report in project Issues
- Contact: iam5stilllearning@foxmail.com
