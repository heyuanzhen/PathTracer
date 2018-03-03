//
//  PathIntegrator.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/26.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "PathIntegrator.h"
#include "Light.h"
#include <iostream>

//PathIntegrator::PathIntegrator() {
//    ray = nullptr;
//    scene = nullptr;
//    sampler = nullptr;
//    maxDepth = 0;
//}


PathIntegrator::PathIntegrator(Ray* r, Scene* sce, Sampler* nsp, int mD) :
ray(r), scene(sce), normalSampler(nsp), maxDepth(mD) {}

PathIntegrator::~PathIntegrator() {}

Spectrum3f PathIntegrator::Li() {
    Spectrum3f L(0.0, 0.0, 0.0), beta(1.0, 1.0, 1.0);
    bool specularBounce = false;
    for (int bounces = 0; ; bounces++) {
//        (1)⟨Intersect ray with scene and store intersection in isect 877⟩
//        (2)⟨Possibly add emitted light at intersection 877⟩
//        (3)⟨Terminate path if ray escaped or maxDepth was reached 877⟩
//        (4)⟨Compute scattering functions and skip over medium boundaries 878⟩
//        (5)⟨Sample illumination from lights  to find path contribution 878⟩
//        (6)⟨Sample BSDF to get new path direction 878⟩
//        (7)⟨Account for subsurface scattering, if applicable 915⟩
//        (8)⟨Possibly terminate the path with Russian roulette 879⟩
        bool isInter = ray->findIntersection(scene);    //
        Intersection* inter = ray->getIntersection();   //--(1)
        
        if (bounces == 0 || specularBounce) {
            //⟨Addemittedlightatpathvertexorfromtheenvironment 877⟩ //(2)c
        }
        
        if (!isInter || bounces >= maxDepth)  break; //(3)
        
        //⟨Compute scattering functions and skip over medium boundaries⟩  //(4)
        
        //⟨Sample illumination from lights to find path contribution⟩ //(5)
//        std::cout<<"here"<<std::endl;
        uniformSampleOneLight(inter, scene, normalSampler, false);
        
        
        //⟨Sample BSDF to get new path direction⟩ //(6)
        
        //⟨Account for subsurface scattering, if applicable⟩ (7)
        
        //⟨Possibly terminate the path with Russian roulette⟩ (8)
        
        L = Spectrum3f(1.0, 1.0, 1.0); // for test
    }
    ray->setRadiance(L);
    return ray->getRadiance();
}

Spectrum3f PathIntegrator::uniformSampleOneLight(const Intersection* it, const Scene* scene,
                                                 Sampler* sampler, bool handleMedia) {
    // Randomly choose a single light to sample, _light_
    int nLights = int(scene->getLightCount());
    if (nLights == 0) return Spectrum3f(0.0, 0.0, 0.0);
    int lightNum;
    float lightPdf;
    
    lightNum = std::min((int)(sampler->get1D() * nLights), nLights - 1);
    lightPdf = 1.0 / nLights;
    Light* light = scene->getLight(lightNum);
    Point2f uLight = sampler->get2D();
    Point2f uScattering = sampler->get2D();
//    bool isSpecular = it->getMaterial()-
    bool isSpecular = false;//need to be done
    return estimateDirectLightOnly(it, uScattering, light, uLight,
                                   scene, normalSampler, false, isSpecular)/ lightPdf;
//    return Spectrum3f(0.0, 0.0, 0.0);
}

Spectrum3f PathIntegrator::estimateDirectLightOnly(const Intersection* it, const Point2f uScattering,
                                                    const Light* light, const Point2f uLight,
                                                    const Scene* scene, Sampler* sampler,
                                                    bool handleMedia, bool specular) {
    Spectrum3f Ld(0.0, 0.0, 0.0);
    Vector3f wi;
    float lightPdf = 0.0, scatteringPdf = 0.0;
    bool visibility;
    Spectrum3f Li = light->Sample_Li(it, uLight, wi, lightPdf, visibility, scene);
    
    return Ld;
}



