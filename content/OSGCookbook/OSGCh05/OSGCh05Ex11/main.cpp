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
#include <iostream>
#include "Common.h"
#include "PickHandler.h"
#include <NxPhysics.h>

#pragma comment(lib, "PhysXCore64.lib")
#pragma comment(lib, "PhysXCooking64.lib")
#pragma comment(lib, "NxCharacter64.lib")
#pragma comment(lib, "PhysXLoader64.lib")

class PhysXInterface : public osg::Referenced
{
public:
	static PhysXInterface* instance();

	void createWorld(const osg::Plane& plane, const osg::Vec3& gravity);
	void createBox(int id, const osg::Vec3& dim, double mass);
	void createSphere(int id, double radius, double mass);

	void setVelocity(int id, const osg::Vec3& pos);
	void setMatrix(int id, const osg::Matrix& matrix);
	osg::Matrix getMatrix(int id);

	void simulate(double step);

protected:
	PhysXInterface();
	virtual ~PhysXInterface();

	void createActor(int id, NxShapeDesc* shape, NxBodyDesc* body);

	typedef std::map<int, NxActor*> ActorMap;
	ActorMap _actors;
	NxPhysicsSDK* _physicsSDK;
	NxScene* _scene;
};

PhysXInterface* PhysXInterface::instance()
{
	static osg::ref_ptr<PhysXInterface> s_registry = new PhysXInterface;
	return s_registry.get();
}

PhysXInterface::PhysXInterface()
	: _scene(NULL)
{
	NxPhysicsSDKDesc desc;
	NxSDKCreateError errorCode = NXCE_NO_ERROR;
	_physicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, NULL, desc, &errorCode);
	if (!_physicsSDK)
	{
		OSG_WARN << "Unable to initialize the PhysX SDK, error code: "
			<< errorCode << std::endl;
	}
}

PhysXInterface::~PhysXInterface()
{
	if (_scene)
	{
		for (ActorMap::iterator itr = _actors.begin(); itr != _actors.end(); ++itr)
			_scene->releaseActor(*(itr->second));
		_physicsSDK->releaseScene(*_scene);
	}
	NxReleasePhysicsSDK(_physicsSDK);
}

void PhysXInterface::createWorld(const osg::Plane& plane, const osg::Vec3& gravity)
{
	NxSceneDesc sceneDesc;
	sceneDesc.gravity = NxVec3(gravity.x(), gravity.y(), gravity.z());
	_scene = _physicsSDK->createScene(sceneDesc);

	NxMaterial* defaultMaterial = _scene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(0.5f);
	defaultMaterial->setStaticFriction(0.5f);
	defaultMaterial->setDynamicFriction(0.5f);

	// Create the ground plane
	NxPlaneShapeDesc shapeDesc;
	shapeDesc.normal = NxVec3(plane[0], plane[1], plane[2]);
	shapeDesc.d = plane[3];
	createActor(-1, &shapeDesc, NULL);
}

void PhysXInterface::createBox(int id, const osg::Vec3& dim, double mass)
{
	NxBoxShapeDesc shapeDesc; shapeDesc.dimensions = NxVec3(dim.x(), dim.y(), dim.z());
	NxBodyDesc bodyDesc; bodyDesc.mass = mass;
	createActor(id, &shapeDesc, &bodyDesc);
}

void PhysXInterface::createSphere(int id, double radius, double mass)
{
	NxSphereShapeDesc shapeDesc; shapeDesc.radius = radius;
	NxBodyDesc bodyDesc; bodyDesc.mass = mass;
	createActor(id, &shapeDesc, &bodyDesc);
}

void PhysXInterface::setVelocity(int id, const osg::Vec3& vec)
{
	NxActor* actor = _actors[id];
	actor->setLinearVelocity(NxVec3(vec.x(), vec.y(), vec.z()));
}

void PhysXInterface::setMatrix(int id, const osg::Matrix& matrix)
{
	NxF32 d[16];
	for (int i = 0; i < 16; ++i)
		d[i] = *(matrix.ptr() + i);
	NxMat34 nxMat; nxMat.setColumnMajor44(&d[0]);

	NxActor* actor = _actors[id];
	actor->setGlobalPose(nxMat);
}

osg::Matrix PhysXInterface::getMatrix(int id)
{
	float mat[16];
	NxActor* actor = _actors[id];
	actor->getGlobalPose().getColumnMajor44(mat);
	return osg::Matrix(&mat[0]);
}

void PhysXInterface::simulate(double step)
{
	_scene->simulate(step);
	_scene->flushStream();
	_scene->fetchResults(NX_RIGID_BODY_FINISHED, true);
}

void PhysXInterface::createActor(int id, NxShapeDesc* shape, NxBodyDesc* body)
{
	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack(shape);
	actorDesc.body = body;

	NxActor* actor = _scene->createActor(actorDesc);
	_actors[id] = actor;
}

class PhysicsUpdater : public osgGA::GUIEventHandler
{
public:
	PhysicsUpdater(osg::Group* root) : _root(root) {}

	void addGround(const osg::Vec3& gravity)
	{
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		geode->addDrawable(new osg::ShapeDrawable(
			new osg::Box(osg::Vec3(0.0f, 0.0f, -0.5f), 100.0f, 100.0f, 1.0f)));

		osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
		mt->addChild(geode.get());
		_root->addChild(mt.get());

		PhysXInterface::instance()->createWorld(osg::Plane(0.0f, 0.0f, 1.0f, 0.0f), gravity);
	}

	void addPhysicsBox(osg::Box* shape, const osg::Vec3& pos, const osg::Vec3& vel, double mass)
	{
		int id = _physicsNodes.size();
		PhysXInterface::instance()->createBox(id, shape->getHalfLengths(), mass);
		addPhysicsData(id, shape, pos, vel, mass);
	}

	void addPhysicsSphere(osg::Sphere* shape, const osg::Vec3& pos, const osg::Vec3& vel, double mass)
	{
		int id = _physicsNodes.size();
		PhysXInterface::instance()->createSphere(id, shape->getRadius(), mass);
		addPhysicsData(id, shape, pos, vel, mass);
	}

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		osgViewer::View* view = static_cast<osgViewer::View*>(&aa);
		if (!view || !_root) return false;

		switch (ea.getEventType())
		{
		case osgGA::GUIEventAdapter::KEYUP:
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Return)
			{
				osg::Vec3 eye, center, up, dir;
				view->getCamera()->getViewMatrixAsLookAt(eye, center, up);
				dir = center - eye; dir.normalize();
				addPhysicsSphere(new osg::Sphere(osg::Vec3(), 0.5f), eye, dir * 60.0f, 2.0);
			}
			break;
		case osgGA::GUIEventAdapter::FRAME:
			PhysXInterface::instance()->simulate(0.02);
			for (NodeMap::iterator itr = _physicsNodes.begin();
				itr != _physicsNodes.end(); ++itr)
			{
				osg::Matrix matrix = PhysXInterface::instance()->getMatrix(itr->first);
				itr->second->setMatrix(matrix);
			}
			break;
		default: break;
		}
		return false;
	}

protected:
	void addPhysicsData(int id, osg::Shape* shape, const osg::Vec3& pos,
		const osg::Vec3& vel, double mass)
	{
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		geode->addDrawable(new osg::ShapeDrawable(shape));

		osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
		mt->addChild(geode.get());
		_root->addChild(mt.get());

		PhysXInterface::instance()->setMatrix(id, osg::Matrix::translate(pos));
		PhysXInterface::instance()->setVelocity(id, vel);
		_physicsNodes[id] = mt;
	}

	typedef std::map<int, osg::observer_ptr<osg::MatrixTransform> > NodeMap;
	NodeMap _physicsNodes;
	osg::observer_ptr<osg::Group> _root;
};

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Group> root = new osg::Group;
	osg::ref_ptr<PhysicsUpdater> updater = new PhysicsUpdater(root.get());

	updater->addGround(osg::Vec3(0.0f, 0.0f, -9.8f));
	for (unsigned int i = 0; i < 10; ++i)
	{
		for (unsigned int j = 0; j < 10; ++j)
		{
			updater->addPhysicsBox(new osg::Box(osg::Vec3(), 0.99f), osg::Vec3((float)i, 0.0f, (float)j + 0.5f), osg::Vec3(), 1.0f);
		}
	}

	osgViewer::Viewer viewer;
	viewer.addEventHandler(updater.get());
	viewer.setUpViewInWindow(50, 50, 1440, 900);
	viewer.setSceneData(root.get());
	return viewer.run();
}
