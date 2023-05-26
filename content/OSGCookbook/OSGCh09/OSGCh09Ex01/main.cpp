#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/ShapeDrawable>

#include <osgUtil/Optimizer>

#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>

#include <osgSim/OverlayNode>

#include <osgText/Font>
#include <osgText/Text>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgQt/GraphicsWindowQt>
#include <QApplication>
#include <iostream>
#include <QTimer>
#include <QLayout>
#include "Common.h"
#include "PickHandler.h"

#ifdef _DEBUG
#pragma comment(lib, "Qt5Cored.lib")
#pragma comment(lib, "Qt5Guid.lib")
#pragma comment(lib, "Qt5OpenGLd.lib")
#pragma comment(lib, "Qt5Widgetsd.lib")
#else
#pragma comment(lib, "Qt5Core.lib")
#pragma comment(lib, "Qt5Gui.lib")
#pragma comment(lib, "Qt5OpenGL.lib")
#pragma comment(lib, "Qt5Widgets.lib")
#endif // !_DEBUG



osg::Camera* createCamera(int x, int y, int w, int h)
{
	osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->windowDecoration = false;
	traits->x = x;
	traits->y = y;
	traits->width = w;
	traits->height = h;
	traits->doubleBuffer = true;

	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setGraphicsContext(new osgQt::GraphicsWindowQt(traits.get()));
	camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
	camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	camera->setProjectionMatrixAsPerspective(
		30.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0f, 10000.0f);
	return camera.release();
}

class ViewerWidget : public QWidget
{
public:
	ViewerWidget(osg::Camera* camera, osg::Node* scene)
		: QWidget()
	{
		_viewer.setCamera(camera);
		_viewer.setSceneData(scene);
		_viewer.addEventHandler(new osgViewer::StatsHandler);
		_viewer.setCameraManipulator(new osgGA::TrackballManipulator);
		_viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);

		osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>(camera->getGraphicsContext());
		if (gw)
		{
			QVBoxLayout* layout = new QVBoxLayout;
			layout->addWidget(gw->getGLWidget());
			setLayout(layout);
		}

		connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
		_timer.start(40);
	}

protected:
	virtual void paintEvent(QPaintEvent* event)
	{
		_viewer.frame();
	}

	osgViewer::Viewer _viewer;
	QTimer _timer;
};

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	osg::Camera* camera = createCamera(50, 50, 640, 480);
	osg::Node* scene = osgDB::readNodeFile("cow.osg");

	ViewerWidget* widget = new ViewerWidget(camera, scene);
	widget->setGeometry(100, 100, 800, 600);
	widget->show();
	return app.exec();
}
