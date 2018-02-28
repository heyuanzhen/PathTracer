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


#endif /* operation_h */
