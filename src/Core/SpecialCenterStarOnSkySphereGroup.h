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
  class AngularDistanceStar {
   public:
    AngularDistanceStar() {}
    AngularDistanceStar(float angular_distance, const StarOnSkySphere &star) : angular_distance(angular_distance), star(star) {}
    float angular_distance{};
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
    CalculateAngularDistance();
    Shirk();
  }
  float GetValidRegionRadio() const {
    return valid_region_radio_;
  }
  void SetValidRegionRadio(float valid_region_radio) {
    valid_region_radio_ = valid_region_radio;
    Shirk();
  }
  void Shirk() {
    auto last_iter = stars_.begin();
    for (auto iter = last_iter; iter != stars_.end(); ++iter) {
      if (iter->second.angular_distance >= valid_region_radio_ || special_center_.GetName() == iter->first) {
        if (iter == stars_.begin()) {
          stars_.erase(iter);
          // TODO ?
          if (stars_.empty()) {
            break;
          }
          iter = stars_.begin();
          last_iter = iter;
        } else {
          stars_.erase(iter);
          iter = last_iter;
        }
      } else {
        last_iter = iter;
      }
    }
  }
  void CalculateAngularDistance(){
    for (auto iter = stars_.begin(); iter != stars_.end(); ++iter) {
      iter->second.angular_distance = iter->second.star.AngularDistance(special_center_);
    }
  }
  int Size() const { return stars_.size(); }
  void Clear();
 private:
  float valid_region_radio_{};
  StarOnSkySphere special_center_;
  std::map<std::string, AngularDistanceStar> stars_;
};

#endif //STARMAPRECOGNITION_SRC_CORE_SPECIALCENTERSTARONSKYSPHEREGROUP_H_
