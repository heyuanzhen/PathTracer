//
//  Shape.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/19.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "Shape.h"
#include "Light.h"
#include "sampling.h"
#include "operation.h"
#include <iostream>

////Shape class
Shape::Shape(ShapeType tp, bool isE) : type(tp), isEmmit(isE), material(nullptr), areaLight(nullptr) {}

Shape::~Shape() {}

void Shape::transRayToLocal(Ray* rayW, Ray& rayL) {
    Matrix4d invM = trans.getInvM();
    Matrix4d invMRot = trans.getInvMRot();
    Point3d oL = worldToLocal(invM, rayW->getOrigin());
    Vector3d dL = worldToLocal(invMRot, rayW->getDirection());
    rayL.setRay(oL, dL, 0.0);
}

void Shape::setAreaLight(AreaLight *al) {
    if (al->type != Light::AREA) {
        std::cout<<"This light source is not area light !"<<std::endl;
        return;
    }
    areaLight = al;
}

Material* Shape::getMaterial() const {
    return material;
}

AreaLight* Shape::getAreaLight() const {
    return areaLight;
}

void Shape::setMaterial(Material *material) {
    this->material = material;
}

Point3d Shape::sample(Point2d u, double &pdf) const {
    std::cout<<"This shape cannot be sampled !"<<std::endl;
    return Point3d(0.0, 0.0, 0.0);
}

Point3d Shape::sample(const Intersection *it, const Point2d u, double &pdf) const {
    std::cout<<"This shape cannot be sampled !"<<std::endl;
    return Point3d(0.0, 0.0, 0.0);
}

double Shape::pdf(const Intersection* itSur, Point3d pLi, Vector3d wi) const {
    std::cout<<"This shape cannot calculate pdf !"<<std::endl;
    return 0.0;
}

////Sphere class
Sphere::Sphere(double _r, Vector3d _cenPos, bool isE) : radius(_r), center(_cenPos), Shape(SPHERE, isE) {
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

Vector3d Sphere::getNormal(Vector3d pWorld) const {
    Vector3d normal = pWorld - center;
    if (normal.norm() - radius > eps * 1e5) {
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
        double t2 = (-b + sqrt(delta)) / (2.0 * a);
        if (t1 < 0.0) {
            if (t2 < 0.0) {
                return MAX_DOUBLE;
            }
            else {
                return t2;
            }
        }
        else{
            return  t1;
        }
    }
}

double Sphere::Area() const {
    return 4 * M_PI * radius * radius;
}

Point3d Sphere::sample(Point2d u, double &pdf) const {
    pdf = 1.0 / Area();
    return center + uniformSampleSphere(u) * radius;
}

Point3d Sphere::sample(const Intersection *it, const Point2d u, double &pdf) const {
    Point3d pRefToCen = it->getInterPoint() - center;
    double dc = pRefToCen.norm();
    if (dc - radius < eps) {
//        std::cout<<"Sample point is in the sphere !"<<", p: "<<it->getInterPoint().transpose()<<
//        ", cen = "<<center.transpose()<<", dc = "<<dc<<", radius = "<<radius<<std::endl;
        pdf = 1.0;
        return it->getInterPoint();
    }
    double sinThetaMax2 = radius * radius / (dc * dc);
    double cosThetaMax = sqrt(std::max(0.0, 1.0 - sinThetaMax2));
    double cosTheta = (1.0 - u[0]) + u[0] * cosThetaMax;
    double sinTheta = sqrt(1.0 - cosTheta * cosTheta);
    double phi = u[1] * 2 * Pi;
    Vector3d negWiL = Point3d(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
    pdf = uniformSampleConePDF(cosThetaMax);
    
    double da = dc * sinTheta;
    double dtd = std::max(0.0, sqrt(radius * radius - da * da));
    double dsd = dc * cosTheta - dtd;
    Matrix3d rotM = calcRotateMartix(-pRefToCen.normalized(), Vector3d(0.0, 0.0, 1.0)).inverse();
    Vector3d negWiW = rotM * negWiL;
    return it->getInterPoint() + negWiW * dsd;
}

double Sphere::pdf(const Intersection *itSur, Point3d pLi, Vector3d wi) const {
    Point3d pRefToCen = itSur->getInterPoint() - center;
    double dc = pRefToCen.norm();
    if (dc - radius < eps) {
        return 1.0;
    }
    double sinThetaMax2 = radius * radius / (dc * dc);
    double cosThetaMax = sqrt(std::max(0.0, 1.0 - sinThetaMax2));
    return uniformSampleConePDF(cosThetaMax);
}

////Triangle Shape
Triangle::Triangle(Point3d _p0, Point3d _p1, Point3d _p2, bool isE) : p0(_p0), p1(_p1), p2(_p2), pn0(Vector3d(0.0, 0.0, 0.0)), pn1(Vector3d(0.0, 0.0, 0.0)), pn2(Vector3d(0.0, 0.0, 0.0)), Shape(TRIANGLE, isE) {
    e1 = p1 - p0;
    e2 = p2 - p0;
    n = -e1.cross(e2).normalized();
}

Triangle::Triangle(Point3d _p0, Point3d _p1, Point3d _p2, Vector3d _n0,Vector3d _n1, Vector3d _n2,
                   bool isE) :p0(_p0), p1(_p1), p2(_p2), pn0(_n0), pn1(_n1), pn2(_n2), Shape(TRIANGLE, isE)
{
    e1 = p1 - p0;
    e2 = p2 - p0;
    n = -e1.cross(e2).normalized();
}
Triangle::~Triangle() {}

Point3d Triangle::getPointByUV(double uu, double vv) const {
    return (1.0 - uu - vv) * p0 + uu * p1 + vv * p2;
}

Vector3d Triangle::getNormal(Point3d pWorld) const {
    if (pn0.norm() < eps || pn1.norm() < eps || pn2.norm() < eps) {
        return n;
    }
    else { //need to be done
//        std::cout<<"here"<<std::endl;
        return ((pn0 + pn1 + pn2) * oneOverThree).normalized();
    }
}

void Triangle::setNormal(Vector3d nor) {
    n = nor;
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
    double t = f * (e2.dot(r));
    if (t < 0.0) {
        return MAX_DOUBLE;
    }
    return t;
}

Point3d Triangle::sample(Point2d u, double &pdf) const {
    pdf = 1.0 / Area();
    return getPointByUV(u[0], u[1]);
}



Point3d Triangle::sample(const Intersection *it, const Point2d u, double &pdf) const {
    Point3d pShape = sample(u, pdf);
    Vector3d wi = (it->getInterPoint() - pShape).normalized();
    if (wi.norm() < eps) {
        pdf = 0.0;
    }
    else{
        if (getNormal(pShape).dot(wi) < eps) {
            pdf = 0.0;
        }
        else{
            pdf *= distanceSquared(it->getInterPoint(), pShape) / abs(getNormal(pShape).dot(wi));
        }
    }
    return pShape;
}


double Triangle::pdf(const Intersection *itSur, Point3d pLi, Vector3d wi) const {
    Point3d pSur = itSur->getInterPoint();
    double cosA = getNormal(pLi).dot(wi);
    if (cosA < eps) {
        return 0.0;
    }
    return distanceSquared(pSur, pLi) / abs(getNormal(pLi).dot(wi));
}

double Triangle::Area() const {
    return 0.5 * e1.cross(e2).norm();
}

Rectangular::Rectangular(Point3d _p0, Point3d _p1, Point3d _p2, bool isE) :p0(_p0), p1(_p1), p2(_p2), tri1(Triangle(_p0, _p1, _p2, isE)), tri2(Triangle(_p2, p0 + p2 - p1, _p0, isE)), Shape(RECTANGULAR, isE) {
    e1 = p1 - p0;
    e2 = p2 - p1;
}

Rectangular::~Rectangular() {}

void Rectangular::setNormal(Vector3d nor) {
    tri1.setNormal(nor);
    tri2.setNormal(nor);
}

Vector3d Rectangular::getNormal(Point3d pWorld) const {
    return tri1.getNormal(pWorld);
}

double Rectangular::isIntersected(Ray *ray) {
    return std::min(tri1.isIntersected(ray), tri2.isIntersected(ray));
}

Point3d Rectangular::sample(Point2d u, double &pdf) const {
    pdf = 1.0 / Area();
    return p0 + e1 * u[0] + e2 * u[1];
}

Point3d Rectangular::sample(const Intersection *it, const Point2d u, double &pdf) const {
    Point3d pShape = sample(u, pdf);
    Vector3d wi = (it->getInterPoint() - pShape).normalized();
    if (wi.norm() < eps) {
        pdf = 0.0;
    }
    else{
        if (getNormal(pShape).dot(wi) < eps) {
            pdf = 0.0;
        }
        else{
            pdf *= distanceSquared(it->getInterPoint(), pShape) / abs(getNormal(pShape).dot(wi));
        }
    }
    return pShape;
}

double Rectangular::pdf(const Intersection *itSur, Point3d pLi, Vector3d wi) const {
    Point3d pSur = itSur->getInterPoint();
    double cosA = getNormal(pLi).dot(wi);
    if (cosA < eps) {
        return 0.0;
    }
    return distanceSquared(pSur, pLi) / abs(getNormal(pLi).dot(wi));
}

double Rectangular::Area() const {
    return tri1.Area() + tri2.Area();
}


