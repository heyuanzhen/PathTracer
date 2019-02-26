//
//  Intersection.h
//  PathTracer
//
//  Created by HYZ on 2018/2/21.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#ifndef Intersection_h
#define Intersection_h

#include "typeAlias.h"
#include "Material.h"

class Shape;
class Ray;

class Intersection {
private:
    Ray* ray;
    Shape* shape;
    Point3d interP;
    Vector3d localN;
    Material* matetial;
    bool isInter;
public:
    Intersection();
    Intersection(Ray* r, Shape* sp, Point3d iP, Vector3d lN, Material* mt);
    ~Intersection();
    
    Ray* getRay() const;
    Shape* getShape() const;
    Point3d getInterPoint() const;
    Vector3d getLocalNormal() const;
    Material* getMaterial() const;
    Ray* generateNewRay() const;
    bool getIsInter() const;
};


#endif /* Intersection_h */
