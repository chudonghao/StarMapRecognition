//
// Created by cdh on 19-9-22.
//

#ifndef STARMAPRECOGNITION_SRC_CORE_STARONGRAPH_H_
#define STARMAPRECOGNITION_SRC_CORE_STARONGRAPH_H_

#include <string>
#include "StarPos.h"
#include "SpecialCenterStarOnSkySphereGroup.h"
#include "../log.h"

class StarOnGraph__ {
 public:
  StarOnGraph__() : name_(), star_graph_pos_() {}
  explicit StarOnGraph__(const StarGraphPos &star_graph_pos) : star_graph_pos_(star_graph_pos) {}
  explicit StarOnGraph__(const std::string &name) : name_(name) {}
  StarOnGraph__(const std::string &name, const StarGraphPos &star_graph_pos) : name_(name), star_graph_pos_(star_graph_pos) {}
  const std::string &GetName() const {
    return name_;
  }
  void SetName(const std::string &name) {
    name_ = name;
  }
  const StarGraphPos &GetStarGraphPos() const {
    return star_graph_pos_;
  }
  void SetStarGraphPos(const StarGraphPos &star_graph_pos) {
    star_graph_pos_ = star_graph_pos;
  }
  const SpecialCenterStarOnSkySphereGroup &GetSpecialStarGroup() const {
    return special_star_group_;
  }
  SpecialCenterStarOnSkySphereGroup &GetSpecialStarGroup() {
    return special_star_group_;
  }
  void SetSpecialStarGroup(const SpecialCenterStarOnSkySphereGroup &special_star_group) {
    if (special_star_group.GetSpecialCenter().GetName() != GetName()) {
      LOG_WARNING << "Operator wrong.";
    }
    special_star_group_ = special_star_group;
  }
  bool operator==(const StarOnGraph__ &rhs) const {
    return name_ == rhs.name_;
  }
  bool operator!=(const StarOnGraph__ &rhs) const {
    return !(rhs == *this);
  }
  StarOnSkySphere ToStarOnSkySphere(const StarGraphPos &center_star_graph_pos, double pixel_f, double graph_size);
 private:
  std::string name_;
  StarGraphPos star_graph_pos_;
  SpecialCenterStarOnSkySphereGroup special_star_group_;
};

#endif //STARMAPRECOGNITION_SRC_CORE_STARONGRAPH_H_
