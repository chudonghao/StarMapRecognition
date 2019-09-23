//
// Created by cdh on 19-9-22.
//

#include "StarOnGraph.h"

using namespace std;
//using namespace osg;

StarOnSkySphere StarOnGraph__::ToStarOnSkySphere(const StarGraphPos &center_star_graph_pos, double pixel_f, double graph_size) const {
  SkySpherePos sky_sphere_pos;
  Convert(pixel_f, StarGraphPos(graph_size/2., graph_size/2.), center_star_graph_pos, star_graph_pos_, sky_sphere_pos);
  return StarOnSkySphere(name_, sky_sphere_pos);
}
StarOnGraph__::StarOnGraph__(StarTablePos pos) {
  name_ = pos.id;
  star_graph_pos_ = StarGraphPos(pos);
}
