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
#include <thread>
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

template<int ii, int jj>
void Display(Descriptor2<ii, jj> &des) {
  stringstream ss;
  for (int i = 0; i < ii; ++i) {
    ss << std::setw(2) << des.GetRadialSpecial()[i];
  }
  ss << " ====== ";
  for (int i = 0; i < jj; ++i) {
    ss << std::setw(2) << des.GetCircularSpecial()[i];
  }
  LOG_INFO << des.GetSpecialCenterStarOnSkySphereGroup().GetSpecialCenter().GetName() << " : "
           << des.GetStarNum() << " : " << " : " << ss.str();
}

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
  //star_graph.InitFrom("xingtu01.xml", 512., 12.);
  star_graph.InitFrom("xingtu02.xml", 512., 12.);
  //star_graph.InitFrom("xingtu03.xml", 512., 12.);
  //star_graph.InitFrom("xingtu04.xml", 512., 12.);
  //star_graph.InitFrom("xingtu05.xml", 512., 12.);
  //star_graph.InitFrom("xingtu06.xml", 512., 12.);
  //star_graph.InitFrom("xingtu07.xml", 1024., 20.);
  //star_graph.InitFrom("xingtu08.xml", 1024., 20.);
  star_graph.DebugShowToSkySpherePos(string("xingtu02.xml"));

  //star_graph.DebugInitFromStarTable(SkySpherePos(90., 0.), 512, 12);
  //star_graph.DebugShowToSkySpherePos(string("(90.,0.)"));
  //star_group->addChild(star_graph.DebugShow());

  multimap<float, string> match1;
#if 0
  std::map<std::string, Descriptor2<80, 16>> *dess1 = star_graph.GetDescriptor2s<80, 16>();

  std::map<std::string, Descriptor2<80, 16>> *dess2 = StarTable::instance()->CreateDescriptorDatabase<80, 16>();

  multimap<float, string> match2;
  set<string> match3;
  set<string> match2_input;
  for (auto &des1:*dess1) {
    LOG_TRACE << des1.first << ":" << des1.second.GetStarNum();
    match1.clear();
    match2.clear();
    match3.clear();
    match2_input.clear();
    for (auto &des2:*dess2) {
      float similarity1 = des1.second.Similarity1(des2.second);
      match1.emplace(similarity1, des2.first);
    }
    int i = 0;
    float max = 0.f;
    for (auto iter = match1.rbegin(); iter != match1.rend(); ++iter) {
      if (iter->first > max) {
        max = iter->first;
      }
      if (max <= 0.f || max - iter->first > 1.5f || (max - iter->first > 0.5f && i >= 30)) {
        break;
      } else {
        //LOG_TRACE << max << ":" << iter->first << ":" << i;
      }
      match2_input.insert(iter->second);
      ++i;
    }
    LOG_TRACE << i;
    if (i) {
      for (auto &match2_in : match2_input) {
        //Display(des1.second);
        //Display((*dess2)[match2_in]);
        //auto des2 = (*dess2)[match2_in];
        //des2.Reshape(des1.second.GetRegionRadio());
        //Display(des2);

        float similarity2 = des1.second.Similarity2((*dess2)[match2_in]);
        match2.emplace(similarity2, match2_in);

      }
    }
    i = 0;
    max = 0.f;
    for (auto iter = match2.rbegin(); iter != match2.rend(); ++iter) {
      if (iter->first > max) {
        max = iter->first;
      }
      if (max <= 0.f || max - iter->first > 1.5f || (max - iter->first > 0.5f && i >= 30)) {
        break;
      } else {
        //LOG_TRACE << max << ":" << iter->first << ":" << i;
      }
      match3.insert(iter->second);
      ++i;
    }
    LOG_TRACE << i;
    static Vec4 color[20]{
        Vec4(1., 0., 0., 1.),
        Vec4(0., 1., 0., 1.),
        Vec4(0., 0., 1., 1.),
        Vec4(1., 1., 0., 1.),
        Vec4(1., 0., 1., 1.),
        Vec4(0., 1., 1., 1.),
        Vec4(0.8, 0., 0., 1.),
        Vec4(0., 0.8, 0., 1.),
        Vec4(0., 0., 0.8, 1.),
        Vec4(0.8, 0.8, 0., 1.),
        Vec4(0.8, 0., 0.8, 1.),
        Vec4(0., 0.8, 0.8, 1.),
        Vec4(1., 0.5, 0., 1.),
        Vec4(0., 1., 0.5, 1.),
        Vec4(0., 0.5, 1., 1.),
        Vec4(1., 1., 0.5, 1.),
        Vec4(1., 0.5, 1., 1.),
        Vec4(0.5, 1., 1., 1.),
        Vec4(1., 0.5, 0.9, 1.),
        Vec4(0.5, 1., 0.8, 1.),
    };
    static Vec3 start[20]{
        Vec3(0.*3, 0., 4000.),
        Vec3(200.*3, 0., 4000.),
        Vec3(400.*3, 0., 4000.),
        Vec3(800.*3, 0., 4000.),
        Vec3(1000.*3, 0., 4000.),
        Vec3(1200.*3, 0., 4000.),
        Vec3(1400.*3, 0., 4000.),
        Vec3(1600.*3, 0., 4000.),
        Vec3(1800.*3, 0., 4000.),
        Vec3(-1800.*3, 0., 4000.),
        Vec3(-1600.*3, 0., 4000.),
        Vec3(-1400.*3, 0., 4000.),
        Vec3(-1600.*3, 0., 4000.),
        Vec3(-1800.*3, 0., 4000.),
        Vec3(-2200.*3, 0., 4000.),
        Vec3(2200.*3, 0., 4000.),
        Vec3(2400.*3, 0., 4000.),
        Vec3(-2400.*3, 0., 4000.),
    };
    static int iii = 0;
    if (match3.size() <= 30) {
      for (auto &name  : match3) {
        osg::Vec2d pos = StarOnSkySphere(StarTable::instance()->Table()[name]).GetSkySpherePos();
        auto rotate_a = Matrix::rotate(DegreesToRadians(pos.x()), Vec3d(0., 0., 1.));
        auto rotate_b = Matrix::rotate(DegreesToRadians(pos.y()), Vec3d(0., -1., 0.));
        osg::Vec3 target = (Matrix::translate(Vec3(2000., 0., 0.))*rotate_b*rotate_a).getTrans();
        osg::Geometry *line = createLine(start[iii], target, Vec3(color[iii].x(), color[iii].y(), color[iii].z()));
        osg::Geode *geode = new Geode;
        geode->addDrawable(line);
        star_group->addChild(geode);
        star_group->HighLight(name, color[iii]);
      }
      ++iii;
    }
  }
#endif
#if 0
  DescriptorConverter<SpecialCenterStarOnSkySphereGroup, Descriptor2<80, 16>> converter;
  Descriptor2<80, 16> des1;
  converter(star_graph.GetMasterGroup(), des1, true);

  std::map<std::string, Descriptor2<80, 16>> *dess2 = StarTable::instance()->CreateDescriptorDatabase<80, 16>();

  multimap<float, string> match2;
  set<string> match3;
  set<string> match2_input;

    LOG_TRACE << ":" << des1.GetStarNum();
    match1.clear();
    match2.clear();
    match3.clear();
    match2_input.clear();
    for (auto &des2:*dess2) {
      float similarity1 = des1.Similarity1(des2.second);
      match1.emplace(similarity1, des2.first);
    }
    int i = 0;
    float max = 0.f;
    for (auto iter = match1.rbegin(); iter != match1.rend(); ++iter) {
      if (iter->first > max) {
        max = iter->first;
      }
      if (max <= 0.f || max - iter->first > 1.5f || (max - iter->first > 0.5f && i >= 30)) {
        break;
      } else {
        //LOG_TRACE << max << ":" << iter->first << ":" << i;
      }
      match2_input.insert(iter->second);
      ++i;
    }
    LOG_TRACE << i;
    if (i) {
      for (auto &match2_in : match2_input) {
        //Display(des1.second);
        //Display((*dess2)[match2_in]);
        //auto des2 = (*dess2)[match2_in];
        //des2.Reshape(des1.second.GetRegionRadio());
        //Display(des2);

        float similarity2 = des1.Similarity2((*dess2)[match2_in]);
        match2.emplace(similarity2, match2_in);

      }
    }
    i = 0;
    max = 0.f;
    for (auto iter = match2.rbegin(); iter != match2.rend(); ++iter) {
      if (iter->first > max) {
        max = iter->first;
      }
      if (max <= 0.f || max - iter->first > 1.5f || (max - iter->first > 0.5f && i >= 30)) {
        break;
      } else {
        //LOG_TRACE << max << ":" << iter->first << ":" << i;
      }
      match3.insert(iter->second);
      ++i;
    }
    LOG_TRACE << i;
    static Vec4 color[20]{
        Vec4(1., 0., 0., 1.),
        Vec4(0., 1., 0., 1.),
        Vec4(0., 0., 1., 1.),
        Vec4(1., 1., 0., 1.),
        Vec4(1., 0., 1., 1.),
        Vec4(0., 1., 1., 1.),
        Vec4(0.8, 0., 0., 1.),
        Vec4(0., 0.8, 0., 1.),
        Vec4(0., 0., 0.8, 1.),
        Vec4(0.8, 0.8, 0., 1.),
        Vec4(0.8, 0., 0.8, 1.),
        Vec4(0., 0.8, 0.8, 1.),
        Vec4(1., 0.5, 0., 1.),
        Vec4(0., 1., 0.5, 1.),
        Vec4(0., 0.5, 1., 1.),
        Vec4(1., 1., 0.5, 1.),
        Vec4(1., 0.5, 1., 1.),
        Vec4(0.5, 1., 1., 1.),
    };
    static int iii = 0;
    if (match3.size() <= 30) {
      for (auto &name  : match3) {
        osg::Vec2d pos = StarOnSkySphere(StarTable::instance()->Table()[name]).GetSkySpherePos();
        auto rotate_a = Matrix::rotate(DegreesToRadians(pos.x()), Vec3d(0., 0., 1.));
        auto rotate_b = Matrix::rotate(DegreesToRadians(pos.y()), Vec3d(0., -1., 0.));
        osg::Vec3 target = (Matrix::translate(Vec3(2000., 0., 0.)) * rotate_b * rotate_a).getTrans();
        osg::Geometry *line = createLine(osg::Vec3(), target, Vec3(color[iii].x(),color[iii].y(),color[iii].z()));
        osg::Geode *geode = new Geode;
        geode->addDrawable(line);
        star_group->addChild(geode);
        star_group->HighLight(name, color[iii]);
      }
      ++iii;

  }
#endif
#if 1
  constexpr int precise = 101;
  auto master_group = star_graph.GetMasterGroup();
  master_group.Shirk();
  DescriptorConverter<SpecialCenterStarOnSkySphereGroup, Descriptor3<precise>> converter;
  Descriptor3<precise> descriptor_3;
  converter(master_group, descriptor_3);
  LOG_DEBUG << master_group.GetSpecialCenter().GetName() << ":" << master_group.Size() << ":"
            << master_group.GetValidRegionRadio();

  auto database =
      StarTable::instance()->CreateDescriptor3Database<precise>(master_group.GetValidRegionRadio(),
                                                                master_group.Size());

  for (auto &item:*database) {
    float sim = descriptor_3.Similarity(item.second);
    match1.emplace(sim, item.first);
  }

  LOG_TRACE << descriptor_3.GetSpecialCenterStarOnSkySphereGroup().Size();
  descriptor_3.DebugShow();
  //for (auto &star:descriptor_3.GetSpecialCenterStarOnSkySphereGroup().GetStaresOnSkyShphere()) {
  //  Planet *p = new Planet(star.second.star.GetName(), Vec4(1.f, 0.f, 0.f, 1.f), 10.f);
  //
  //  osg::Vec2d pos = star.second.star.GetSkySpherePos();
  //  auto rotate_a = Matrix::rotate(DegreesToRadians(pos.x()), Vec3d(0., 0., 1.));
  //  auto rotate_b = Matrix::rotate(DegreesToRadians(pos.y()), Vec3d(0., -1., 0.));
  //  Matrix m = Matrix::translate(Vec3(2000., 0., 0.)) * rotate_b * rotate_a;
  //  MatrixTransform *mt = new MatrixTransform();
  //  mt->setMatrix(m);
  //  mt->addChild(p);
  //  star_group->addChild(mt);
  //}

  int i = 0;
  for (auto iter = match1.rbegin(); iter != match1.rend(); ++iter) {
    auto star_data = StarTable::instance()->Table()[iter->second];
    LOG_TRACE << iter->second << ":" << iter->first << "\t" << star_data.a << ":" << star_data.b;

    LOG_TRACE << (*database)[iter->second].GetSpecialCenterStarOnSkySphereGroup().Size();
    for (auto &star:(*database)[iter->second].GetSpecialCenterStarOnSkySphereGroup().GetStaresOnSkyShphere()) {
      star_group->HighLight(star.second.star.GetName(), Vec4(0.f, 1.f, 0.f, 1.f));
    }
    Vec2d dir1 = descriptor_3.GetSpecialDir();
    Vec2d dir2 = (*database)[iter->second].GetSpecialDir();
    StarOnSkySphere s(StarTable::instance()->Table()[iter->second]);
    Matrix rotate_1 = Matrix::rotate(Vec3d(0., -dir2.x(), dir2.y()), Vec3d(0., -dir1.x(), dir1.y()));
    Matrix rotate_lon = Matrix::rotate(DegreesToRadians(s.GetSkySpherePos().GetLongitude()), Vec3(0.f, 0.f, 1.f));
    Matrix rotate_lat = Matrix::rotate(DegreesToRadians(s.GetSkySpherePos().GetLatitude()), Vec3(0.f, -1.f, 0.f));

    for (auto &star:descriptor_3.GetSpecialCenterStarOnSkySphereGroup().GetStaresOnSkyShphere()) {
      Planet *p = new Planet(star.second.star.GetName(), Vec4(1.f, 0.f, 0.f, 1.f), 10.f);
      osg::Vec2d pos = star.second.star.GetSkySpherePos();
      auto rotate_a = Matrix::rotate(DegreesToRadians(pos.x()), Vec3d(0., 0., 1.));
      auto rotate_b = Matrix::rotate(DegreesToRadians(pos.y()), Vec3d(0., -1., 0.));
      Matrix m = Matrix::translate(Vec3(2000., 0., 0.))*rotate_b*rotate_a*Matrix::scale(Vec3(1.005f, 1.005f, 1.005f));
      MatrixTransform *mt = new MatrixTransform();
      mt->setMatrix(m*rotate_1*rotate_lat*rotate_lon);
      mt->addChild(p);
      star_group->addChild(mt);
    }
    ++i;
    if (i == 10) {
      break;
    }
  }
  StarGraph star_graph1;
  SkySpherePos pos = SkySpherePos(StarTable::instance()->Table()[match1.rbegin()->second]);
  star_graph1.DebugInitFromStarTable(pos, 512, 12.);
  star_graph1.DebugShow(string("Match"));
  (*database)[match1.rbegin()->second].DebugShow();

  star_graph.DebugShow(string("From"));


#endif
  viewer->addView(master_view.GetView());
  viewer->addView(sensor_view.GetView());
  thread t([]() {
    cv::waitKey(0);
  });
  viewer->run();
  t.join();
  return 0;
}
