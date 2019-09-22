//
// Created by cdh on 19-9-22.
//

#include "Descriptor2.h"

template<>
bool CalculateDescriptor(const SpecialCenterStarOnSkySphereGroup &, Descriptor2<32, 16> &) {
  // TODO
  return false;
}
template<>
float diff(const Descriptor2<32, 16> &, const Descriptor2<32, 16> &) {
  // TODO
  return 0;
}
