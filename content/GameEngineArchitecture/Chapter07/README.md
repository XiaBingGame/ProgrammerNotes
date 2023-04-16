# 第七章 游戏循环及实时模拟
## 7.1 渲染循环
## 7.2 游戏循环
* 不同子系统的更新频率可能不一样, 例如动画子系统 30Hz 或 60Hz, 动力学(物理)模拟可能需要更高的频率, 如 120Hz. 人工智能可能每秒1-2次更新.

## 7.3 游戏循环的架构风格
* Windows 平台的窗口消息泵
* 回调驱动框架
	- OGRE 库
		- Ogre::FrameListener 派生一个类, 覆写两个虚函数 frameStart() 和 frameEnded()

## 7.4 抽象时间线
* 真实时间, CPU 的高分辨率计时寄存器的值, 度量单位为CPU周期(或其倍数). 乘以高分辨率计时器频率得到秒数.
* 游戏时间, 暂停和慢动作的实现. 还可用于调试, 注意暂停只是暂停统计游戏时间, 但游戏循环仍继续.
* 局部及全局时间线: 控制动画的播放速率, 通过局部和全局时间线之间的映射实现.

## 7.5 测量及处理时间
* 受CPU速度影响的早期游戏, CPU越快, 游戏速度越快.
* 在帧开始时和结束时分别取时间, 可以得到帧时.
	- 问题: 下一帧可能因为某些原因, 比本帧消耗更多(或更少)时间,我们称此类事件为帧率尖峰(frame-rate spike).
	- 计算连续几帧的平均时间,用来预估下一帧的帧时.
* 调控帧率
	- 确保每帧的准确耗时, 若本帧耗时比理想时间短,只需让主线程休眠,直至到达目标时间.如耗时比理想时间长,则等到下一个目标时间. 
* 开发时, 使用"可变帧率"模式, 如游戏能持续地达到目标帧率, 则开启帧率调控, 获取其好处
* 帧率稳定有利于物理模拟, 动画稳定, 避免画面撕裂, 录播功能可靠.
* 游戏录播功能实现: 记录游戏进行时的所有相关事件,将这些事件及其时间戳存储下来,播放时使用相同的初始条件和随机种子,就能重播这些事件.
* 画面撕裂: 出现于背景缓冲区与前景缓冲区交换时,显示硬件只"绘制"了部分屏幕.
* 等待垂直消隐区间(vertical blanking interval)称为垂直同步(v-sync). 这是另一种帧率调控方法.
* C 程序库函数 time(), 返回自 1970年1月1日零点至今的秒数, 单位为秒, 返回值为整数.
* 使用高分辨率计时器(high-resolution timer)计时.
* Windows API QueryPerformanceCounter() 读取CPU的 64 位计数寄存器, QueryPerformanceFrequency() 返回本 CPU 的每秒计数器递增次数.
* 高分辨率计数器的漂移: 多核处理器各自有独立的高分辨率计时器, 它们彼此漂移(drift).
* 64位整数时钟可以保持最高的精度
* 32位整数衡量高精度但较短的时间
```
	// 剖析一段代码的效能
	U64 begin_ticks = readHiResTimer();
	
	// 想要测量性能的代码
	doSomething();
	doSomethingElse();
	nowReallyDoSomething();
	
	// 测量经过时间
	U64 end_ticks = readHiResTimer();
	U32 dt_ticks = static_cast<U32>(end_ticks - begin_ticks);
```
* 以秒为单位使用32位浮点数计时
```
// 设为理想帧时
F32 dt_seconds = 1.0f / 30.0f;

// 循环开始读取当前时间
U64 begin_ticks = readHiResTimer();

while(true)
{
	runOneIterationOfGameLoop(dtSeconds);
		
	U64 end_ticks = readHiResTimer();
	dt_seconds = (F32)(end_ticks - begin_ticks) / (F32)getHiResTimerFrequency();
		
	begin_ticks = end_ticks;
}
```
* 浮点数存储较大的时间时, 时间将变得不准确.
* 自定义事件单位, 如(1/300)s为时间单位, 使用整数表示.
* 由于调试时中断会造成较大的帧间间隔, 我们可以强行设置超过预设上限的帧间间隔设为固定值, 如 1/30s 或 1/60s.
```
// 设为理想帧时
F32 dt_seconds = 1.0f / 30.0f;

// 循环开始读取当前时间
U64 begin_ticks = readHiResTimer();

while(true)
{
	updateSubsystemA(dt);
	updateSubsystemB(dt);
	// ...
	renderScene();
	swapBuffers();
		
	U64 end_ticks = readHiResTimer();
	dt_seconds = (F32)(end_ticks - begin_ticks) / (F32)getHiResTimerFrequency();
	
	if(dt_seconds > 1.0f)
	{
		dt = 1.0f / 30.0f;
	}
		
	begin_ticks = end_ticks;
}
```
* 时钟类 Clock

## 7.6 多处理器的游戏循环
* 参考文献[20]介绍多处理器如何处理游戏循环
* Xbox 360
	- 3个 PowerPC 处理器核, 每个核有其专用的 L1 指令缓存和 L1 数据缓存, 3 个核公用一个 L2 缓存, 3个核和GPU共用一个统一的512M内存.
* PS3
	- 把内存切割为多个区块,每块为提升系统中特定处理器的效率而设计
	- 主CPU为Power处理部件(Power Processing Unit, PPU), 6个副处理器,协同处理部件(Synergistic Processing Unit, SPU).
	- PS3的GPU称之为RSX, 256M显存, PPU能直接存取256M系统内存, SPU含专用256K内存区
* PS4
* Xbox One
* SIMD: 单指令多数据指令集(single instruction multiple data, SIMD)指令集.
* 分叉及汇合, 把一个单位的工作分割成更小的子单位,再把这些工作量分配到多个核或硬件线程(分叉), 最后待所有工作完成后再合并结果(汇合).
* 每个子系统运行于独立线程
	- 某些引擎子系统置于独立线程上运行. 主控线程(master thread)负责控制及同步这些子系统的次级子系统, 并继续处理游戏的大部分高级逻辑(游戏主循环)
* 作业模型:工作分为多个细小比较独立的作业(job),作业准备就绪后就可以加入队列中, 待有闲置的处理器,作业才会从队列中被取出执行.
* 异步程序设计:异步发出操作请求后,不能立即得到结果. 许多时候,异步代码可以在某帧启动请求,在下一帧提取结果.

## 7.7 网络多人游戏循环
* 网络多人游戏的详细细节可见参考文献[3]
* 主从式模型 client-server model
	- 大部分游戏逻辑运行在单个服务器上. 多个客户端可链接至服务器. 客户端基本上只是一个"非智能(dumb)"渲染引擎. 客户端会读取人体学接口设备的数据以及控制本地的玩家角色.
	- 客户端和服务器的代码可能以不同频率进行更新
* 点对点模型(peer-to-peer): 游戏中的每个动态对象,都有其对应的单一机器管辖. 每台机器对其拥有管辖权(authority)的对象就如同服务器,对于其他无管辖权的对象,机器就如同客户端.

