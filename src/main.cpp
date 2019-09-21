//
// Created by cdh on 19-9-20.
//

#include <iostream>
#include <sstream>
#include <pugixml.hpp>
#include <osgViewer/CompositeViewer>
#include <osg/MatrixTransform>
#include <osg/Group>
#include <osg/Node>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/TrackballManipulator>

#include "CameraManipulator.h"
#include "Shape.h"
#include "CommonFunc.h"
#include "CameraFrameNode.h"
#include "SecondCameraManipulator.h"
#include "Planet.h"

using namespace std;
using namespace osg;

struct StarTableItem {
  int id;
  double a, b, l;
};

vector<StarTableItem> star_table;

int main() {
  pugi::xml_document xml_document;
  pugi::xml_parse_result parse_result = xml_document.load_file("star_table.xml");

  if (!parse_result) {
    cout << "Can NOT parse xml file.";
  }

  int count = 0;
  for (const auto &item : xml_document.child("star_table").children("item")) {
    int id;
    double a, b, l;
    id = item.child("id").text().as_int();
    a = item.child("a").text().as_double();
    b = item.child("b").text().as_double();
    l = item.child("l").text().as_double();
    star_table.emplace_back(StarTableItem{id, a, b, l});
    ++count;
  }

  cout << "Got " << count << " stars.";

  auto viewer = new osgViewer::CompositeViewer;
  auto wsi = GraphicsContext::getWindowingSystemInterface();
  char *display_ = getenv("DISPLAY");
  int display_num = 0;
  if (strlen(display_) >= 2) {
    display_ = display_ + 1;
    stringstream ss;
    ss << display_;
    ss >> display_num;
  } else {
    cout << "没有得到DISPLAY环境变量" << endl;
  }
  osg::DisplaySettings *ds = osg::DisplaySettings::instance().get();
  GraphicsContext::ScreenIdentifier si;
  si.displayNum = display_num;
  GraphicsContext::ScreenSettings ss;
  //cout << display_ << endl;
  wsi->getScreenSettings(si, ss);
  //cout << si.displayNum << ' ' << si.hostName << ' ' << si.displayNum << endl;
  auto traits = new GraphicsContext::Traits;
  traits->displayNum = si.displayNum;
  traits->hostName = si.hostName;
  traits->screenNum = si.screenNum;
  traits->x = 100;
  traits->y = 100;
  traits->width = 1000;
  traits->height = 1000;
  traits->windowDecoration = true;
  traits->doubleBuffer = true;
  traits->sharedContext = 0;
  traits->alpha = ds->getMinimumNumAlphaBits();
  traits->stencil = ds->getMinimumNumStencilBits();
  traits->sampleBuffers = ds->getMultiSamples();
  traits->samples = ds->getNumMultiSamples();

  osg::GraphicsContext *gc = osg::GraphicsContext::createGraphicsContext(traits);

  osgViewer::View *master_view = new osgViewer::View;

  master_view->getCamera()->setGraphicsContext(gc);
  master_view->getCamera()->setProjectionMatrixAsPerspective(60., 1., 100., 3000.);
  master_view->getCamera()->setViewport(0, 0, 1000, 1000);

  auto *master_root = new Group;
  auto *starGroup = new Group;
  auto *textGroup = new Group;
  auto *otherGroup = new Group;

  master_root->addChild(starGroup);
  master_root->addChild(textGroup);
  master_root->addChild(otherGroup);
  master_root->getOrCreateStateSet()->setMode(GL_LIGHTING, StateAttribute::OFF);

  auto *text_mt = new MatrixTransform;
  textGroup->addChild(text_mt);
  auto *text_node = new Geode;
  text_mt->addChild(text_node);
  auto *text_shown = new osgText::Text;
  text_node->addDrawable(text_shown);

  text_shown->setCharacterSizeMode(osgText::Text::CharacterSizeMode::SCREEN_COORDS);
  text_shown->setCharacterSize(32.f);
  text_shown->setAxisAlignment(osgText::Text::AxisAlignment::SCREEN);

  master_view->setSceneData(master_root);

  auto *camera_manipulator = new CameraManipulator;
  //auto *manipulator = new osgGA::KeySwitchMatrixManipulator;
  //manipulator->addMatrixManipulator(osgGA::GUIEventAdapter::KEY_F1,"master",camera_manipulator);
  //manipulator->addMatrixManipulator(osgGA::GUIEventAdapter::KEY_F2,"master1",new osgGA::TrackballManipulator);
  camera_manipulator->SetOnObjectTouched([=](const NodePath &np) {
    Matrix m = computeLocalToWorld(np);
    text_mt->setMatrix(m);
    Node *node = nullptr;
    for (int i = np.size() - 1; i >= 0; --i) {
      if (!np[i]->getName().empty()) {
        node = np[i];
        break;
      }
    }
    if (node) {
      text_shown->setText(node->getName());
    }
  });
  camera_manipulator->SetOnObjectClicked([=](const NodePath &np) {
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
      starGroup->addChild(geode);
      Planet *planet = dynamic_cast<Planet*>(node);
      planet->SetColor(Vec4(0.f,0.f,1.f,1.f));
    }
  });
  master_view->setCameraManipulator(camera_manipulator);
  //master_view->setCameraManipulator(manipulator);

  for (const auto &item : star_table) {
    auto *mt = new MatrixTransform;
    starGroup->addChild(mt);
    auto *planet = new Planet(Vec4(1.f,1.f,1.f,1.f),item.l);
    mt->addChild(planet);

    auto rotate_a = Matrix::rotate(DegreesToRadians(item.a), Vec3d(0., 0., 1.));
    //auto rotate_axis_ny = Vec3d(0., -1., 0.)*rotate_a;
    auto rotate_b = Matrix::rotate(DegreesToRadians(item.b), Vec3d(0., -1., 0.));

    planet->setName(to_string(item.id) + ':' + to_string(item.a) + ':' + to_string(item.b) + ':' + to_string(item.l));
    mt->setMatrix(Matrix::translate(Vec3(2000., 0., 0.))*rotate_b*rotate_a);
  }
  CreateLongitudeLatitude(otherGroup, 2000.);

  Group *second_root = new Group;
  second_root->addChild(starGroup);
  second_root->addChild(otherGroup);
  second_root->getOrCreateStateSet()->setMode(GL_LIGHTING, StateAttribute::OFF);

  osgViewer::View *second_view = new osgViewer::View;

  second_view->setSceneData(second_root);
  second_view->setCameraManipulator(new SecondCameraManipulator());

  Camera *second_camera = second_view->getCamera();
  second_camera->setGraphicsContext(gc);
  second_camera->setViewport(0, 0, 200, 200);
  //second_camera->setClearMask(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
  second_camera->setProjectionMatrixAsPerspective(20., 1., 100., 3000.);
  second_camera->setComputeNearFarMode(Camera::ComputeNearFarMode::DO_NOT_COMPUTE_NEAR_FAR);

  master_root->addChild(new CameraFrameNode(second_camera));

  viewer->addView(master_view);
  viewer->addView(second_view);

  viewer->run();

  return 0;
}
