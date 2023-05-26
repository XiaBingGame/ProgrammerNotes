# OSGCh06Ex01
* 实现 Bump mapping 技术
* 在每个顶点增加两个属性切线向量和副切线向量.
* osg::Program::addBindAttribLocation 给顶点属性位置
* osgUtil::TangentSpaceGenerator 可以给一个几何体添加切线和副法线数组.
    - osgUtil::TangentSpaceGenerator::generate() --- 生成
    - osgUtil::TangentSpaceGenerator::getTangentArray()
    - osgUtil::TangentSpaceGenerator::getBinormalArray()
* osg::Geometry::setVertexAttribArray --- 设置定点属性数组.
* 使用位置 1, 6, 7 用于自己的数组.
* 本例使用法线, 副法线, 切线建立切线空间矩阵, 这样视觉空间的光线方向乘以该矩阵进入世界坐标系空间内.
* bump 纹理生成凹凸样式.
* 注意学习如何给顶点添加属性数组.
* 默认索引: 0 位置, 2 法线, 3 颜色, 4 辅助颜色, 5 雾坐标, 8-15 纹理坐标

# OSGCh06Ex02
* 独立于视图的阴影, 依赖于视图椎体, 资源耗费小. 尤其是当场景数据和相机动态变化时效果明显.
* 设置接收和投射 mask
* osg::Node::setNodeMask
* osgShadow::ViewDependentShadowMap
* osgShadow::ShadowedScene
    - setShadowTechnique
    - setReceivesShadowTraversalMask
    - setCastsShadowTraversalMask
    - addChild
* osgGA::TrackballManipulator
    - setHomePosition --- 设置其 home 位置.

# OSGCh06Ex03
* osgFX 库提供了一个框架可用于多 pass 渲染. 所有希望添加多 pass 渲染功能的子场景图都应加载 osgFX::Effect 节点下.
* osgFX::Scribe / osgFX::Outline 技术
* 本例实现一个多 pass 透明技术, 消除复杂透明物体的绘制错误. 这里整个物体都是透明的, 只有最靠近相机的片段被绘制了一边并和背景进行透明混合.
* 从 osgFX::Technique 派生一个技术类. validate() 方法用于验证当前硬件是否支持这个技术.
* 这里创建了两个 pass, 第一个 pass 只写入深度值. 第二个 pass 表示等于该深度值的片段可以写入颜色.
* public osgFX::Effect 派生一个特效节点.
    - define_techniques 添加一个技术.
* 物体逐渐消失可以使用该技术, 这样不会产生多重透明混合的效果.

# OSGCh06Ex04
* 读取和显示 depth buffer 的内容.
* 渲染目标有纹理, 图像, frame buffer object(FBO)
* 本例的思路
    - 将原始场景渲染至纹理.
    - 创建一个四边形填充整个正在渲染的窗口, 将纹理映射至该四边形.
    - 四边形上可以使用着色器实现自己想要的效果
    - 你可以在其上增加你既更多的效果. 仅仅需要创建另一个四边形包含之前渲染的结果.
    - 使用 HUD 相机渲染包含最终渲染纹理的四边形. 并显示
* 本例 RTT 相机继承了主相机的视图矩阵和投影矩阵(没有用绝对坐标系). 所以加载相同子场景图时显示相同的内容.
* 本例禁止自动的远近截面距离计算.
    - osg::Camera::setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
* 除了 FRAME_BUFFER_OBJECT 还有其他的渲染目标方法:
    - FRAME_BUFFER, 使用 OpenGL 的 glCopySubImage() 函数, 其工作于早期不支持 FBO 或 PBO 的设备.
    - PIXEL_BUFFER, 使用 OpenGL 的 Pixel Buffer Object(PBO) 扩展实现渲染至纹理的功能
    - PIXEL_BUFFER_RTT, 使用 WGL_ARB_render_texture 扩展实现渲染至纹理的功能
    - SEPERATE_WINDOW, 使用一个单独的窗口显示 buffer 中的内容, 有时用来调试非常有用.

# OSGCh06Ex05
* 实现夜视视觉效果(night vision), 显示非常低光的状况. 常用于军事目标. 使用大镜头收集和聚焦光. 如夜视护目镜.
* Light Interence Filters(LIF) 光干涉过滤的反射会导致绿色镜头, 噪值, 模糊. 本例使用 noise map, 加强绿色成分实现该效果.
* 使用噪值纹理实现噪声效果.
* 使用 RTT 实现夜视效果.
* 如果 RTT 相机设置为 RELATIVE_RF, 其会继承主相机的视图和投影矩阵.
* 如果有多个 pass, 其他的 RTT 则设置 ABSOLUTE_RF.

# OSGCh06Ex06
* 实现 depth of field(DOF) 景深效果.
* 模糊着色器, 读取之前的颜色纹理, 累积相邻的纹素
* 组合原始的颜色纹理以及模糊的纹理, 根据每个像素的深度值进行混合, 近处的物体使用原始纹理, 远处的物体使用模糊纹理.
* 根据场景节点创建颜色输入和深度输入 RTT 相机
* 使用输入的纹理创建创建一个模糊 pass.
    - 如果使用独立相机系统, 则会设置一个正交投影矩阵的, 并在其子节点放置一个相应大小的四边形.
* 一共有四个 pass. 读取颜色和深度buffer, 水平和垂直颜色.
* 注意禁止远近截平面距离的计算.
* 通过深度值计算距离的默认公式
```
// Znear = 1.0 & Zfar = 10000.0
z = 2.0 * (10000 + 1.0) / (10000 + 1.0) - z *
    (10000 - 1.0)) - 1.0
  = 2.0 * 10001.0 / 10001.0 - z * 9999.0) - 1.0
```
* 考虑多渲染目标用于优化的解决方案. 见例子 osgmultiplerendertargets.
* 一些关于后期处理效果的材料:
    - 高斯模糊: http://www.gamerendering.com/2008/10/11/gaussian-blur-filter-shader/
    - 运动模糊: http://http.developer.nvidia.com/GPUGems3/gpugems3_ch27.html
    - Bloom: http://en.wikipedia.org/wiki/Bloom_(shader_effect)
    - 高动态范围(HDR): http://transporter-game.googlecode.com/files/HDRRenderingInOpenGL.pdf
    - Screen Space Ambient Occlusion (SSAO): http://en.wikipedia.org/wiki/Screen_Space_Ambient_Occlusion
    - Fast Approximate Anti-Aliasing (FXAA): http://developer.download.nvidia.com/assets/gamedev/files/sdk/11/FXAA_WhitePaper.pdf
* NVIDIA 着色器库: http://developer.download.nvidia.com/shaderlibrary/webpages/shader_library.html

# OSGCh06Ex07
* 立方体映射显示天空盒
* 其他构造天空背景的技术有 sky dome.
* 本例设计了新的变换节点表示 sky. 最重要是的重写了方法 computeLocalToWorldMatrix() 和 computeWorldToLocalMatrix(). 前者在 cull 遍历中常被调用, 用于计算模型矩阵.
* osg::Node::setCullingActive(false) --- 禁止该节点的任何进一步的 cull 操作.
    - 这是因为天空盒随眼睛而动, 其位置与边界很难计算(位置和边界可用于视图锥剔除).
    - osg::StateSet::setRenderBinDetails() --- 设置渲染次序.
* osg::Texture::setResizeNonPowerOfTwoHint --- 当图像大小非2的幂次方, 是否改变其大小为2的幂次方.
* computeLocalToWorldMatrix 方法 --- 主要目的是将代表天空盒的球体移动到相机为中心的位置处.
    - 从 osgUtil::CullVisitor 对象获取相机的位置, 根据该位置设置天空盒的矩阵.
    - osg::NodeVisitor::getVisitorType() 判断访问者类型, 这里判断是否 osg::NodeVisitor::CULL_VISITOR, 因为剔除遍历会调用该函数.
    - 将 osg::NodeVisitor 转换为 osgUtil::CullVisitor
    - osgUtil::CullVisitor::getEyeLocal() 得到相机位置
    - preMult 相机位置的平移矩阵
* computeWorldToLocalMatrix 方法
    - 做上面方法的相反工作.
* 使用球体做背景. 需要为球体几何体计算纹理坐标
```
skybox->getOrCreateStateSet()->setTextureAttributeAndModes(
  0, new osg::TexGen );
```

# OSGCh06Ex08
* 创建一个简单的水效果, 水的仿真包括水波, 涟漪, 反射, 折射, 水沫, 波峰.
* 本例主要实现一个反射场景的水面
* 片段着色器至少要有两个输入纹理: 默认的水纹理, 反射 map, 折射 map, 用于光照的法线 map.
* 研究海洋 http://code.google.com/p/osgocean/.
* 默认的 uniform 变量 osg_FrameTime
* 使用 clip 坐标计算出新的纹理坐标, 这样就无需重新设置相机
* 本例使用噪值纹理简化生成波形的代码(可以构造网格几何数据来仿真波形). 本例将光线方向和视觉方向变换至切线空间进行计算.
* osgOcean 项目

# OSGCh06Ex09
* 仿真云
* 从 osg::Drawable 派生而来, 最重要的方法是实现 computeBound() 和 drawImplementation()
* 每个云都有其位置, 亮度, 颜色值.
* 使用仿函数排序所有云层
* osg::RenderInfo::getState 获取 osg::State
* osg::State::getModelViewMatrix() 可以获取模型视图矩阵
* osg 的矩阵是以行存储的方式存储矩阵, 通过右乘得到变换后的矩阵位置
```
(v0 v1 v2 v3) *
|m00 m01 m02 m03|
|m10 m11 m12 m13|
|m20 m21 m22 m23|
|m30 m31 m32 m33|

nv1 = v0 * m00 + v1 * m10 + v2 * m20 + v3 * m30
nv2 = v0 * m01 + v1 * m11 + v2 * m21 + v3 * m31
nv3 = v0 * m02 + v1 * m12 + v2 * m22 + v3 * m32
```
* 所以本例要算出新的深度值 v0 * matrix(0, 2) + v1 * matrix(1, 2) + v2 * matrix(2, 2) + v3 * matrix(3, 2);
    - 又因为深度值在最后的视觉矩阵中, 越往后深度值越大, 因此本例要将上面的值取反
    - 视觉坐标系 z 轴正值在相机后方. 负值在相机前方.
* 书本的解释是当前云的位置和相机的front(前向)向量的点乘结果作为深度值. 这里的前向向量为相机的方向.
* 用一个矩阵变换一个向量
* osg::BoudingBox::expandby 设置一个围绕盒
* osg::Matrix::transform3x3 通过矩阵变换一个向量
    - 这是左乘矩阵

# OSGCh06Ex10
* 自定义状态属性
* 演示 osg::StateAttribute 派生类的使用.