//
// Created by chudonghao on 2019/9/21.
//

#ifndef STARMAPRECOGNITION_SRC_STARGROUP_H_
#define STARMAPRECOGNITION_SRC_STARGROUP_H_

#include <osg/Group>
class Planet;

class StarGroup : public osg::Group {
 public:
  explicit StarGroup(float radio);
 private:
  void InitPlanets();
  void InitLongitudeLatitude();
  float radio_;
  std::map<std::string, Planet *> id_planet_map_;
};

#endif //STARMAPRECOGNITION_SRC_STARGROUP_H_
