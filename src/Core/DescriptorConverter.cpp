//
// Created by cdh on 19-9-22.
//

#include "DescriptorConverter.h"
#include "../log.h"

bool DescriptorConverter<SpecialCenterStarOnSkySphereGroup, Descriptor2<32, 16>>::operator()(const SpecialCenterStarOnSkySphereGroup &from,
                                                                                             Descriptor2<32, 16> &) {
  // TODO
  float r = from.GetValidRegionRadio();
  if (debug)
    LOG_TRACE << r;
  from.GetSpecialCenter();
  return false;
}
