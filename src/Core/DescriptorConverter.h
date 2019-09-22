//
// Created by cdh on 19-9-22.
//

#ifndef STARMAPRECOGNITION_SRC_CORE_DESCRIPTORCONVERTER_H_
#define STARMAPRECOGNITION_SRC_CORE_DESCRIPTORCONVERTER_H_

#include "Descriptor2.h"

template<typename FromT_, typename DescriptorT_>
class DescriptorConverter {};

template<>
class DescriptorConverter<SpecialCenterStarOnSkySphereGroup, Descriptor2<32, 16>> {
 public:
  DescriptorConverter() {}
  DescriptorConverter(bool debug) : debug(debug) {}
  typedef SpecialCenterStarOnSkySphereGroup FromT;
  typedef Descriptor2<32, 16> DescriptorT;
  bool operator()(const FromT &from, DescriptorT &);
 private:
  bool debug{};
};

#endif //STARMAPRECOGNITION_SRC_CORE_DESCRIPTORCONVERTER_H_
