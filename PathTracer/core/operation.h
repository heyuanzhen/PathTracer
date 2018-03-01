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

inline float lengthSquared(Vector3f vec) {
    return vec.x() * vec.x() + vec.y() * vec.y() + vec.z() * vec.z();
}

inline float distanceSquared(Point3f p1, Point3f p2) {
    return lengthSquared(p2 - p1);
}

inline float getMaxDistanceInOneDim(Point3f p1, Point3f p2, int& dim) {
    float deltaX = p2.x() - p1.x();
    float deltaY = p2.y() - p1.y();
    float deltaZ = p2.z() - p1.z();
    float maxD = 0.0;
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
