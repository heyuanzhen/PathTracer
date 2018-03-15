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
    Matrix4d invM = trans.getInvM();
    Matrix4d invMRot = trans.getInvMRot();
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

Point3d Shape::sample(Point2d u, double &pdf) const {
    std::cout<<"This shape cannot be sampled !"<<std::endl;
    return Point3d(0.0, 0.0, 0.0);
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
        return MAX_DOUBLE;
    }
    else{
        double t1 = (-b - sqrt(delta)) / (2.0 * a);
        if (t1 < 0.0) {
            return MAX_DOUBLE;
        }
        else{
            return  t1;
        }
    }
}

double Sphere::Area() const {
    return 4 * M_PI * radius * radius;
}

////Triangle Shape
Triangle::Triangle(Point3d _p0, Point3d _p1, Point3d _p2) : p0(_p0), p1(_p1), p2(_p2), Shape(TRIANGLE) {
    e1 = p1 - p0;
    e2 = p2 - p0;
    n = e1.cross(e2).normalized();
}

Triangle::~Triangle() {}

Point3d Triangle::getPointByUV(double uu, double vv) const {
    return (1.0 - uu - vv) * p0 + uu * p1 + vv * p2;
}

Vector3d Triangle::getNormal(Point3d pWorld) {
    return n;
}

double Triangle::isIntersected(Ray *ray) {
    Vector3d q = ray->getDirection().cross(e2);
    double a = e1.dot(q);
    if (abs(a) < eps) {
        return MAX_DOUBLE;
    }
    double f = 1.0 / a;
    Vector3d s = ray->getOrigin() - p0;
    double u = f * (s.dot(q));
    if (u < 0.0) {
        return MAX_DOUBLE;
    }
    Vector3d r = s.cross(e1);
    double v = f * (ray->getDirection().dot(r));
    if (v < 0.0 || (u + v) > 1.0) {
        return MAX_DOUBLE;
    }
//    std::cout<<f<<", "<<e2.transpose()<<", "<<q.transpose()<<std::endl;
    double t = f * (e2.dot(r));
    if (t < 0.0) {
        return MAX_DOUBLE;
    }
//    std::cout<<t<<std::endl;
    return t;
}

Point3d Triangle::sample(Point2d u, double &pdf) const {
    pdf = 1.0 / Area();
    return getPointByUV(u[0], u[1]);
}

double Triangle::Area() const {
//    double x1 = p0.x(), y1 = p0.y(), z1 = p0.z();
//    double x2 = p1.x(), y2 = p1.y(), z2 = p1.z();
//    double x3 = p2.x(), y3 = p2.y(), z3 = p2.z();
//    return 0.5 * abs((x1 * y2 - x2 * y1)+(x2 * y3 - x3 * y2) + (x3 * y1 - x1 * y3));
    return 0.5 * e1.cross(e2).norm();
}

