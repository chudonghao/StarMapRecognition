//
// Created by cdh on 19-9-22.
//

#ifndef STARMAPRECOGNITION_SRC_CORE_DESCRIPTORCONVERTER_H_
#define STARMAPRECOGNITION_SRC_CORE_DESCRIPTORCONVERTER_H_

#include "../log.h"
#include "../Util/CommonFunc.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <osg/Vec2>
#include "SpecialCenterStarOnSkySphereGroup.h"

template<typename FromT_, typename DescriptorT_>
class DescriptorConverter {};

template <int,int>
class Descriptor2;

template<int radial_special_size, int circular_special_size>
class DescriptorConverter<SpecialCenterStarOnSkySphereGroup, Descriptor2<radial_special_size, circular_special_size>> {
 public:
  DescriptorConverter() {}
  DescriptorConverter(bool debug) : debug(debug) {}
  typedef SpecialCenterStarOnSkySphereGroup FromT;
  typedef Descriptor2<radial_special_size, circular_special_size> DescriptorT;
  bool operator()(const FromT &from, DescriptorT &, bool convert_circular_);
 private:
  bool debug{};
};

template<int radial_special_size, int circular_special_size>
bool DescriptorConverter<SpecialCenterStarOnSkySphereGroup,
                         Descriptor2<radial_special_size, circular_special_size>>::
operator()(
    const SpecialCenterStarOnSkySphereGroup &from,
    Descriptor2<radial_special_size, circular_special_size> &des,
    bool convert_circular_
) {
  using namespace std;
  using namespace osg;
  des.SetSpecialCenterStarOnSkySphereGroup(from);
  float r = from.GetValidRegionRadio();
  for (int i = 0; i < radial_special_size; ++i) {
    des.GetRadialSpecial()[i] = 0.f;
  }
  for (int i = 0; i < circular_special_size; ++i) {
    des.GetCircularSpecial()[i] = 0.f;
  }
  auto stars = from.GetStaresOnSkyShphere();
  int star_count = 0;
  for (auto &star_struct:stars) {
    if (from.GetSpecialCenter().GetName() == star_struct.second.star.GetName()) {
      continue;
    }
    if (star_struct.second.angular_distance < r) {
      // 径向
      // 0->10度分布到0->31
      des.GetRadialSpecial()[static_cast<int>(star_struct.second.angular_distance / 10.f * radial_special_size)] = 1.f;
      ++star_count;
      if (convert_circular_) {
        // 环向
        // 0->360度分布到0->circular_special_size
        SkySpherePos pos;
        // TODO 求解正确参数
        Convert(from.GetSpecialCenter().GetSkySpherePos(), star_struct.second.star.GetSkySpherePos(), pos);
        //LOG_TRACE << pos.GetLongitude() << " : " << pos.GetLatitude();
        auto rad = Radians(Vec2(1.f, 0.f), Vec2(pos));
        if (pos.y() < 0.) {
          rad = 2. * PI - rad;
        }
        //LOG_DEBUG << pos.GetLongitude() << ":" << pos.GetLatitude() << ":" << rad;
        des.GetCircularSpecial()[static_cast<int>(rad / (PI * 2.) * circular_special_size)] = 1.f;
      }
    } else {
      //LOG_TRACE << star_struct.second.angular_distance;
    }
  }
  des.SetStarNum(star_count);
  des.SetRegionRadio(from.GetValidRegionRadio());
  if (convert_circular_) {
    std::stringstream ss;
    for (int i = 0; i < radial_special_size; ++i) {
      ss << std::setw(2) << des.GetRadialSpecial()[i];
    }
    ss << " ====== ";
    for (int i = 0; i < circular_special_size; ++i) {
      ss << std::setw(2) << des.GetCircularSpecial()[i];
    }
    //LOG_DEBUG << from.GetSpecialCenter().GetName() << " : " << stars.size() << " : " << r << " : " << star_count << " : " << ss.str();
  }
  return true;
}

#endif //STARMAPRECOGNITION_SRC_CORE_DESCRIPTORCONVERTER_H_
