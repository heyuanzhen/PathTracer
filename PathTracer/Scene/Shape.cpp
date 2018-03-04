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
Shape::Shape(ShapeType tp) : type(tp), material(nullptr) {}

Shape::~Shape() {}

void Shape::transRayToLocal(Ray* rayW, Ray& rayL) {
    Matrix4f invM = trans.getInvM();
    Matrix4f invMRot = trans.getInvMRot();
    Point3d oL = worldToLocal(invM, rayW->getOrigin());
    Vector3d dL = worldToLocal(invMRot, rayW->getDirection());
    rayL.setRay(oL, dL, 0.0);
}

Material* Shape::getMaterial() const {
    return material;
}

void Shape::setMaterial(Material *material) {
    this->material = material;
}

////Sphere class
Sphere::Sphere(double _r, Vector3d _cenPos) : radius(_r), center(_cenPos), Shape(SPHERE) {
    double ds[3] = {_cenPos(0), _cenPos(1), _cenPos(2)};
    trans.setTranslation(ds);
}

Sphere::~Sphere() {}

Point3d Sphere::getCenPos() const {
    return center;
}

double Sphere::getR() const {
    return radius;
}

Vector3d Sphere::getNormal(Vector3d pWorld) {
    Vector3d normal = pWorld - center;
    if (normal.norm() - radius > eps) {
        std::cout<<normal.transpose()<<", "<<normal.norm() - radius<<std::endl;
        std::cout<<"This point is not on the sphere's surface !"<<std::endl;
        return Vector3d(0.0, 0.0, 0.0);
    }
    return normal.normalized();
}

double Sphere::isIntersected(Ray *rayW) {
    //1.cordinate transform
    //2.get quadratic equation's coefficient
    //3.delta judgement
    //4.back surface test
    Ray rayL = Ray(Point3d(0.0, 0.0, 0.0), Vector3d(0.0, 0.0, 0.0), 0.0);
    transRayToLocal(rayW, rayL);
    Point3d oL = rayL.getOrigin();
    Vector3d dL = rayL.getDirection();
//    std::cout<<dL.transpose()<<std::endl;
    double a = dL(0) * dL(0) + dL(1) * dL(1) + dL(2) * dL(2);
    double b = 2.0 * (dL(0) * oL(0) + dL(1) * oL(1) + dL(2) * oL(2));
    double c = oL(0) * oL(0) + oL(1) * oL(1) + oL(2) * oL(2) - radius * radius;
    double delta = b * b - 4.0 * a * c; //b^2-4ac
    if (delta < 0.0) {
        return MAX_double;
    }
    else{
        double t1 = (-b - sqrt(delta)) / (2.0 * a);
        if (t1 < 0.0) {
            return MAX_double;
        }
        else{
            return  t1;
        }
    }
}





