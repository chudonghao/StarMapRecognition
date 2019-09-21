//
// Created by chudonghao on 2019/9/21.
//

#include "StarTable.h"
#include <iostream>
#include <pugixml.hpp>

using namespace std;
map<int, PlanetData> &StarTable() {
  static map<int, PlanetData> star_table;
  if (star_table.empty()) {

    pugi::xml_document xml_document;
    pugi::xml_parse_result parse_result = xml_document.load_file("star_table.xml");

    if (!parse_result) {
      cout << "Can NOT parse xml file.";
    }
    int count = 0;
    for (const auto &item : xml_document.child("star_table").children("item")) {
      int id;
      double a, b, l;
      id = item.child("id").text().as_int();
      a = item.child("a").text().as_double();
      b = item.child("b").text().as_double();
      l = item.child("l").text().as_double();
      star_table[id] = PlanetData{id, a, b, l};
      ++count;
    }

    cout << "Got " << count << " stars.";
  }
  return star_table;
}
