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

  viewer->addView(master_view.GetView());
  viewer->addView(sensor_view.GetView());
  viewer->realize();
  viewer->frame();
  //StarGraph star_graph;
  //star_graph.InitFrom("xingtu01.xml", 512., 12.);
  //star_graph.DebugShow_ToSkySpherePos(string("xingtu01.xml"));
  //star_graph.InitFrom("xingtu02.xml", 512., 12.);
  //star_graph.DebugShow_ToSkySpherePos(string("xingtu02.xml"));
  //star_graph.InitFrom("xingtu03.xml", 512., 12.);
  //star_graph.DebugShow_ToSkySpherePos(string("xingtu03.xml"));
  //star_graph.InitFrom("xingtu04.xml", 512., 12.);
  //star_graph.DebugShow_ToSkySpherePos(string("xingtu04.xml"));
  //star_graph.InitFrom("xingtu05.xml", 512., 12.);
  //star_graph.DebugShow_ToSkySpherePos(string("xingtu05.xml"));
  //star_graph.InitFrom("xingtu06.xml", 512., 12.);
  //star_graph.DebugShow_ToSkySpherePos(string("xingtu06.xml"));
  //star_graph.InitFrom("xingtu07.xml", 1024., 20.);
  //star_graph.DebugShow_ToSkySpherePos(string("xingtu07.xml"));
  //star_graph.InitFrom("xingtu08.xml", 1024., 20.);
  //star_graph.DebugShow_ToSkySpherePos(string("xingtu08.xml"));

  //star_graph.DebugInitFromStarTable(StarSkySpherePos(90., 0.), 512, 12);
  //star_graph.DebugShow_ToSkySpherePos(string("(90.,0.)"));
  //star_group->addChild(star_graph.DebugShow());


#if 0

  std::map<std::string, Descriptor2<80, 16>> *dess1 = star_graph.GetDescriptor2s<80, 16>();

  std::map<std::string, Descriptor2<80, 16>> *dess2 = StarTable::instance()->CreateDescriptorDatabase<80, 16>();

  multimap<float, string> match1;
  multimap<float, string> match2;
  set<string> matched;
  set<string> match2_input;
  for (auto &des1:*dess1) {
    LOG_TRACE << des1.first << ":" << des1.second.GetStarNum();
    match1.clear();
    match2.clear();
    matched.clear();
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
      matched.insert(iter->second);
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
    if (matched.size() <= 30) {
      for (auto &name  : matched) {
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
  //////////////////////////////////////
  // Descriptor Config
  constexpr int precise = 101;
  StarGraph star_graph3;
  //string star_graph_id = "01";
  //double star_graph_size = 512.;
  //double star_graph_fov = 12.;
  //string star_graph_id = "02";
  //double star_graph_size = 512.;
  //double star_graph_fov = 12.;
  //string star_graph_id = "03";
  //double star_graph_size = 512.;
  //double star_graph_fov = 12.;
  //string star_graph_id = "04";
  //double star_graph_size = 512.;
  //double star_graph_fov = 12.;
  //string star_graph_id = "05";
  //double star_graph_size = 512.;
  //double star_graph_fov = 12.;
  //string star_graph_id = "06";
  //double star_graph_size = 512.;
  //double star_graph_fov = 12.;
  //string star_graph_id = "07";
  //double star_graph_size = 1024.;
  //double star_graph_fov = 20.;
  string star_graph_id = "08";
  double star_graph_size = 1024.;
  double star_graph_fov = 20.;
  star_graph3.InitFrom(string("xingtu") + star_graph_id + ".xml", star_graph_size, star_graph_fov);
  //star_graph3.DebugInitFromStarTable(StarSkySpherePos(180., 31.), 512., 12.);
  star_graph3.DebugShow_ToSkySpherePos(string(star_graph_id));
  //////////////////////////////////////
  // match result set
  multimap<float, string> matches;

  //////////////////////////////////////
  // Get descriptor for test star
  auto master_star_tested_group = star_graph3.GetMasterGroup();
  master_star_tested_group.Shirk();
  DescriptorConverter<SpecialCenterStarOnSkySphereGroup, Descriptor3<precise>> converter;
  Descriptor3<precise> star_graph_des;
  converter(master_star_tested_group, star_graph_des);

  auto star_table_des_set =
      StarTable::instance()->CreateDescriptor3Database<precise>(master_star_tested_group.GetValidRegionRadio(),
                                                                master_star_tested_group.Size());
  if(!star_table_des_set){
    LOG_FATAL << "Can NOT create star table descriptor set.";
  }

  for (auto &item:*star_table_des_set) {
    float sim = star_graph_des.Similarity(item.second);
    matches.emplace(sim, item.first);
  }
  if (matches.empty()) {
    LOG_WARNING << "Something Wrong.";
  } else {
    auto star_tested_rel_on_sky_sphere = star_graph_des.GetSpecialCenterStarOnSkySphereGroup().GetSpecialCenter();
    auto star_tested_name = star_tested_rel_on_sky_sphere.GetName();

    auto star_matched_name = matches.rbegin()->second;
    auto &star_matched_des = (*star_table_des_set)[star_matched_name];
    auto star_matched_on_sky_sphere = StarOnSkySphere(StarTable::instance()->Table()[star_matched_name]);
    auto star_matched_id = star_matched_name;
    auto star_matched_longitude = star_matched_on_sky_sphere.GetSkySpherePos().GetLongitude();
    auto star_matched_latitude = star_matched_on_sky_sphere.GetSkySpherePos().GetLatitude();
    // TODO more specialized
    auto match_reliability = star_graph_des.GetStarNum() >= 5 ? matches.rbegin()->first/(star_graph_des.GetStarNum()*(1.6)) : matches.rbegin()->first/5.;

    ///////////////////////////////////////////////////////////////////////////
    // Highlight matched stars on sky sphere
    for (auto &star:star_matched_des.GetSpecialCenterStarOnSkySphereGroup().GetStaresOnSkyShphere()) {
      star_group->HighLight(star.second.star.GetName(), Vec4(0.f, 1.f, 0.f, 1.f));
    }

    ///////////////////////////////////////////////////////////////////////////
    // Display tested star on sky sphere
    Vec2d dir1 = star_graph_des.GetSpecialDir();
    Vec2d dir2 = star_matched_des.GetSpecialDir();

    // Rotate to 0 0
    Matrix rotate_0 = Matrix::rotate(star_tested_rel_on_sky_sphere.GetSkySpherePos().WorldPosition(1.), Vec3d(1., 0., 0.));
    // Rotate special dir
    Matrix rotate_1 = Matrix::rotate(Vec3d(0., -dir2.x(), dir2.y()), Vec3d(0., -dir1.x(), dir1.y()));
    Matrix rotate_lon = Matrix::rotate(DegreesToRadians(star_matched_on_sky_sphere.GetSkySpherePos().GetLongitude()), Vec3(0.f, 0.f, 1.f));
    Matrix rotate_lat = Matrix::rotate(DegreesToRadians(star_matched_on_sky_sphere.GetSkySpherePos().GetLatitude()), Vec3(0.f, -1.f, 0.f));

    // Draw tested stars
    auto star_group_relative_to_view_center = star_graph3.GetStarGroupRelativeToViewCenter();
    for (auto &star:star_group_relative_to_view_center.GetStaresOnSkyShphere()) {
      auto &star_on_sky_sphere_rel = star.second.star;

      Planet *p = new Planet(star_on_sky_sphere_rel.GetName(), Vec4(1.f, 0.f, 0.f, 1.f), 10.f);
      osg::Vec2d pos = star_on_sky_sphere_rel.GetSkySpherePos();
      auto rotate_a = Matrix::rotate(DegreesToRadians(pos.x()), Vec3d(0., 0., 1.));
      auto rotate_b = Matrix::rotate(DegreesToRadians(pos.y()), Vec3d(0., -1., 0.));
      Matrix m = Matrix::translate(Vec3(2000., 0., 0.))*rotate_b*rotate_a*Matrix::scale(Vec3(1.005f, 1.005f, 1.005f));
      MatrixTransform *mt = new MatrixTransform();
      mt->setMatrix(m*rotate_0*rotate_1*rotate_lat*rotate_lon);
      mt->addChild(p);
      star_group->addChild(mt);
    }
    ///////////////////////////////////////////
    // Show result
    Vec3d D_ = Vec3d(1., 0., 0.);
    StarSkySpherePos D;
    D_ = D_*rotate_0*rotate_1*rotate_lat*rotate_lon;
    D.FromWorldPosition(D_);

    stringstream ss_match_list;
    int count = 0;
    for (auto pair_iter = matches.rbegin(); pair_iter != matches.rend(); ++pair_iter) {
      auto &star_data = StarTable::instance()->Table()[pair_iter->second];
      ss_match_list << "Star: " << star_data.id << "(" << star_data.a << "," << star_data.b << "," << star_data.l << ")" << " < > Value: " << pair_iter->first << "\n";
      ++count;
      if (count >= 10) {
        ss_match_list << "...\n";
        break;
      }
    }
    LOG_INFO << "\n" <<
             "=========================Test==========================\n" <<
             "Testing star: id=" << master_star_tested_group.GetSpecialCenter().GetName() << "\n" <<
             "Companion star size: " << master_star_tested_group.Size() << "\n" <<
             "Valid region radio: " <<
             master_star_tested_group.GetValidRegionRadio() << "\n" <<
             "=========================MatchList=====================\n" <<
             ss_match_list.str() <<
             "=========================Match=========================\n" <<
             "Matched star: id=" << star_matched_id << " longitude=" << star_matched_longitude << " latitude=" << star_matched_latitude << "\n" <<
             "Match reliability: " << match_reliability << " " <<
             "(" << matches.rbegin()->first << "/" << star_graph_des.GetStarNum() << "/" << star_graph3.GetStarGroupRelativeToViewCenter().Size() << ")\n" <<
             "=========================Result========================\n" <<
             "Sensor direction: longitude=" << D.GetLongitude() << " latitude=" << D.GetLatitude();

    ///////////////////////////////////////////////////////////////////////////
    // Display Descriptor and graph
    StarGraph star_matched_graph;
    star_matched_graph.DebugInitFromStarTable(D, star_graph_size, star_graph_fov);
    star_matched_des.DebugShow("Matched");
    star_matched_graph.DebugShow(string("Matched"));

    star_graph3.DebugShow(string("Tested"));
    star_graph_des.DebugShow("Tested");
  }
#endif
  thread t([]() {
    cv::waitKey(0);
  });
  viewer->run();
  t.join();
  return 0;
}
