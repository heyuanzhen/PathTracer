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
    Sampler* normalSampler;
    int maxDepth;
public:
//    PathIntegrator();
    PathIntegrator(Ray* r, Scene* sce, Sampler* nsp, int mD);
    ~PathIntegrator();
    
    Spectrum3f Li();
    Spectrum3f uniformSampleOneLight(const Intersection* it, const Scene* scene,
                          Sampler* sampler, Vector3f wo, bool handleMedia);
    Spectrum3f estimateDirectLightOnly(const Intersection* it, const Point2f uScattering,
                                       const Light* light, const Point2f uLight,
                                       const Scene* scene, Sampler* sampler,
                                       Vector3f wo, bool handleMedia, bool specular);
};

#endif /* PathIntegrator_h */
