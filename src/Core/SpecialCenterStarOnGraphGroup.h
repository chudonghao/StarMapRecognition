//
// Created by cdh on 19-9-22.
//

#ifndef STARMAPRECOGNITION_SRC_CORE_SPECIALCENTERSTARONGRAPHGROUP_H_
#define STARMAPRECOGNITION_SRC_CORE_SPECIALCENTERSTARONGRAPHGROUP_H_

#include "StarOnGraph.h"

namespace unused {

class SpecialCenterStarOnGraphGroup {
 public:
  SpecialCenterStarOnGraphGroup() : special_center_() {}
  explicit SpecialCenterStarOnGraphGroup(const StarOnGraph__ &special_center) : special_center_(special_center) {}
  const std::map<std::string, StarOnGraph__> &GetSkySpherePoses() const {
    return stars_;
  }
  std::map<std::string, StarOnGraph__> &GetSkySpherePoses() {
    return stars_;
  }
  void Add(const std::string &name, StarOnGraph__ star) {
    stars_[name] = std::move(star);
  }
  const StarOnGraph__ &GetSpecialCenter() const {
    return special_center_;
  }
  void SetSpecialCenter(const StarOnGraph__ &special_center) {
    special_center_ = special_center;
  }
 private:
  StarOnGraph__ special_center_;
  std::map<std::string, StarOnGraph__> stars_;
};

}
#endif //STARMAPRECOGNITION_SRC_CORE_SPECIALCENTERSTARONGRAPHGROUP_H_
