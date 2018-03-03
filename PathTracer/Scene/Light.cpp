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

bool Light::testVisibility(Point3f pL, Point3f pS, const Scene* scene) const {
    Ray r = Ray(pL, (pS - pL).normalized(), 0.0);
    int dim = 0;
    float maxD = getMaxDistanceInOneDim(pL, pS, dim);
    float tmax = maxD / r.getDirection()[dim]; //pL and pS must be this order
//    std::cout<<"maxD = "<<maxD<<"maxDir = "<<r.getDirection()[dim]<<std::endl;
    return !r.findInterBetween(scene, tmax);
}


////Point light Source
PointLight::PointLight(Point3f _pos, Spectrum3f _I) : pos(_pos), I(_I), Light(POINT) {}

PointLight::~PointLight() {}

Point3f PointLight::getPos() const {
    return pos;
}

Spectrum3f PointLight::Sample_Li(const Intersection* inter, const Point2f u, Vector3f& wi,
                                 float &pdf, bool &vis, const Scene* scene) const {
    Point3f pS = inter->getInterPoint();
    wi = (pS - pos).normalized();
    pdf = 1.0;
    vis = testVisibility(pos, pS, scene);
    return I / distanceSquared(pos, pS);
}


////Directional light source
DirectionalLight::DirectionalLight(Vector3f _dir, Spectrum3f _I, Point3f wC, float wR) :
                    dir(_dir), I(_I), worldCenter(wC), worldRadius(wR), Light(DIRECTIONAL) {}

DirectionalLight::~DirectionalLight() {}

Spectrum3f DirectionalLight::Sample_Li(const Intersection* inter, const Point2f u, Vector3f &wi,
                                       float &pdf, bool &vis, const Scene *scene) const {
    wi = dir;
    pdf = 1.0;
    Point3f pS = inter->getInterPoint();
    Point3f pOutside = pS - dir * (2 * worldRadius); //"+" or "-" ???
    vis = testVisibility(pOutside, pS, scene);
    return I;
}



