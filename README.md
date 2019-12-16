原作将 Zen6 或者 Zen7 转成命令行 GTP，开发环境：Windows XP + VS2013 Update5 + Boost 1.64.

本 Fork 主要更新如下：

开发环境：将 Windows VS Boost 均更新至最新稳定版。

项目设置：移除 `Debug` 解决方案；修改编译输出中间目录；默认运行库为 `MultiThreadedDLL` 故依赖 [VC++ 2015-2019 x86](https://aka.ms/vs/16/release/vc_redist.x86.exe) —— 该项对应设置：`项目属性 - 配置属性 - C/C++ - 代码生成 - 运行库`，否则无法正确识别 Boost 库。

默认参数：参考 [GoAIRatings](https://github.com/breakwa11/GoAIRatings/blob/master/AIcmds.md#zen7-gtp4zen)，默认设为 Zen7 最强。

GTP 输出：版本号为纯数字，避免带 `v` 时第一个小数点前出现空格；应用名随调用的 `Zen.dll` 版本而变为 `GTP4Zen7` 或 `GTP4Zen6`。

文件编码：将 `gtp4zen.cpp` `zen6gtp.cpp` `zen7gtp.cpp` 三个文件编码从 `GB2312` 转为 `UTF-8+BOM`，使 VSCode 和 Github 等 PC 软件可以正确读取中文注释且 VS 编译不报错。

其他改进：修订文字说明；增加简易编译步骤；等等。

……

Windows 10 编译步骤如下：

下载 [Boost C++ Libraries](https://www.boost.org/)，解压。

默认将 Boost 库置于 `D:\Libs\Boost`。如需自定义 Boost 版本或目录，请修改 `/gtp4zen/boost-static.props` 中相应字段，切记以 `\` 结尾。

需部分编译 Boost：

`cd Boost`

`.\bootstrap.bat`

`.\b2.exe --with-date_time --with-filesystem --with-program_options`

用 VS 打开 `gtp4zen.sln` 文件，直接编译即可。

以 x64 编译的 gtp4zen.exe 无法识别 32 位的 `zen.dll`，故仅提供 win32 版。
