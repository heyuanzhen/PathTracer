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

Intersection::Intersection(Ray* r, Shape* sp, Point3f iP, Vector3f lN) :
                           ray(r), shape(sp), interP(iP), localN(lN) {}

Intersection::~Intersection() {}

Ray* Intersection::getRay() const {
    return ray;
}

Shape* Intersection::getShape() const {
    return shape;
}

Point3f Intersection::getInterPoint() const {
    return interP;
}

Vector3f Intersection::getLocalNormal() const {
    return localN;
}
