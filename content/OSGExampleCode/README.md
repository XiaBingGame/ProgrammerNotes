### osgautotransform
* 演示如何使用 osg::AutoTransform
    - osg::AutoTransform::ROTATE_TO_CAMERA
    - osg::AutoTransform::ROTATE_TO_SCREEN
    - 自动缩放, 有最大和最小值
* osgViewer::CompositeViewer 演示添加多个 view


###　osgbillboard
* createSqaure 创建带纹理的四边形
* 两种公告板模式: osg::Billboard::POINT_ROT_EYE / osg::Billboard::AXIAL_ROT


### osgbindlesstext --- TODO
* 替换一些 GLSL 变量
    ftransform --- gl_ModelViewProjectionMatrix * gl_Vertex
* createCube 演示了如何创建一个立方体
* 创建 1000 个实例，通过顶点着色器设置其位置
    prim->setNumInstances(1000);
* 类 BindlessBuffer: Uniform Buffer Object 或 UBO 的包装类
```
    typedef osg::ref_ptr<osg::UniformBufferObject>  UniBufferObjRef;
    typedef osg::ref_ptr<osg::UniformBufferBinding> UniBufferBindingRef;
    typedef osg::ref_ptr<osg::UInt64Array> HandleArrayRef;
    typedef osg::ref_ptr<BindlessBuffer>  BindlessBufferRef;

```
* BufferData::setBufferObject 可将数据绑定至 BufferObject 上
* osg::UniformBufferBinding: Uniform buffer 绑定
* 涉及到底层编译大量纹理和实现UBO的知识，待学习

### osgblenddrawbuffers
* 创建相机，写入四个纹理
camera->attach( osg::Camera::BufferComponent(osg::Camera::COLOR_BUFFER0+i), tex );
* 着色器写入四个输出
```
void main() {
   gl_FragData[0] = gl_Color * vec4(1.0, 1.0, 1.0, 0.7);
   gl_FragData[1] = vec4(0.0, 1.0, 1.0, 0.0);
   gl_FragData[2] = vec4(1.0, 0.0, 1.0, 0.3);
   gl_FragData[3] = vec4(1.0, 1.0, 0.0, 1.0);
}
```
* 只允许第一个绘制buffer允许混合，其他三个输出不进行混合
```
    ss->setAttribute( new osg::Enablei(GL_BLEND, 0) );
    ss->setAttribute( new osg::Disablei(GL_BLEND, 1) );
    ss->setAttribute( new osg::Disablei(GL_BLEND, 2) );
    ss->setAttribute( new osg::Disablei(GL_BLEND, 3) );
```
* osg::BlendFunci 和 osg::ColorMaski 的使用
```
        osg::ref_ptr<osg::BlendFunci> blend0 = new osg::BlendFunci(0, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        osg::ref_ptr<osg::ColorMaski> colormask3 = new osg::ColorMaski(3, false, true, false, true);
        ss->setAttribute( blend0 );
        ss->setAttributeAndModes( colormask3 );
```
* 演示了 HUD 相机的创建


### osgblendequation
* 演示 osg::BlendEquation 的使用
    osg::BlendEquation::FUNC_ADD
    osg::BlendEquation::FUNC_SUBTRACT
    osg::BlendEquation::FUNC_REVERSE_SUBTRACT
    osg::BlendEquation::RGBA_MIN
    osg::BlendEquation::RGBA_MAX
    osg::BlendEquation::ALPHA_MIN
    osg::BlendEquation::ALPHA_MAX
    osg::BlendEquation::LOGIC_OP

### osgcallback 回调
* 实现自己的读取文件回调，可以捕获所有的读取文件请求
osgDB::Registry::instance()->setReadFileCallback(new MyReadFileCallback());
* 从 osgDB::Registry::ReadFileCallback 派生实现自定义的文件回调， 内部调用 osgDB::Registry::instance()->readNodeImplementation(fileName,options);
* 相机有更新回调和事件回调
* InsertCallbacksVisitor 给节点插入回调

### osgcamera 相机
* 相机测试
singleWindowMultipleCameras() 单窗口多相机
    1. 没有主相机
    2. 创建 gc, 多个 slave 相机共享， 但视口不同
multipleWindowMultipleCameras() 
    1. 多窗口， 多相机
    2. 添加 slave 相机要有偏移实现屏幕拼接效果
```
viewer.addSlave(camera.get(), osg::Matrix::scale(aspectRatioScale, 1.0, 1.0)*osg::Matrix::translate(translate_x, 0.0, 0.0), osg::Matrix() );
```
* osgDB::readRefObjectFile/osgDB::writeObjectFile 的使用