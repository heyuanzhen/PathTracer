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
        POINT = 1
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
    virtual Spectrum3f Sample_Li(const Intersection inter, const Point2f &u, Vector3f& wi,
                                 float& pdf, bool& vis, Scene* scene) const;
};


#endif /* Light_h */
