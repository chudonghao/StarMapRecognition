//
// Created by cdh on 19-9-22.
//

#ifndef STARMAPRECOGNITION_SRC_CORE_STARPOS_H_
#define STARMAPRECOGNITION_SRC_CORE_STARPOS_H_

#include <string>
#include <osg/Vec2d>
#include <map>

/*
 * 星表信息
 */
struct StarTablePos {
 public:
  std::string id;
  double a, b, l;
};
/**
 * 星图上的像素坐标
 */
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
/**
 * 经纬度坐标
 */
struct SkySpherePos : public osg::Vec2d {
  explicit SkySpherePos(const StarTablePos &table_pos) {
    SetLongitude(table_pos.a);
    SetLatitude(table_pos.b);
  }
  SkySpherePos() {}
  SkySpherePos(const osg::Vec2f &vec) : Vec2d(vec) {}
  SkySpherePos(value_type x, value_type y) : Vec2d(x, y) {}
  SkySpherePos &operator=(const osg::Vec2d &r) {
    SetLongitude(r.x());
    SetLatitude(r.y());
  }
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
  double AngularDistance(const SkySpherePos &pos2);
};

void Convert(double pixel_f,
             const StarGraphPos &graph_center,
             const StarGraphPos &special_center,
             const StarGraphPos &from,
             SkySpherePos &to);

#endif //STARMAPRECOGNITION_SRC_CORE_STARPOS_H_
