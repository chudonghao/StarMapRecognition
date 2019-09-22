//
// Created by chudonghao on 2019/9/21.
//

#ifndef STARMAPRECOGNITION__MASTERSCENE_H_
#define STARMAPRECOGNITION__MASTERSCENE_H_

#include <osg/GraphicsContext>
#include <osg/Group>
#include <osgViewer/View>
#include <osgText/Text>
#include "CameraFrameNode.h"
#include "StarGroup.h"
class Planet;
class MasterCameraManipulator;
class MasterView {
 public:
  explicit MasterView(osg::GraphicsContext *gc);
  void SetGraphicsContext(osg::GraphicsContext *graphics_context);
  osgViewer::View *GetView(){ return view_;};
  void ShowSensorCamera(osg::Camera *sensor_camera);
  void SetStarNode(StarGroup *star_group);
 private:
  osgViewer::View *view_;
  osg::Group *root_{};
  osg::GraphicsContext *graphics_context_{};
  osg::Camera *camera_;
  osg::Camera *sensor_camera_;
  CameraFrameNode *camera_frame_node_;
  MasterCameraManipulator *camera_manipulator_;
  osgText::Text *hint_text_;
  osg::MatrixTransform *hint_text_mt_;
  struct Input {
    double a, b, r;
  };

  Input input_[3];
  void InitHintTextNode();
  void InitCameraManipulator();
};

#endif //STARMAPRECOGNITION__MASTERSCENE_H_
