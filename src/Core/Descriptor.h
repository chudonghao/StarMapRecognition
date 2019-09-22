//
// Created by chudonghao on 2019/9/21.
//

#ifndef STARMAPRECOGNITION__DESCRIPTOR_H_
#define STARMAPRECOGNITION__DESCRIPTOR_H_

#include <osg/Vec2d>
#include "limits"
#include "SpecialCenterStarOnSkySphereGroup.h"

using namespace std;

class Descriptor {
 public:
  Descriptor() : column_size_(), data_(), feature_vec_() {}
  void SetDataColumnSize(int column_size) {
    column_size_ = column_size;
  }
  void SetData(float *data) {
    data_ = data;
  }
  void SetFeatureVec(const osg::Vec2d &feature_vec) {
    feature_vec_ = feature_vec;
  }
  void SetStarSize(int star_size) {
    star_size_ = star_size;
  }
  void SetRadio(float radio) {
    radio_ = radio;
  }
  int GetColumnSize() const {
    return column_size_;
  }
  float *GetData() const {
    return data_;
  }
  int GetStarSize() const {
    return star_size_;
  }
  float GetRadio() const {
    return radio_;
  }
  const osg::Vec2d &GetFeatureVec() const {
    return feature_vec_;
  }
 private:
  int column_size_;
  float *data_;

  int star_size_;
  float radio_;
  osg::Vec2d feature_vec_;
};

class DescriptorConvDiff {
 public:
  DescriptorConvDiff() : conv_(0.) {}
  static DescriptorConvDiff diff(const Descriptor &test, const Descriptor &database);
  double Conv() const { return conv_; }
 private:
  double conv_;
  friend bool operator<(const DescriptorConvDiff &l, const DescriptorConvDiff &r);
};

//template<typename From_, typename To_>
//bool CalculateDescriptor(const From_ &, To_ &) { return false; }

#endif //STARMAPRECOGNITION__DESCRIPTOR_H_
