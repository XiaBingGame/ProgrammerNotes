# 第9章 调试及开发工具
## 9.1 日志及跟踪
* OutputDebugString() 向 Visual Studio 的调试主控台(debug console)打印信息.
	- OutputDebugString() 不支持格式化输出
```
#include <stdio.h>
#include <windows.h>

int VDebugPrintfF(const char* format, va_list argList)
{
	const unsigned int MAX_CHARS = 1024;
	static char s_buffer[MAX_CHARS];

	int charsWritten = vsnprintf(s_buffer, MAX_CHARS, format, argList);
	OutputDebugString(s_buffer);
	return charsWritten;
}

int DebugPrintF(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);
	int charsWritten = VDebugPrintfF(format, argList);
	va_end(argList);

	return charsWritten;
}
```
* 冗长级别(verbosity level)
* 频道(channel), 比如某频道接收动画系统的消息,另一频道接收物理系统的消息.
	- 可使用过滤器(filter)
* 把输出同时抄写至日志文件, 考虑是否每次调用调试输出函数后对日志文件清空缓冲(flush)
* 崩溃报告: 设置一个顶层异常处理函数(top-level exception handler), 以捕获大部分崩溃情景, 崩溃报告可包含各种信息:
	- 崩溃时玩家在玩的关卡
	- 崩溃时玩家角色所在的世界空间位置
	- 游戏崩溃时玩家角色的动画/动作状态
	- 崩溃时正在运行的一个或多个游戏脚本
	- 堆栈跟踪, 把堆栈中所有非内联函数的符号化名称打印出来
	- 引擎中所有内存分配器的状态(余下内存的大小,内存碎片程度等)
	- 其他可能相关信息
	- 游戏崩溃一刻的截屏

## 9.2 调试用的绘图功能
* 引擎提供一组 API, 去绘制有颜色的线条, 简单图形, 三维文本. 该组 API 称之为调试绘图功能.
* 调试绘图 API 通常满足下列要求:
	* 简单易用
	* 支持图元:直线,球体,点,坐标轴,包围盒,格式化文本
	* 控制颜色,线宽,球体半径,点的大小,坐标轴的长度等等
	* 可控制是否使用深度测试.
	* 可以在任何地方调用该 API.
	* 每个调试图元包含生命期.
	* 能高效处理大量调试图元

## 9.3 游戏内置菜单
* 可做事情:
	* 切换全局布尔设定
	* 调校全局整数及浮点数值
	* 调用一些引擎提供的函数, 执行任何任务
	* 开启子菜单

## 9.4 游戏内置主控台
* 内置主控台(in-game console): 提供命令行接口使用游戏引擎的功能

## 9.5 调试用摄像机和游戏暂停
* 内置菜单和主控台应当附带的功能: 自由摄像机和暂停功能
* 慢动作模式实现

## 9.6 作弊
* 便于调试
* 任意移动、不死身、任意武器、无尽弹药、选择角色

## 9.7 屏幕截图及录像
* 选项
	- 截图是否包含调使用的图形及文本
	- 截图是否包含 HUD
	- 分辨率，高分辨率，通过偏移投影矩阵+拼接实现高分辨率截图
	- 简单的摄像机动画。
* 录屏软件: Fraps, Camtasia Softerware 的 Camtasia, ExKode 的 Dxtory, NCH Software 的 Debut 和 Mirillis 的 Action! 等.

## 9.8 游戏内置性能剖析
* 记录代码段落的用时
* 剖析工具可以测量每个函数调用的包含(inclusive)执行时间和排他(exclusive)执行时间。找出耗时较长或调用次数很多的函数。
* 剖析游戏的性能时，可以测量每个主要阶段的执行时间
```
while(!quitGame)
{
	{
		PROFILE(SID('Poll Joypad'));
		PollJoypad();
	}
	{
		PROFILE(SID('Animation'));
		UpdateAllAnimations();
	}
	// ...
	{
		PROFILE(SID('Rendering'));
		RenderScene();
	}
}
```
* 无法用于深层的递归调用, 可以预先定义所有样本箱, 表示层级关系
```
ProfilerDeclareSampleBinSID('Rendering', NULL);
	ProfilerDeclareSampleBinSID('Visibility', SID('Rendering'));
	ProfilerDeclareSampleBinSID('ShaderSetUp', SID('Rendering'));
		ProfilerDeclareSampleBinSID('Materials', SID('ShaderSetup'));
	ProfilerDeclareSampleBinSID('SubmitGeo', SID('Rendering'));
ProfilerDeclareSampleBinSID('Audio', NULL);
```
	- 仅限于每个子函数有一个父函数的情况
* 导出至 excel, 使用逗号分隔型取值(csv)

## 9.9 游戏内置的内存统计和泄漏检测
* 内存追踪工具，了解每个引擎子系统花费了多少内存，以及是否有内存泄漏。
* 跟踪内存的问题:
	* 不能控制第三方库的分配和释放
	* 显存无法跟踪
	* 不同形式的分配器.
* 内存分析工具的要点:
	* 提供准确信息
	* 把数据以方便及令问题显而易见的方式呈现
	* 提供上下文信息以协助团队跟踪问题根源
