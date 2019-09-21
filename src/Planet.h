//
// Created by cdh on 19-9-21.
//

#ifndef STARMAPRECOGNITION__PLANET_H_
#define STARMAPRECOGNITION__PLANET_H_

#include <osg/Geode>
#include <osg/ShapeDrawable>
class Planet : public osg::Geode {
 public:
  Planet(const osg::Vec4 &color, float radio);
  void SetColor(const osg::Vec4 &color);
  void PushColor(const osg::Vec4 &color);
  void PopColor();
 private:
  osg::ShapeDrawable *shape_drawable_;
  osg::Vec4 origin_color_;
};

#endif //STARMAPRECOGNITION__PLANET_H_
