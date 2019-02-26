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
    Point3d o;
    Vector3d d;
    double t;
    Intersection its;
    Spectrum3d radiance;
    
    bool brutalWayToFind(const Scene *scene);
    bool brutalWayToFindBetween(const Scene* scene, double tmax);
public:
    Ray();
    Ray(Point3d _o, Vector3d _d, double _t);
    ~Ray();
    Point3d getOrigin() const;
    Vector3d getDirection() const;
    double getT() const;
    Intersection* getIntersection();
    Spectrum3d getRadiance() const;
    
    Point3d calcP() const;
    bool isInit() const;
    void setRay(Point3d _o, Vector3d _d, double _t);
    void setT(double _t);
    void setRadiance(Spectrum3d rad);
    bool findIntersection(const Scene* scene); //this method will change t and its!
    bool findInterBetween(const Scene* scene, double tmax);
};

#endif /* Ray_h */
