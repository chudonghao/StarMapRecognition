//
// Created by cdh on 19-9-22.
//

#ifndef STARMAPRECOGNITION_SRC_CORE_STARONSKYSPHERE_H_
#define STARMAPRECOGNITION_SRC_CORE_STARONSKYSPHERE_H_

#include "StarPos.h"

class StarOnSkySphere {
 public:
  const SkySpherePos &GetSkySpherePos() const {
    return sky_sphere_pos_;
  }
  void SetSkySpherePos(const SkySpherePos &sky_sphere_pos) {
    sky_sphere_pos_ = sky_sphere_pos;
  }
  const std::string &GetName() const {
    return name_;
  }
  void SetName(const std::string &name) {
    name_ = name;
  }
 private:
  SkySpherePos sky_sphere_pos_;
  std::string name_;
};

#endif //STARMAPRECOGNITION_SRC_CORE_STARONSKYSPHERE_H_
