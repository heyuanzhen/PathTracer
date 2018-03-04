//
//  Intersection.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/21.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "Intersection.h"
#include "Ray.h"
#include "Shape.h"

Intersection::Intersection() {
    ray = nullptr;
    shape = nullptr;
    matetial = nullptr;
    isInter = false;
}

Intersection::Intersection(Ray* r, Shape* sp, Point3d iP, Vector3d lN, Material* mt) :
                           ray(r), shape(sp), interP(iP), localN(lN), matetial(mt), isInter(true) {}

Intersection::~Intersection() {}

Ray* Intersection::getRay() const {
    return ray;
}

Shape* Intersection::getShape() const {
    return shape;
}

Point3d Intersection::getInterPoint() const {
    return interP;
}

Vector3d Intersection::getLocalNormal() const {
    return localN;
}

Material* Intersection::getMaterial() const {
    return matetial;
}

bool Intersection::getIsInter() const {
    return isInter;
}
