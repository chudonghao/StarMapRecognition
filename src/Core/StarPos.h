//
// Created by cdh on 19-9-22.
//

#ifndef STARMAPRECOGNITION_SRC_CORE_STARPOS_H_
#define STARMAPRECOGNITION_SRC_CORE_STARPOS_H_

#include <string>
#include <osg/Vec2d>
#include <map>
#include <osg/Vec3d>
#include <osg/Matrix>
#include <osg/Vec3>

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
  StarGraphPos(double pixel_x, double pixel_y) : Vec2d(pixel_x, pixel_y) {}
  explicit StarGraphPos(StarTablePos pos) {
    x() = pos.a;
    y() = pos.b;
  }
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
  SkySpherePos() : Vec2d() {}
  SkySpherePos(const osg::Vec2f &vec) : Vec2d(vec) {}
  SkySpherePos(value_type x, value_type y) : Vec2d(x, y) {}
  SkySpherePos &operator=(const osg::Vec2d &r) {
    SetLongitude(r.x());
    SetLatitude(r.y());
    return *this;
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

  osg::Vec3d WorldPosition(double r = 1.) {
    using namespace osg;
    auto rotate_a = Matrix::rotate(DegreesToRadians(x()), Vec3d(0., 0., 1.));
    auto rotate_b = Matrix::rotate(DegreesToRadians(y()), Vec3d(0., -1., 0.));
    return Vec3d(r, 0., 0.)*rotate_b*rotate_a;
  }

  osg::Vec3d ScreenPosition(double pixel_num, const osg::Matrix &view, const osg::Matrix &proj) {
    using namespace osg;
    Vec3d pos_p = WorldPosition()*view*proj;
    double x = pos_p.x(), y = pos_p.y();
    x = (x + 1)*pixel_num/2.;
    y = (y + 1)*pixel_num/2.;
    return Vec3d(x, y, pos_p.z());
  }
};

void Convert(double pixel_f,
             const StarGraphPos &graph_center,
             const StarGraphPos &special_center,
             const StarGraphPos &from,
             SkySpherePos &to);

void Convert(const SkySpherePos &special_center, const SkySpherePos &from, SkySpherePos &to);

#endif //STARMAPRECOGNITION_SRC_CORE_STARPOS_H_
