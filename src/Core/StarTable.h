//
// Created by chudonghao on 2019/9/21.
//

#ifndef STARMAPRECOGNITION_SRC_STARTABLE_H_
#define STARMAPRECOGNITION_SRC_STARTABLE_H_

#include <map>
#include "StarPos.h"
#include "Descriptor2.h"

class StarTable {
 public:
  static StarTable *instance();
  std::map<string, StarTablePos> &Table() { return star_table_; }
  //std::map<string, Descriptor> CreateDescriptorDatabase(double max_r, int planet_num);
  std::map<string,Descriptor2<32,16>> CreateDescriptorDatabase();
 private:
  std::map<string, StarTablePos> star_table_;
};

#endif //STARMAPRECOGNITION_SRC_STARTABLE_H_
