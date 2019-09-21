//
// Created by chudonghao on 19-9-21.
//

#ifndef STARMAPRECOGNITION_CAMERA_STAR_FUNC_H
#define STARMAPRECOGNITION_CAMERA_STAR_FUNC_H
#include <ceres/ceres.h>

struct CameraStarFunc {
  CameraStarFunc(double f, double a, double b, double r) : f_(f), b_(b), a_(a), r_(r) {}
  template<typename T_>
  bool operator()(const T_ *const a, const T_ *const b, T_ *residual) const {
    residual[0] = cos(b_) * cos(b[0]) * cos(a_ - a[0]) + sin(b_) * sin(b[0]) - f_ / sqrt(r_ * r_ + f_ * f_);
    return true;
  };
  double f_;
  double a_;
  double b_;
  double r_;
};

struct vec2 {
  double x, y;
};
vec2 solver_(double a1, double b1, double a2, double b2, double a3, double b3, double r1, double r2, double r3);
#endif //STARMAPRECOGNITION_CAMERA_STAR_FUNC_H
