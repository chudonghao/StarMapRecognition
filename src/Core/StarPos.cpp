//
// Created by cdh on 19-9-22.
//

#include "StarPos.h"
#include "../Util/CommonFunc.h"
#include <osg/Vec3d>
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

double SkySpherePos::AngularDistance(const SkySpherePos &pos2) {
  double a1, b1, a2, b2;
  a1 = GetLongitude();//a1
  b1 = GetLatitude();//b1
  a2 = pos2.GetLongitude();//a2
  b2 = pos2.GetLatitude();//b2
  double cos_value = cos(b1)*cos(b2)*cos(a1 - a2) + sin(b1)*sin(b2);
  double cos_radians = acos(cos_value);
  return RadiansToDegrees(cos_radians);

}

