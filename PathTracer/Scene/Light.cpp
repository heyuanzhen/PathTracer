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

Light::Light(LightType tp) : type(tp) {}

Light::~Light() {}

bool Light::testVisibility(Point3f pL, Point3f pS, Scene* scene) const {
    Ray r = Ray(pL, (pS - pL).normalized(), 0.0);
    return r.findIntersection(scene); // should be modified!!!
}


////Point Light
PointLight::PointLight(Point3f _pos, Spectrum3f _I) : pos(_pos), I(_I), Light(POINT) {}

PointLight::~PointLight() {}

Spectrum3f PointLight::Sample_Li(const Intersection inter, const Point2f &u, Vector3f &wi,
                                 float &pdf, bool &vis, Scene* scene) const {
    Point3f pS = inter.getInterPoint();
    wi = (pS - pos).normalized();
    pdf = 1.0;
    vis = testVisibility(pos, pS, scene);
    return I / distanceSquared(pos, pS);
}
