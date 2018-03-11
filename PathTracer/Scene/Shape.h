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
        SPHERE = 1,
        TRIANGLE = 2
    };
    const ShapeType type;
    Transform trans;
    Material* material;
    
    Shape(ShapeType tp);
    ~Shape();
    
    void transRayToLocal(Ray* rayW, Ray& rayL);
    Material* getMaterial() const;
    void setMaterial(Material* material);
    virtual Vector3d getNormal(Point3d pWorld) = 0;
    virtual double isIntersected(Ray* ray) = 0;
};

class Sphere : public Shape {
    double radius;
    Point3d center;
    
public:
    Sphere(double _r, Point3d _cenPos);
    ~Sphere();
    
    Point3d getCenPos() const;
    double getR() const;
    virtual Vector3d getNormal(Vector3d pWorld);
    virtual double isIntersected(Ray* rayW);
};


class Triangle : public Shape {
    Point3d p0, p1, p2;
    Vector3d e1, e2, n;
public:
    Triangle(Point3d _p0, Point3d _p1, Point3d _p2);
    ~Triangle();
    
    Point3d getPointByUV(double uu, double vv) const;
    virtual Vector3d getNormal(Point3d pWorld);
    virtual double isIntersected(Ray* ray);
};

#endif /* Shape_h */

