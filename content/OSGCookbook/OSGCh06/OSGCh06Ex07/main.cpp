#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Depth>
#include <osg/TexGen>
#include <osg/TextureCubeMap>

#include <osgUtil/Optimizer>
#include <osgUtil/CullVisitor>

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
#include "SkyBox"

int main(int argc, char** argv)
{
	osg::ArgumentParser arguments(&argc, argv);
	osg::ref_ptr<osg::Node> scene = osgDB::readNodeFiles(arguments);
	if (!scene) scene = osgDB::readNodeFile("lz.osg.90,0,0.rot");

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(new osg::ShapeDrawable(
		new osg::Sphere(osg::Vec3(), scene->getBound().radius())));
	geode->setCullingActive(false);

	osg::ref_ptr<SkyBox> skybox = new SkyBox;
	skybox->getOrCreateStateSet()->setTextureAttributeAndModes(0, new osg::TexGen);
	skybox->setEnvironmentMap(0,
		osgDB::readImageFile("Cubemap_snow/posx.jpg"), osgDB::readImageFile("Cubemap_snow/negx.jpg"),
		osgDB::readImageFile("Cubemap_snow/posy.jpg"), osgDB::readImageFile("Cubemap_snow/negy.jpg"),
		osgDB::readImageFile("Cubemap_snow/posz.jpg"), osgDB::readImageFile("Cubemap_snow/negz.jpg"));
	skybox->addChild(geode.get());

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(scene.get());
	root->addChild(skybox.get());

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
