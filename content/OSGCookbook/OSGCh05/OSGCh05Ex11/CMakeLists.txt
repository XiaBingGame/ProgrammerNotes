cmake_minimum_required(VERSION 2.6)
set(ProjName OSGCh05Ex11)
set(UtilFile ../common/common.cpp ../common/PickHandler.cpp)
project(${ProjName})

FIND_PATH(PHYSX_SDK_DIR Physics/include/NxPhysics.h)
FIND_LIBRARY(PHYSX_LIBRARY PhysXLoader.lib libPhysXLoader.so)
SET(EXTERNAL_INCLUDE_DIR
    "${PHYSX_SDK_DIR}/PhysXLoader/include"
    "${PHYSX_SDK_DIR}/Physics/include"
    "${PHYSX_SDK_DIR}/Foundation/include")

find_package(OpenThreads)
find_package(osg)
find_package(osgDB)
find_package(osgUtil)
find_package(osgViewer)
find_package(osgGA)
find_package(osgAnimation)
find_package(osgText)

macro(config_project PROJNAME LIBNAME)
	include_directories( ${${LIBNAME}_INCLUDE_DIR} )
	target_link_libraries( ${PROJNAME} ${${LIBNAME}_LIBRARY})
endmacro()

include_directories(../common)
add_executable(${ProjName} ${UtilFile} main.cpp)
config_project(${ProjName} OPENTHREADS)
config_project(${ProjName} OSG)
config_project(${ProjName} OSGDB)
config_project(${ProjName} OSGUTIL)
config_project(${ProjName} OSGVIEWER)
config_project(${ProjName} OSGGA)
config_project(${ProjName} OSGANIMATION)
config_project(${ProjName} OSGTEXT)
