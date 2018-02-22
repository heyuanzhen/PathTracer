//
//  Shape.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/19.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "Shape.h"
#include <iostream>

////Shape class
Shape::Shape(ShapeType tp) : type(tp) {}
Shape::~Shape() {}


Sphere::Sphere(float _r, Vector3f _cenPos) : radius(_r), center(_cenPos), Shape(SPHERE) {}

Sphere::~Sphere() {}

Point3f Sphere::getCenPos() const {
    return center;
}

float Sphere::getR() const {
    return radius;
}

Vector3f Sphere::getNormal(Vector3f pWorld) {
    Vector3f normal = pWorld - center;
    if (normal.norm() - radius > eps) {
        std::cout<<"This point is not on the sphere's surface !"<<std::endl;
        return Vector3f(0.0, 0.0, 0.0);
    }
    return normal.normalized();
}

bool Sphere::isIntersected(Ray *ray, Intersection* its) {
    Matrix4f invM = trans.getInvM();
    return false;
}
