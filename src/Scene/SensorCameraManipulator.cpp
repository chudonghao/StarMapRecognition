//
// Created by cdh on 19-9-21.
//

#include "SensorCameraManipulator.h"
using namespace std;
using namespace osg;

SensorCameraManipulator::SensorCameraManipulator() : /*key_state_(),*/ eye_(0.f, 0.f, 0.f), center_(0.f, 1.f, 0.f), up_(0.f, 0.f, 1.f) {}

void SensorCameraManipulator::setByMatrix(const osg::Matrixd &matrix) {
  setByInverseMatrix(Matrix::inverse(matrix));
}
void SensorCameraManipulator::setByInverseMatrix(const osg::Matrixd &matrix) {
  matrix.getLookAt(eye_, center_, up_);
  matrix_ = matrix;
}
osg::Matrixd SensorCameraManipulator::getMatrix() const {
  return Matrix::inverse(matrix_);
}
osg::Matrixd SensorCameraManipulator::getInverseMatrix() const {
  return matrix_;
}

bool SensorCameraManipulator::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {
  switch (ea.getEventType()) {
    case osgGA::GUIEventAdapter::DRAG: {
      //if (key_state_) {
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
          center_ = eye_ + dir;
          if(on_rotated_){
            on_rotated_(eye_,center_);
          }
        }
      //}
      last_pos_.x() = ea.getX();
      last_pos_.y() = ea.getY();
      break;
    }
    case osgGA::GUIEventAdapter::PUSH: {
      last_pos_.x() = ea.getX();
      last_pos_.y() = ea.getY();
      break;
    }
    case osgGA::GUIEventAdapter::MOVE: {
      break;
    }
    case osgGA::GUIEventAdapter::FRAME: {
      matrix_ = Matrix::lookAt(eye_, center_, up_);
      break;
    }
    //case osgGA::GUIEventAdapter::KEYDOWN: {
    //  if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Alt_L) {
    //    //key_state_ = 1;
    //  }
    //  break;
    //}
    //case osgGA::GUIEventAdapter::KEYUP: {
    //  if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Alt_L) {
        //key_state_ = 0;
      //}
      //break;
    //}
    case osgGA::GUIEventAdapter::RESIZE:break;
    case osgGA::GUIEventAdapter::SCROLL:break;
  }
  return false;
}
