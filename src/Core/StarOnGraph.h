//
// Created by cdh on 19-9-22.
//

#ifndef STARMAPRECOGNITION_SRC_CORE_STARONGRAPH_H_
#define STARMAPRECOGNITION_SRC_CORE_STARONGRAPH_H_

#include <string>
#include "StarPos.h"

class StarOnGraph__ {
 public:
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
 private:
  std::string name_;
  StarGraphPos star_graph_pos_;
};

#endif //STARMAPRECOGNITION_SRC_CORE_STARONGRAPH_H_
