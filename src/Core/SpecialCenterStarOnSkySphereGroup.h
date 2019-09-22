//
// Created by cdh on 19-9-22.
//

#ifndef STARMAPRECOGNITION_SRC_CORE_SPECIALCENTERSTARONSKYSPHEREGROUP_H_
#define STARMAPRECOGNITION_SRC_CORE_SPECIALCENTERSTARONSKYSPHEREGROUP_H_

#include <map>
#include "StarOnSkySphere.h"

class SpecialCenterStarOnSkySphereGroup {
 public:
  SpecialCenterStarOnSkySphereGroup() : special_center_() {}
  explicit SpecialCenterStarOnSkySphereGroup(const StarOnSkySphere &special_center) : special_center_(special_center) {}
  struct AngularDistanceStar {
    float angular_distance;
    StarOnSkySphere star;
  };
  const std::map<std::string, AngularDistanceStar> &GetStaresOnSkyShphere() const {
    return stars_;
  }
  void Add(const std::string &name, StarOnSkySphere star);
  const StarOnSkySphere &GetSpecialCenter() const {
    return special_center_;
  }
  void SetSpecialCenter(const StarOnSkySphere &special_center) {
    special_center_ = special_center;
  }
  float GetValidRegionRadio() const {
    return valid_region_radio_;
  }
  void SetValidRegionRadio(float valid_region_radio) {
    valid_region_radio_ = valid_region_radio;
  }
 private:
  float valid_region_radio_;
  StarOnSkySphere special_center_;
  std::map<std::string, AngularDistanceStar> stars_;
};

#endif //STARMAPRECOGNITION_SRC_CORE_SPECIALCENTERSTARONSKYSPHEREGROUP_H_
