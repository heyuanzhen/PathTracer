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
    const Scene* scene;
    Sampler* normalSampler;
    int maxDepth;
    Matrix3d M; //M is a matrix that transform a normal to (0, 0, 1)
    Matrix3d invM;
public:
//    PathIntegrator();
    PathIntegrator(Ray* r, const Scene* sce, Sampler* nsp, int mD);
    ~PathIntegrator();
    void calcRotateMartix(const Vector3d nW, const Vector3d nG);
    Spectrum3d uniformSampleOneLight(const Intersection* it, const Scene* scene,
                                     Sampler* sampler, Vector3d wo, bool handleMedia);
    Spectrum3d estimateDirectLightOnly(const Intersection* it, const Point2d uScattering,
                                       const Light* light, const Point2d uLight,
                                       const Scene* scene, Sampler* sampler, Vector3d wo,
                                       bool handleMedia, bool specular);
    void generateNewRay(const Intersection* it, Vector3d wi, Vector3d nW, bool isEnter, Ray& r);
    Spectrum3d Li();
    
};

#endif /* PathIntegrator_h */
