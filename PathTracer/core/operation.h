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

#endif /* operation_h */
