* OSG 典型的动画类型有路径动画, 纹理动画, 变形动画, 状态动画, 粒子动画, 骨骼动画.
* osgAnimation 库提供了一个完整的框架用于处理不同类型的动画.
* osgParticle 库提供框架设计和渲染粒子

# OSGCh05Ex01
* 门和入口在一些空间索引算法中很重要, 例如 PVS(潜在可见集-potentially visible set).
* 动画系统中的重要内容
    - 管理器回调(osgAnimation::BasicAnimatioinManager 或者其他) 管理动画数据, 其包括不同类型的 channels 和 keyframes. 其必须设置在根节点上, 这样才能处理所有子节点的动画.
    - updaters(osgAnimation::UpdateMatrixTransform及其它)必须设置为动画节点的回调. updaters 可以与一个channel 或多个 channel 连接作为动画目标. 它的 stacked 元素, 表示不同的动画关键类型, 必须与 channel 相匹配.
    - osg 的 channel 可看成关键帧数据和插值器的轨迹. stacked elements 看成关键过滤器, 定义可动的关键类型(位置, 旋转等等)
* 本例的主要思路
    - 创建 channel, channel 主要是有关键帧容器和插值器组成, 设置好其相应的关键帧
    - 创建动画 osgAnimation::Animation, 包含相应 channel, 播放动画的时候则更新 channel 的值
    - 创建动画的更新矩阵变换对象, OsgAnimation::UpdateMatrixTransform, 其根据频道的名称添加相应的旋转或者平移 StackedElement, 其会根据频道的值进行旋转或者平移
    - 创建 BasicAnimationManager, 这是一个回调, 主要用来保管多个动画, 并播放动画
* osgAnimation 目前支持五种类型的 stacked 元素, 全都是可变换的元素.
    - StackedMatrixElement, MatrixLinearChannel, osg::Matrix
    - StackedQuaternionElement, QuatSphericalLinearChannel, osg::Quat
    - StackedRotateAxisElement, FloatLinearChannel, float(沿着特定轴)
    - StackedScaleElement, Vec3LinearChannel, osg::Vec3
    - StackedTranslateElement, Vec3LinearChannel, osg::Vec3

# OSGCh05Ex02
* 在3D世界内播放视频
* 两种方法显示连续的图像, 一种从硬盘读取图像序列, 一种是从摄像头读取帧. 后者 osgdb_ffmpeg 插件会做底层的工作.
* 使用 osg::ImageStream 播放图像流, 其为 osg::Image 的派生类, 主要用于动态图像
* 同样可以读视频文件的插件 
    - osgdb_directshow: (需要DirectX SDK的 DirectShow http://msdn.microsoft.com/en-us/directx)
    - osgdb_gif: 读取静止和动态的GIF图像. 需要 GifLib 库(http://sourceforge.net/projects/giflib/) 
    - osgdb_quicktime: 读取 Apple Quicktime 支持的格式. 要求 QuickTime SDK (http://developer.apple.com/quicktime/) 
    - osgdb_QTKit: 读取 Apple Quicktime 支持的格式, 要求 QTKit 框架 (http://developer.apple.com/quicktime/) 
* osgART 库. 提供一系列API 实现 OSG 的 AR 功能. 以自己的方式渲染摄像头设备的动态图像.

# OSGCh05Ex03
* 本例演示一行在屏幕上连续滚动的文本.
* HTML 的 marquee 标签.
* osgAnimation::LinearMotion 获取插值后的数值
    - update() 更新, getValue() 获取其值
* osg::Drawable::UpdateCallback 更新 osgText::Text
* osgText::Text: setPosition() 设置位置, setText() 设置内容
* std::setprecision 设置精度, std::setw 设置宽度, std::setfill 设置填充内容.
* osgAnimation::LinearMotion 类, 属于 EaseMotion 头文件. Easing 表示速度中的变化. 在 osgAnimation 的实现中, ease motion 表示运动和静止状态之间的过渡.
* 一个物体启动并加速, 称之为"in"动作, 一个物体减速并停止, 则称之为"out"动作. 我们将"一半对一半"的比例混合它们, 则称之为"in-out" motion. "in-out-cubic" motion 在启动和结束时使用三次方方程计算速度.
* Linear motion 其运动时速度不会发生变化, 瞬间启动和瞬间停止.
* OSG 包含了以下 motion, 都有 'in', 'out', 'in-out' 形式, QuadMotion, CubicMotion, SineMotion, BackMotion, CircMotion, ExpoMotion 等.
* 对于 ease motion, 可见文章 http://www.robertpenner.com/easing

# OSGCh05Ex04
* 演示变形几何体
* osgAnimation::MorphGeometry, 其可以在实时环境中产生不错的效果.
* 本例演示了函数指针的做法
* 这里使用 0.0 到 1.0 变化的动画频道, 然后动画添加该频道, 使用 BasicAnimationManager 播放该动画
* osgAnimation::MorphGeometry 添加源几何体, 目标几何体
* 使用 osgAnimation::UpdateMorph 更新几何体
* 原理为源和目标必须为 osg::Geometry 对象, 且有相同的顶点数量进行变形
* 主要通过 osgAnimation::MorphGeometry 和 osgAnimation::UpdateMorph(主要用于连接动画频道)实现
* 例子有问题, 包括源码并没起作用, 不知道是哪里的问题
* 可见官方例子 osganimationmorph

# OSGCh05Ex05
* 物体逐渐消失或出现, 通过透明的程度来实现
* 使用 osgAnimation::InOutCubicMotion 进行插值计算
* osgUtil::CullVisitor Cull 回调时, NodeVisitor 可以转换成该访问器.
    - osg::Node 调用 addCullCallback 添加该类作为回调就可以转换
* osgUtil::CullVisitor::getDistanceFromEyePoint 可以获取一个节点的视觉空间距离
* 只有 cull visitor 可以获取模型和视图矩阵.

# OSGCh05Ex06
* 主要演示粒子系统
* emitter 添加在一个变换矩阵节点下, 设置其位置, 所以粒子在哪里出现和 emitter 位于哪里有关系
* osgParticle::ParticleSystem 则只是一个 drawable, 但需要将其添加至场景才会看到粒子系统.
* osgParticle::ParticleSystemUpdater 是一个 osg::Node 主要更新粒子系统
* osgParticle 框架, emitter 控制粒子的出生, program 控制粒子出生后的行为, updater 管理多个系统并更新它们. emitter , program, updater 都是场景节点.
* 本例模拟 cessnafire.osg 文件. 可以记事本打开研究其内的结构

# OSGCh05Ex07
* 使用着色器实现光照
* osg::NodeVisitor::getFrameStamp() 可以得到 osg::FrameStamp
* osg::Uniform::Callback 可以用于更新 Uniform 变量
* GLSL 可以使用数组 gl_LightSource[]

# OSGCh05Ex08
* 实现二维飞机游戏
* osg::Matrix::getTrans() --- 可以得到平移部分
* 主要设置子弹发射, 并判断子弹是否击中对方, 自动移除实体

# OSGCh05Ex09
* 演示骨骼动画
* 所有的骨头都在父骨头的局部坐标系内
* osgAnimation::UpdateBone 回调控制骨头的动画
    - setMatrixInSkeletonSpace --- 设置骨头在父骨头的坐标空间内. 注意该方法是设置骨骼空间内, 如果你想要设置在父骨头的空间内, 你需要转换偏差矩阵至骨骼空间.
```
bone->setMatrixInSkeletonSpace(
  osg::Matrix::translate(trans) *
    bone->getMatrixInSkeletonSpace() );
```
* 确保骨头首先被遍历和更新, 起码保证在任何其他几何体绘制之前
```
osg::ref_ptr<osgAnimation::Bone> bone = new osgAnimation::Bone;
parent->insertChild( 0, bone.get() );
parent->addChild( createBoneShape(trans, osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)) );
```
* channel 的目标名称对应一个 osgAnimation::UpdateMatrixTransform, 其会寻找和其关联的 channel.
* 注意每个 bone 旋转的是另一端, 一端不动, 另一端旋转, 所以下级的 bone 会相应旋转

# OSGCh05Ex10
* 给骨骼蒙皮
* 具体的使用还是要研究例子
* 蒙皮简单的说就是一个顶点受到多个bone的影响: http://tech-artists.org/wiki/Vertex_Skinning
* 使用 osgAnimation::RigGeometry 记录 bone 和顶点的关系.
* osgAnimation::VertexInfluenceMap 设置顶点影响映射表
* osg 提供了两个例子解释骨骼动画的使用 osganimationskinning 展示了如何构建一个简单的骨骼并进行蒙皮. osganimationhardware 则展示如何改变用于osgAnimation::RigGeometry 的变换技术
* 记住, 不要通过代码构建一个完整的人类模型和骨骼动画, 这是一个很沉重的工作. 应当用常见的标准格式或者建模软件来实现, 例如 DAE 和 FBX 格式
* OSG 支持 Collada DAE 和 Autodesk FBX 格式

# OSGCh05Ex11
* 使用 PhysX 物理库
* 主要是根据ID建立一一的对应关系
* 通过更新物理引擎来更新世界模型的相关矩阵
* 将物理计算和渲染分开
* 其他的物理引擎选择 ODE (http://www.ode.org/), Bullet Physics (http://bulletphysics.org/), Newton (http://newtondynamics.com/)
* 使用的时候要分开物理对象和渲染对象, 避免混淆