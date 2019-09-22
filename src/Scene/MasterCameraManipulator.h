//
// Created by cdh on 19-9-21.
//

#ifndef STARMAPRECOGNITION_SRC_MASTERCAMERAMANIPULATOR_H_
#define STARMAPRECOGNITION_SRC_MASTERCAMERAMANIPULATOR_H_

#include <osgGA/CameraManipulator>
#include <osgText/Text>

class MasterCameraManipulator : public osgGA::CameraManipulator {
 public:
  MasterCameraManipulator();
  void setByMatrix(const osg::Matrixd &matrix) override;
  void setByInverseMatrix(const osg::Matrixd &matrix) override;
  osg::Matrixd getMatrix() const override;
  osg::Matrixd getInverseMatrix() const override;
  void init(const osgGA::GUIEventAdapter &adapter, osgGA::GUIActionAdapter &action_adapter) override;
  bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us) override;
  void SetOnObjectTouched(const std::function<void(const osg::NodePath &)> &on_object_touched) { on_object_touched_ = on_object_touched; }
  void SetOnObjectClicked(const std::function<void(const osg::NodePath &)> &on_object_clicked) { on_object_clicked_ = on_object_clicked; }
 private:
  std::function<void(const osg::NodePath &)> on_object_touched_;
  std::function<void(const osg::NodePath &)> on_object_clicked_;
  osg::Vec3 eye_;
  osg::Vec3 center_;
  osg::Vec3 up_;
  osg::Matrix matrix_;
  osg::Vec2 last_pos_;
};

#endif //STARMAPRECOGNITION_SRC_MASTERCAMERAMANIPULATOR_H_
