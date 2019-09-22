//
// Created by chudonghao on 2019/9/21.
//

#include "StarGraph.h"
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <pugixml.hpp>
#include <osg/Vec3>
#include <osg/Vec3d>
#include <osg/Matrixd>
#include <osg/Matrix>
#include <opencv/cv.h>
#include <osg/Vec2d>
#include <iomanip>

#include "../Util/CommonFunc.h"
#include "../log.h"
#include "DescriptorConverter.h"

using namespace std;
using namespace osg;

//
//void Convert(const StarTablePos &special_center, const StarTablePos &from, SkySpherePos &to) {
//  auto rotate_a = Matrix::rotate(DegreesToRadians(special_center.a), Vec3d(0., 0., 1.));
//  //auto rotate_axis_ny = Vec3d(0., -1., 0.)*rotate_a;
//  auto rotate_b = Matrix::rotate(DegreesToRadians(special_center.b), Vec3d(0., -1., 0.));
//  //Vec3d eye = Vec3d();
//  //Vec3d center = Vec3d(1., 0., 0.)*rotate_b*rotate_a;
//  Vec3d axis_y = Vec3d(0., 0., 1.)*rotate_b*rotate_a;
//  Vec3d axis_x = Vec3d(0., -1., 0.)*rotate_b*rotate_a;
//  auto rotate_a2 = Matrix::rotate(DegreesToRadians(from.a), Vec3d(0., 0., 1.));
//  auto rotate_b2 = Matrix::rotate(DegreesToRadians(from.b), Vec3d(0., -1., 0.));
//  Vec3d p = Vec3d(1., 0., 0.)*rotate_b2*rotate_a2;
//  double angle = RadiansToDegrees(Radians(axis_x, p));
//  double angle2 = RadiansToDegrees(Radians(axis_y, p));
//  double lat, lon;
//  lon = 90. - angle;
//  lat = 90. - angle2;
//  to.name = to_string(from.id);
//  to.pos = Vec2d(lon, lat);
//}
//
//void CreateStarOnSkySphereGroupWithSpecialCenter(const vector<StarGraphPos> &stars_on_graph,
//                                                 double pixel_f,
//                                                 const StarGraphPos &graph_center,
//                                                 const StarGraphPos &special_center,
//                                                 SkySpherePosGroupWithSpecialCenter &res) {  // 计算有效半径
//
//  StarGraphPos special_center_left_bottom{"", special_center.pixel_x, special_center.pixel_y};
//  if (special_center.pixel_x > graph_center.pixel_x) {
//    special_center_left_bottom.pixel_x = graph_center.pixel_x*2. - special_center.pixel_x;
//  }
//  if (special_center.pixel_y > graph_center.pixel_y) {
//    special_center_left_bottom.pixel_y = graph_center.pixel_y*2. - special_center.pixel_y;
//  }
//  SkySpherePos sos_max;
//  Convert(pixel_f, graph_center, special_center_left_bottom, StarGraphPos{"", 0., 0.}, sos_max);
//  res.SetRegionRadio1(min(-sos_max.pos.x(), -sos_max.pos.y()));
//
//  res.SetSpecialCenterName(special_center.name);
//  SkySpherePos sos;
//  for (auto &sog : stars_on_graph) {
//    if (sog.name != special_center.name) {
//      Convert(pixel_f, graph_center, special_center, sog, sos);
//      res.Add(sos);
//    }
//  }
//};

bool StarGraph::InitFrom(const std::string &file_name, double graph_size, double graph_a) {
  stars_.clear();
  graph_size_ = graph_size;
  graph_a_ = graph_a;
  double pixel_f = (graph_size/2.)/tan(DegreesToRadians(graph_a)/2.);
  pugi::xml_document xml_document;
  pugi::xml_parse_result result = xml_document.load_file(file_name.c_str());
  if (!result) {
    LOG_WARNING << "Can NOT read star graph file. file name = " << file_name << " ERROR: " << result.description();
    return false;
  }
  for (const auto &row_node :xml_document.select_nodes("//Table/Row")) {
    string name = "";
    double pixel_x = 0.;
    double pixel_y = 0.;
    int i = 0;
    for (const auto &data_node:row_node.node().select_nodes("Cell/Data")) {
      if (i == 0) {
        name = data_node.node().text().as_string();
      } else if (i == 1) {
        pixel_x = data_node.node().text().as_double();
      } else if (i == 2) {
        pixel_y = data_node.node().text().as_double();
      }
      ++i;
      if (i == 3) {
        break;
      }
    }
    if (!name.empty()) {
      StarOnGraph__ star(name, StarGraphPos(pixel_x, pixel_y));
      stars_[name] = star;
    } else {
      LOG_WARNING << "!name";
    }
  }

  // 以视点中心为0，0，将星星映射到SkySphere坐标
  std::map<string, StarOnSkySphere> stars_on_sky_sphere;
  for (auto &name_star_pair: stars_) {
    StarOnSkySphere star_on_sky_sphere = name_star_pair.second.ToStarOnSkySphere(StarGraphPos(graph_size_/2., graph_size_/2.), pixel_f, graph_size_);
    stars_on_sky_sphere[name_star_pair.first] = star_on_sky_sphere;
    //LOG_TRACE << star_on_sky_sphere.GetName();
    //LOG_TRACE << star_on_sky_sphere.GetSkySpherePos().x();
    //LOG_TRACE << star_on_sky_sphere.GetSkySpherePos().y();
  }

  for (auto &name_star_pair: stars_) {
    auto &current_star_name = name_star_pair.first;
    auto &current_star = name_star_pair.second;
    SpecialCenterStarOnSkySphereGroup scsg;
    // 计算有效半径
    const StarGraphPos &special_center = current_star.GetStarGraphPos();
    StarGraphPos graph_center = StarGraphPos(graph_size_/2., graph_size_/2.);
    StarGraphPos special_center_left_bottom(special_center.GetPixelX(), special_center.GetPixelY());
    if (special_center.GetPixelX() > graph_center.GetPixelX()) {
      special_center_left_bottom.SetPixelX(graph_size_ - special_center.GetPixelX());
    }
    if (special_center.GetPixelY() > graph_center.GetPixelY()) {
      special_center_left_bottom.SetPixelY(graph_size_ - special_center.GetPixelY());
    }
    SkySpherePos sky_sphere_pos;
    Convert(pixel_f, graph_center, special_center_left_bottom, StarGraphPos(0., 0.), sky_sphere_pos);
    // 设置有效半径
    scsg.SetValidRegionRadio(min(-sky_sphere_pos.GetLongitude(), -sky_sphere_pos.GetLatitude()));
    // 设置中心星为当前星星
    scsg.SetSpecialCenter(stars_on_sky_sphere[current_star_name]);
    //LOG_TRACE << "special center: "<<stars_on_sky_sphere[current_star_name].GetName();
    //LOG_TRACE << "special center lon: "<<stars_on_sky_sphere[current_star_name].GetSkySpherePos().GetLongitude();
    //LOG_TRACE << "special center lat: "<<stars_on_sky_sphere[current_star_name].GetSkySpherePos().GetLatitude();
    //LOG_TRACE << "valid region radio: " << scsg.GetValidRegionRadio();
    for (auto &star_on_sky_sphere_pair : stars_on_sky_sphere) {
      scsg.Add(star_on_sky_sphere_pair.first, star_on_sky_sphere_pair.second);
    }
    current_star.SetSpecialStarGroup(scsg);
    //SkySpherePosGroupWithSpecialCenter sos_group;
    //LOG_TRACE << "=============================";
    //for (auto &star : sos_group.stars) {
    //  LOG_TRACE << star.name << ":" << star.pos.x() << ":" << star.pos.y();
    //}
    //LOG_TRACE << "=============================";
    //for (auto &star : sos_group.stars) {
    //  LOG_TRACE << star.name << ":" << star.pos.x() << ":" << star.pos.y();
    //}
    //LOG_TRACE << "=============================";
    //LOG_TRACE << sos_group.max_lon;
    //LOG_TRACE << sos_group.max_lat;
    //LOG_TRACE << sos_group.GetAppropriateRadio();
    //LOG_TRACE << sos_group.GetAppropriateSize();

  }
  //LOG_TRACE << "!!!!!!!!!!!!!!!!!!!!!!!!!!!";
  //LOG_TRACE << "max_count: " << max_count;

  return true;
}

StarGraph::StarGraph() {}

//void CreateStarOnSkySphereGroupWithSpecialCenter(const std::vector<StarTablePos> &stars,
//                                                 const StarTablePos &special_center,
//                                                 double max_r,
//                                                 int planet_num,
//                                                 SkySpherePosGroupWithSpecialCenter &res) {
//  multimap<double, SkySpherePos> lightness_planet_map;
//  for (const auto &star: stars) {
//    SkySpherePos sos;
//    Convert(special_center, star, sos);
//    if (sos.pos.length() < max_r) {
//      lightness_planet_map.insert(make_pair(star.l, sos));
//    }
//  }
//  auto pair_iter = lightness_planet_map.end();
//  --pair_iter;
//  int size = 0;
//  for (; pair_iter != lightness_planet_map.end(); --pair_iter) {
//    LOG_TRACE << pair_iter->first;
//    res.Add(pair_iter->second);
//    ++size;
//    if (size == planet_num) {
//      break;
//    }
//  }
//}
//Descriptor CreateDescriptor(const SkySpherePosGroupWithSpecialCenter &sos_group) {
//  using namespace cv;
//  const static int column_size = 24;
//
//  Descriptor res;
//  if (sos_group.size() <= 0) {
//    return res;
//  }
//
//  auto data_mat = Mat_<double>(2, sos_group.size() + 1);
//  data_mat.at<double>(0, 0) = 0;// center
//  data_mat.at<double>(1, 0) = 0;
//  int i = 1;
//  for (const auto &star  : sos_group.GetStars()) {
//    data_mat.at<double>(0, i) = star.pos.x();
//    data_mat.at<double>(1, i) = star.pos.y();
//    ++i;
//  }
//  Mat data = std::move(data_mat);
//
//  PCA pca(data, Mat(), CV_PCA_DATA_AS_COL, 1);
//  Mat dst = pca.project(data);
//  double tar = 0.;
//  for (int i = 0; i < dst.size[0]; ++i) {
//    tar += dst.at<double>(i);
//  }
//
//  //LOG_TRACE << tar;
//  //LOG_TRACE << pca.project(data);
//  //LOG_TRACE << pca.eigenvectors;
//  //LOG_TRACE << pca.mean;
//  //LOG_TRACE << pca.eigenvalues;
//  osg::Vec3d dir(pca.eigenvectors.at<double>(0), pca.eigenvectors.at<double>(1), 0.);
//  if (tar < 0.) {
//    dir.x() = -dir.x();
//    dir.y() = -dir.y();
//  }
//  dir.normalize();
//  Matrixd rotate = Matrixd::rotate(dir, osg::Vec3d(1., 0., 0.));
//  float *data_;
//  data_ = new float[column_size*column_size];
//  for (int i = 0; i < column_size; ++i) {
//    for (int j = 0; j < column_size; ++j) {
//      data_[i*column_size + j] = 0.f;
//    }
//  }
//  auto max_r = sos_group.GetRegionRadio();
//  for (auto &star: sos_group.GetStars()) {
//    osg::Vec3d pos = osg::Vec3d(star.pos, 0.);
//    pos = pos*rotate;
//    int i = pos.x()/sos_group.GetRegionRadio()*(column_size/2.) + column_size/2.;
//    int i1;
//    int j = pos.y()/sos_group.GetRegionRadio()*(column_size/2.) + column_size/2.;
//    int j1;
//    data_[i*column_size + j] = 1.f;
//    i1 = i - 1;
//    j1 = j - 1;
//    if (i1 > 0 && j1 > 0) {
//      data_[i1*column_size + j1] += 0.25f;
//    }
//    i1 = i + 1;
//    j1 = j - 1;
//    if (i1 < column_size && j1 > 0) {
//      data_[i1*column_size + j1] += 0.25f;
//    }
//    i1 = i - 1;
//    j1 = j + 1;
//    if (i1 > 0 && j1 < column_size) {
//      data_[i1*column_size + j1] += 0.25f;
//    }
//    i1 = i + 1;
//    j1 = j + 1;
//    if (i1 < column_size && j1 < column_size) {
//      data_[i1*column_size + j1] += 0.25f;
//    }
//    i1 = i;
//    j1 = j - 1;
//    if (j1 > 0) {
//      data_[i1*column_size + j1] += 0.5f;
//    }
//    i1 = i;
//    j1 = j + 1;
//    if (j1 < column_size) {
//      data_[i1*column_size + j1] += 0.5f;
//    }
//    i1 = i - 1;
//    j1 = j;
//    if (i1 > 0) {
//      data_[i1*column_size + j1] += 0.5f;
//    }
//    i1 = i + 1;
//    j1 = j;
//    if (i1 < column_size) {
//      data_[i1*column_size + j1] += 0.5f;
//    }
//  }
//  res.SetDataColumnSize(column_size);
//  res.SetData(data_);
//  res.SetFeatureVec(osg::Vec2d(dir.x(), dir.y()));
//  res.SetRadio(sos_group.GetRegionRadio());
//  res.SetStarSize(sos_group.size());
//  //for (int i = column_size - 1; i >= 0; --i) {
//  //  for (int j = 0; j < column_size; ++j) {
//  //    LOG_TRACE << setw(5) << data_[i*column_size + j] << "\t";
//  //  }
//  //  LOG_TRACE;
//  //}
//  //LOG_TRACE << dir.x() << ":" << dir.y();
//  return res;
//}

//StarGraph::StarGraph() = default;
//
//std::map<std::string, Descriptor2<32, 16>> StarGraph::GetDescriptor2s() {
//  std::map<std::string, Descriptor2<32, 16>> res;
//  DescriptorConverter<SpecialCenterStarOnSkySphereGroup, Descriptor2<32, 16>> converter;
//  for (auto &pair:stars_) {
//    Descriptor2<32, 16> des;
//    if (converter(pair.second.GetSpecialStarGroup(), des)) {
//      res[pair.first] = des;
//    } else {
//      LOG_INFO << "Convert failed.";
//    }
//  }
//  return res;
//}
//
//void SkySpherePosGroupWithSpecialCenter::RemoveInvalidStars() {
//  auto star_iter = stars_.begin();
//  for (; star_iter != stars_.end(); ++star_iter) {
//    if (sqrt(star_iter->pos.x()*star_iter->pos.x() + star_iter->pos.y()*star_iter->pos.y()) > region_radio_) {
//      auto old_star_iter = star_iter - 1;
//      if (old_star_iter == stars_.end()) {
//        old_star_iter = stars_.begin();
//      }
//      stars_.erase(star_iter);
//      star_iter = old_star_iter;
//    }
//  }
//}
//double SkySpherePosGroupWithSpecialCenter::GetMoreAppropriateRegionRadio() const {
//  double max2 = 0.;
//  auto star_iter = stars_.begin();
//  for (; star_iter != stars_.end(); ++star_iter) {
//    if (sqrt(star_iter->pos.x()*star_iter->pos.x() + star_iter->pos.y()*star_iter->pos.y()) > region_radio_) {
//      // 不合适的星点
//    } else {
//      // 更准确的半径计算方法
//      double r = star_iter->pos.length();
//      if (max2 < r) {
//        max2 = r;
//      }
//    }
//  }
//  if (max2 == 0.) {
//    return 0.;
//  } else {
//    return (region_radio_ + max2)/2.;
//  }
//}
//int SkySpherePosGroupWithSpecialCenter::GetAppropriateSize() const {
//  const_cast<SkySpherePosGroupWithSpecialCenter *>(this)->RemoveInvalidStars();
//  return stars_.size();
//}
//void SkySpherePosGroupWithSpecialCenter::swap(SkySpherePosGroupWithSpecialCenter &group) {
//  special_center_name_.swap(group.special_center_name_);
//  stars_.swap(group.stars_);
//  double tmp = region_radio_;
//  region_radio_ = group.region_radio_;
//  group.region_radio_ = tmp;
//}
//void SkySpherePosGroupWithSpecialCenter::Clear() {
//  stars_.clear();
//  special_center_name_ = "";
//  region_radio_ = 0.;
//}
//void SkySpherePosGroupWithSpecialCenter::SetRegionRadio1(double region_radio) {
//  region_radio_ = region_radio;
//}
//void SkySpherePosGroupWithSpecialCenter::Add(SkySpherePos star_on_sky_sphere) {
//  if (star_on_sky_sphere.pos.length() < region_radio_) {
//    for (auto &star : stars_) {
//      if (star.name == star_on_sky_sphere.name) {
//        return;
//      }
//    }
//    stars_.push_back(star_on_sky_sphere);
//  }
//}
//void SkySpherePosGroupWithSpecialCenter::SetSpecialCenterName(const string &special_center_name) {
//  SkySpherePosGroupWithSpecialCenter::special_center_name_ = special_center_name;
//}
//const vector<SkySpherePos> &SkySpherePosGroupWithSpecialCenter::GetStars() const {
//  return stars_;
//}
//double SkySpherePosGroupWithSpecialCenter::GetRegionRadio() const {
//  return region_radio_;
//}
//SkySpherePosGroupWithSpecialCenter::SkySpherePosGroupWithSpecialCenter(double region_radio) : region_radio_(region_radio) {}
