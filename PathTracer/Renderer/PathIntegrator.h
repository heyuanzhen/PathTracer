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
#include "Scene.h"
#include "Sampler.h"

class PathIntegrator{
    Ray* ray;
    Scene* scene;
    Sampler* sampler;
    int maxDepth;
public:
//    PathIntegrator();
    PathIntegrator(Ray* r, Scene* sce, Sampler* sp, int mD);
    ~PathIntegrator();
    
    Spectrum3f Li();
};

#endif /* PathIntegrator_h */
