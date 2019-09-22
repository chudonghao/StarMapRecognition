//
// Created by cdh on 19-9-22.
//

#ifndef STARMAPRECOGNITION_SRC_CORE_DESCRIPTOR2_H_
#define STARMAPRECOGNITION_SRC_CORE_DESCRIPTOR2_H_

#include "Descriptor.h"

template<int radial_special_size, int circular_special_size>
class Descriptor2 {
 public:
  Descriptor2() : radial_special_(), circular_special_() {}
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
 private:
  float radial_special_[radial_special_size];
  float circular_special_[circular_special_size];
};

template<>
float diff(const Descriptor2<32, 16> &, const Descriptor2<32, 16> &);

#endif //STARMAPRECOGNITION_SRC_CORE_DESCRIPTOR2_H_
