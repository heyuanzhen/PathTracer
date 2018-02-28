//
//  PathIntegrator.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/26.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "PathIntegrator.h"

//PathIntegrator::PathIntegrator() {
//    ray = nullptr;
//    scene = nullptr;
//    sampler = nullptr;
//    maxDepth = 0;
//}

PathIntegrator::PathIntegrator(Ray* r, Scene* sce, Sampler* sp, int mD) :
ray(r), scene(sce), sampler(sp), maxDepth(mD) {}

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
        
        //⟨Sample BSDF to get new path direction⟩ //(6)
        
        //⟨Account for subsurface scattering, if applicable⟩ (7)
        
        //⟨Possibly terminate the path with Russian roulette⟩ (8)
        
        L = Spectrum3f(1.0, 1.0, 1.0); // for test
    }
    ray->setRadiance(L);
    return ray->getRadiance();
}
