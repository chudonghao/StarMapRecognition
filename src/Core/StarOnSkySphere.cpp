//
// Created by cdh on 19-9-22.
//

#include "StarOnSkySphere.h"
#include <osg/Vec3>
using namespace std;
using namespace osg;

double StarOnSkySphere::AngularDistance(const StarOnSkySphere &star) {
  return sky_sphere_pos_.AngularDistance(star.GetSkySpherePos());
}
StarOnSkySphere::StarOnSkySphere(StarTablePos pos) {
  SetName(pos.id);
  SetSkySpherePos(SkySpherePos(pos));
}
