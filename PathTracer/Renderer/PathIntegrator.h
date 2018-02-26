//
//  PathIntegrator.h
//  PathTracer
//
//  Created by HYZ on 2018/2/26.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#ifndef PathIntegrator_h
#define PathIntegrator_h

#include "Ray.h"


class PathIntegrator {
    Ray* ray;
    
public:
    PathIntegrator();
    ~PathIntegrator();
};

#endif /* PathIntegrator_h */
