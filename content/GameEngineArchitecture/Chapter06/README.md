# 第6章 资源及文件系统 
## 6.1 文件系统
* 路径分隔符不同操作系统之间的区别:
	- UNIX 正斜符(/), DOS 及早期 Windows 反斜符( \), 新版 Windows 使用正反斜符
	- macOS 8和9 使用冒号(:), macOS X 支持UNIX的正斜线分隔符记号法.
	- 远端共享, 双反斜线好加上远端计算机名字和共享目录/资源名字指明. 双反斜线号是通用命名规则(universal naming convention, UNC)
	- 当前工作目录命令, Windows cd, UNIX pwd
	- PS3: /dev_bdvd/ 蓝光驱动, /dev_hddx/ 多个硬盘, /app_home/ 映射至用户定义的开发主机路径.
* Windows 下 \ 表示当前工作卷的根目录, \game\assets\animation\walk.anim (当前工作卷)
* OGRE 通过 resources.cfg 添加搜寻路径.
* Windows 通过 shlwapi.dll 实现路径 API.
* 标准 C 程序库提供了两组 API 用于文件操作. 两组 API 分别为有缓冲(buffered)功能和无缓冲(unbuffered)功能. 被称为缓冲区的数据块, 供程序和磁盘之间传送来源或目的字节. 由程序员负责管理数据缓冲时,称之为无缓冲功能的API.由API负责管理所需的输入\输出数据缓冲时,称之为缓冲功能的I/O API. 在 C 标准程序库中, 有 I/O 缓冲功能的函数有时候会被称为流输入/输出(stream I/O). 因为这些 API 把磁盘文件抽象成字节流.

| 操作          | 有缓冲功能 | 无缓冲功能 |
| ------------ | ---------- | ---------- |
| 开启文件     | fopen      | open       |
| 关闭文件     | fclose     | close      |
| 读取文件     | fread      | read       |
| 写入文件     | fwrite     | write      |
| 移动访问位置 | fseek      | seek       |
| 返回当前位置 | ftell      | tell       |
| 读入单行     | fgets      | /          |
| 写入单行     | fputs      | /          |
| 格式化读取   | fscanf     | /          |
| 格式化写入   | fprintf    | /          |
| 查询文件状态 | fstat      | stat       |

* Windows 自身底层文件 API, CreateFile(), ReadFile(), WriteFile(), CloseFile()
* 艺电(红色警戒), 日志输出, 内存满才写入文件, 同时在另外一个线程内写入文件.
* 可包装系统的 I/O API. 以便可实现跨平台, 简化, 延伸功能.
* C 程序库的两种文件 I/O 库都是同步的(synchronous), 程序发出 I/O 请求以后, 必须等待读/写数据完毕, 程序才能继续运行.
* 串流(streaming)是指在背景载入数据, 而主程序同时继续运行. 使用异步(asynchronous)文件 I/O 库实现. 有些操作系统自带异步文件 I/O 库. 如 Windows 通用语言运行平台(common language runtime, CLR) 提供了 System.IO.BegineRead() 及 System.UI.BeginWrite() 函数. PS3 和 PS4 也提供了名为 files 的异步 API.
* 异步文件 I/O 的工作主要通过利用另一个线程处理 I/O 请求. 方法如下:
	- 主线程调用异步函数, 把请求放入一个队列, 并立即返回.
	- I/O 线程从队列中取出请求, 处理 I/O 函数.
	- 请求工作完成后, 调用主线程之前提供的回调函数, 告知操作完成.
	- 若主线程选择等待完成 I/O 请求, 使用信号量(semaphore)处理.
* 任何同步操作, 都能通过将代码置于另一线程而转变为异步操作. 或者将代码移至物理上独立的处理器.

## 6.2 资源管理器
* 资源管理器的两个工作: (1) 建立资源的离线工具(创建资产及把它们转换成引擎可用的形式); (2) 执行期载入,卸载以及操作资源.
* 版本控制系统管理美术资产的问题(美术资产过大, 复制到本地代价过高). 顽皮狗通过私有工具解决此问题.
* 资产调节管道(asset conditioning pipeline, ACP), 把资产转换为引擎所需的格式. 资源包括元数据(metadata), 比如纹理的压缩算法类型, 动画片段的帧范围.
* 资源数据库的形式: (1) 嵌入资产源文件本身. (2) 每个资源文件伴随一个小文本文件. (3) 源文件写入一组 XML 文件中. (4) 使用关系数据库.
* 资源数据库必须提供以下功能:
	- 处理多种类型的资源
	- 创建新资源
	- 删除资源
	- 查看及修改现存的资源
	- 移动资源的文件位置.
	- 资源可以交叉引用其他资源
	- 维持数据库内所有交叉引用的引用完整性(referential integrity). 执行所有常见操作后, 如删除或移动资源, 仍能保持引用完整性.
	- 保存版本历史, 以及记录改动者及事由的日志.
	- 支持不同形式的搜寻和查询.
* 一些资源数据库设计
	- 虚幻4: 万用工具 UnrealEd. 其本身是游戏引擎的一部分.
	- 顽皮狗: MYSQL 数据库以及操作数据库的界面软件.
	- OGRE: 运行时资源管理器
	- XNA: visual Studio 插件 Game Studio Express
* 资产调节管道
	- 数字内容创作(digital content creation, DCC)工具(Maya, ZBrush, Photoshop, Houdini)创建资源数据. 通过资产调节管道(asset conditioning pipeline, ACP)转换成游戏引擎所用的数据. ACP 有时被称为资源调节管道(resource conditioning pipeline, RCP)或工具链(tool chain).
	- 资产的三个处理阶段:
		- 导出器(exporter): 转换为我们能处理的格式. 插件或者我们生成能处理的格式.
		- 资源编译器(resource compiler): 一些改造工作, 如三角形数据重排列为三角形带, 压缩纹理等.
		- 资源链接器(resource linker): 多个资源结合成一个复杂的资源集合, 如一个角色三维模型由多个网格数据, 材质文件, 骨骼文件和动画文件组成.
	- 资源的依赖关系影响了资产在管道内的处理次序, 同时当某个资产做出改动后, 要重新生成哪些资产.
	- 改动资源的数据格式, 可能需要更新使用该格式的所有资源.
* 运行时资源管理的责任:
	- 任何时候同一资源在内存中只有一份副本
	- 管理资源的生命期
	- 复合资源的载入. 复合资源是多个资源组成的资源, 如人物模型.
	- 维护引用完整性
	- 管理资源载入后的内存用量.
	- 资源登录或资源载入初始化. 载入资源后执行自定义的处理(按资源类型)
	- 提供单一统一接口管理多种资源类型.
	- 处理串流(streaming)(即异步资源载入)
* 有些引擎会把多个资源打包为单一文件, 如 ZIP 存档(archive)或其他复合文件(也许是自定义格式的). 优点是减少载入时间.
* 文件载入数据的三大开销:寻道时间(seek time), 开启每个文件的时间, 从文件将数据读入内存的时间. 
* 固态硬盘(solid-state drive)没有寻道时间的问题.
* zip 存档内的虚拟文件也有相对路径. zip 压缩文件蓄意设计成"像"文件系统的样子. OGRE 资源管理器以貌似文件系统路径的字符串去识别所有资源. zip 文档可被压缩. 可将 zip 存档视为模块.(OGRE)
* 虚幻引擎3也采用 zip 的方案, 但在虚幻引擎中, 所有资源都必须置于大型合成文件之中,这些文件成为包(package, "pak文件"), 不允许资源在磁盘上以独立文件出现
* 自定义自设的文件格式. (1) 引擎所需的部分信息可能没有标准格式可以存储. (2) 引擎尽力对资源做脱机处理, 以降低在运行时载入及处理资源数据的时间.
* 可以用文件系统路径作为全局唯一标识符(globally unique identifier, GUID). 还可使用较不直观的GUID类型, 如128位散列码.
* 通过字典(dictionary)键值对实现资源注册表(resource registry), 确保载入的每个资源只有一个副本.
* 为了确保资源加载不影响帧率变化, 两种方法:
	* 游戏关卡的所有资源在游戏进行前全部加载.
	* 资源以异步形式加载(串流), 如玩家在玩关卡A时,关卡B的资源在后台加载.
* 资源的生命期
	* 载入并驻留(load-and-stay-resident, LSR)资源, 表开始时必须被载入, 并驻留在内存直至整个游戏结束.
	* 生命期和游戏关卡相关
	* 生命期少于游戏关卡时间, 如过场动画所使用的动画和音频短片.
	* 即时使用的生命期
* 通过引用计数解决进入下一个关卡的资源重复加载问题. 先下一个关卡的资源引用计数加一, 而后上一个关卡的资源引用计数减一, 最后卸载引用计数为0的资源.
* 在游戏引擎中, 内存分配子系统的设计, 通常与资源管理的设计有密切相关. 尽量运用已有的内存分配器设计资源系统; 或反过来先设计内存分配器, 以配合资源管理所需.
	- 基于堆的资源分配: 个人计算机操作系统支持高级的虚拟内存分配,有能力把不连续的物理内存映射为连续的虚拟内存空间. 对于没有高级虚拟内存技术的主机, 则定期使用内存碎片整理解决.
	- 基于堆栈的资源分配: 适用于资源有限的关卡游戏. 还可使用双端堆栈分配器.
	- 基于池的资源分配.
		- 数据结构大小不能大于组块的大小, 或不需要连续内存仍可正常运作的数据结构(链表).
		- 易造成空间浪费. PS3 组块大小 512K, PS4 组块大小 1M. 
		- 选择组块大小时, 考虑将其大小设为操作系统输入/输出缓冲区大小的倍数.
	- 资源组块分配器, 对于组块那些浪费的内存, 可以通过特殊的内存分配器管理, 这里称之为资源组块分配器.
		- 管理链表, 内含所有为用满内存的组块, 每笔数据还包含自由内存块的位置及大小.
		- 只利用资源组块分配器分配一些和对应关卡生命期相同的内存.
	- 分段的资源文件, 文件段(file section), 典型的资源文件可能包含1~4段, 每段分为一个或多个组块, 每段分为一个或多个组块, 以配合上述基于池的资源分配.
* 游戏资源数据库可表达为, 由互相依赖的数据对象所组成的有向图, 交叉引用分为内部引用(单个文件里两个对象的引用)和外部引用(引用另一个文件中的对象).
	- 一组自给自足, 由相互依赖的资源所组成的资源称为复合资源. 例如三维模型, 包含一个或多个三角形网格, 骨骼, 动画集合, 每个网格对应一个材质,材质引用一个或多个纹理.
* 处理资源间的交叉引用
	- 全局统一标识符: 把交叉引用存储为字符串或散列码, 内含被引用对象的唯一标识符. 资源管理器维护一个全局资源查找表. 每次将资源对象载入内存后, 把其指针以 GUID 为键加进查找表中.
	- 指针修正表
		- 对象存储至二进制文件, 把指针转换为文件偏移值(file offset).
		- 将文件载入内存时,将偏移值转换回指针. 这种转换称为指针修正.
	- 存储 C++ 对象为二进制映像:构造函数
		- 二进制文件完全不支持C++对象.限制系统使其仅支持PODS结构(plain old data structure)
		- 或者无需函数, 只含不做事情的平凡构造函数(trivial contructor)
		- 把非PODS对象的偏移值组成一个表, 表里记录对象属于哪个类, 而后将表写入二进制文件.
	- 处理外部引用, 载入所有文件, 使用主查找表把所有指针转换一遍, 从 GUID 或文件偏移值转换为真实的内存地址.
* 载入后初始化, 资源载入后需要一些"整理"才能使用.
