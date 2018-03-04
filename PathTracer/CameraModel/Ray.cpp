//
//  Ray.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/19.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "Ray.h"
#include "Shape.h"
#include <iostream>

Ray::Ray() {
    o = Vector3d(0.0, 0.0, 0.0);
    d = Vector3d(0.0, 0.0, 0.0);
    t = 0.0;
}

Ray::Ray(Point3d _o, Vector3d _d, double _t) : o(_o), d(_d), t(_t){}

Ray::~Ray() {}

Point3d Ray::getOrigin() const {
    return o;
}

Vector3d Ray::getDirection() const {
    return d;
}

double Ray::getT() const {
    return t;
}

Intersection* Ray::getIntersection(){
    return &its;
}

Spectrum3d Ray::getRadiance() const {
    return radiance;
}

Point3d Ray::calcP() const {
    return o + t * d;
}

bool Ray::isInit() const {
    return d.norm() > eps ? true : false;
}

void Ray::setRay(Point3d _o, Vector3d _d, double _t) {
    o = _o;
    d = _d;
    t = _t;
}

void Ray::setT(double _t) {
    t = _t;
}

void Ray::setRadiance(Spectrum3d rad) {
    radiance = rad;
}

bool Ray::findIntersection(const Scene* scene) {
    return brutalWayToFind(scene);
}

bool Ray::findInterBetween(const Scene *scene, double tmax) {
    return brutalWayToFindBetween(scene, tmax);
}

bool Ray::brutalWayToFind(const Scene* scene) {
    bool isIntersected = false;
    if (!scene->getShapeCount()) {
        std::cout<<"No shape in the scene !"<<std::endl;
    }
    Vector3d interP;
    double t_min = MAX_double;
    Intersection its_now;
    for (int i = 0; i < scene->getShapeCount(); i++) {
//        std::cout<<i<<std::endl;
        Shape* sp = scene->getShape(i);
        double t_now = sp->isIntersected(this);
        if (t_now < t_min && t_now > 0.0) {
            t_min = t_now;
            t = t_now;
            its = Intersection(this, sp, calcP(), sp->getNormal(calcP()), sp->getMaterial());
            isIntersected = true;
        }
    }
    return isIntersected;
}

bool Ray::brutalWayToFindBetween(const Scene *scene, double tmax) {
    if (!scene->getShapeCount()) {
        std::cout<<"No shape in the scene !"<<std::endl;
    }
    Vector3d interP;
    Intersection its_now;
    for (int i = 0; i < scene->getShapeCount(); i++) {
        //        std::cout<<i<<std::endl;
        Shape* sp = scene->getShape(i);
        double t_now = sp->isIntersected(this);
        if ((t_now < (tmax - 1e-5)) && (t_now > 0.0)) {
            return true;
        }
    }
    return false;
}







