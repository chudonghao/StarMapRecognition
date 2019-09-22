//
// Created by cdh on 19-9-22.
//

#include <iomanip>
#include "DescriptorConverter.h"
#include "../log.h"

bool DescriptorConverter<SpecialCenterStarOnSkySphereGroup, Descriptor2<32, 16>>::operator()(const SpecialCenterStarOnSkySphereGroup &from,
                                                                                             Descriptor2<32, 16> &des) {
  // TODO
  float r = from.GetValidRegionRadio();
  for (int i = 0; i < 32; ++i) {
    des.GetRadialSpecial()[i] = 0.f;
  }
  for (int i = 0; i < 16; ++i) {
    des.GetCircularSpecial()[i] = 0.f;
  }
  auto stars = from.GetStaresOnSkyShphere();
  int star_count = 0;
  for (auto &star_struct:stars) {
    if (from.GetSpecialCenter().GetName() == star_struct.second.star.GetName()) {
      continue;
    }
    // 径向
    // 0->10度分布到0->31
    if (star_struct.second.angular_distance < r) {
      des.GetRadialSpecial()[static_cast<int>(star_struct.second.angular_distance/10.f*32.f)] = 1.f;
      ++star_count;
    } else {
      //LOG_TRACE << star_struct.second.angular_distance;
    }
    // 环向
    // 0->360度分布到0->16
    if (star_struct.second.angular_distance < r) {


      des.GetCircularSpecial()[static_cast<int>(star_struct.second.angular_distance/10.f*32.f)] = 1.f;
      ++star_count;
    } else {
      //LOG_TRACE << star_struct.second.angular_distance;
    }
  }
  stringstream ss;
  for (int i = 0; i < 32; ++i) {
    ss << setw(2) << des.GetRadialSpecial()[i];
  }
  LOG_DEBUG << from.GetSpecialCenter().GetName() << " : " << stars.size() << " : " << r << " : " << star_count << " : " << ss.str();
  return false;
}
