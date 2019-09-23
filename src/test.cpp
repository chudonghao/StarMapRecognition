//
// Created by cdh on 19-9-23.
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
#include <cv.hpp>

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

  StarGraph star_graph;
  //star_graph.InitFrom("xingtu01.xml", 512., 12.);
  //star_graph.InitFrom("xingtu02.xml", 512., 12.);
  //star_graph.InitFrom("xingtu03.xml", 512., 12.);
  //star_graph.InitFrom("xingtu04.xml", 512., 12.);
  star_graph.InitFrom("xingtu05.xml", 512., 12.);
  //star_graph.InitFrom("xingtu06.xml", 512., 12.);
  //star_graph.InitFrom("xingtu07.xml", 1024., 20.);
  //star_graph.InitFrom("xingtu08.xml", 1024., 20.);
  star_graph.DebugShowToSkySpherePos("xingtu05.xml");
  star_graph.DebugInitFromStarTable(SkySpherePos(90., 0.), 512, 12);
  star_graph.DebugShowToSkySpherePos("90. 0.");
  star_graph.DebugShow("90. 0.");
  //star_group->addChild(star_graph.DebugShow());

  cv::waitKey(0);
  return 0;
}
