//
// Created by chudonghao on 2019/9/21.
//

#include <iostream>
#include "Descriptor.h"

using namespace std;

DescriptorConvDiff DescriptorConvDiff::diff(const Descriptor &test, const Descriptor &database) {
  DescriptorConvDiff res;
  double conv = 0.;
  if (test.GetColumnSize() != database.GetColumnSize()) {
    cout << "Error" << __FILE__ << ":" << __LINE__ << endl;
    return res;
  } else {
    int size = test.GetColumnSize();
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        conv += test.GetData()[i*size + j]*database.GetData()[i*size + j];
      }
    }
  }
  res.conv_ = conv;
  return res;
}

bool operator<(const DescriptorConvDiff &l, const DescriptorConvDiff &r) {
  return l.conv_ > r.conv_;
}
