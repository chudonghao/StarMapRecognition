//
// Created by cdh on 19-9-22.
//

#ifndef STARMAPRECOGNITION_SRC_CORE_STARONSKYSPHERE_H_
#define STARMAPRECOGNITION_SRC_CORE_STARONSKYSPHERE_H_

#include "StarPos.h"

class StarOnSkySphere {
 public:
  StarOnSkySphere() : sky_sphere_pos_(), name_("") {}
  explicit StarOnSkySphere(StarTablePos pos);
  StarOnSkySphere(const std::string &name, const SkySpherePos &sky_sphere_pos) : name_(name), sky_sphere_pos_(sky_sphere_pos) {}
  StarOnSkySphere(const StarOnSkySphere &from) : name_(from.name_), sky_sphere_pos_(from.sky_sphere_pos_) {}

  const SkySpherePos &GetSkySpherePos() const {
    return sky_sphere_pos_;
  }
  void SetSkySpherePos(const SkySpherePos &sky_sphere_pos) {
    sky_sphere_pos_ = sky_sphere_pos;
  }
  std::string GetName() const {
    return name_;
  }
  void SetName(const std::string &name) {
    name_ = name;
  }
  double AngularDistance(const StarOnSkySphere &star);

  bool operator==(const StarOnSkySphere &rhs) const {
    return name_ == rhs.name_;
  }
  bool operator!=(const StarOnSkySphere &rhs) const {
    return !(rhs == *this);
  }
 private:
  std::string name_;
  SkySpherePos sky_sphere_pos_;
};

#endif //STARMAPRECOGNITION_SRC_CORE_STARONSKYSPHERE_H_
