//
// Created by chudonghao on 2019/9/23.
//

#ifndef STARMAPRECOGNITION_SRC_CORE_DESCRIPTOR3_H_
#define STARMAPRECOGNITION_SRC_CORE_DESCRIPTOR3_H_

#include <sstream>
#include <iomanip>
#include <cv.hpp>
#include <cmath>
#include "SpecialCenterStarOnSkySphereGroup.h"
#include "../log.h"
#include "../Util/CommonFunc.h"

template<int size>
class Descriptor3 {
 public:
  Descriptor3() {}
  const SpecialCenterStarOnSkySphereGroup &GetSpecialCenterStarOnSkySphereGroup() const {
    return special_center_star_on_sky_sphere_group_;
  }
  void SetSpecialCenterStarOnSkySphereGroup(const SpecialCenterStarOnSkySphereGroup &special_center_star_on_sky_sphere_group) {
    special_center_star_on_sky_sphere_group_ = special_center_star_on_sky_sphere_group;
  }
  float Similarity(const Descriptor3 &des) {
    float res = 0.f;
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        res += special_[i][j]*des.special_[i][j];
      }
    }
    return res;
  }
  float *GetSpecials() { return &special_[0][0]; }
  int GetStarNum() const {
    return special_center_star_on_sky_sphere_group_.GetStaresOnSkyShphere().size();
  }
  float GetRegionRadio() const {
    return special_center_star_on_sky_sphere_group_.GetValidRegionRadio();
  }
  void SetRegionRadio(float region_radio) {
    special_center_star_on_sky_sphere_group_.SetValidRegionRadio(region_radio);
  }
  const osg::Vec2d &GetSpecialDir() const {
    return special_dir_;
  }
  void SetSpecialDir(const osg::Vec2d &special_dir) {
    special_dir_ = special_dir;
  }
  void DebugPrint() {
    std::stringstream ss;
    ss << "\n";
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        ss << std::setw(4) << special_[i][j];
      }
      ss << "\n";
    }
    LOG_DEBUG << ss.str();
  }
  void DebugShow(const std::string &debug_name) {
    using namespace cv;
    using namespace std;
    Mat m(size, size, CV_32FC3, Scalar(0., 0., 0.));
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        m.at<Vec3f>(j, i)[0] = special_[i][j];
        m.at<Vec3f>(j, i)[1] = special_[i][j];
        m.at<Vec3f>(j, i)[2] = special_[i][j];
        //m.at<Vec3f>(j, i)[0] = 255.;
        //m.at<Vec3f>(j, i)[1] = 255.;
        //m.at<Vec3f>(j, i)[2] = 255.;
      }
    }
    //LOG_DEBUG << special_dir_.x() << special_dir_.y();
    double rad = Radians(special_dir_, osg::Vec2d(1., 0.));
    if (special_dir_.y() < 0.) {
      rad = 2.*M_PI - rad;
    }
    Mat rotate = getRotationMatrix2D(Point2f(size/2, size/2), osg::RadiansToDegrees(rad), 1.);
    warpAffine(m.clone(), m, rotate, Size(size, size));
    line(m, Point((special_dir_.x() + 1.)*m.rows/2., (special_dir_.y() + 1.)*m.cols/2.), Point(size/2, size/2), Scalar(0., 1., 1.), 1);
    //LOG_DEBUG << Point((special_dir_.y() + 1.)*m.rows/2., (special_dir_.x() + 1.)*m.cols/2.);
    flip(m, m, 0);
    //flip(m, m, 1);
    while (m.rows < 400)
      pyrUp(m, m);
    static int i = 0;
    imshow(debug_name + string("Descriptor3:") + to_string(i), m);
    ++i;
  }
 private:
  osg::Vec2d special_dir_;
  float special_[size][size]{};
  SpecialCenterStarOnSkySphereGroup special_center_star_on_sky_sphere_group_;
};

#endif //STARMAPRECOGNITION_SRC_CORE_DESCRIPTOR3_H_
