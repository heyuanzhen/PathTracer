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
    Point3f interP;
    Vector3f localN;
    Material* matetial;
public:
    Intersection(Ray* r, Shape* sp, Point3f iP, Vector3f lN, Material* mt);
    ~Intersection();
    
    Ray* getRay() const;
    Shape* getShape() const;
    Point3f getInterPoint() const;
    Vector3f getLocalNormal() const;
    Material* getMaterial() const;
};


#endif /* Intersection_h */
