# MouseWriter

## 项目简介
MouseWriter 是一个基于 Qt 的全屏手写输入应用，使用 HID 数位笔/手写板采集坐标并通过本地字符识别库（`CharReco.dll` 和 `HTopRecg.dll`）进行汉字识别。程序支持平滑笔迹渲染和自动清屏识别。

### 主要功能
- **透明/无边框全屏手写**，支持始终置顶。
- **HID 读取**：支持特定 VID/PID 的手写设备（默认 `VID: 0x08f2`，`PID: 0x6370`），可在代码中调整。
- **本地字符识别**：识别通过 `CharReco_Stroking`（落笔）和 `CharReco_StrokeEnd`（提笔）触发的笔画。
- **系统托盘功能**：右键清屏，显示识别结果，退出程序。

## 目录结构
- `main.cpp`：程序入口，启动应用。
- `mainwindow.*`：界面逻辑，手写区域绘制和识别调用。
- `thread.*`：处理 HID 读取与坐标传递到 UI。
- `CharReco*.dll`、`HTopRecg.dll`：手写识别所需的本地 DLL 库。
- `build/`、`release/`：构建生成文件和依赖 DLL。
- `Makefile`、`*.pro`：构建配置文件。

## 环境要求
- **操作系统**：Windows。
- **Qt 版本**：Qt 5.6 及以上（曾使用 Qt 5.6.3，MinGW 32bit 编译）。
- **编译器**：MinGW 32bit（与 DLL 配合）。
- **依赖库**：`hidapi.dll` 和手写识别 DLL（随仓库提供）。
- 
## 关键实现点（便于调试） 
- 程序以动态加载本地识别库为主，在 [mainwindow.cpp](mainwindow.cpp) 中通过 QLibrary resolve 取得函数指针： - 类型定义：[CharReco_CharRecoInitFunction](mainwindow.cpp), [CharReco_CharRecoGetParaFunction](mainwindow.cpp) 等 - 全局指针：[CharReco_CharRecoInit](mainwindow.cpp), [CharRecoGetPara](mainwindow.cpp) - 服务端识别相关：[CharRecoSvrRecognize](mainwindow.cpp), [CharRecoSvrInit](mainwindow.cpp) - CharReco 的类与导出函数在 [CharReco.h](CharReco.h) 中声明（例如类 [CCharReco](CharReco.h)，函数 [CharRecoStroking](CharReco.h)、[CharRecoStrokeEnd](CharReco.h)）。

## 构建与运行
1. **打开与编译**：
   - 使用 Qt Creator 打开 `MouseWriter.pro` 项目文件。
   - 确保包含目录与库路径正确指向仓库中的 `hidapi.h`、`hidapi.lib` 以及识别库 DLL 路径。
   - 选择合适的构建套件并编译。

2. **运行**：
   - 将识别库和数据文件（如 `CharReco.dll`、`HTopRecg.dll`、`HSEngine.dll` 等）放在可执行文件同一目录或系统可搜索路径中。
   - 连接 HID 手写设备（默认 VID/PID: `0x08f2`/`0x6370`）。
   - 启动程序，窗口将全屏置顶，左键书写，右键清屏并触发识别。

## 操作提示
- **书写操作**：
  - **左键按下/移动/抬起**：落笔、行笔、提笔操作。
  - **提笔超过 2 秒**：自动清屏并触发识别。
- **右键清屏**：立即清屏并识别当前笔迹。
  
## 常见调整
- **修改设备 VID/PID**：在 `thread.cpp` 中调整 `hid_enumerate` 和 `hid_open`。
- **修改识别匹配范围**：在 `mainwindow.cpp` 中调整 `m_bMatchMulti` 和 `m_uMatchRange`。
- **调整透明度/窗口置顶**：在 `mainwindow.cpp` 构造函数中配置 `setWindowOpacity` 和 `setWindowFlags`。

## 已知限制
- 识别算法依赖于随附的闭源 DLL 和词典数据，缺失时无法编译或运行。
- 项目未包含自动化测试，建议在目标设备上进行实机验证。

## 构建（命令行）
1. 确保 Qt 与编译器工具链（如 MinGW 或 MSVC）在 `PATH` 中，或者使用 Qt Creator 打开 `MouseWriter.pro`。
2. 命令行构建（示例使用 `qmake` 和 `make`）：
   ```sh
   qmake MouseWriter.pro
   make        # 或 mingw32-make / nmake，取决于工具链
   ```
