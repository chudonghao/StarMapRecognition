//
// Created by cdh on 19-9-21.
//

#include "Planet.h"
using namespace osg;
using namespace std;
Planet::Planet(const std::string &name, const osg::Vec4 &color, float radio) {
  setName(name);
  shape_drawable_ = new ShapeDrawable(new Sphere(Vec3(), radio));
  shape_drawable_->setColor(color);
  addDrawable(shape_drawable_);
  origin_color_ = color;
}
void Planet::SetColor(const Vec4 &color) {
  origin_color_ = color;
  shape_drawable_->setColor(color);
}
void Planet::PushColor(const osg::Vec4 &color) {
  shape_drawable_->setColor(color);
}
void Planet::PopColor() {
  shape_drawable_->setColor(origin_color_);
}
