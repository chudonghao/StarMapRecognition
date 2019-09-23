//
// Created by chudonghao on 2019/9/21.
//

#ifndef STARMAPRECOGNITION__STARGRAPH_H_
#define STARMAPRECOGNITION__STARGRAPH_H_

#include <string>
#include <vector>
#include <osg/Group>
#include "Descriptor.h"
#include "StarPos.h"
#include "StarOnGraph.h"
#include "Descriptor2.h"
#include "Descriptor3.h"
#include "DescriptorConverter.h"

//class SkySpherePosGroupWithSpecialCenter {
// public:
//  SkySpherePosGroupWithSpecialCenter(double region_radio);
//  SkySpherePosGroupWithSpecialCenter() : region_radio_(0.f) {}
//  void Clear();
//  void swap(SkySpherePosGroupWithSpecialCenter &group);
//  void SetRegionRadio1(double region_radio);
//  void Add(StarSkySpherePos star_on_sky_sphere);
//  double GetRegionRadio() const;
//  double GetMoreAppropriateRegionRadio() const;
//  int size() const { return stars_.size(); }
//  void SetSpecialCenterName(const string &special_center_name);
//  const vector<StarSkySpherePos> &GetStars() const;
// private:
//  std::string special_center_name_;
//  std::vector<StarSkySpherePos> stars_;
//  double region_radio_;
//
//  void RemoveInvalidStars();
//  int GetAppropriateSize() const;
//};
//
//void Convert(const StarTableData &special_center,
//             const StarTableData &from,
//             StarSkySpherePos &to);
//
//void CreateStarOnSkySphereGroupWithSpecialCenter(const std::vector<StarGraphPos> &stars_on_graph,
//                                                 double pixel_f,
//                                                 const StarGraphPos &graph_center,
//                                                 const StarGraphPos &special_center,
//                                                 SkySpherePosGroupWithSpecialCenter &res);
//
//void CreateStarOnSkySphereGroupWithSpecialCenter(const std::vector<StarTableData> &stars,
//                                                 const StarTableData &special_center,
//                                                 double max_r,
//                                                 int planet_num,
//                                                 SkySpherePosGroupWithSpecialCenter &res);
//
//Descriptor CreateDescriptor(const SkySpherePosGroupWithSpecialCenter &sos_group);

class StarGraph {
 public:
  StarGraph();
  bool InitFrom(const std::string &file_name, double graph_size, double graph_a);
  bool DebugInitFromStarTable(StarSkySpherePos pos, double graph_size, double graph_a);
  void DebugShow(string debug_name);
  void DebugShow_ToSkySpherePos(string debug_name);
  //Descriptor GetDescriptor();

  SpecialCenterStarOnSkySphereGroup GetMasterGroup();
  SpecialCenterStarOnSkySphereGroup GetStarGroupRelativeToViewCenter();

  template<int s1, int s2>
  std::map<std::string, Descriptor2<s1, s2>> *GetDescriptor2s() {
    // TODO
    auto *res = new map<string, Descriptor2<s1, s2>>;
    for (auto &name_star_pair:stars_) {
      DescriptorConverter<SpecialCenterStarOnSkySphereGroup, Descriptor2<s1, s2>> converter;
      Descriptor2<s1, s2> *r = new Descriptor2<s1, s2>;
      if (converter(name_star_pair.second.GetSpecialStarGroup(), *r, true)) {
        if (r->GetStarNum() > 0) {
          res->emplace(name_star_pair.first, *r);
        }
      }
      delete r;
    }
    return res;
  }
 private:

  std::map<std::string, StarOnGraph__> stars_;
  SpecialCenterStarOnSkySphereGroup star_group_relative_to_view_center_;
  int graph_size_{};
  float graph_fovy_{};
  float pixel_f_;
  void Reset(double graph_size, double graph_a);
  void ResetStarOnSkySphereGroupRelativeToViewCenter();
  void ResetPerStarGroup();
};

#endif //STARMAPRECOGNITION__STARGRAPH_H_
