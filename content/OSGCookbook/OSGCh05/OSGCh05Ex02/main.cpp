#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/ImageStream>

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
	osg::ref_ptr<osg::Image> image;
	if (arguments.argc() > 1)
		image = osgDB::readImageFile(arguments[1]);
	else
	{
#ifdef WIN32
		image = osgDB::readImageFile("0.ffmpeg", new osgDB::Options("format=vfwcap frame_rate=25"));
#else
		image = osgDB::readImageFile("/dev/video0.ffmpeg");
#endif
	}
	osg::ImageStream* imageStream =
		dynamic_cast<osg::ImageStream*>(image.get());
	if (imageStream) imageStream->play();

	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	texture->setImage(image.get());
	osg::ref_ptr<osg::Drawable> quad =
		osg::createTexturedQuadGeometry(
			osg::Vec3(), osg::Vec3(1.0f, 0.0f, 0.0f), osg::Vec3(
				0.0f, 0.0f, 1.0f));
	quad->getOrCreateStateSet()->setTextureAttributeAndModes(
		0, texture.get());

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(quad.get());

	osgViewer::Viewer viewer;
	viewer.setSceneData(geode.get());
	return viewer.run();
}
