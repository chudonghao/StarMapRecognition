//
// Created by chudonghao on 2019/9/21.
//

#ifndef STARMAPRECOGNITION_SRC_SENSORVIEW_H_
#define STARMAPRECOGNITION_SRC_SENSORVIEW_H_

#include <osg/GraphicsContext>
#include <osgViewer/View>
#include <osgText/Text>
class StarGroup;
class SensorCameraManipulator;
class SensorView {
 public:
  SensorView(osg::GraphicsContext *graphics_context);
  osgViewer::View *GetView() const;
  osg::Camera *GetCamera() const;
  void SetStarNode(StarGroup *star_group);
 private:
  void OnCameraRotated(const osg::Vec3&eye,const osg::Vec3&center);
  osg::GraphicsContext *graphics_context_;
  osgViewer::View *view_;
  osg::Camera *camera_;
  osg::Camera *hud_camera_;
  SensorCameraManipulator *camera_manipulator_;
  osg::Group *root_;
  osgText::Text *target_hint_text_;
};

#endif //STARMAPRECOGNITION_SRC_SENSORVIEW_H_
