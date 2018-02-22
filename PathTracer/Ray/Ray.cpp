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

Ray::Ray(Point3f _o, Vector3f _d, float _t) : o(_o), d(_d), t(_t), its(nullptr) {}

Ray::~Ray() {}

void Ray::findIntersection(Scene* scene) {
    brutalWayToFind(scene);
}

void Ray::brutalWayToFind(Scene* scene) {
    if (!scene->getShapeCount()) {
        std::cout<<"No shape in the scene !"<<std::endl;
    }
    Vector3f interP;
    float t_min = MAX_FLOAT;
    Intersection* its_now = nullptr;
    for (int i = 0; i < scene->getShapeCount(); i++) {
        Shape* sp = scene->getShape(i);
        sp->isIntersected(this, its_now);
        if (t < t_min) {
            t_min = t;
            its = its_now;
        }
    }
}

