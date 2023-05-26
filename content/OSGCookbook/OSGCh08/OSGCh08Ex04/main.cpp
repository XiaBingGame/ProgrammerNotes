#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Texture>

#include <osgUtil/Optimizer>

#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/DatabasePager>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>

#include <osgSim/OverlayNode>

#include <osgUtil/PrintVisitor>

#include <osgText/Font>
#include <osgText/Text>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <iostream>
#include "Common.h"
#include "PickHandler.h"


int main(int argc, char** argv)
{
	osg::ArgumentParser arguments(&argc, argv);
	osg::ref_ptr<osg::Node> root = osgDB::readNodeFiles(arguments);
	// osg::Texture::getTextureObjectManager(0)->setMaxTexturePoolSize(64000);

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	osgDB::DatabasePager* pager = viewer.getDatabasePager();
	pager->setDoPreCompile(true);
	pager->setTargetMaximumNumberOfPageLOD(10);

	viewer.setSceneData(root.get());
	viewer.addEventHandler(new osgViewer::StatsHandler);
	return viewer.run();
}
