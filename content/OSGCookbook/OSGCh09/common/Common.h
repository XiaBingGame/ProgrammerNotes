#pragma once
#include <string>
#include <osg/Vec3>
#include <osgText/Font>
#include <osgText/Text>

namespace osg
{
	class Camera;
	class AnimationPathCallback;
}
namespace osgText
{
	class Text;
}

namespace osgCookBook
{
	osg::Camera* createHUDCamera(double left, double right, double bottom, double top);
	osgText::Text* createText(const osg::Vec3& pos, const std::string& content, float size);
	osg::AnimationPathCallback* createAnimationPathCallback(float radius, float time);
	osg::Geode* createScreenQuad(float width, float height, float scale = 1.0f);
	osg::Camera* createRTTCamera(osg::Camera::BufferComponent buffer, osg::Texture* tex, bool isAbsolute = false);
}
