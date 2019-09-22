//
// Created by chudonghao on 2019/9/21.
//

#ifndef STARMAPRECOGNITION_SRC_STARTABLE_H_
#define STARMAPRECOGNITION_SRC_STARTABLE_H_

#include <map>
#include "StarPos.h"
#include "Descriptor.h"

class StarTable {
 public:
  static StarTable *instance();
  std::map<int, StarTablePos> &Table() { return star_table_; }
  std::map<int, Descriptor> CreateDescriptorDatabase(double max_r, int planet_num);

 private:
  std::map<int, StarTablePos> star_table_;
};

#endif //STARMAPRECOGNITION_SRC_STARTABLE_H_
