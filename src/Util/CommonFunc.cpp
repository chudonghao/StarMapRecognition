//
// Created by cdh on 19-9-21.
//

#include "CommonFunc.h"

#include <osg/Geode>
#include <osg/MatrixTransform>

#include "../Scene/Shape.h"
using namespace std;
using namespace osg;

double Radians(osg::Vec3d &left, osg::Vec3d &right) {
  double sita;
  sita = left*right/left.length()/right.length();
  if (sita > 1.) {
    sita = 1.;
  }
  if (sita < -1.) {
    sita = -1.;
  }
  return acos(sita);
}
