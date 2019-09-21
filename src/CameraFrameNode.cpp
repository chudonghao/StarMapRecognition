//
// Created by cdh on 19-9-21.
//

#include "CameraFrameNode.h"
#include <osg/NodeCallback>
#include <osg/Geometry>
#include <osg/PrimitiveSet>
#include <osg/Geode>

using namespace std;
using namespace osg;
CameraFrameNode::CameraFrameNode(Camera *camera) : camera_(camera) {
  class UpdateCallback : public NodeCallback {
   public:
    explicit UpdateCallback(CameraFrameNode *camera_frame_node) : camera_frame_node_(camera_frame_node) {}
    void operator()(Node *node, NodeVisitor *nv) override {
      double fovy, radio, near, far;
      Vec3 eye, center, up;
      camera_frame_node_->camera_->getProjectionMatrixAsPerspective(fovy, radio, near, far);
      camera_frame_node_->camera_->getViewMatrixAsLookAt(eye, center, up);
      Vec3 nDir = center - eye;
      nDir.normalize();
      Vec3 nSide = up ^nDir;
      nSide.normalize();
      Vec3 nUp = nDir ^nSide;
      nUp.normalize();

      Matrix rotate(-nSide[0], -nSide[1], -nSide[2], 0.,
                    nDir[0], nDir[1], nDir[2], 0.,
                    nUp[0], nUp[1], nUp[2], 0.,
                    eye[0], eye[1], eye[2], 1.);
      camera_frame_node_->setMatrix(rotate);
      double near_y_2 = near*tan((fovy/2)/180.*PI);
      double far_y_2 = far*tan((fovy/2)/180.*PI);
      Vec3Array *vertex_array = new Vec3Array(9);
      vertex_array->at(1).set(Vec3(near_y_2*radio, near, near_y_2));
      vertex_array->at(2).set(Vec3(near_y_2*radio, near, -near_y_2));
      vertex_array->at(3).set(Vec3(-near_y_2*radio, near, -near_y_2));
      vertex_array->at(4).set(Vec3(-near_y_2*radio, near, near_y_2));
      vertex_array->at(5).set(Vec3(far_y_2*radio, far, far_y_2));
      vertex_array->at(6).set(Vec3(far_y_2*radio, far, -far_y_2));
      vertex_array->at(7).set(Vec3(-far_y_2*radio, far, -far_y_2));
      vertex_array->at(8).set(Vec3(-far_y_2*radio, far, far_y_2));
      camera_frame_node_->geo_->setVertexArray(vertex_array);
      //camera_frame_node_->vertex_array_->at(1).set(Vec3(near_y_2*radio, near, near_y_2));
      //camera_frame_node_->vertex_array_->at(2).set(Vec3(near_y_2*radio, near, -near_y_2));
      //camera_frame_node_->vertex_array_->at(3).set(Vec3(-near_y_2*radio, near, -near_y_2));
      //camera_frame_node_->vertex_array_->at(4).set(Vec3(-near_y_2*radio, near, near_y_2));
      //camera_frame_node_->vertex_array_->at(5).set(Vec3(far_y_2*radio, far, far_y_2));
      //camera_frame_node_->vertex_array_->at(6).set(Vec3(far_y_2*radio, far, -far_y_2));
      //camera_frame_node_->vertex_array_->at(7).set(Vec3(-far_y_2*radio, far, -far_y_2));
      //camera_frame_node_->vertex_array_->at(8).set(Vec3(-far_y_2*radio, far, far_y_2));
      //camera_frame_node_->geo_->dirtyDisplayList();
      traverse(node, nv);
    }
   public:
    CameraFrameNode *camera_frame_node_;
  };

  Vec3Array *vertex_array = new Vec3Array;
  vertex_array->resize(9);
  Vec3Array *color_array = new Vec3Array;
  color_array->push_back(Vec3(1.f, 1.f, 1.f));

  geode_ = new Geode;
  addChild(geode_);

  geo_ = new Geometry;
  geode_->addDrawable(geo_);

  geo_->setVertexArray(vertex_array);
  geo_->setColorArray(color_array);
  geo_->setColorBinding(Geometry::BIND_OVERALL);

  auto draw = new DrawElementsUShort(GL_LINES);
  draw->push_back(0);
  draw->push_back(1);
  draw->push_back(0);
  draw->push_back(2);
  draw->push_back(0);
  draw->push_back(3);
  draw->push_back(0);
  draw->push_back(4);
  draw->push_back(1);
  draw->push_back(5);
  draw->push_back(2);
  draw->push_back(6);
  draw->push_back(3);
  draw->push_back(7);
  draw->push_back(4);
  draw->push_back(8);
  draw->push_back(1);
  draw->push_back(2);
  draw->push_back(2);
  draw->push_back(3);
  draw->push_back(3);
  draw->push_back(4);
  draw->push_back(4);
  draw->push_back(1);
  draw->push_back(5);
  draw->push_back(6);
  draw->push_back(6);
  draw->push_back(7);
  draw->push_back(7);
  draw->push_back(8);
  draw->push_back(8);
  draw->push_back(5);
  geo_->addPrimitiveSet(draw);

  addUpdateCallback(new UpdateCallback(this));
}
