//
// Created by cdh on 19-9-21.
//

#include "CommonFunc.h"

#include <osg/Geode>
#include <osg/MatrixTransform>

#include "Shape.h"
using namespace std;
using namespace osg;
void CreateLongitudeLatitude(Group *root, float radio) {
  float cos30 = cos(30./180.*PI);
  float sin30 = sin(30./180.*PI);
  {
    auto *geode = new Geode;
    root->addChild(geode);
    auto *circle = CreateCircle(Vec3(1.f, 1.f, 1.f), radio, 100);
    geode->addDrawable(circle);
  }
  {
    auto *mt = new MatrixTransform;
    root->addChild(mt);
    auto *geode = new Geode;
    mt->addChild(geode);
    auto *circle = CreateCircle(Vec3(1.f, 1.f, 1.f), cos30*radio, 100);
    geode->addDrawable(circle);

    mt->setMatrix(Matrix::translate(0., 0., sin30*radio));
  }
  {
    auto *mt = new MatrixTransform;
    root->addChild(mt);
    auto *geode = new Geode;
    mt->addChild(geode);
    auto *circle = CreateCircle(Vec3(1.f, 1.f, 1.f), cos30*radio, 100);
    geode->addDrawable(circle);

    mt->setMatrix(Matrix::translate(0., 0., -sin30*radio));
  }
  {
    auto *mt = new MatrixTransform;
    root->addChild(mt);
    auto *geode = new Geode;
    mt->addChild(geode);
    auto *circle = CreateCircle(Vec3(1.f, 1.f, 1.f), sin30*radio, 100);
    geode->addDrawable(circle);

    mt->setMatrix(Matrix::translate(0., 0., cos30*radio));
  }
  {
    auto *mt = new MatrixTransform;
    root->addChild(mt);
    auto *geode = new Geode;
    mt->addChild(geode);
    auto *circle = CreateCircle(Vec3(1.f, 1.f, 1.f), sin30*radio, 100);
    geode->addDrawable(circle);

    mt->setMatrix(Matrix::translate(0., 0., -cos30*radio));
  }
  {
    auto *mt = new MatrixTransform;
    root->addChild(mt);
    auto *geode = new Geode;
    mt->addChild(geode);
    auto *circle = CreateCircle(Vec3(1.f, 1.f, 0.f), radio, 100);
    geode->addDrawable(circle);

    mt->setMatrix(Matrix::rotate(Vec3(0., 0., 1.), Vec3(-1., 0., 0.)));
  }
  {
    auto *mt = new MatrixTransform;
    root->addChild(mt);
    auto *geode = new Geode;
    mt->addChild(geode);
    auto *circle = CreateCircle(Vec3(1.f, 1.f, 0.f), radio, 100);
    geode->addDrawable(circle);

    mt->setMatrix(Matrix::rotate(Vec3(0., 0., 1.), Vec3(0., 1., 0.)));
  }
  {
    auto *mt = new MatrixTransform;
    root->addChild(mt);
    auto *geode = new Geode;
    mt->addChild(geode);
    auto *circle = CreateCircle(Vec3(1.f, 1.f, 0.f), radio, 100);
    geode->addDrawable(circle);

    mt->setMatrix(Matrix::rotate(Vec3(0., 0., 1.), Vec3(1., 1., 0.)));
  }
  {
    auto *mt = new MatrixTransform;
    root->addChild(mt);
    auto *geode = new Geode;
    mt->addChild(geode);
    auto *circle = CreateCircle(Vec3(1.f, 1.f, 0.f), radio, 100);
    geode->addDrawable(circle);

    mt->setMatrix(Matrix::rotate(Vec3(0., 0., 1.), Vec3(1., -1., 0.)));
  }
}

