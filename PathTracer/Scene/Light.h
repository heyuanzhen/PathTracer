//
//  Light.h
//  PathTracer
//
//  Created by HYZ on 2018/2/20.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#ifndef Light_h
#define Light_h

#include "typeAlias.h"
#include "Intersection.h"

class Scene;

class Light {
public:
    enum LightType{
        POINT = 1,
        DIRECTIONAL = 2
    };
    const LightType type;
    
    Light(LightType tp);
    ~Light();
    bool testVisibility(Point3d pL, Point3d pS, const Scene* scene) const;
    bool isDeltaLight() const;
    virtual Spectrum3d Sample_Li(const Intersection* inter, const Point2d u, Vector3d& wi,
                                 double& pdf, bool& vis, const Scene* scene) const = 0;
};

class PointLight : public Light {
    const Point3d pos;
    const Spectrum3d I;
    
public:
    PointLight(Point3d _pos, Spectrum3d _I);
    ~PointLight();
    Point3d getPos() const;
    virtual Spectrum3d Sample_Li(const Intersection* inter, const Point2d u, Vector3d& wi,
                                 double& pdf, bool& vis, const Scene* scene) const;
};

class DirectionalLight : public Light{
    const Vector3d dir;
    const Spectrum3d I;
    Point3d worldCenter;
    double worldRadius;
    
public:
    DirectionalLight(Vector3d _dir, Spectrum3d _I, Point3d wC, double wR);
    ~DirectionalLight();
    virtual Spectrum3d Sample_Li(const Intersection* inter, const Point2d u, Vector3d& wi,
                                 double& pdf, bool& vis, const Scene* scene) const;
};

#endif /* Light_h */
