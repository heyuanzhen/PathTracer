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
    bool testVisibility(Point3f pL, Point3f pS, Scene* scene) const;
    virtual Spectrum3f Sample_Li(const Intersection inter, const Point2f &u, Vector3f& wi,
                                 float& pdf, bool& vis, Scene* scene) const = 0;
};

class PointLight : public Light {
    const Point3f pos;
    const Spectrum3f I;
    
public:
    PointLight(Point3f _pos, Spectrum3f _I);
    ~PointLight();
    Point3f getPos() const;
    virtual Spectrum3f Sample_Li(const Intersection inter, const Point2f &u, Vector3f& wi,
                                 float& pdf, bool& vis, Scene* scene) const;
};

class DirectionalLight : public Light{
    const Vector3f dir;
    const Spectrum3f I;
    Point3f worldCenter;
    float worldRadius;
    
public:
    DirectionalLight(Vector3f _dir, Spectrum3f _I, Point3f wC, float wR);
    ~DirectionalLight();
    virtual Spectrum3f Sample_Li(const Intersection inter, const Point2f &u, Vector3f& wi,
                                 float& pdf, bool& vis, Scene* scene) const;
};

#endif /* Light_h */
