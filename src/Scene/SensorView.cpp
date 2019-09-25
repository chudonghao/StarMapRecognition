//
// Created by chudonghao on 2019/9/21.
//

#include "SensorView.h"
#include "SensorCameraManipulator.h"
#include "StarGroup.h"
#include "../log.h"

using namespace std;
using namespace osg;
SensorView::SensorView(GraphicsContext *graphics_context) : graphics_context_(graphics_context) {
  view_ = new osgViewer::View;
  camera_ = view_->getCamera();
  camera_->setGraphicsContext(graphics_context);
  camera_->setViewport(0, 0, 200, 200);
  //second_camera->setClearMask(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
  camera_->setProjectionMatrixAsPerspective(12., 1., 100., 3000.);
  camera_->setComputeNearFarMode(Camera::ComputeNearFarMode::DO_NOT_COMPUTE_NEAR_FAR);
  camera_->setClearColor(Vec4(0.1f, 0.1f, 0.1f, 1.f));
  root_ = new Group;
  view_->setSceneData(root_);

  camera_manipulator_ = new SensorCameraManipulator();
  view_->setCameraManipulator(camera_manipulator_);

  hud_camera_ = new Camera;
  root_->addChild(hud_camera_);

  Geode *text_node = new Geode;
  hud_camera_->addChild(text_node);
  target_hint_text_ = new osgText::Text;
  text_node->addDrawable(target_hint_text_);

  camera_manipulator_->SetOnRotated([=](const Vec3 &eye, const Vec3 &center) {
    OnCameraRotated(eye, center);
  });

  root_->getOrCreateStateSet()->setMode(GL_LIGHTING, StateAttribute::OFF);

  hud_camera_->setGraphicsContext(graphics_context_);
  hud_camera_->setReferenceFrame(Camera::ReferenceFrame::ABSOLUTE_RF);
  hud_camera_->setClearMask(GL_DEPTH_BUFFER_BIT);
  hud_camera_->setProjectionMatrixAsOrtho2D(0., 200., 0., 200.);
  hud_camera_->setViewMatrix(Matrix::identity());

  target_hint_text_->setCharacterSize(32.f);
  target_hint_text_->setCharacterSizeMode(osgText::Text::CharacterSizeMode::SCREEN_COORDS);
  target_hint_text_->setAutoRotateToScreen(true);
  target_hint_text_->setText("90.:0.");
  // 解决崩溃问题
  target_hint_text_->setDataVariance(osgText::Text::DataVariance::DYNAMIC);
  root_->getOrCreateStateSet()->setMode(GL_LIGHTING, StateAttribute::OFF);
}
osgViewer::View *SensorView::GetView() const {
  return view_;
}
Camera *SensorView::GetCamera() const {
  return camera_;
}
void SensorView::SetStarNode(StarGroup *star_group) {
  if (star_group) {
    root_->addChild(star_group);
  }
}
void SensorView::OnCameraRotated(const osg::Vec3 &eye, const osg::Vec3 &center) {
  // TODO error
  Vec3 dir = center - eye;
  dir.normalize();
  float lon = RadiansToDegrees(atan(dir.y() / dir.x()));
  float la = RadiansToDegrees(atan(dir.z()));
  if (dir.x() <= 0.f) {
    lon += 180.f;
  } else if (dir.y() <= 0.f) {
    lon += 360.f;
  }
  //LOG_INFO << "Sensor target: " << lon << ":" << la;
  target_hint_text_->setText(to_string(int(lon)) + ":" + to_string(int(la)));
}
