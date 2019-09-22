//
// Created by chudonghao on 2019/9/21.
//

#include "StarTable.h"
#include <iostream>
#include <algorithm>
#include <pugixml.hpp>
#include <memory>
#include "StarGraph.h"
#include "DescriptorConverter.h"
#include "SpecialCenterStarOnSkySphereGroup.h"
#include "../log.h"

using namespace std;

StarTable *StarTable::instance() {
  static StarTable *star_table = nullptr;
  if (!star_table) {
    star_table = new StarTable;
    pugi::xml_document xml_document;
    pugi::xml_parse_result parse_result = xml_document.load_file("star_table.xml");

    if (!parse_result) {
      cout << "Can NOT parse xml file." << endl;
    }
    int count = 0;
    for (const auto &item : xml_document.child("star_table").children("item")) {
      string id;
      double a, b, l;
      id = item.child("id").text().as_string();
      a = item.child("a").text().as_double();
      b = item.child("b").text().as_double();
      l = item.child("l").text().as_double();
      star_table->star_table_[id] = StarTablePos{id, a, b, l};
      ++count;
    }
    cout << "Got " << count << " stars.";
  }
  return star_table;
}

//map<int, Descriptor> StarTable::CreateDescriptorDatabase(double max_r, int planet_num) {
//  map<int, Descriptor> res;
//  int i = 0;
//  for (auto &star:star_table_) {
//    ++i;
//    if (i == 100) {
//      cout << star.first << endl;
//      i = 0;
//    }
//    SkySpherePosGroupWithSpecialCenter sos_group(max_r);
//    auto *lightness_planet_map = new multimap<double, SkySpherePos>;
//    for (const auto &star1: star_table_) {
//      SkySpherePos sos;
//      Convert(star.second, star1.second, sos);
//      if (sos.pos.length() < max_r) {
//        lightness_planet_map->insert(make_pair(star1.second.l, sos));
//      }
//    }
//    auto pair_iter = lightness_planet_map->end();
//    --pair_iter;
//    int size = 0;
//    for (;; --pair_iter) {
//      //cout << pair_iter->first << endl;
//      sos_group.Add(pair_iter->second);
//      if (sos_group.size() == planet_num || pair_iter == lightness_planet_map->begin()) {
//        break;
//      }
//    }
//    res[star.first] = CreateDescriptor(sos_group);
//    delete lightness_planet_map;
//  }
//  return res;
//}
