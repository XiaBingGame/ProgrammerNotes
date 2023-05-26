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

#include <osgAnimation/EaseMotion>

#include <osgViewer/Viewer>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Common.h"
#include "PickHandler.h"

#define RAND(min, max) ((min) + (float)rand() / (RAND_MAX+1) * ( (max)-(min) ))

class ScrollTextCallback : public osg::Drawable::UpdateCallback
{
public:
	ScrollTextCallback() {
		_motion = new osgAnimation::LinearMotion;
		computeNewPosition();
	}

	virtual void update(osg::NodeVisitor* nv, osg::Drawable* drawable);

	void computeNewPosition()
	{
		_motion->reset();
		_currentPos.y() = RAND(50.0, 500.0);
	}

protected:
	osg::ref_ptr<osgAnimation::LinearMotion> _motion;
	osg::Vec3 _currentPos;
};

void ScrollTextCallback::update(osg::NodeVisitor* nv, osg::Drawable* drawable)
{
	osgText::Text* text = dynamic_cast<osgText::Text*>(drawable);
	if (!text)
		return;

	_motion->update(0.0001);
	float value = _motion->getValue();
	if (value >= 1.0) computeNewPosition();
	else _currentPos.x() = value * 800.0;

	std::stringstream ss; ss << std::setprecision(5);
	ss << "XPos: " << std::setw(5) << std::setfill(' ') << _currentPos.x() << "; YPos:"
		<< std::setw(5) << std::setfill(' ') << _currentPos.y();
	text->setPosition(_currentPos);
	text->setText(ss.str());
}

int main(int argc, char** argv)
{
	osgText::Text* text = osgCookBook::createText(osg::Vec3(), "", 20.0f);
	text->setUpdateCallback(new ScrollTextCallback);

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(text);

	osg::ref_ptr<osg::Camera> hudCamera = osgCookBook::createHUDCamera(0, 800, 0, 600);
	hudCamera->addChild(geode.get());

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 1440, 900);
	viewer.setSceneData(hudCamera.get());
	return viewer.run();
}
