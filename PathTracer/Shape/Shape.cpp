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

void Shape::transRayToLocal(Ray* rayW, Ray& rayL) {
    Matrix4f invM = trans.getInvM();
    Matrix4f invMRot = trans.getInvMRot();
    Point3f oL = worldToLocal(invM, rayW->getOrigin());
    Vector3f dL = worldToLocal(invMRot, rayW->getDirection());
    rayL.setRay(oL, dL, 0.0);
}


////Sphere class
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

float Sphere::isIntersected(Ray *rayW, Intersection* its) {
    //1.cordinate transform
    //2.get quadratic equation's coefficient
    //3.delta judgement
    //4.back surface test
    Ray rayL = Ray(Point3f(0.0, 0.0, 0.0), Vector3f(0.0, 0.0, 0.0), 0.0);
    transRayToLocal(rayW, rayL);
    Point3f oL = rayL.getOrigin();
    Vector3f dL = rayL.getDirection();
    float a = dL(0) * dL(0) + dL(1) * dL(1) + dL(2) * dL(2);
    float b = 2.0 * (dL(0) * oL(0) + dL(1) * oL(1) + dL(2) * oL(2));
    float c = oL(0) * oL(0) + oL(1) * oL(1) + oL(2) * oL(2) - radius * radius;
    float delta = b * b - 4.0 * a * c; //b^2-4ac
    if (delta < 0.0) {
        return MAX_FLOAT;
    }
    else{
        float x1 = (-b - sqrt(delta)) / (2.0 * a);
        if (x1 < 0.0) {
            return MAX_FLOAT;
        }
        else{
            return  x1;
        }
    }
    return false;
}





