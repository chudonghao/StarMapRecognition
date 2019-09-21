//
// Created by cdh on 19-9-21.
//

#include <osgUtil/LineSegmentIntersector>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include <osg/View>
#include <osg/MatrixTransform>
#include <osgViewer/Viewer>
#include <iostream>
#include "CameraManipulator.h"

using namespace osg;
using namespace std;

CameraManipulator::CameraManipulator() : eye_(0.f, -100.f, 0.f), center_(), up_(0.f, 0.f, 1.f) {}

void CameraManipulator::setByMatrix(const osg::Matrixd &matrix) {
  setByInverseMatrix(Matrix::inverse(matrix));
}
void CameraManipulator::setByInverseMatrix(const osg::Matrixd &matrix) {
  matrix.getLookAt(eye_, center_, up_);
  matrix_ = matrix;
}
osg::Matrixd CameraManipulator::getMatrix() const {
  return Matrix::inverse(matrix_);
}
osg::Matrixd CameraManipulator::getInverseMatrix() const {
  return matrix_;
}
bool CameraManipulator::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {
  switch (ea.getEventType()) {
    case osgGA::GUIEventAdapter::SCROLL: {
      switch (ea.getScrollingMotion()) {
        case osgGA::GUIEventAdapter::SCROLL_UP: {
          Vec3 dir = center_ - eye_;
          dir *= 1.1f;
          eye_ = center_ - dir;
          break;
        }
        case osgGA::GUIEventAdapter::SCROLL_DOWN: {
          Vec3 dir = center_ - eye_;
          dir *= 0.9f;
          eye_ = center_ - dir;
          break;
        }
      }
      break;
    }
    case osgGA::GUIEventAdapter::KEYDOWN: {
      if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Space) {
        init(ea, aa);
      }
      break;
    }
    case osgGA::GUIEventAdapter::DRAG: {
      float dx = ea.getX() - last_pos_.x();
      float dy = ea.getY() - last_pos_.y();
      dx *= 0.005f;
      dy *= 0.005f;

      Vec3 dir = center_ - eye_;
      Vec3 last_dir = dir;
      Vec3 n_dir = dir;
      n_dir.normalize();
      dir = dir*Matrix::rotate(dx, up_);
      Vec3 n_side = up_ ^dir;
      n_side.normalize();
      dir = dir*Matrix::rotate(dy, n_side);
      Vec3 new_side = up_ ^dir;

      if (new_side*n_side > 0.f) {
        eye_ = center_ - dir;
      }

      last_pos_.x() = ea.getX();
      last_pos_.y() = ea.getY();
      break;
    }
    case osgGA::GUIEventAdapter::PUSH: {
      osgUtil::LineSegmentIntersector::Intersections ins;
      osgViewer::View *view = dynamic_cast<osgViewer::View *>(&aa);
      view->computeIntersections(ea, ins);
      if (!ins.empty()) {
        if (on_object_clicked_) { on_object_clicked_(ins.begin()->nodePath); }
      }
      last_pos_.x() = ea.getX();
      last_pos_.y() = ea.getY();
      break;
    }
    case osgGA::GUIEventAdapter::MOVE: {
      break;
    }
    case osgGA::GUIEventAdapter::FRAME: {
      osgUtil::LineSegmentIntersector::Intersections ins;
      osgViewer::View *view = dynamic_cast<osgViewer::View *>(&aa);
      view->computeIntersections(ea, ins);
      if (!ins.empty()) {
        if (on_object_touched_) { on_object_touched_(ins.begin()->nodePath); }
      }
      matrix_ = Matrix::lookAt(eye_, center_, up_);
      break;
    }
  }
  return false;
}
void CameraManipulator::init(const osgGA::GUIEventAdapter &adapter, osgGA::GUIActionAdapter &action_adapter) {
  eye_ = Vec3(0.f, -100.f, 0.f);
  center_ = Vec3();
  up_ = Vec3(0.f, 0.f, 1.f);
}
