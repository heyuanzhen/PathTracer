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
    o = Vector3f(0.0, 0.0, 0.0);
    d = Vector3f(0.0, 0.0, 0.0);
    t = 0.0;
}

Ray::Ray(Point3f _o, Vector3f _d, float _t) : o(_o), d(_d), t(_t){}

Ray::~Ray() {}

Point3f Ray::getOrigin() const {
    return o;
}

Vector3f Ray::getDirection() const {
    return d;
}

float Ray::getT() const {
    return t;
}

Intersection* Ray::getIntersection() {
    return &its;
}

Point3f Ray::calcP() const {
    return o + t * d;
}

void Ray::setRay(Point3f _o, Vector3f _d, float _t) {
    o = _o;
    d = _d;
    t = _t;
}

void Ray::setT(float _t) {
    t = _t;
}

void Ray::findIntersection(Scene* scene) {
    brutalWayToFind(scene);
}

void Ray::brutalWayToFind(Scene* scene) {
    if (!scene->getShapeCount()) {
        std::cout<<"No shape in the scene !"<<std::endl;
    }
    Vector3f interP;
    float t_min = MAX_FLOAT;
    Intersection its_now;
    for (int i = 0; i < scene->getShapeCount(); i++) {
//        std::cout<<i<<std::endl;
        Shape* sp = scene->getShape(i);
        float t_now = sp->isIntersected(this);
        if (t_now < t_min) {
            t_min = t_now;
            t = t_now;
            its = Intersection(this, sp, calcP(), sp->getNormal(calcP()), sp->getMaterial());
        }
        else{
            continue;
        }
    }
}







