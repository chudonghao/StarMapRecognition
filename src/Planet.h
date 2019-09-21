//
// Created by cdh on 19-9-21.
//

#ifndef STARMAPRECOGNITION__PLANET_H_
#define STARMAPRECOGNITION__PLANET_H_

#include <osg/Geode>
#include <osg/ShapeDrawable>

struct PlanetData {
  int id;
  double a, b, l;
};

class Planet : public osg::Geode {
 public:
  Planet(int id, const osg::Vec4 &color, float radio);
  void SetColor(const osg::Vec4 &color);
  void PushColor(const osg::Vec4 &color);
  void PopColor();
  int GetId() { return id_; }
 private:
  osg::ShapeDrawable *shape_drawable_;
  osg::Vec4 origin_color_;
  int id_;
};

#endif //STARMAPRECOGNITION__PLANET_H_
