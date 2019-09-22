//
// Created by chudonghao on 2019/9/23.
//

#ifndef STARMAPRECOGNITION_SRC_CORE_DESCRIPTOR3_H_
#define STARMAPRECOGNITION_SRC_CORE_DESCRIPTOR3_H_

#include "SpecialCenterStarOnSkySphereGroup.h"

template<int size>
class Descriptor3 {
 public:
  Descriptor3() {}
  const SpecialCenterStarOnSkySphereGroup &GetSpecialCenterStarOnSkySphereGroup() const {
    return special_center_star_on_sky_sphere_group_;
  }
  void SetSpecialCenterStarOnSkySphereGroup(const SpecialCenterStarOnSkySphereGroup &special_center_star_on_sky_sphere_group) {
    special_center_star_on_sky_sphere_group_ = special_center_star_on_sky_sphere_group;
  }
  float Similarity(const Descriptor3 &des) {
    float res = 0.f;
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        res += special_[i][j] * des.special_[i][j];
      }
    }
    return res;
  }
  float *GetSpecials() { return &special_[0][0]; }
  int GetStarNum() const {
    return special_center_star_on_sky_sphere_group_.GetStaresOnSkyShphere().size();
  }
  float GetRegionRadio() const {
    return special_center_star_on_sky_sphere_group_.GetValidRegionRadio();
  }
  void SetRegionRadio(float region_radio) {
    special_center_star_on_sky_sphere_group_.SetValidRegionRadio(region_radio);
  }
  const osg::Vec2d &GetSpecialDir() const {
    return special_dir_;
  }
  void SetSpecialDir(const osg::Vec2d &special_dir) {
    special_dir_ = special_dir;
  }
 private:
  osg::Vec2d special_dir_;
  float special_[size][size]{};
  SpecialCenterStarOnSkySphereGroup special_center_star_on_sky_sphere_group_;
};

#endif //STARMAPRECOGNITION_SRC_CORE_DESCRIPTOR3_H_
