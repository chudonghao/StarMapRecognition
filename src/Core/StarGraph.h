//
// Created by chudonghao on 2019/9/21.
//

#ifndef STARMAPRECOGNITION__STARGRAPH_H_
#define STARMAPRECOGNITION__STARGRAPH_H_

#include <string>
#include <vector>
#include "Descriptor.h"
#include "StarPos.h"
#include "StarOnGraph.h"
#include "Descriptor2.h"

//class SkySpherePosGroupWithSpecialCenter {
// public:
//  SkySpherePosGroupWithSpecialCenter(double region_radio);
//  SkySpherePosGroupWithSpecialCenter() : region_radio_(0.f) {}
//  void Clear();
//  void swap(SkySpherePosGroupWithSpecialCenter &group);
//  void SetRegionRadio1(double region_radio);
//  void Add(SkySpherePos star_on_sky_sphere);
//  double GetRegionRadio() const;
//  double GetMoreAppropriateRegionRadio() const;
//  int size() const { return stars_.size(); }
//  void SetSpecialCenterName(const string &special_center_name);
//  const vector<SkySpherePos> &GetStars() const;
// private:
//  std::string special_center_name_;
//  std::vector<SkySpherePos> stars_;
//  double region_radio_;
//
//  void RemoveInvalidStars();
//  int GetAppropriateSize() const;
//};
//
//void Convert(const StarTablePos &special_center,
//             const StarTablePos &from,
//             SkySpherePos &to);
//
//void CreateStarOnSkySphereGroupWithSpecialCenter(const std::vector<StarGraphPos> &stars_on_graph,
//                                                 double pixel_f,
//                                                 const StarGraphPos &graph_center,
//                                                 const StarGraphPos &special_center,
//                                                 SkySpherePosGroupWithSpecialCenter &res);
//
//void CreateStarOnSkySphereGroupWithSpecialCenter(const std::vector<StarTablePos> &stars,
//                                                 const StarTablePos &special_center,
//                                                 double max_r,
//                                                 int planet_num,
//                                                 SkySpherePosGroupWithSpecialCenter &res);
//
//Descriptor CreateDescriptor(const SkySpherePosGroupWithSpecialCenter &sos_group);

class StarGraph {
 public:
  StarGraph();
  bool InitFrom(const std::string &file_name, double graph_size, double graph_a);
  //Descriptor GetDescriptor();
  std::map<std::string, Descriptor2<32, 16>> GetDescriptors();
 private:

  std::map<std::string, StarOnGraph__> stars_;
  int graph_size_{};
  float graph_a_{};
};

#endif //STARMAPRECOGNITION__STARGRAPH_H_
