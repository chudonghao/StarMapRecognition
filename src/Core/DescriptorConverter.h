//
// Created by cdh on 19-9-22.
//

#ifndef STARMAPRECOGNITION_SRC_CORE_DESCRIPTORCONVERTER_H_
#define STARMAPRECOGNITION_SRC_CORE_DESCRIPTORCONVERTER_H_

#include "../log.h"
#include "../Util/CommonFunc.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <osg/Vec2>
#include <opencv2/core.hpp>
#include <osg/Matrixd>
#include <osg/Vec3d>

#include "SpecialCenterStarOnSkySphereGroup.h"

template<typename FromT_, typename DescriptorT_>
class DescriptorConverter {};

template<int, int>
class Descriptor2;

template<int>
class Descriptor3;

template<int radial_special_size, int circular_special_size>
class DescriptorConverter<SpecialCenterStarOnSkySphereGroup, Descriptor2<radial_special_size, circular_special_size>> {
 public:
  DescriptorConverter() {}
  DescriptorConverter(bool debug) : debug(debug) {}
  typedef SpecialCenterStarOnSkySphereGroup FromT;
  typedef Descriptor2<radial_special_size, circular_special_size> DescriptorT;
  bool operator()(const FromT &from, DescriptorT &, bool convert_circular_);
 private:
  bool debug{};
};

template<int special_size>
class DescriptorConverter<SpecialCenterStarOnSkySphereGroup, Descriptor3<special_size>> {
 public:
  DescriptorConverter() {}
  DescriptorConverter(bool debug) : debug(debug) {}
  typedef SpecialCenterStarOnSkySphereGroup FromT;
  typedef Descriptor3<special_size> DescriptorT;
  bool operator()(const FromT &from, DescriptorT &);
 private:
  bool debug{};
};

template<int radial_special_size, int circular_special_size>
bool DescriptorConverter<SpecialCenterStarOnSkySphereGroup,
                         Descriptor2<radial_special_size, circular_special_size>>::
operator()(
    const SpecialCenterStarOnSkySphereGroup &from,
    Descriptor2<radial_special_size, circular_special_size> &des,
    bool convert_circular_
) {
  using namespace std;
  using namespace osg;
  des.SetSpecialCenterStarOnSkySphereGroup(from);
  float r = from.GetValidRegionRadio();
  for (int i = 0; i < radial_special_size; ++i) {
    des.GetRadialSpecial()[i] = 0.f;
  }
  for (int i = 0; i < circular_special_size; ++i) {
    des.GetCircularSpecial()[i] = 0.f;
  }
  auto &stars = from.GetStaresOnSkyShphere();
  int star_count = 0;
  for (auto &star_struct:stars) {
    if (from.GetSpecialCenter().GetName() == star_struct.second.star.GetName()) {
      continue;
    }
    if (star_struct.second.angular_distance < r) {
      // 径向
      // 0->10度分布到0->31
      des.GetRadialSpecial()[static_cast<int>(star_struct.second.angular_distance / 10.f * radial_special_size)] = 1.f;
      ++star_count;
      if (convert_circular_) {
        // 环向
        // 0->360度分布到0->circular_special_size
        SkySpherePos pos;
        // TODO 求解正确参数
        Convert(from.GetSpecialCenter().GetSkySpherePos(), star_struct.second.star.GetSkySpherePos(), pos);
        //LOG_TRACE << pos.GetLongitude() << " : " << pos.GetLatitude();
        auto rad = Radians(Vec2(1.f, 0.f), Vec2(pos));
        if (pos.y() < 0.) {
          rad = 2. * PI - rad;
        }
        //LOG_DEBUG << pos.GetLongitude() << ":" << pos.GetLatitude() << ":" << rad;
        des.GetCircularSpecial()[static_cast<int>(rad / (PI * 2.) * circular_special_size)] = 1.f;
      }
    } else {
      //LOG_TRACE << star_struct.second.angular_distance;
    }
  }
  des.SetStarNum(star_count);
  des.SetRegionRadio(from.GetValidRegionRadio());
  if (convert_circular_) {
    std::stringstream ss;
    for (int i = 0; i < radial_special_size; ++i) {
      ss << std::setw(2) << des.GetRadialSpecial()[i];
    }
    ss << " ====== ";
    for (int i = 0; i < circular_special_size; ++i) {
      ss << std::setw(2) << des.GetCircularSpecial()[i];
    }
    LOG_DEBUG << from.GetSpecialCenter().GetName() << " : " << stars.size() << " : " << r << " : " << star_count
              << " : " << ss.str();
  }
  return true;
}

template<int special_size>
bool DescriptorConverter<SpecialCenterStarOnSkySphereGroup,
                         Descriptor3<special_size>>::operator()(const SpecialCenterStarOnSkySphereGroup &from,
                                                                Descriptor3<special_size> &des) {
  using namespace std;
  using namespace osg;
  using namespace cv;
  des.SetSpecialCenterStarOnSkySphereGroup(from);
  float r = from.GetValidRegionRadio();
  for (int i = 0; i < special_size; ++i) {
    for (int j = 0; j < special_size; ++j) {
      des.GetSpecials()[i * special_size + j] = 0.f;
    }
  }

  auto data_mat = Mat_<double>(2, from.Size() + 1);
  data_mat.at<double>(0, 0) = 0;// center
  data_mat.at<double>(1, 0) = 0;
  int i = 1;
  for (const auto &star  : from.GetStaresOnSkyShphere()) {
    data_mat.at<double>(0, i) = star.second.star.GetSkySpherePos().GetLongitude();
    data_mat.at<double>(1, i) = star.second.star.GetSkySpherePos().GetLatitude();
    ++i;
  }
  Mat data = std::move(data_mat);

  PCA pca(data, Mat(), CV_PCA_DATA_AS_COL, 1);
  Mat dst = pca.project(data);
  double tar = 0.;
  for (int i = 0; i < dst.size[0]; ++i) {
    tar += dst.at<double>(i);
  }

  //LOG_TRACE << tar;
  //LOG_TRACE << pca.project(data);
  //LOG_TRACE << pca.eigenvectors;
  //LOG_TRACE << pca.mean;
  //LOG_TRACE << pca.eigenvalues;
  osg::Vec3d dir(pca.eigenvectors.at<double>(0), pca.eigenvectors.at<double>(1), 0.);
  if (tar < 0.) {
    dir.x() = -dir.x();
    dir.y() = -dir.y();
  }
  dir.normalize();
  Matrixd rotate = Matrixd::rotate(dir, osg::Vec3d(1., 0., 0.));
  float *data_ = des.GetSpecials();
  for (int i = 0; i < special_size; ++i) {
    for (int j = 0; j < special_size; ++j) {
      data_[i * special_size + j] = 0.f;
    }
  }
  //auto max_r = from.GetValidRegionRadio();
  for (auto &star: from.GetStaresOnSkyShphere()) {
    SkySpherePos pos_;
    Convert(from.GetSpecialCenter().GetSkySpherePos(), star.second.star.GetSkySpherePos(), pos_);
    des.SetSpecialDir(pos_);
    osg::Vec3d pos = osg::Vec3d(pos_, 0.);
    pos = pos * rotate;
    int i = pos.x() / from.GetValidRegionRadio() * (special_size / 2.) + special_size / 2.;
    int i1;
    int j = pos.y() / from.GetValidRegionRadio() * (special_size / 2.) + special_size / 2.;
    int j1;
    data_[i * special_size + j] = 1.f;
    i1 = i - 1;
    j1 = j - 1;
    if (i1 >= 0 && j1 > 0) {
      data_[i1 * special_size + j1] += 0.25f;
    }
    i1 = i + 1;
    j1 = j - 1;
    if (i1 < special_size && j1 >= 0) {
      data_[i1 * special_size + j1] += 0.25f;
    }
    i1 = i - 1;
    j1 = j + 1;
    if (i1 >= 0 && j1 < special_size) {
      data_[i1 * special_size + j1] += 0.25f;
    }
    i1 = i + 1;
    j1 = j + 1;
    if (i1 < special_size && j1 < special_size) {
      data_[i1 * special_size + j1] += 0.25f;
    }
    i1 = i;
    j1 = j - 1;
    if (j1 >= 0) {
      data_[i1 * special_size + j1] += 0.5f;
    }
    i1 = i;
    j1 = j + 1;
    if (j1 < special_size) {
      data_[i1 * special_size + j1] += 0.5f;
    }
    i1 = i - 1;
    j1 = j;
    if (i1 >= 0) {
      data_[i1 * special_size + j1] += 0.5f;
    }
    i1 = i + 1;
    j1 = j;
    if (i1 < special_size) {
      data_[i1 * special_size + j1] += 0.5f;
    }
  }
  //res.SetDataColumnSize(column_size);
  //res.SetData(data_);
  //res.SetFeatureVec(osg::Vec2d(dir.x(), dir.y()));
  //res.SetRadio(sos_group.GetRegionRadio());
  //res.SetStarSize(sos_group.size());
  //stringstream ss;
  //ss << "\n";
  //for (int i = special_size - 1; i >= 0; --i) {
  //  for (int j = 0; j < special_size; ++j) {
  //    ss << setw(5) << data_[i * special_size + j] << "\t";
  //  }
  //  ss << "\n";
  //}
  //LOG_TRACE << ss.str();
  //LOG_TRACE << dir.x() << ":" << dir.y();
  return true;
}

#endif //STARMAPRECOGNITION_SRC_CORE_DESCRIPTORCONVERTER_H_
