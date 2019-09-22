//
// Created by cdh on 19-9-22.
//

#include "StarPos.h"
#include "../Util/CommonFunc.h"
#include <osg/Vec3d>
#include <osg/Matrix>

using namespace std;
using namespace osg;

void Convert(double pixel_f,
             const StarGraphPos &graph_center,
             const StarGraphPos &special_center,
             const StarGraphPos &from,
             SkySpherePos &to) {
  Vec3d eye = Vec3d(graph_center.GetPixelX(), graph_center.GetPixelY(), pixel_f);
  Vec3d from_Q1 = Vec3d(from.GetPixelX(), special_center.GetPixelY(), 0.);
  Vec3d from_Q2 = Vec3d(special_center.GetPixelX(), from.GetPixelY(), 0.);
  Vec3d special_center_O = Vec3d(special_center.GetPixelX(), special_center.GetPixelY(), 0.);
  Vec3d dir_Q1 = from_Q1 - eye;
  Vec3d dir_Q2 = from_Q2 - eye;
  Vec3d dir = special_center_O - eye;
  double dir_lon = from.GetPixelX() < special_center.GetPixelX() ? -1. : 1.;
  double dir_lat = from.GetPixelY() < special_center.GetPixelY() ? -1. : 1.;
  to.SetLongitude(RadiansToDegrees(Radians(dir_Q1, dir))*dir_lon);
  to.SetLatitude(RadiansToDegrees(Radians(dir_Q2, dir))*dir_lat);
}

void Convert(const SkySpherePos &special_center, const SkySpherePos &from, SkySpherePos &to) {
  // TODO 更好的计算方式
  // 经度
  auto rotate_a = Matrix::rotate(DegreesToRadians(special_center.GetLongitude()), Vec3d(0., 0., 1.));
  // 纬度
  auto rotate_b = Matrix::rotate(DegreesToRadians(special_center.GetLatitude()), Vec3d(0., -1., 0.));
  //Vec3d eye = Vec3d();
  //Vec3d center = Vec3d(1., 0., 0.)*rotate_b*rotate_a;

  // Z轴的新方向
  Vec3d axis_y = Vec3d(0., 0., 1.)*rotate_b*rotate_a;
  // Y负半轴新方向
  Vec3d axis_x = Vec3d(0., -1., 0.)*rotate_b*rotate_a;

  // from 点坐标
  auto rotate_a2 = Matrix::rotate(DegreesToRadians(from.GetLongitude()), Vec3d(0., 0., 1.));
  auto rotate_b2 = Matrix::rotate(DegreesToRadians(from.GetLatitude()), Vec3d(0., -1., 0.));
  Vec3d p = Vec3d(1., 0., 0.)*rotate_b2*rotate_a2;

  double angle = RadiansToDegrees(Radians(axis_x, p));
  double angle2 = RadiansToDegrees(Radians(axis_y, p));
  double lat, lon;
  lon = 90. - angle;
  lat = 90. - angle2;
  to = Vec2d(lon, lat);
}

double SkySpherePos::AngularDistance(const SkySpherePos &pos2) {
  double a1, b1, a2, b2;
  a1 = DegreesToRadians(GetLongitude());//a1
  b1 = DegreesToRadians(GetLatitude());//b1
  a2 = DegreesToRadians(pos2.GetLongitude());//a2
  b2 = DegreesToRadians(pos2.GetLatitude());//b2
  double cos_value = cos(b1)*cos(b2)*cos(a1 - a2) + sin(b1)*sin(b2);
  double cos_radians = acos(cos_value);
  return RadiansToDegrees(cos_radians);
}

