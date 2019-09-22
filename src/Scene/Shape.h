//
// Created by cdh on 19-9-21.
//

#ifndef STARMAPRECOGNITION_SRC_SHAPE_H_
#define STARMAPRECOGNITION_SRC_SHAPE_H_

#include <osg/Geometry>
#include <osg/Vec3>

osg::Geometry *CreateCircle(const osg::Vec3 &color, float radio, int precision);

osg::Geometry *createLine(const osg::Vec3 &start, const osg::Vec3 &end, const osg::Vec3 &color);

#endif //STARMAPRECOGNITION_SRC_SHAPE_H_
