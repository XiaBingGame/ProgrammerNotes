# OSGCh10Ex06
* 实现截图功能
* 要点为当图形上下文为当前上下文时, osg::Camera::DrawCallback 内实现截图.
* 实现 osg::Camera::DrawCallback 的 operator() 方法. 利用 metadata 实现
```
if ( _image.valid() && _image->getUserValue("Capture", capturing) )
{
  osg::GraphicsContext* gc = renderInfo.getState()->getGraphicsContext();
  if ( capturing && gc->getTraits() )
  {
    int width = gc->getTraits()->width;
    int height = gc->getTraits()->height;
    GLenum pixelFormat = (gc->getTraits()->alpha ? GL_RGBA : GL_RGB);
    _image->readPixels( 0, 0, width, height, pixelFormat, GL_UNSIGNED_BYTE );
    std::stringstream ss; ss << "Image_" << (++_fileIndex) << ".bmp";
    if ( osgDB::writeImageFile(*_image, ss.str()) && _text.valid() )
    	_text->setText( std::string("Saved to ") + ss.str() );
  }
  _image->setUserValue( "Capture", false );
}
```
* osgGA::GUIEventHandler 派生类实现触发截图
```
class PhotoHandler : public osgGA::GUIEventHandler
{
public:
  PhotoHandler( osg::Image* img ) : _image(img) {}
  virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
  {
    if ( _image.valid() && ea.getEventType() == osgGA::GUIEventAdapter::KEYUP )
    {
      if ( ea.getKey()=='p' || ea.getKey()=='P' )
      	_image->setUserValue( "Capture", true );
    }
    return false;
  }
protected:
  osg::ref_ptr<osg::Image> _image;
};
```
	- osg::Image::readPixels() --- 可以实现截屏
* 本例有意思的地方在于截图并不会截取 HUD 相机内的文字部分, 这是因为 HUD 相机是"post-drawing"相机, 其在父相机的所有子节点绘制完后再进行绘制, 所以在父相机的post-draw回调之后才绘制 HUD 部分.
* 相机回调的调用
	- 执行 getInitialDrawCallback() 中的初始回调
	- 遍历和渲染标识为"pre-render"的子相机
	- 执行 getPreDrawCallback() 内包含的前期绘制回调
	- 遍历和渲染子节点(相机除外)
	- 执行 getPostDrawCallback() 内包含的后期绘制回调
	- 遍历和渲染标识为"posg-render"的子相机
	- 执行 getFinalDrawCallback() 内的最终绘制回调
* osgposter 例子使用多个带有 FRAME_BUFFER_OBJECT 参数的相机截屏.
* osgautocapture 例子如何离线模式截图(无窗口显示)
* osgViewer::ScreenCaptureHandler 类使用 Pixel Buffer 截屏.

# OSGCh10Ex13