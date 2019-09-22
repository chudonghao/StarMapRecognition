//
// Created by cdh on 19-9-22.
//

#ifndef STARMAPRECOGNITION_SRC_CORE_SPECIALCENTERSTARONSKYSPHEREGROUP_H_
#define STARMAPRECOGNITION_SRC_CORE_SPECIALCENTERSTARONSKYSPHEREGROUP_H_

#include <map>
#include "StarOnSkySphere.h"

class SpecialCenterStarOnSkySphereGroup {
 public:
  const std::map<std::string, StarOnSkySphere> &GetSkySpherePoses() const {
    return stars_;
  }
  std::map<std::string, StarOnSkySphere> &GetSkySpherePoses() {
    return stars_;
  }
  void SetSkySpherePoses(const std::map<std::string, StarOnSkySphere> &sky_sphere_poses) {
    stars_ = sky_sphere_poses;
  }
 private:
  std::map<std::string, StarOnSkySphere> stars_;
};

#endif //STARMAPRECOGNITION_SRC_CORE_SPECIALCENTERSTARONSKYSPHEREGROUP_H_
