# OSG Cookbook 笔记
----------------------------------------------------
[通用详细内容](/common/README.md)
* Common
	- 创建关于 HUD 的相机
	- 创建文本
* PickHandler
	- osgGA::GUIEventHandler 派生事件处理器 addEventHandler

[第2章详细内容 - 设计场景图](/OSGCh02/README.md)
* OSGCh02Ex01 左键点击删除可绘制对象
* OSGCh02Ex02 演示两种拷贝效果(浅拷贝深拷贝)
* OSGCh02Ex03 演示一个围绕盒如何随物体运动而变化
* OSGCh02Ex04 动画路径实现轮子滚动
* OSGCh02Ex05 镜像和ClipNode
* OSGCh02Ex06 广度优先访问遍历(BFS)
* OSGCh02Ex07 绘制背景图像
* OSGCh02Ex08 始终朝向屏幕
* OSGCh02Ex10 指南针

[第3章详细内容 - 编辑几何模型](/OSGCh03/README.md)
* OSGCh03Ex01 分形化一个凹多边形
* OSGCh03Ex02 从一个二维多边形推拉出一个三维立方体
* OSGCh03Ex03 从 osg::Drawable 派生, 实现 opengl 自己的绘制工作.
* OSGCh03Ex04 绘制一个时钟
* OSGCh03Ex05 绘制一个缎带特效
* OSGCh03Ex06 高亮选中模型
* OSGCh03Ex07 选择模型的一个三角面
* OSGCh03Ex08 选中模型的一个顶点
* OSGCh03Ex09 Vertex displacement mapping
* OSGCh03Ex10 Geometry Instancing

[第4章详细内容 - 操作视图](/OSGCh04/README.md)
* OSGCh04Ex01 多显示器多相机
* OSGCh04Ex02 多个从属相机实现威力墙(多个视图显示一个场景)
* OSGCh04Ex03 使用 osgViewer 的深度划分显示巨大的场景
* OSGCh04Ex04 创建雷达小地图(用相机的CullMask实现)
* OSGCh04Ex05 显示模型的前后侧面和顶视图
* OSGCh04Ex06 对场景的前顶侧面视图进行操作
* OSGCh04Ex07 相机跟随物体运动(在帧事件中实现跟踪)
* OSGCh04Ex08 使用相机操作器跟踪节点
* OSGCh04Ex09 设计二维的相机操作器

[第5章详细内容 - 动画](/OSGCh05/README.md)
* OSGCh05Ex01 用 osgAnimation 库实现开关门动画
* OSGCh05Ex02 在三维场景中播放视频
* OSGCh05Ex03 滚动的文字, 通过Ease Motion实现
* OSGCh05Ex04 变形几何体(osgAnimation::MorphGeometry)
* OSGCh05Ex05 实现物体的淡出淡入
* OSGCh05Ex06 模拟飞行器着火的效果
* OSGCh05Ex07 使用着色器实现光照
* OSGCh05Ex08 创建一个战机游戏
* OSGCh05Ex09 创建一个骨骼动画
* OSGCh05Ex10 给骨骼蒙皮
* OSGCh05Ex11 整合物理引擎

[第6章详细内容 - 特效](/OSGCh06/README.md)
* OSGCh06Ex01 实现 bump mapping 技术
* OSGCh06Ex02 独立于视图的阴影
* OSGCh06Ex03 osgFX 实现多 pass 渲染技术
* OSGCh06Ex04 读取和显示 depth buffer 的内容
* OSGCh06Ex05 实现夜视效果
* OSGCh06Ex06 RTT实现景深效果
* OSGCh06Ex07 立方体映射实现天空盒
* OSGCh06Ex08 实现水特效
* OSGCh06Ex09 实现云层

[第7章详细内容 - 浏览世界](/OSGCh07/README.md)
* OSGCh07Ex01 构建 VPB
* OSGCh07Ex02 使用两个bmp文件生成一个小的地形文件
* OSGCh07Ex03 生成地球上的地形数据
* OSGCh07Ex04 使用多个图像和高度数据
* OSGCh07Ex05 在已有的地形数据库上更新数据
* OSGCh07Ex06 NVIDA Texture Tools(NVTT), 开源的图像处理和纹理操作项目
* OSGCh07Ex07 使用 SSH 实现群集生成
* OSGCh07Ex08 从网络下载和渲染地形

[第8章详细内容 - 管理数据](/OSGCh08/README.md)
* OSGCh08Ex01 合并几何体
* OSGCh08Ex02 压缩纹理

[第9章详细内容 - 集成GUI](/OSGCh09/README.md)
