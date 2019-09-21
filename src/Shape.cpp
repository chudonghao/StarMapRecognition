//
// Created by cdh on 19-9-21.
//

#include "Shape.h"
#include <osg/Geometry>
using namespace osg;
using namespace std;
Geometry *CreateCircle(const Vec3 &color, float radio, int precision) {
  if (precision < 10) {
    precision = 10;
  }
  auto *vertex = new Vec3Array;
  auto *color_array = new Vec3Array;
  for (int i = 0; i < precision; ++i) {
    float sita = (float) i/precision*2.f*static_cast<float>(PI);
    Vec3 point = Vec3(cos(sita)*radio, sin(sita)*radio, 0.f);
    vertex->push_back(point);
  }
  color_array->push_back(color);
  auto *geo = new Geometry;
  geo->setVertexArray(vertex);
  geo->setColorArray(color_array);
  geo->setColorBinding(Geometry::AttributeBinding::BIND_OVERALL);
  geo->addPrimitiveSet(new DrawArrays(GL_LINE_LOOP, 0, vertex->size()));
  return geo;
}

osg::Geometry *createLine(const osg::Vec3 &start, const osg::Vec3 &end, const osg::Vec3 &color) {

  auto *vertex = new Vec3Array;
  auto *color_array = new Vec3Array;
  vertex->push_back(start);
  vertex->push_back(end);
  color_array->push_back(color);
  auto *geo = new Geometry;
  geo->setVertexArray(vertex);
  geo->setColorArray(color_array);
  geo->setColorBinding(Geometry::AttributeBinding::BIND_OVERALL);
  geo->addPrimitiveSet(new DrawArrays(GL_LINE_LOOP, 0, vertex->size()));
  return geo;
}
