* 一些辅助函数
    - randomValue
    - randomVector
    - randomMatrix
# OSGCh08Ex01
* 合并几何数据
* osg::Geometry::BIND_PER_PRIMITIVE_SET 设置数组每个图元读取一个数组, 比如颜色数组, 法线数组.
* 调用每个显示列表其实都是需要花费一定代价的. 每个几何体如果是静止物体的话, 都是一个显示列表渲染的结果. 因此, 尽可能将物体合并成一个几何体减少渲染的代价.
* 当然, 如果物体使用不同的纹理渲染, 则不可能使用一个几何体渲染所有的内容. 这次我们可以用四叉树或八叉树来分割场景.

# OSGCh08Ex02
* 压缩纹理
* Process Explorer 工具. 可以即时看到 CPU 和 GPU 使用率, View->System Information 菜单, 对话框选择 GPU 标签.
* osg::Image::data() 可以访问其数据
* 内部格式: osg::Texture2D::USE_S3TC_DXT1_COMPRESSION
```
texture->setInternalFormatMode(osg::Texture2D::USE_S3TC_DXT1_COMPRESSION );
texture->setUnRefImageDataAfterApply( true );
```
* 其他支持的格式
    - USE_IMAGE_DATA_FORMAT
    - USE_USER_DEFINED_FORMAT --- 开发者通过调用 setInternalFormat() 决定支持的纹理格式
    - USE_ARB_COMPRESSION --- ARB_texture_compression
    - USE_S3TC_DXT1_COMPRESSION ---  S3TC DXT1
    - USE_S3TC_DXT3_COMPRESSION ---  S3TC DXT3
    - USE_S3TC_DXT5_COMPRESSION ---  S3TC DXT5
    - USE_PVRTC_2BPP_COMPRESSION --- GLES 的压缩类型.
    - USE_PVRTC_4BPP_COMPRESSION --- GLES 的压缩类型
    - USE_ETC_COMPRESSION --- GLES 的压缩类型
    - USE_RGTC1_COMPRESSION --- OpenGL2.0 的压缩类型
    - USE_RGTC2_COMPRESSION --- OpenGL2.0 的压缩类型
* 更好的做法是渲染之前就预先压缩. 例如 NVTT, 可以生成 .DDS 图像, 该格式图像使用压缩的格式.
* setUnrefImageDataAfterApply() --- 可以强制在图像编译至GPU内后释放 osg::Image 对象.

# OSGCh08Ex03
* 共享场景的对象
* osgDB::SharedStateManager 可以自动收集与共享 paged nodes(分页节点)的纹理对象.
* 使用一个共享列表管理从硬盘文件读取的节点. 通过比较存储在列表中的文件名实现 cache. 如果列表中有个节点不再被其他节点或场景对象引用, 则将其从系统内存中移除出去.
* 使用 osgDB::ReadFileCallback 的派生类代替默认的实现.
    - 如果文件不存在, 则使用默认的实现流程
```
    osgDB::ReaderWriter::ReadResult rr = 
    osgDB::Registry::instance()->readNodeImplementation(filename, options );
```
    - 判断引用计数是否小于或等于1, 如果是的, 则删除掉该节点.
* osgGA::GUIEventAdapter::getTime() 可以获取时间(在osgGA::GUIEventHandler的派生类可以用到)
    - 处理 osgGA::GUIEventAdapter::FRAME 事件
* 替换默认的读取
```
osg::ref_ptr<ReadAndShareCallback> sharer = new ReadAndShareCallback;
osgDB::Registry::instance()->setReadFileCallback( sharer.get() );
```
* osgDB::Options 类的 setObjectCacheHint() 方法设置是否应该由OSG内部的注册机制记录避免重复加载文件.
```
osg::ref_ptr<osgDB::Options> options = new osgDB::Options;
options->setObjectCacheHint( osgDB::Options::CACHE_NODES );
osg::Node* model = osgDB::readNodeFile( "cow.osg", options.get() );
```
* osgDB::readImageFile() 和 CACHE_IMAGES 可以用于 cache 图像.

# OSGCh08Ex04
* 配置一个 database pager
* osgDB::DatabasePager --- 在一个单独的线程里管理外部文件的加载.
* osg::PagedLOD 和 osg::ProxyNode, 两者都使用 database pager 实现内部的功能.
* TextureObjectManager::setMaxTexturePoolSize 设置最大纹理池大小.
```
osg::Texture::getTextureObjectManager(0)->setMaxTexturePoolSize( 64000 );
```
* 每个 viewer 都有自己的 database pager.
```
osgViewer::Viewer viewer;
osgDB::DatabasePager* pager = viewer.getDatabasePager();
pager->setDoPreCompile( true);
pager->setTargetMaximumNumberOfPageLOD( 10 );
```
* 纹理池可以回收孤儿纹理和过期的纹理.
* setDoPreCompile --- 避免渲染的时候才编译. 如果我们有很多的对象需要同时编译, 则最好开启这个方法.
* 其他可用的方法: 更平衡的场景图, 压缩和预先mipmapped的纹理, 一致的纹理大小都有助于优化性能.
* setTargetMaximumNumberOfPageLOD() 可以设置 paged LOD 的加载过程.
    - 受 osgDB::DatabasePager*::setTargetMaximumNumberOfPageLOD(10) 的影响

# OSGCh08Ex05
* 设计一个简单的剔除策略(culling strategy)
* 讲一个场景对象发送至渲染管线之前判断其是否在相机视线范围之内
* 从 osgGA::FirstPersonManipulator 派生类用于相机操作.
  * getMatrix() 获取矩阵
    - getTrans() 位置
  * setByMatrix 更新矩阵
* 设置节点自己的剔除回调, 从 public osg::NodeCallback 派生.
    - 重新实现 operator() 方法, 相机位置由 cull visitor(nv) 提供
        - osg::NodeVisitor::getEyePoint()
        - 判断节点是否在可见区域内.
    - 通过设置该操作不调用 traverse() 方法实现剔除