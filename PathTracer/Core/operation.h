//
//  operation.h
//  PathTracer
//
//  Created by HYZ on 2018/2/28.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#ifndef operation_h
#define operation_h

#include "typeAlias.h"

inline double lengthSquared(Vector3d vec) {
    return vec.x() * vec.x() + vec.y() * vec.y() + vec.z() * vec.z();
}

inline double distanceSquared(Point3d p1, Point3d p2) {
    return lengthSquared(p2 - p1);
}

inline double getMaxDistanceInOneDim(Point3d p1, Point3d p2, int& dim) {
    double deltaX = p2.x() - p1.x();
    double deltaY = p2.y() - p1.y();
    double deltaZ = p2.z() - p1.z();
    double maxD = 0.0;
    if (abs(deltaX) > abs(deltaY)) {
        if (abs(deltaX) > abs(deltaZ)) {
            maxD = deltaX;
            dim = 0;
        }
        else {
            maxD = deltaZ;
            dim = 2;
        }
    }
    else{
        if (abs(deltaY) > abs(deltaZ)) {
            maxD = deltaY;
            dim = 1;
        }
        else {
            maxD = deltaZ;
            dim = 2;
        }
    }
    return maxD;
}

inline Matrix3d calcRotateMartix(const Vector3d nW, const Vector3d nG){
    Matrix3d M;
    if ((abs(nW.x() - nG.x()) < eps) && (abs(nW.y() - nG.y()) < eps) && (abs(nW.z() - nG.z()) < eps)) {
        M.setIdentity();
        return M;
    }
    Vector3d v = nW.cross(nG);
    double s = v.norm();
    double c = nW.dot(nG);
    Matrix3d vx;
    vx << 0.0, -v.z(), v.y(),
    v.z(), 0.0, -v.x(),
    -v.y(), v.x(), 0.0;
    Matrix3d I = Eigen::Matrix3d::Identity();
    M = I + vx + vx * vx * (1.0 - c) / (s * s);
    return M;
}

#endif /* operation_h */
