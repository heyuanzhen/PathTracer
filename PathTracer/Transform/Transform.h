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

inline Point3f worldToLocalPoint(Matrix4f invM, Point3f pW) {
    Point4f homoPW(pW(0), pW(1), pW(2), 1.0);
    Point4f homoPL = invM * homoPW;
    return Point3f(homoPL(0), homoPL(1), homoPL(2));
}

inline Point3f localToWorldPoint(Matrix4f M, Point3f pL) {
    Point4f homoPL(pL(0), pL(1), pL(2), 1.0);
    Point4f homoPW = M * homoPL;
    return Point3f(homoPW(0), homoPW(1), homoPW(2));
}

class Transform {
    Matrix4f M;
    Matrix4f invM;
    Matrix4f MRot;
    Matrix4f invMRot;
    bool isInvMCalc;
public:
    Transform();
    ~Transform();
    
    void calcInvM();
    void setTranslation(float* ds);
    void setScale(float* s);
    void setRotateX(float rad);
    void setRotateY(float rad);
    void setRotateZ(float rad);
    void setIdentityM();
    
    Matrix4f getM();
    
    // you should call this function after called calcInvM() !
    Matrix4f getInvM();
    

    
};

#endif /* Transform_h */
