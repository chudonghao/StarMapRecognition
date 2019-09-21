//
// Created by chudonghao on 2019/9/21.
//

#include <string>
#include <pugixml.hpp>
#include <iostream>
#include <vector>
#include <osg/Vec3>
#include <osg/Vec3d>
#include "StarGraph.h"
using namespace std;
using namespace osg;

struct StarOnSkySphere {
  string name;
  double a;
  double b;
};

struct StarOnSkySphereGroup {
  vector<StarOnSkySphere> stars;
  double max_long;
  double max_lat;
};

void Convert(double pixel_f,
             const StarGraph::StarOnGraph &center,
             const StarGraph::StarOnGraph &from,
             StarOnSkySphere &to) {
  
}

bool StarGraph::InitFrom(const std::string &file_name, double graph_size, double max_a) {
  pugi::xml_document xml_document;
  pugi::xml_parse_result result = xml_document.load_file(file_name.c_str());
  if (!result) {
    cout << "Can NOT read star graph file. file name = " << file_name << " ERROR: " << result.description() << endl;
  }
  for (const auto &item : xml_document.child("").children("")) {
    string name = item.child("").text().as_string();
    double pixel_x = item.child("").text().as_double();
    double pixel_y = item.child("").text().as_double();
    StarOnGraph star_on_graph = {name, pixel_x, pixel_y};
  }
  return false;
}
