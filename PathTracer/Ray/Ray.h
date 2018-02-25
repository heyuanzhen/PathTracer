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
    Point3f o;
    Vector3f d;
    float t;
    Intersection* its;
public:
    Ray();
    Ray(Point3f _o, Vector3f _d, float _t);
    ~Ray();
    Point3f getOrigin() const;
    Vector3f getDirection() const;
    float getT() const;
    void setRay(Point3f _o, Vector3f _d, float _t);
    void setT(float _t);
    void findIntersection(Scene* scene);
    void calcInterPoint();
    
    void brutalWayToFind(Scene *scene);
};

#endif /* Ray_h */
