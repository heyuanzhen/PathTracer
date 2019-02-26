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
#include "Sampler.h"

class Scene;

class Light {
public:
    enum LightType{
        POINT = 1,
        DIRECTIONAL = 2,
        AREA = 3,
        AMBIENT = 4
    };
    const LightType type;
    static double powerSum;
    
    Light(LightType tp);
    ~Light();
    bool testVisibility(Point3d pL, Point3d pS, const Scene* scene) const;
    bool isDeltaLight() const;
    virtual Spectrum3d Sample_Li(const Intersection* inter, const Point2d u, Vector3d& wi,
                                 double& pdf, bool& vis, const Scene* scene) const = 0;
    virtual double getPower() const = 0;
    virtual Shape* getShape() const;
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
    virtual double getPower() const;
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
    virtual double getPower() const;
};


class AreaLight : public Light {
private:
    Spectrum3d Lemit;
    Shape* shape;
public:
    AreaLight(Spectrum3d Le, Shape* sp);
    ~AreaLight();
    
    virtual Shape* getShape() const;
    virtual Spectrum3d Sample_Li(const Intersection* inter, const Point2d u, Vector3d& wi,
                                 double& pdf, bool& vis, const Scene* scene) const;
    Spectrum3d L(Point3d lightP, Vector3d wi) const;
    double pdf(Vector3d wi) const;
    virtual double getPower() const;
};

class AmbientLight : public Light {
private:
    Spectrum3d Lamb;
public:
    AmbientLight(Spectrum3d La);
    ~AmbientLight();
    
    virtual Spectrum3d Sample_Li(const Intersection* inter, const Point2d u, Vector3d& wi,
                                 double& pdf, bool& vis, const Scene* scene) const;
    virtual double getPower() const;
};

#endif /* Light_h */
