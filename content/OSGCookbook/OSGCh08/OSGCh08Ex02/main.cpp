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
#include <iostream>
#include "Common.h"
#include "PickHandler.h"

#define COMPRESS_TEXTURE  // Comment this to disable compressing textures

osg::Image* createRandomImage(int width, int height)
{
	osg::ref_ptr<osg::Image> image = new osg::Image;
	image->allocateImage(width, height, 1, GL_RGB, GL_UNSIGNED_BYTE);

	unsigned char* data = image->data();
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			*(data++) = osgCookBook::randomValue(0.0f, 255.0f);
			*(data++) = osgCookBook::randomValue(0.0f, 255.0f);
			*(data++) = osgCookBook::randomValue(0.0f, 255.0f);
		}
	}
	return image.release();
}

osg::Node* createQuads(unsigned int cols, unsigned int rows)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	for (unsigned int y = 0; y < rows; ++y)
	{
		for (unsigned int x = 0; x < cols; ++x)
		{
			osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
			texture->setImage(createRandomImage(512, 512));
#ifdef COMPRESS_TEXTURE
			texture->setInternalFormatMode(osg::Texture2D::USE_S3TC_DXT1_COMPRESSION);
			texture->setUnRefImageDataAfterApply(true);
#endif

			osg::Vec3 center((float)x, 0.0f, (float)y);
			osg::ref_ptr<osg::Drawable> quad = osg::createTexturedQuadGeometry(
				center, osg::Vec3(0.9f, 0.0f, 0.0f), osg::Vec3(0.0f, 0.0f, 0.9f));
			quad->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());
			geode->addDrawable(quad.get());
		}
	}
	return geode.release();
}

int main(int argc, char** argv)
{
	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(createQuads(20, 20));
	viewer.addEventHandler(new osgViewer::StatsHandler);
	return viewer.run();
}
