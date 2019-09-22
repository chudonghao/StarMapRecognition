//
// Created by chudonghao on 2019/9/21.
//

#include "MasterView.h"
#include <osg/MatrixTransform>
#include <iostream>
#include "MasterCameraManipulator.h"
#include "Shape.h"
#include "Planet.h"
#include "../Core/CameraStarFunc.h"
#include "../Core/StarTable.h"

using namespace std;
using namespace osg;

MasterView::MasterView(GraphicsContext *gc) {
  view_ = new osgViewer::View;
  camera_ = view_->getCamera();
  camera_->setGraphicsContext(gc);
  camera_->setGraphicsContext(gc);
  camera_->setProjectionMatrixAsPerspective(60., 1., 100., 3000.);
  camera_->setViewport(0, 0, 1000, 1000);
  sensor_camera_ = nullptr;
  root_ = new Group;
  view_->setSceneData(root_);

  InitHintTextNode();

  InitCameraManipulator();

  camera_frame_node_ = nullptr;
  root_->getOrCreateStateSet()->setMode(GL_LIGHTING, StateAttribute::OFF);
}
void MasterView::InitCameraManipulator() {
  camera_manipulator_ = new MasterCameraManipulator;
  camera_manipulator_->SetOnObjectTouched([=](const NodePath &np) {
    Matrix m = computeLocalToWorld(np);
    hint_text_mt_->setMatrix(m);
    Node *node = nullptr;
    for (int i = np.size() - 1; i >= 0; --i) {
      if (!np[i]->getName().empty()) {
        node = np[i];
        break;
      }
    }
    if (node) {
      hint_text_->setText(node->getName());
    }
  });
  camera_manipulator_->SetOnObjectClicked([=](const NodePath &np) {
    Matrix m = computeLocalToWorld(np);
    Node *node = nullptr;
    for (int i = np.size() - 1; i >= 0; --i) {
      if (!np[i]->getName().empty()) {
        node = np[i];
        break;
      }
    }
    if (node) {
      Vec3 target = Vec3()*m;
      Geode *geode = new Geode;
      geode->addDrawable(createLine(Vec3(), target, Vec3(0.f, 0.f, 1.f)));
      root_->addChild(geode);
      Planet *planet = dynamic_cast<Planet *>(node);
      planet->SetColor(Vec4(0.f, 0.f, 1.f, 1.f));

      if (sensor_camera_) {
        Vec3 window = target*sensor_camera_->getViewMatrix()*sensor_camera_->getProjectionMatrix();
        cout << window.x() << ":" << window.y() << ":" << window.z() << endl;
        static int i = 0;
        auto &star_table = StarTable::instance()->Table();
        input_[i] = Input{star_table[planet->GetId()].a, star_table[planet->GetId()].b, sqrt(window.x()*window.x() + window.y()*window.y())};
        ++i;
        if (i == 3) {
          i = 0;
          solver_(input_[0].a,
                  input_[0].b,
                  input_[1].a,
                  input_[1].b,
                  input_[2].a,
                  input_[2].b,
                  input_[0].r,
                  input_[1].r,
                  input_[2].r);
        }
      }
    }
  });
  view_->setCameraManipulator(camera_manipulator_);
}
void MasterView::InitHintTextNode() {
  hint_text_mt_ = new MatrixTransform;
  root_->addChild(hint_text_mt_);
  auto *text_node = new Geode;
  hint_text_mt_->addChild(text_node);
  hint_text_ = new osgText::Text;
  text_node->addDrawable(hint_text_);

  hint_text_->setCharacterSizeMode(osgText::TextBase::SCREEN_COORDS);
  hint_text_->setCharacterSize(32.f);
  hint_text_->setAxisAlignment(osgText::TextBase::SCREEN);
}
void MasterView::SetGraphicsContext(osg::GraphicsContext *graphics_context) {
  graphics_context_ = graphics_context;
  camera_->setGraphicsContext(graphics_context_);
}
void MasterView::ShowSensorCamera(Camera *sensor_camera) {
  sensor_camera_ = sensor_camera;
  if (sensor_camera_) {
    if (camera_frame_node_) {
      root_->removeChild(camera_frame_node_);
    }
    camera_frame_node_ = new CameraFrameNode(sensor_camera_);
    root_->addChild(camera_frame_node_);
  }
}
void MasterView::SetStarNode(StarGroup *star_group) {
  root_->addChild(star_group);
}
