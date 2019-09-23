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
struct StarTableData {
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
  explicit StarGraphPos(StarTableData pos) {
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
struct StarSkySpherePos : public osg::Vec2d {
  explicit StarSkySpherePos(const StarTableData &table_pos) {
    SetLongitude(table_pos.a);
    SetLatitude(table_pos.b);
  }
  StarSkySpherePos() : Vec2d() {}
  StarSkySpherePos(const osg::Vec2f &vec) : Vec2d(vec) {}
  StarSkySpherePos(value_type x, value_type y) : Vec2d(x, y) {}
  StarSkySpherePos &operator=(const osg::Vec2d &r) {
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
  double AngularDistance(const StarSkySpherePos &pos2);

  osg::Vec3d WorldPosition(double r = 1.) const {
    using namespace osg;
    auto rotate_a = Matrix::rotate(DegreesToRadians(x()), Vec3d(0., 0., 1.));
    auto rotate_b = Matrix::rotate(DegreesToRadians(y()), Vec3d(0., -1., 0.));
    return Vec3d(r, 0., 0.)*rotate_b*rotate_a;
  }
  void FromWorldPosition(osg::Vec3d pos) {
    using namespace osg;
    pos.normalize();

    if (pos.x() == 0.) {
      if (pos.y() < 0.) {
        SetLongitude(270.);
      } else if (pos.y() > 0.) {
        SetLongitude(90.);
      } else {
        SetLatitude(90.);
        return;
      }
    }
    if (pos.y() == 0.) {
      if (pos.x() < 0.) {
        SetLongitude(180.);
      } else if (pos.x() > 0.) {
        SetLongitude(0.);
      } else {
        //SetLatitude(90.);
      }
    }
    if (pos.x() != 0.) {
      double longitude_t = RadiansToDegrees(atan(pos.y()/pos.x()));
      if (pos.x() > 0.) {
        if (pos.y() > 0.) {
          SetLongitude(longitude_t);
        } else {
          SetLongitude(longitude_t + 360.);
        }
      } else {
        SetLongitude(180. + longitude_t);
        //if (pos.y() > 0.) {
        //  SetLongitude(180. + longitude_t);
        //} else {
        //  SetLongitude(180. + longitude_t);
        //}
      }
    }
    double latitude_t = RadiansToDegrees(atan(pos.z()/sqrt(pos.x()*pos.x() + pos.y()*pos.y())));
    SetLatitude(latitude_t);
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
             StarSkySpherePos &to);

void Convert(const StarSkySpherePos &special_center, const StarSkySpherePos &from, StarSkySpherePos &to);

#endif //STARMAPRECOGNITION_SRC_CORE_STARPOS_H_
