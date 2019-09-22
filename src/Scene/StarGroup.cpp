//
// Created by chudonghao on 2019/9/21.
//

#include "StarGroup.h"
#include <osg/MatrixTransform>
#include "../Core/StarTable.h"
#include "Shape.h"
#include "Planet.h"

using namespace std;
using namespace osg;
StarGroup::StarGroup(float radio) : radio_(radio) {
  InitPlanets();
  InitLongitudeLatitude();
}
void StarGroup::InitPlanets() {
  auto *star_group = new Group;
  addChild(star_group);
  for (const auto &item : StarTable::instance()->Table()) {
    auto *mt = new MatrixTransform;
    star_group->addChild(mt);
    auto *planet = new Planet(item.second.id, Vec4(1.f, 1.f, 1.f, 1.f), exp(log(1.3797)*(-item.second.l + 6.))*3.f);
    mt->addChild(planet);

    id_planet_map_[item.second.id] = planet;

    auto rotate_a = Matrix::rotate(DegreesToRadians(item.second.a), Vec3d(0., 0., 1.));
    //auto rotate_axis_ny = Vec3d(0., -1., 0.)*rotate_a;
    auto rotate_b = Matrix::rotate(DegreesToRadians(item.second.b), Vec3d(0., -1., 0.));

    planet->setName(item.second.id + ':' + to_string(item.second.a) + ':' + to_string(item.second.b) + ':'
                        + to_string(item.second.l));
    mt->setMatrix(Matrix::translate(Vec3(2000., 0., 0.))*rotate_b*rotate_a);
  }
  auto *long_lat_group = new Group;
  addChild(long_lat_group);
}
void StarGroup::InitLongitudeLatitude() {
  float cos30 = cos(30./180.*PI);
  float sin30 = sin(30./180.*PI);
  {
    auto *geode = new Geode;
    addChild(geode);
    auto *circle = CreateCircle(Vec3(1.f, 1.f, 1.f), radio_, 100);
    geode->addDrawable(circle);
  }
  {
    auto *mt = new MatrixTransform;
    addChild(mt);
    auto *geode = new Geode;
    mt->addChild(geode);
    auto *circle = CreateCircle(Vec3(1.f, 1.f, 1.f), cos30*radio_, 100);
    geode->addDrawable(circle);

    mt->setMatrix(Matrix::translate(0., 0., sin30*radio_));
  }
  {
    auto *mt = new MatrixTransform;
    addChild(mt);
    auto *geode = new Geode;
    mt->addChild(geode);
    auto *circle = CreateCircle(Vec3(1.f, 1.f, 1.f), cos30*radio_, 100);
    geode->addDrawable(circle);

    mt->setMatrix(Matrix::translate(0., 0., -sin30*radio_));
  }
  {
    auto *mt = new MatrixTransform;
    addChild(mt);
    auto *geode = new Geode;
    mt->addChild(geode);
    auto *circle = CreateCircle(Vec3(1.f, 1.f, 1.f), sin30*radio_, 100);
    geode->addDrawable(circle);

    mt->setMatrix(Matrix::translate(0., 0., cos30*radio_));
  }
  {
    auto *mt = new MatrixTransform;
    addChild(mt);
    auto *geode = new Geode;
    mt->addChild(geode);
    auto *circle = CreateCircle(Vec3(1.f, 1.f, 1.f), sin30*radio_, 100);
    geode->addDrawable(circle);

    mt->setMatrix(Matrix::translate(0., 0., -cos30*radio_));
  }
  {
    auto *mt = new MatrixTransform;
    addChild(mt);
    auto *geode = new Geode;
    mt->addChild(geode);
    auto *circle = CreateCircle(Vec3(1.f, 1.f, 0.f), radio_, 100);
    geode->addDrawable(circle);

    mt->setMatrix(Matrix::rotate(Vec3(0., 0., 1.), Vec3(-1., 0., 0.)));
  }
  {
    auto *mt = new MatrixTransform;
    addChild(mt);
    auto *geode = new Geode;
    mt->addChild(geode);
    auto *circle = CreateCircle(Vec3(1.f, 1.f, 0.f), radio_, 100);
    geode->addDrawable(circle);

    mt->setMatrix(Matrix::rotate(Vec3(0., 0., 1.), Vec3(0., 1., 0.)));
  }
  {
    auto *mt = new MatrixTransform;
    addChild(mt);
    auto *geode = new Geode;
    mt->addChild(geode);
    auto *circle = CreateCircle(Vec3(1.f, 1.f, 0.f), radio_, 100);
    geode->addDrawable(circle);

    mt->setMatrix(Matrix::rotate(Vec3(0., 0., 1.), Vec3(1., 1., 0.)));
  }
  {
    auto *mt = new MatrixTransform;
    addChild(mt);
    auto *geode = new Geode;
    mt->addChild(geode);
    auto *circle = CreateCircle(Vec3(1.f, 1.f, 0.f), radio_, 100);
    geode->addDrawable(circle);

    mt->setMatrix(Matrix::rotate(Vec3(0., 0., 1.), Vec3(1., -1., 0.)));
  }
}
