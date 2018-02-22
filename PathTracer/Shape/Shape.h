//
//  Shape.h
//  PathTracer
//
//  Created by HYZ on 2018/2/19.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#ifndef Shape_h
#define Shape_h

#include "typeAlias.h"
#include "globalConstants.h"
#include "Ray.h"
#include "Intersection.h"
#include "Transform.h"



class Shape {
public:
    enum ShapeType{
        SPHERE = 1
    };
    
    Shape(ShapeType tp);
    ~Shape();
    
    
    const ShapeType type;
    Transform trans;
    
    virtual Vector3f getNormal(Point3f pWorld) = 0;
    virtual bool isIntersected(Ray* ray, Intersection* its) = 0;
};

class Sphere : public Shape {
    float radius;
    Point3f center;
    
public:
    Sphere(float _r, Point3f _cenPos);
    ~Sphere();
    
    Point3f getCenPos() const;
    float getR() const;
    virtual Vector3f getNormal(Vector3f pWorld);
    virtual bool isIntersected(Ray* ray, Intersection* its);
};

#endif /* Shape_h */

