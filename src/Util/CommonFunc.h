//
// Created by cdh on 19-9-21.
//

#ifndef STARMAPRECOGNITION_SRC_COMMONFUNC_H_
#define STARMAPRECOGNITION_SRC_COMMONFUNC_H_

#include <osg/Vec3d>

template<typename VecT_>
double Radians(const VecT_ &left, const VecT_ &right) {
  using namespace std;
  using namespace osg;
  double sita;
  sita = left * right / left.length() / right.length();
  if (sita > 1.) {
    sita = 1.;
  }
  if (sita < -1.) {
    sita = -1.;
  }
  return acos(sita);
}

#endif //STARMAPRECOGNITION_SRC_COMMONFUNC_H_
