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
#include <iomanip>

#include "Scene/MasterCameraManipulator.h"
#include "Scene/Shape.h"
#include "Util/CommonFunc.h"
#include "Scene/CameraFrameNode.h"
#include "Scene/SensorCameraManipulator.h"
#include "Scene/Planet.h"
#include "Core/CameraStarFunc.h"
#include "Scene/MasterView.h"
#include "Core/StarTable.h"
#include "Scene/SensorView.h"
#include "Scene/StarGroup.h"
#include "Core/StarGraph.h"
#include "log.h"

using namespace std;
using namespace osg;

int main(int argc, char *argv[]) {
  cdh::log::InitLogging();
  google::InitGoogleLogging(argv[0]);

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

  MasterView master_view(gc);
  SensorView sensor_view(gc);
  auto *star_group = new StarGroup(2000.);
  master_view.SetStarNode(star_group);
  master_view.ShowSensorCamera(sensor_view.GetCamera());
  sensor_view.SetStarNode(star_group);

  StarGraph star_graph;
  star_graph.InitFrom("xingtu01.xml", 512., 12.);
  star_graph.GetDescriptors();
  star_graph.InitFrom("xingtu02.xml", 512., 12.);
  star_graph.GetDescriptors();
  star_graph.InitFrom("xingtu03.xml", 512., 12.);
  star_graph.GetDescriptors();
  star_graph.InitFrom("xingtu04.xml", 512., 12.);
  star_graph.GetDescriptors();
  star_graph.InitFrom("xingtu05.xml", 512., 12.);
  star_graph.GetDescriptors();
  star_graph.InitFrom("xingtu06.xml", 512., 12.);
  //star_graph.InitFrom("xingtu07.xml", 1024., 20.);
  //star_graph.InitFrom("xingtu08.xml", 1024., 20.);
  std::map<std::string, Descriptor2<32, 16>> dess1 = star_graph.GetDescriptors();

  std::map<std::string, Descriptor2<32, 16>> dess2 = StarTable::instance()->CreateDescriptorDatabase();

  map<float, string> match;
  for (auto &des1:dess1) {
    for (auto &des2:dess2) {
      float dif = diff<Descriptor2<32, 16>>(des1.second, des2.second);
      match[dif] = des1.first;
    }
    LOG_TRACE << "star: " << des1.first << ", descriptor: ";
  }
  auto iter = match.begin();
  for (int i = 0; i < 10; ++i) {
    if (iter != match.end()) {
      cout << iter->first << ":" << iter->second << endl;
      auto &star = StarTable::instance()->Table()[iter->second];
      cout << star.a << ":" << star.b << endl;
      ++iter;
    }
  }

  viewer->addView(master_view.GetView());
  viewer->addView(sensor_view.GetView());

  viewer->run();
  //solver_();
  return 0;
}
