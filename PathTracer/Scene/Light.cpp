//
//  Light.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/20.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "Light.h"
#include "Ray.h"
#include "operation.h"
#include <iostream>

////Light abstract class

Light::Light(LightType tp) : type(tp) {}

Light::~Light() {}

bool Light::testVisibility(Point3d pL, Point3d pS, const Scene* scene) const {
    Ray r = Ray(pL, (pS - pL).normalized(), 0.0);
    int dim = 0;
    double maxD = getMaxDistanceInOneDim(pL, pS, dim);
    double tmax = maxD / r.getDirection()[dim]; //pL and pS must be this order
//    std::cout<<"maxD = "<<maxD<<"maxDir = "<<r.getDirection()[dim]<<std::endl;
    return !r.findInterBetween(scene, tmax);
}

bool Light::isDeltaLight() const {
    return type == POINT || type == DIRECTIONAL;
}


////Point light Source
PointLight::PointLight(Point3d _pos, Spectrum3d _I) : pos(_pos), I(_I), Light(POINT) {}

PointLight::~PointLight() {}

Point3d PointLight::getPos() const {
    return pos;
}


Spectrum3d PointLight::Sample_Li(const Intersection* inter, const Point2d u, Vector3d& wi,
                                 double &pdf, bool &vis, const Scene* scene) const {
    Point3d pS = inter->getInterPoint();
    wi = (pS - pos).normalized();
    pdf = 1.0;
    vis = testVisibility(pos, pS, scene);
    return I / distanceSquared(pos, pS);
}


////Directional light source
DirectionalLight::DirectionalLight(Vector3d _dir, Spectrum3d _I, Point3d wC, double wR) :
                    dir(_dir), I(_I), worldCenter(wC), worldRadius(wR), Light(DIRECTIONAL) {}

DirectionalLight::~DirectionalLight() {}

Spectrum3d DirectionalLight::Sample_Li(const Intersection* inter, const Point2d u, Vector3d &wi,
                                       double &pdf, bool &vis, const Scene *scene) const {
    wi = dir;
    pdf = 1.0;
    Point3d pS = inter->getInterPoint();
    Point3d pOutside = pS - dir * (2 * worldRadius); //"+" or "-" ???
    vis = testVisibility(pOutside, pS, scene);
    return I;
}



