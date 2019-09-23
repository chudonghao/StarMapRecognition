//
// Created by cdh on 19-9-22.
//

#include "SpecialCenterStarOnSkySphereGroup.h"
#include "../log.h"

void SpecialCenterStarOnSkySphereGroup::Add(const std::string &name, StarOnSkySphere star) {
  if (special_center_.GetName() == "") {
    LOG_WARNING << "Set special_center before add star.";
  }
  if (special_center_.GetName() == star.GetName()) {
    return;
  }
  float ad = star.AngularDistance(special_center_);
  //LOG_TRACE << "ad: " << ad;
  stars_[name] = AngularDistanceStar(ad, star);
}
void SpecialCenterStarOnSkySphereGroup::Clear() {
  stars_.clear();
  special_center_ = StarOnSkySphere();
  valid_region_radio_ = 0.;
}
