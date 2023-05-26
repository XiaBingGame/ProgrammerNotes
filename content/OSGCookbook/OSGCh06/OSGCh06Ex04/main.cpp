#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>

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
#include <iostream>
#include "Common.h"
#include "PickHandler.h"


int main(int argc, char** argv)
{
	osg::ArgumentParser arguments(&argc, argv);
	osg::ref_ptr<osg::Node> scene = osgDB::readNodeFiles(arguments);
	if (!scene) scene = osgDB::readNodeFile("cessna.osg");

	osg::ref_ptr<osg::Texture2D> tex2D = new osg::Texture2D;
	tex2D->setTextureSize(1024, 1024);
	tex2D->setInternalFormat(GL_DEPTH_COMPONENT24);
	tex2D->setSourceFormat(GL_DEPTH_COMPONENT);
	tex2D->setSourceType(GL_FLOAT);

	osg::ref_ptr<osg::Camera> rttCamera = osgCookBook::createRTTCamera(osg::Camera::DEPTH_BUFFER, tex2D.get());
	rttCamera->addChild(scene.get());

	osg::ref_ptr<osg::Camera> hudCamera = osgCookBook::createHUDCamera(0.0, 1.0, 0.0, 1.0);
	hudCamera->addChild(osgCookBook::createScreenQuad(0.5f, 0.5f));
	hudCamera->getOrCreateStateSet()->setTextureAttributeAndModes(0, tex2D.get());

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(rttCamera.get());
	root->addChild(hudCamera.get());
	root->addChild(scene.get());

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.getCamera()->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	viewer.setSceneData(root.get());
	return viewer.run();
}
