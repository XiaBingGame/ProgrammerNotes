- [第5章 着色基础](#第5章-着色基础)
  - [1 Gooch 着色模型](#1-gooch-着色模型)
  - [2 光源](#2-光源)
    - [2.1 点光源](#21-点光源)
    - [2.2 聚光灯](#22-聚光灯)
  - [3 着色模型](#3-着色模型)
- [4 失真和反失真](#4-失真和反失真)
  - [5 透明，Alpha, 合成](#5-透明alpha-合成)
  - [6 显示编码](#6-显示编码)

# 第5章 着色基础
## 1 Gooch 着色模型
* Gooch 着色模型： 法线与光线方向越趋于一致，色调越暖，否则色调越冷

![](Media/DragonGoothShadingModel.png)

图1 带高亮效果的Gooth着色模型
* 主要公式如下

![](Media/Equation_05_01.png)

![](Media/Equation_05_02.png)

    x 带加减符号类似 opengl 的 clamp(0.0, 1.0)， 将x的值裁剪至0.0到1.0之间的范围。
    r 向量计算出太阳光方向在表面上反射的向量

## 2 光源
* 光照公式分为有光照和无光照部分

![](Media/Equation_05_03.png)

![](Media/Equation_05_04.png)

![](Media/Equation_05_05.png)

    最后一个公式是通用的公式

* 下面的公式是基于物理的, 公式中的 + 号表示小于 0 的值被裁剪掉

![](Media/Equation_05_06.png)

* 上面公式中光照函数最简单的形式如下, 可称之为郎伯(Lambert Light Model)光照模型

![](Media/Equation_05_07.png)

![](Media/Equation_05_08.png)

### 2.1 点光源
* 一些参数计算如下：

![](Media/Equation_05_09.png)

![](Media/Equation_05_10.png)
    
    * l 为光的方向
    * r 还可用于衰减光照的强度
* 光强和距离的关系

![](Media/Equation_05_11.png)

    * r0 表示该处的光强为 clight0, 取得一个参考值

![](Media/Equation_05_12.png)

    * 增加一个参数表示光源大小

![](Media/Equation_05_13.png)

    * CryEngine 的做法

* 由于光源太远，计算光照效果会消耗性能，因此使用窗口函数限定计算的范围,下面公式是虚幻引擎的做法

![](Media/Equation_05_14.png)

* 和距离有关的公司可以简化为下面的公式

![](Media/Equation_05_15.png)

* Just Cause 2 使用的公式如下

![](Media/Equation_05_16.png)

### 2.2 聚光灯

* 聚光灯公式，方向向量也参与了计算

![](Media/Equation_05_17.png)

* 聚光灯的内角和外角， 半影角(penumbra angle)和阴影角(umbra angle)

![](Media/Figure_05_02.png)

* 聚光灯方向函数的两种实现，分别用于 Frostbite 游戏引擎和 three.js 浏览器图形库

![](Media/Equation_05_18.png)

* 下图展示了效果，分别为方向光，点光源，聚光灯

![](Media/Figure_05_03.png)

## 3 着色模型
* 着色模型的计算频率，和位于管道中的 stage 有关
    - Vertex shader --- 逐顶点计算
    - Hull shader --- 逐 surface patch 计算
    - Domain shader --- 逐 post-tessellation 顶点计算
    - Geometry shader --- 逐几何图元计算
    - Pixel shader --- 逐像素计算
* 通常着色模型的高亮部分是非线性的，因此最好不要在顶点阶段进行计算
* 在片元着色器中计算该片元的观察方向(物体到相机的方向)，使用插值后的物体位置进行计算。也就是说不要在顶点着色器中计算观察方向。
* 着色模型计算使用统一的坐标系。如果场景有多个光源，使用世界坐标系，否则使用相机坐标系
* 如果要实现类似鳞片面状的效果，可以使用几何着色器和顶点着色器，近年来顶点着色器用的更多，使用第一个顶点的属性作为整个面的属性，并禁止插值操作。
* 扩展 Gooch 模型(多光源)

![](Media/Equation_05_19.png)

![](Media/Equation_05_20.png)

![](Media/Equation_05_21.png)

![](Media/Equation_05_22.png)

* 美工一般不直接操作着色器，而是操作材质。材质数据包括材质模板(material template)和材质实例(material instance). UE 的材质系统可以继承与派生。
* 着色器的一致变量可以在运行时设置，也可以在编译时设置。
* 材质系统对着色器功能的划分
    * 几何操作：刚体变换，顶点混合(vertex blending)，变形，tessellation，实例化，裁剪。(都是和网格有关的操作)
    * 片段操作：片段丢弃和片段混合
    * 设计一个着色模型，而后使用不同的着色参数
    * 材质的特色功能各自独立
    * 光照计算。
* 在源码层次上实现上面所有的组合划分，使用字符串连接和替换功能实现，或者使用 #if, #include, #define 宏实现。
* 通过条件预处理在编译时选择不同的代码，或者使用分支控制运行不同选项的代码，注意分支选择会产生大量的寄存器，引起性能下降。
* 大量着色器的解决策略
    - 代码重用：在共享文件内编写函数，用#include包含。
    - 减除不必要的功能： 、通过预处理器和分支控制实现
    - 功能的不同部分定义为节点，作为输入输出连接器组合起来，可用文本或可视化编辑器实现。
    - 常见着色器模型参数的计算和着色器模型的计算分离开来。
* 《WebGL Insight》介绍了不同引擎如何控制它们的着色器管线。
* 优化： 找出不同更新频率的变量，并将这些变量按照更新频率分组。再合适的时候按组更新这些变量。

# 4 失真和反失真
* 从采样的信号重构称之为过滤(filtering)
* Nyquist rate/Nyquist limit： 要重构原先的信号，采样频率要大于被采样信号频率的两倍。
* 过滤
    - box filter: 最近邻居
    - tent filter: 线性插值
    - low pass filter: 低通道过滤，移除高频部分。  理想的低通道过滤器为 sinc filter

![](Media/Equation_05_23.png)

* 重采样，重构得到连续信号后，重采样以匹配另一个大小。
* 信号重采样的间隔更小(放大操作)，信号重采样的间隔更大(缩小操作)。
    - 放大操作：在重构的信号上直接采样
    - 缩小操作：移除掉更多高频部分

* 基于屏幕的反失真(Screen-Based Antialiasing): 在管线的输出样本上进行反失真操作。

![](Media/Equation_05_24.png)

上图中，左图每个屏幕栅格只有一个样本，则该栅格要么是红色，要么是白色。右图每个栅格使用四个样本，则有多个结果。

* 反锯齿策略：使用一个采样模式，而后对样本进行权重求和操作，最后计算出最终的像素颜色值。

![](Media/Equation_05_25.png)

* 一些概念
    - supersampling(oversampling)：每个像素采样总的样本数量大于一个。
    - full-scene antialiasing(FSAA),也称之为 supersampling antialiasing(SSAA): 以更高的分辨率渲染场景，而后使用相邻采样创建一张图像。
* NVIDIA 的 dynamic super resolution 是 supersampling 更精巧的形式。使用一个更高的分辨率渲染场景，而后使用一个 13-sample Gaussian filter 生成显示的图像。
* 使用 accumulation buffer 实现 supersampling, 沿着x,y轴移动0.5个像素生成四张图象。OpenGL 3.0 支持该功能。在片段着色器中，通过使用更高精度的颜色格式用于输出 buffer，实现 accumulation buffer.

![](Media/Equation_05_26.png)

上图展示不同的像素采样方案。Quincunx 共享角落的样本，同时中心的样本权重为0.5. 2x2 rotated grid 在近水平的边上表现比 straight 2x2 grid 更好。 8 rooks 也表现比 4x4 grid 好， 虽然使用更少的样本。

* Multisampling antialiasing(MSAA)：每个像素只着色一次。如果MSAA所有位置上的样本都被片段所覆盖，则这些样本的着色使用像素的中心位置的内容去计算。否则计算样本所在位置的像素内容。MSAA 最重要的一点是样本会共享计算的结果。例如下图，三个红色覆盖的样本，共享像素中间位置计算的结果. 四个样本里存储的是索引值。NVIDIA 2006年引入了 coverage sampling antialiasing(CSAA)， AMD 则使用了 enhanced quality antialiasing(EQAA), 这些技术在更高的采样率下仅存储片段的覆盖率。

![](Media/Equation_05_27.png)

* 在 multiple-sample buffer 内渲染完所有的几何体后，执行 resolve 操作，其平均所有的样本颜色，生成像素最终的颜色。
* * custom filter antialiasing(CFAA): 能够使用 narrow 和 wide tent filter 
* 近水平，近垂直，45度斜线都是容易产生锯齿的地方。
* Rotated grid supersampling(RGSS) 使用一个旋转的四边形模式. 处理近水平或近垂直的边效果较好。
* N-rooks 采样， NXN个子网格， 每行每列最多一个样本。 其模式很重要， 如下图左图效果就很差。所以设置采样模式时避免将两个样本靠在一起

![](Media/Figure_05_01.png)

* 如果场景有一些微小的物体，使用常量间隔采样容易产生莫尔条纹和其他干涉图案。
* 每个像素使用随机的采样方式可以有效地减少失真。
* FLIPQUAD模式最初是为移动图形开发的，结合了Quincunx和RGSS。 它的优点是成本仅为每个像素两个样本，并且质量类似于RGSS（每个像素四个样本）。 此采样模式如下图所示

![](Media/Figure_05_04.png)

* morphological antialiasing(MLAA): 形态抗锯齿。“形态”表示和结构或形状有关的内容。在后期处理中执行。依赖于其他缓冲区(深度和法线）。subpixel antialiasing(SRAA)仅适用于几何边缘处的抗锯齿。 geometry buffer antialiasing(GBAA),distance-to-edge antialiasing (DEAA) 计算三角形边缘位于何处的附加信息，例如边缘距离像素中心多远.
* 使用颜色缓冲区的方案，directionally localized antialiasing(DLAA): 接近垂直的边水平模糊，接近水平的边与其邻居垂直模糊。

![](Media/Figure_05_05.png)

    上图演示了MLAA， 左图为失真图像，中间的图演示通过相邻像素得到可能的边。使用猜测得到的最好的边混合相邻像素计算出自身像素的颜色值。

* 基于图像的抗锯齿，比基于样本的抗锯齿计算的结果更好。对于基于样本的技术，如果使用四个样本，则最多五种可能性，分别为没有样本，一个样本，两个样本，三个样本，四个样本。基于图像的样本技术也有缺点，例如尖角会变得圆润，文字质量受到影响，还有小于一个像素的物体可能产生间隙，例如电线。此外执行时间和场景内容有关，场景内容复杂则执行时间更长。例如一个草场的抗锯齿是天空的三倍。

## 5 透明，Alpha, 合成
* 半透明物体的渲染，主要分为两类，分别为基于光学物理的效果和基于视觉的效果。本章节主要讲述基于视觉的效果。
* Screen-door transparent: 使用像素对齐的棋盘填充图案实现。交错填充像素。该方法缺点是不能同时有多个透明物体在不透明物体之上。例如红色透明物体和绿色透明物体在蓝色不透明物体之上只能看到两种颜色。同时只能使用 50% 的棋盘模式。该方法的优点是可以任意顺序绘制物体。
* 像素的 alpha 值还可以表示覆盖率。
* 混合顺序 over operator

![](Media/Equation_05_28.png)

* over 操作模拟透过有色玻璃和塑料观看场景的效果较差。
* additive blending， 像素值被简单的累加起来。实现类似闪电或火花的发光效果，不会使之后的像素内容衰减。

![](Media/Equation_05_29.png)

* 混合之前根据相机到物体中心的距离对物体进行排序,渲染透明物体时关闭z-buffer区的写入，只允许 z-buffer 区的测试，这样做仍会产生一些渲染不正确的问题。

![](Media/Figure_05_06.png)

* 左图使用 z-buffer 渲染透明物体。其中部分物体渲染错误。右图使用 deep peeling 技术渲染得到正确的渲染效果。
* under operator, 注意 under 操作要求保持 alpha 值。under 操作混合所有的透明物体至一个单独的颜色缓存，而后使用 over 混合不透明的物体。

![](Media/Equation_05_30.png)

* order-independent transparency(OIT)算法: depth peeling. 该算法无需进行排序操作。使用两个 z-buffer 和多个 passes 实现。第一个zpass 填充当前次序的z值，第二个zpass则用于进行比较。从前往后的顺序进行混合，使用z-buffer逐层剥除最顶面一层透明物体。其间使用 under 操作进行混合。其中何时结束剥除操作，可以采用某一个pass中绘制的像素少于一定数量来判定。
* deep peeling 的一个变种，从后向前剥除，这样可以不需要额外的alpha通道，缺点时可以能会遗失最近透明物体的一些细节。还可以一个pass同时进行最近和最远剥除，这样可以减少一半的渲染pass数量。
* A-buffer，另一种形式的多重采样， 渲染的每个三角形创建一个覆盖率mask。每个像素存储该像素相关片段的列表。不透明的片段可以剔除其下方的片段。仅有不透明物体上方的透明物体片段保存。一旦所有列表建构成功，通过遍历所有片段得到最终的结果。
* 在GPU上创建片段的链接链表，已可在DX11的新功能上实现。该功能利用了 unordered access view(UAVs)和原子操作。在颜色和深度之外，每个像素创建一个指针结构，将该像素上的片段链接起来，而后一个单独pass开始绘制填充整个屏幕的四边形，每个像素调用片段着色器处理这些链接的片段。而后混合这些片段得到最终的颜色。
* A-buffer的缺点是不知道所需的存储量。

![](Media/Figure_05_07.png)

左上角，从后向前alpha混合产生的错误内容，右上角，使用A-buffer实现的效果，右下角，使用代价更廉价的 multi-layer alpha blending。左下角为 A-buffer 和 multi-layer alpha blending 的图像对比结果。

* multi-layer alpha blending: 利用了英特尔称之为 pixel synchronization 的功能。通过重新改变存储和混合，当内存耗尽时，可以平滑的降低质量。DX11.3 引入了 rasterizer order views，移动设备类似的技术为 tile local storage.
* multi-layer alpha blending 方法建立在 k-buffer 这个思路之上。前几个可见层尽可能排序和保存起来，底下的几层通过 weighted average(有权平均)尽可能丢弃和合并。weighted sum和weighted average透明技术都时不依赖于顺序的。该方法的缺点时无法考虑透明物体之间的顺序，例如透明红色围巾在透明蓝色围巾上方时可能产生一个紫色的颜色，事实上应该看到红色的围巾带有一点蓝色的色调
* weighted sum(有权累加)透明的公式如下, 注意公式中的两个累加部分其值可能会超过1，从而产生错误的结果。

![](Media/Equation_05_31.png)

* weighted average(有权平均)公式如下：

![](Media/Equation_05_32.png)

* 添加距离对透明物体的影响，越近的物体影响越大，因此可以看出物体的远近。

![](Media/Figure_05_08.png)
  
  上图给物体演示了距离对透明权重的影响。
- 根据距离添加权重时，在巨大的场景下，彼此靠近的透明物体混合的结果和有权平均的结果类似。此外，当相机位置发生变化时，根据距离设置的权重也会产生变化。
* 增加颜色传播效果，之前的透明算法都是将透明度看成覆盖率。这里可以使用透明的颜色过滤不透明的物体，在片段着色器中，使用透明颜色乘以不透明的物体内容，而后这些生成的内容存储到第三个缓冲区内，当解析透明缓冲区时，使用该缓冲区代替不透明场景。
* 关于透明的论文[1931]，调查[1141]，演讲[1182]
* 预乘 alpha 值的混合操作， 即RGB的值都已经乘以了 alpha 值

![](Media/Equation_05_33.png)

## 6 显示编码
* 伽马修正，着色器的输出颜色会使用(1/2.2)的幂提升，对于输入的纹理和颜色则使用相反的操作。这是由于输入电压和显示的辐射率之间有指数关系。
* sRGB 的 display transfer function, 其中 x 为线性RGB三元组

![](Media/Equation_05_34.png)

可近似为下面的公司，其中指数为 1.0/2.2

![](Media/Equation_05_35.png)

从非线性值转换线性值

![](Media/Equation_05_36.png)

比较简单的形式如下

![](Media/Equation_05_37.png)

更简单的形式如下，用于浏览器和移动应用

![](Media/Equation_05_38.png)

* 如果不采用伽马修正，则可能产生不正确的结果，例如图像看起来偏暗，因为辐射量偏低，同时还可能造成反锯齿的结果不正确。