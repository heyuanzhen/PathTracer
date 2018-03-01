//
//  Ray.h
//  PathTracer
//
//  Created by HYZ on 2018/2/19.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#ifndef Ray_h
#define Ray_h

#include "typeAlias.h"
#include "Scene.h"
#include "Intersection.h"

class Shape;

class Ray {
private:
    Point3f o;
    Vector3f d;
    float t;
    Intersection its;
    Spectrum3f radiance;
    
    bool brutalWayToFind(Scene *scene);
    bool brutalWayToFindBetween(Scene* scene, float tmax);
public:
    Ray();
    Ray(Point3f _o, Vector3f _d, float _t);
    ~Ray();
    Point3f getOrigin() const;
    Vector3f getDirection() const;
    float getT() const;
    Intersection* getIntersection();
    Spectrum3f getRadiance() const;
    
    Point3f calcP() const;
    bool isInit() const;
    void setRay(Point3f _o, Vector3f _d, float _t);
    void setT(float _t);
    void setRadiance(Spectrum3f rad);
    bool findIntersection(Scene* scene); //this method will change t and its!
    bool findInterBetween(Scene* scene, float tmax);
};

#endif /* Ray_h */
