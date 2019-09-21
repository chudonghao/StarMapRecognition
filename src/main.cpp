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

#include "MasterCameraManipulator.h"
#include "Shape.h"
#include "CommonFunc.h"
#include "CameraFrameNode.h"
#include "SensorCameraManipulator.h"
#include "Planet.h"
#include "CameraStarFunc.h"
#include "MasterView.h"
#include "StarTable.h"
#include "SensorView.h"
#include "StarGroup.h"

using namespace std;
using namespace osg;

int main(int argc, char *argv[]) {
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

  viewer->addView(master_view.GetView());
  viewer->addView(sensor_view.GetView());

  viewer->run();
  //solver_();
  return 0;
}
