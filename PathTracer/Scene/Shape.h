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

class AreaLight;

class Shape {
public:
    enum ShapeType{
        SPHERE = 1,
        TRIANGLE = 2,
        RECTANGULAR = 3
    };
    const ShapeType type;
    Transform trans;
    Material* material;
    bool isEmmit;
    AreaLight* areaLight;
    
    Shape(ShapeType tp, bool isE);
    ~Shape();
    
    void transRayToLocal(Ray* rayW, Ray& rayL);
    void setAreaLight(AreaLight* al);
    Material* getMaterial() const;
    AreaLight* getAreaLight() const;
    void setMaterial(Material* material);
    virtual Vector3d getNormal(Point3d pWorld) const = 0;
    virtual double isIntersected(Ray* ray) = 0;
    virtual double Area() const = 0;
    virtual Point3d sample(Point2d u, double& pdf) const;
    virtual Point3d sample(const Intersection* it, const Point2d u, double& pdf) const;
    virtual double pdf(const Intersection* itSur, Point3d pLi, Vector3d wi) const;
};

class Sphere : public Shape {
    double radius;
    Point3d center;
    
public:
    Sphere(double _r, Point3d _cenPos, bool isE);
    ~Sphere();
    
    Point3d getCenPos() const;
    double getR() const;
    virtual Vector3d getNormal(Vector3d pWorld) const;
    virtual double isIntersected(Ray* rayW);
    virtual double Area() const;
    virtual Point3d sample(Point2d u, double& pdf) const;
    virtual Point3d sample(const Intersection* it, const Point2d u, double& pdf) const;
    virtual double pdf(const Intersection* itSur, Point3d pLi, Vector3d wi) const;
};


class Triangle : public Shape {
    Point3d p0, p1, p2;
    Vector3d e1, e2, n;
public:
    Triangle(Point3d _p0, Point3d _p1, Point3d _p2, bool isE);
    ~Triangle();
    
    Point3d getPointByUV(double uu, double vv) const;
    void setNormal(Vector3d nor);
    virtual Vector3d getNormal(Point3d pWorld) const;
    virtual double isIntersected(Ray* ray);
    virtual Point3d sample(Point2d u, double& pdf) const;
    virtual Point3d sample(const Intersection* it, const Point2d u, double& pdf) const;
    virtual double pdf(const Intersection* itSur, Point3d pLi, Vector3d wi) const;
    virtual double Area() const;
};

class Rectangular : public Shape {
    Point3d p0, p1, p2, p3;
    Vector3d e1, e2;
    Triangle tri1, tri2;
public:
    Rectangular(Point3d _p0, Point3d _p1, Point3d _p2, bool isE);
    ~Rectangular();
    
    virtual Vector3d getNormal(Point3d pWorld) const;
    void setNormal(Vector3d nor);
    virtual double isIntersected(Ray* ray);
    virtual Point3d sample(Point2d u, double& pdf) const;
    virtual Point3d sample(const Intersection* it, const Point2d u, double& pdf) const;
    virtual double pdf(const Intersection* itSur, Point3d pLi, Vector3d wi) const;
    virtual double Area() const;
};


#endif /* Shape_h */

