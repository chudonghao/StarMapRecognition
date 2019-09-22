//
// Created by cdh on 19-9-21.
//

#ifndef STARMAPRECOGNITION_SRC_CAMERAFRAMENODE_H_
#define STARMAPRECOGNITION_SRC_CAMERAFRAMENODE_H_

#include <osg/MatrixTransform>
#include <osg/Geometry>

class CameraFrameNode : public osg::MatrixTransform {
 public:
  explicit CameraFrameNode(osg::Camera *camera);
 private:
  osg::Camera *camera_;
  osg::Geode *geode_;
  osg::Geometry *geo_;
};

#endif //STARMAPRECOGNITION_SRC_CAMERAFRAMENODE_H_
