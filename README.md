# MouseWriter

简要说明、构建与运行指引。

## 项目概览
MouseWriter 是一个基于 Qt 的手写识别演示程序，使用本地识别库（CharReco / CharRecoSvr）和 HID 接口。主要源文件与入口：

- 项目文件：[MouseWriter.pro](MouseWriter.pro)  
- 程序入口与 UI：[mainwindow.cpp](mainwindow.cpp)、[mainwindow.h](mainwindow.h)、[mainwindow.ui](mainwindow.ui)  
- 线程示例：[thread.cpp](thread.cpp)、[thread.h](thread.h)  
- 识别接口声明：[`CharReco.h`](CharReco.h)、[`CharRecoSvr.h`](CharRecoSvr.h)  
- HID 抽象：[`hidapi.h`](hidapi.h)  
- 系统 Makefile： [Makefile](Makefile) 及 build 子目录下的生成 Makefile（调试/发布）

## 依赖
- Qt (在项目中使用 Qt Widgets，示例使用 Qt5.6.3)  
- mingw / MSVC（取决于你的环境）  
- 本地 DLL：CharReco.dll, CharRecoSvr.dll（或对应的 .lib 在链接时使用）

## 关键实现点（便于调试）
- 程序以动态加载本地识别库为主，在 [mainwindow.cpp](mainwindow.cpp) 中通过 QLibrary resolve 取得函数指针：
  - 类型定义：[`CharReco_CharRecoInitFunction`](mainwindow.cpp), [`CharReco_CharRecoGetParaFunction`](mainwindow.cpp) 等  
  - 全局指针：[`CharReco_CharRecoInit`](mainwindow.cpp), [`CharRecoGetPara`](mainwindow.cpp)  
  - 服务端识别相关：[`CharRecoSvrRecognize`](mainwindow.cpp), [`CharRecoSvrInit`](mainwindow.cpp)  
- CharReco 的类与导出函数在 [`CharReco.h`](CharReco.h) 中声明（例如类 [`CCharReco`](CharReco.h)，函数 [`CharRecoStroking`](CharReco.h)、[`CharRecoStrokeEnd`](CharReco.h)）。

## 构建（命令行）
1. 确保 Qt 与编译器在 PATH 中，或使用 Qt Creator 打开 [MouseWriter.pro](MouseWriter.pro)。  
2. 命令行构建（示例使用 qmake + make）：
   ```sh
   qmake MouseWriter.pro
   make        # 或 mingw32-make / nmake，取决于你的工具链
