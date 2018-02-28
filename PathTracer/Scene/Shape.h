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
#include "Intersection.h"
#include "Transform.h"


class Shape {
public:
    enum ShapeType{
        SPHERE = 1
    };
    const ShapeType type;
    Transform trans;
    Material* material;
    
    Shape(ShapeType tp);
    ~Shape();
    
    void transRayToLocal(Ray* rayW, Ray& rayL);
    Material* getMaterial() const;
    void setMaterial(Material* material);
    virtual Vector3f getNormal(Point3f pWorld) = 0;
    virtual float isIntersected(Ray* ray) = 0;
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
    virtual float isIntersected(Ray* rayW);
};

#endif /* Shape_h */

