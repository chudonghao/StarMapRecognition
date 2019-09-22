//
// Created by chudonghao on 2019/9/21.
//

#ifndef STARMAPRECOGNITION_SRC_STARTABLE_H_
#define STARMAPRECOGNITION_SRC_STARTABLE_H_

#include <map>
#include <memory>
#include "StarPos.h"
#include "Descriptor2.h"
#include "DescriptorConverter.h"

#include "../log.h"

class StarTable {
 public:
  static StarTable *instance();
  std::map<std::string, StarTablePos> &Table() { return star_table_; }
  //std::map<string, Descriptor> CreateDescriptorDatabase(double max_r, int planet_num);
  template<int radial_special_size, int circular_special_size>
  std::map<std::string, Descriptor2<radial_special_size, circular_special_size>> *CreateDescriptorDatabase();
  template<int special_size>
  std::map<std::string, Descriptor3<special_size>> *CreateDescriptor3Database(float region_radio, int star_num);
 private:
  std::map<std::string, StarTablePos> star_table_;
};

template<int radial_special_size, int circular_special_size>
std::map<std::string, Descriptor2<radial_special_size, circular_special_size>> *StarTable::CreateDescriptorDatabase() {
  using namespace std;
  auto *res = new map<string, Descriptor2<radial_special_size, circular_special_size>>;
  shared_ptr<multimap<double, StarOnSkySphere>> angular_distance_star_map(new multimap<double, StarOnSkySphere>);
  int i = 0;
  for (auto &name_star_pair:star_table_) {
    if (i % 100 == 0) {
      LOG_INFO << i << '/' << star_table_.size();
    }
    angular_distance_star_map->clear();
    StarOnSkySphere star(name_star_pair.second);
    SpecialCenterStarOnSkySphereGroup scsg(star);
    // TODO 最大值
    scsg.SetValidRegionRadio(10.);
    for (const auto &star1: star_table_) {
      double angular_distance = SkySpherePos(name_star_pair.second).AngularDistance(SkySpherePos(star1.second));
      if (angular_distance < 10.)
        angular_distance_star_map->insert(make_pair(star1.second.l, StarOnSkySphere(star1.second)));
    }
    if (i == 0) {
      LOG_TRACE << "star id: " << name_star_pair.first;
      LOG_TRACE << "matched star: " << angular_distance_star_map->size();
    }
    auto pair_iter = angular_distance_star_map->begin();
    for (auto &ad_star_pair:*angular_distance_star_map) {
      //if (i == 0) {
      //  LOG_TRACE << ad_star_pair.first << " " << ad_star_pair.second.GetName();
      //}
      scsg.Add(ad_star_pair.second.GetName(), ad_star_pair.second);
    }
    DescriptorConverter<SpecialCenterStarOnSkySphereGroup, Descriptor2<radial_special_size, circular_special_size>>
        converter;
    Descriptor2<radial_special_size, circular_special_size> descriptor;
    converter.operator()(scsg, descriptor, false);
    res->emplace(star.GetName(), descriptor);
    ++i;
  }
  return res;
}

template<int special_size>
std::map<std::string, Descriptor3<special_size>> *StarTable::CreateDescriptor3Database(float region_radio,
                                                                                       int star_num) {
  using namespace std;
  if (star_num <= 0) {
    return nullptr;
  }
  auto *res = new map<string, Descriptor3<special_size>>;
  shared_ptr<multimap<double, StarOnSkySphere>> brightness_star_map(new multimap<double, StarOnSkySphere>);
  int i = 0;
  for (auto &name_star_pair:star_table_) {
    if (i % 100 == 0) {
      LOG_INFO << i << '/' << star_table_.size();
    }
    brightness_star_map->clear();
    StarOnSkySphere star(name_star_pair.second);
    SpecialCenterStarOnSkySphereGroup scsg(star);
    // TODO 最大值
    scsg.SetValidRegionRadio(region_radio);
    for (const auto &star1: star_table_) {
      double angular_distance = SkySpherePos(name_star_pair.second).AngularDistance(SkySpherePos(star1.second));
      if (angular_distance < region_radio)
        brightness_star_map->insert(make_pair(star1.second.l, StarOnSkySphere(star1.second)));
    }
    if (i == 0) {
      LOG_TRACE << "star id: " << name_star_pair.first;
      LOG_TRACE << "matched star: " << brightness_star_map->size();
    }

    // 选出最亮的若干颗
    for (auto star_iter = brightness_star_map->begin(); star_iter != brightness_star_map->end(); ++star_iter) {
      scsg.Add(star_iter->second.GetName(), star_iter->second);
      if (star_num == scsg.Size()) {
        break;
      }
    }
    DescriptorConverter<SpecialCenterStarOnSkySphereGroup, Descriptor3<special_size>> converter;
    Descriptor3<special_size> descriptor;
    //LOG_TRACE << "star: " << star.GetName() << ":" << star.GetSkySpherePos().GetLongitude() << ":"
    //          << star.GetSkySpherePos().GetLatitude();
    converter(scsg, descriptor);
    res->emplace(star.GetName(), descriptor);
    ++i;
  }
  return res;
}

#endif //STARMAPRECOGNITION_SRC_STARTABLE_H_
