//
// Created by cdh on 19-9-22.
//

#ifndef STARMAPRECOGNITION_SRC_CORE_STARPOS_H_
#define STARMAPRECOGNITION_SRC_CORE_STARPOS_H_

#include <string>
#include <osg/Vec2d>
#include <map>

struct StarTablePos {
  int id;
  double a, b, l;
};

struct StarGraphPos : public osg::Vec2d {
  StarGraphPos() {}
  StarGraphPos(const osg::Vec2f &vec) : Vec2d(vec) {}
  StarGraphPos(value_type pixel_x, value_type pixel_y) : Vec2d(pixel_x, pixel_y) {}
  double GetPixelX() const {
    return x();
  }
  void SetPixelX(double pixel_x) {
    x() = pixel_x;
  }
  double GetPixelY() const {
    return y();
  }
  void SetPixelY(double pixel_y) {
    y() = pixel_y;
  }
};

struct SkySpherePos : public osg::Vec2d {
  SkySpherePos() {}
  SkySpherePos(const osg::Vec2f &vec) : Vec2d(vec) {}
  SkySpherePos(value_type x, value_type y) : Vec2d(x, y) {}
  double GetLongitude() const {
    return x();
  }
  void SetLongitude(double longitude) {
    x() = longitude;
  }
  double GetLatitude() const {
    return y();
  }
  void SetLatitude(double latitude) {
    y() = latitude;
  }
};

#endif //STARMAPRECOGNITION_SRC_CORE_STARPOS_H_
