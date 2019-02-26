//
//  Transform.h
//  PathTracer
//
//  Created by HYZ on 2018/2/19.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#ifndef Transform_h
#define Transform_h

#include "typeAlias.h"
#include "Ray.h"

inline Point3d worldToLocal(Matrix4d invM, Point3d pW) {
    Point4d homoPW(pW(0), pW(1), pW(2), 1.0);
    Point4d homoPL = invM * homoPW;
    return Point3d(homoPL(0), homoPL(1), homoPL(2));
}

inline Point3d localToWorld(Matrix4d M, Point3d pL) {
    Point4d homoPL(pL(0), pL(1), pL(2), 1.0);
    Point4d homoPW = M * homoPL;
    return Point3d(homoPW(0), homoPW(1), homoPW(2));
}

class Transform {
    Matrix4d M;
    Matrix4d invM;
    Matrix4d MRot;
    Matrix4d invMRot;
    bool isInvMCalc;
    bool isInvMRotCalc;
public:
    Transform();
    ~Transform();
    
    void calcInvM();
    void calcInvMRot();
    void setTranslation(double* ds);
    void setScale(double* s);
    void setRotateX(double rad);
    void setRotateY(double rad);
    void setRotateZ(double rad);
    void setIdentityM();
    
    Matrix4d getM() const;
    Matrix4d getMRot() const;
    Matrix4d getInvM();
    Matrix4d getInvMRot();
    

    
};

#endif /* Transform_h */
