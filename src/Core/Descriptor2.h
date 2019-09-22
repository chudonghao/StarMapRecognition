//
// Created by cdh on 19-9-22.
//

#ifndef STARMAPRECOGNITION_SRC_CORE_DESCRIPTOR2_H_
#define STARMAPRECOGNITION_SRC_CORE_DESCRIPTOR2_H_

#include "DescriptorConverter.h"

template<int radial_special_size, int circular_special_size>
class Descriptor2 {
 public:
  Descriptor2() : radial_special_(), circular_special_(), region_radio_() {}
  const float *GetRadialSpecial() const {
    return radial_special_;
  }
  float *GetRadialSpecial() {
    return radial_special_;
  }
  const float *GetCircularSpecial() const {
    return circular_special_;
  }
  float *GetCircularSpecial() {
    return circular_special_;
  }
  float GetRegionRadio() const {
    return region_radio_;
  }
  void SetRegionRadio(float region_radio) {
    region_radio_ = region_radio;
  }
  float Similarity1(const Descriptor2 &des) {
    float res = 0.f;
    for (int i = 0; i < radial_special_size; ++i) {
      res += radial_special_[i] * des.radial_special_[i];
    }
    return res;
  }
  float Similarity2(const Descriptor2 &des) {
    if (des.region_radio_ > region_radio_) {
      Descriptor2 des2 = des;
      des2.Reshape(region_radio_);
      return Similarity2(des2);
    }
    float max = 0.f;
    for (int shift = 0; shift < circular_special_size; ++shift) {
      float res = 0.f;
      for (int i = 0; i < circular_special_size; ++i) {
        int i2 = shift + i;
        if (i2 >= circular_special_size) {
          i2 -= circular_special_size;
        }
        res += circular_special_[i] * des.circular_special_[i2];
      }
      if (res > max) {
        max = res;
      }
    }
    return max;
  }
  int GetStarNum() const {
    return star_num_;
  }
  void SetStarNum(int star_num) {
    star_num_ = star_num;
  }
  const SpecialCenterStarOnSkySphereGroup &GetSpecialCenterStarOnSkySphereGroup() const {
    return special_center_star_on_sky_sphere_group_;
  }
  void SetSpecialCenterStarOnSkySphereGroup(const SpecialCenterStarOnSkySphereGroup &special_center_star_on_sky_sphere_group) {
    special_center_star_on_sky_sphere_group_ = special_center_star_on_sky_sphere_group;
  }
  void Reshape(float region_radio) {
    region_radio_ = region_radio;
    special_center_star_on_sky_sphere_group_.SetValidRegionRadio(region_radio_);
    star_num_ = special_center_star_on_sky_sphere_group_.GetStaresOnSkyShphere().size();
    DescriptorConverter<SpecialCenterStarOnSkySphereGroup, Descriptor2<radial_special_size, circular_special_size>>converter;
    converter(special_center_star_on_sky_sphere_group_, *this, true);
  }
 private:
  int star_num_;
  float region_radio_;
  float radial_special_[radial_special_size];
  float circular_special_[circular_special_size];
  SpecialCenterStarOnSkySphereGroup special_center_star_on_sky_sphere_group_;
};

#endif //STARMAPRECOGNITION_SRC_CORE_DESCRIPTOR2_H_
