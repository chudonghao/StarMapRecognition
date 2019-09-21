//
// Created by chudonghao on 2019/9/21.
//

#ifndef STARMAPRECOGNITION__STARGRAPH_H_
#define STARMAPRECOGNITION__STARGRAPH_H_

class StarGraph {
 public:
  struct StarOnGraph {
    std::string name;
    double pixel_x;
    double pixel_y;
  };
  bool InitFrom(const std::string &file_name, double graph_size, double max_a);
};

#endif //STARMAPRECOGNITION__STARGRAPH_H_
