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
#include <osg/Geode>
#include <iomanip>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <cv.hpp>

#include "../Util/CommonFunc.h"
#include "../log.h"
#include "DescriptorConverter.h"
#include "StarTable.h"

using namespace std;
using namespace osg;

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
//  StarSkySpherePos sos_max;
//  Convert(pixel_f, graph_center, special_center_left_bottom, StarGraphPos{"", 0., 0.}, sos_max);
//  res.SetRegionRadio1(min(-sos_max.pos.x(), -sos_max.pos.y()));
//
//  res.SetSpecialCenterName(special_center.name);
//  StarSkySpherePos sos;
//  for (auto &sog : stars_on_graph) {
//    if (sog.name != special_center.name) {
//      Convert(pixel_f, graph_center, special_center, sog, sos);
//      res.Add(sos);
//    }
//  }
//};

StarGraph::StarGraph() {}

bool StarGraph::InitFrom(const std::string &file_name, double graph_size, double graph_a) {
  Reset(graph_size, graph_a);
  //////////////////////////////
  // Get All stars
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
  //////////////////////////////
  // 以视点中心为0，0，将星星映射到SkySphere坐标
  ResetStarOnSkySphereGroupRelativeToViewCenter();

  //////////////////////////////
  // 设置每个星星相关的星组
  ResetPerStarGroup();
  return true;
}
bool StarGraph::DebugInitFromStarTable(StarSkySpherePos pos, double graph_size, double graph_a) {
  //////////////////////////////
  // Reset
  Reset(graph_size, graph_a);
  //////////////////////////////
  // Get All stars
  auto &tab = StarTable::instance()->Table();
  Vec3 center = pos.WorldPosition(1.);
  int id = 0;
  for (auto &item:tab) {
    //StarSkySpherePos new_pos;
    //Convert(pos,StarSkySpherePos(item.second),new_pos);
    StarTableData &star_table_pos = item.second;
    Vec3d screen_pos = StarSkySpherePos(star_table_pos).
        ScreenPosition(graph_size, Matrix::lookAt(Vec3(), center, Vec3(0.f, 0.f, 1.f)), Matrix::perspective(graph_a, 1., 0.5, 1.));
    if (screen_pos.x() >= 0. && screen_pos.y() >= 0. && screen_pos.x() < graph_size && screen_pos.y() < graph_size && screen_pos.z() <= 1.) {
      string name = string("debug") + to_string(id);
      stars_[name] = StarOnGraph__(name, StarGraphPos(screen_pos.x(), screen_pos.y()));
      ++id;
      //LOG_DEBUG << screen_pos.x() << "\t" << screen_pos.y() << "\t" << star_table_pos.a << "\t" << star_table_pos.b << "\t" << screen_pos.z();
    }
  }
  //////////////////////////////
  // 以视点中心为0，0，将星星映射到SkySphere坐标
  ResetStarOnSkySphereGroupRelativeToViewCenter();
  //////////////////////////////
  // 设置每个星星相关的星组
  ResetPerStarGroup();
  return true;
}
void StarGraph::Reset(double graph_size, double graph_a) {
  stars_.clear();
  star_group_relative_to_view_center_.Clear();
  graph_size_ = graph_size;
  graph_fovy_ = graph_a;
  pixel_f_ = (graph_size/2.)/tan(DegreesToRadians(graph_a)/2.);
}
void StarGraph::ResetStarOnSkySphereGroupRelativeToViewCenter() {
  // 以视点中心为0，0，将星星映射到SkySphere坐标
  star_group_relative_to_view_center_.SetSpecialCenter(StarOnSkySphere("!ghost!", StarSkySpherePos()));
  star_group_relative_to_view_center_.SetValidRegionRadio(graph_fovy_/2.*1.414);
  for (auto &name_star_pair: stars_) {
    const auto &name = name_star_pair.first;
    const auto &star_on_graph = name_star_pair.second;
    StarOnSkySphere star_on_sky_sphere = star_on_graph.ToStarOnSkySphere(StarGraphPos(graph_size_/2., graph_size_/2.), pixel_f_, graph_size_);
    star_group_relative_to_view_center_.Add(name, star_on_sky_sphere);
  }
}
void StarGraph::ResetPerStarGroup() {
  for (auto &name_star_pair: stars_) {
    auto &name = name_star_pair.first;
    auto &star_on_graph = name_star_pair.second;
    // 计算有效半径
    const StarGraphPos &special_center = star_on_graph.GetStarGraphPos();
    StarGraphPos graph_center = StarGraphPos(graph_size_/2., graph_size_/2.);
    StarGraphPos special_center_left_bottom(special_center.GetPixelX(), special_center.GetPixelY());
    if (special_center.GetPixelX() > graph_center.GetPixelX()) {
      special_center_left_bottom.SetPixelX(graph_size_ - special_center.GetPixelX());
    }
    if (special_center.GetPixelY() > graph_center.GetPixelY()) {
      special_center_left_bottom.SetPixelY(graph_size_ - special_center.GetPixelY());
    }
    StarSkySpherePos sky_sphere_pos;
    Convert(pixel_f_, graph_center, special_center_left_bottom, StarGraphPos(0., 0.), sky_sphere_pos);

    //////////////////
    // 生成星组
    SpecialCenterStarOnSkySphereGroup scsg = star_group_relative_to_view_center_;
    // 设置中心星为当前星星
    StarOnSkySphere center = scsg.GetStaresOnSkyShphere().at(name).star;
    scsg.SetSpecialCenter(center);
    // 设置有效半径
    scsg.SetValidRegionRadio(min(sky_sphere_pos.GetLongitude(), -sky_sphere_pos.GetLatitude()));
    //LOG_TRACE << "special center: "<<stars_on_sky_sphere[name].GetName();
    //LOG_TRACE << "special center lon: "<<stars_on_sky_sphere[name].GetSkySpherePos().GetLongitude();
    //LOG_TRACE << "special center lat: "<<stars_on_sky_sphere[name].GetSkySpherePos().GetLatitude();
    //LOG_TRACE << "valid region radio: " << scsg.GetValidRegionRadio();
    star_on_graph.SetSpecialStarGroup(scsg);
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
}
SpecialCenterStarOnSkySphereGroup StarGraph::GetMasterGroup() {
  int max = 0;
  string max_name;
  for (auto &pair:stars_) {
    auto &from = pair.second.GetSpecialStarGroup();
    auto &stars = from.GetStaresOnSkyShphere();
    int star_count = 0;
    from.Shirk();
    star_count = from.Size();
    if (star_count > max) {
      max = star_count;
      max_name = pair.first;
    }
  }
  if (max) {
    return stars_[max_name].GetSpecialStarGroup();
  }
  return SpecialCenterStarOnSkySphereGroup();
}
SpecialCenterStarOnSkySphereGroup StarGraph::GetStarGroupRelativeToViewCenter() {
  return star_group_relative_to_view_center_;
}

void StarGraph::DebugShow(string debug_name) {
  using namespace cv;
  const static int kSize = 500;
  static int i = 0;

  Mat m(kSize, kSize, CV_32FC1, Scalar(0.));
  for (auto &star_pair:stars_) {
    const string &name = star_pair.first;
    StarOnGraph__ &star_on_graph = star_pair.second;
    m.at<float>(
        static_cast<int>(star_on_graph.GetStarGraphPos().GetPixelY()/graph_size_*kSize),
        static_cast<int>(star_on_graph.GetStarGraphPos().GetPixelX()/graph_size_*kSize)
    ) = 1.;
  }
  cv::flip(m, m, 0);
  imshow(debug_name + string(":StarGraph DebugShow:") + to_string(i), m);
  ++i;
}
void StarGraph::DebugShow_ToSkySpherePos(string debug_name) {
  using namespace cv;
  const static int kSize = 200;
  const static float kMaxAngularDistance = 10.f;
  static int i = 0;

  Mat m(kSize, kSize, CV_32FC1, Scalar(0.));

  for (auto &star_pair:stars_) {
    const string &name = star_pair.first;
    StarOnGraph__ &star_on_graph = star_pair.second;
    StarOnSkySphere star_on_sky_sphere = star_on_graph.
        ToStarOnSkySphere(StarGraphPos(graph_size_/2, graph_size_/2), graph_size_/2./tan(DegreesToRadians(graph_fovy_)/2.), graph_size_);

    float ad = star_on_sky_sphere.AngularDistance(StarOnSkySphere("", StarSkySpherePos()));
    if (ad < kMaxAngularDistance) {
      m.at<float>(
          static_cast<int>(star_on_sky_sphere.GetSkySpherePos().GetLatitude()/kMaxAngularDistance*kSize/2. + kSize/2.),
          static_cast<int>(star_on_sky_sphere.GetSkySpherePos().GetLongitude()/kMaxAngularDistance*kSize/2. + kSize/2.)
      ) = 1.;
    } else {
      //LOG_WARNING << ad;
    }
  }
  cv::flip(m, m, 0);
  imshow(debug_name + string(":StarGraph DebugShow_ToSkySpherePos To:") + to_string(i), m);

  m = Mat(m.size(), m.type(), Scalar());
  for (auto &star_pair:stars_) {
    const string &name = star_pair.first;
    StarOnGraph__ &star_on_graph = star_pair.second;
    if (star_on_graph.GetStarGraphPos().GetPixelX() < graph_size_ && star_on_graph.GetStarGraphPos().GetPixelY() < graph_size_) {
      m.at<float>(
          static_cast<int>(star_on_graph.GetStarGraphPos().GetPixelY()/graph_size_*kSize),
          static_cast<int>(star_on_graph.GetStarGraphPos().GetPixelX()/graph_size_*kSize)
      ) = 1.;
    }
    //LOG_DEBUG << star_on_graph.GetStarGraphPos().GetPixelX() << ":" << star_on_graph.GetStarGraphPos().GetPixelY();
  }
  cv::flip(m, m, 0);
  imshow(debug_name + string(":StarGraph DebugShow_ToSkySpherePos From:") + to_string(i), m);
  ++i;
}

//void CreateStarOnSkySphereGroupWithSpecialCenter(const std::vector<StarTableData> &stars,
//                                                 const StarTableData &special_center,
//                                                 double max_r,
//                                                 int planet_num,
//                                                 SkySpherePosGroupWithSpecialCenter &res) {
//  multimap<double, StarSkySpherePos> lightness_planet_map;
//  for (const auto &star: stars) {
//    StarSkySpherePos sos;
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
//void SkySpherePosGroupWithSpecialCenter::Add(StarSkySpherePos star_on_sky_sphere) {
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
//const vector<StarSkySpherePos> &SkySpherePosGroupWithSpecialCenter::GetStars() const {
//  return stars_;
//}
//double SkySpherePosGroupWithSpecialCenter::GetRegionRadio() const {
//  return region_radio_;
//}
//SkySpherePosGroupWithSpecialCenter::SkySpherePosGroupWithSpecialCenter(double region_radio) : region_radio_(region_radio) {}
