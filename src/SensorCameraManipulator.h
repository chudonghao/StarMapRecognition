//
// Created by cdh on 19-9-21.
//

#ifndef STARMAPRECOGNITION__SECONDCAMERAMANIPULATOR_H_
#define STARMAPRECOGNITION__SECONDCAMERAMANIPULATOR_H_
#include <osgGA/CameraManipulator>
#include <functional>

class SensorCameraManipulator : public osgGA::CameraManipulator {
 public:
  void setByMatrix(const osg::Matrixd &matrix) override;
  void setByInverseMatrix(const osg::Matrixd &matrix) override;
  osg::Matrixd getMatrix() const override;
  osg::Matrixd getInverseMatrix() const override;
  SensorCameraManipulator();
  bool handle(const osgGA::GUIEventAdapter &adapter, osgGA::GUIActionAdapter &action_adapter) override;
  void SetOnRotated(const std::function<void(const osg::Vec3 &, const osg::Vec3 &)> &on_rotated) {
    on_rotated_ = on_rotated;
  }
 private:
  std::function<void(const osg::Vec3 &eye, const osg::Vec3 &center)> on_rotated_;
  //int key_state_;
  osg::Vec3 eye_;
  osg::Vec3 center_;
  osg::Vec3 up_;
  osg::Matrix matrix_;
  osg::Vec2 last_pos_;
};

#endif //STARMAPRECOGNITION__SECONDCAMERAMANIPULATOR_H_
