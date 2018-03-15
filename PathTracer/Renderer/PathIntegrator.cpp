//
//  PathIntegrator.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/26.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "PathIntegrator.h"
#include "Light.h"
#include "BxDF.h"
#include "Shape.h"
#include "sampling.h"
#include <iostream>

//PathIntegrator::PathIntegrator() {
//    ray = nullptr;
//    scene = nullptr;
//    sampler = nullptr;
//    maxDepth = 0;
//}


PathIntegrator::PathIntegrator(Ray* r, const Scene* sce, Sampler* nsp, int mD) :
ray(r), scene(sce), normalSampler(nsp), maxDepth(mD) {}

PathIntegrator::~PathIntegrator() {}

Spectrum3d PathIntegrator::uniformSampleOneLight(const Intersection* it, const Scene* scene,
                                                 Sampler* sampler, Vector3d wo, bool handleMedia) {
    // Randomly choose a single light to sample, _light_
    int nLights = int(scene->getLightCount());
    if (nLights == 0) return Spectrum3d(0.0, 0.0, 0.0);
    int lightNum = std::min((int)(sampler->get1D() * nLights), nLights - 1);
    double lightPdf = lightPdf = 1.0 / nLights;
    Light* light = scene->getLight(lightNum);
    Point2d uLight = sampler->get2D();
    Point2d uScattering = sampler->get2D();
    //    bool isSpecular = it->getMaterial()-
    bool isSpecular = false;//need to be done
    return estimateDirectLightOnly(it, uScattering, light, uLight,
                                   scene, normalSampler, wo, false, isSpecular)/ lightPdf;
    //    return Spectrum3d(0.0, 0.0, 0.0);
}

Spectrum3d PathIntegrator::estimateDirectLightOnly(const Intersection* it, const Point2d uScattering,
                                                   const Light* light, const Point2d uLight,
                                                   const Scene* scene, Sampler* sampler,
                                                   Vector3d wo, bool handleMedia, bool specular) {
    Spectrum3d Ld(0.0, 0.0, 0.0);
    Vector3d wi;
    double lightPdf = 0.0, scatteringPdf = 0.0;
    bool visibility;
    
    //get Li, wi, lightPdf, visibility
    Spectrum3d Li = light->Sample_Li(it, uLight, wi, lightPdf, visibility, scene);
//    std::cout<<"Li = "<<Li.transpose()<<", pdf = "<<lightPdf<<std::endl;
    
    if (!visibility) {
//        std::cout<<"occ"<<std::endl;
        return Spectrum3d(0.0, 0.0, 0.0);
    }
    
    if (lightPdf > 0 && !Li.isZero()) {
        Spectrum3d f;
        // Evaluate BSDF for light sampling strategy
        Material* material = it->getMaterial();
        if (!material) {
            std::cout<<"null material !"<<std::endl;
            return Spectrum3d(0.0, 0.0, 0.0);
        }
        
        //get f, scatteringPdf, woL, wiL
        Vector3d woL, wiL;
        material->eval(wo, -wi, woL, wiL, f, scatteringPdf); //wi := -wi
//        std::cout<<"wiL = "<<wiL.transpose()<<std::endl;
        f *= abs(wiL.dot(material->getGeometryNormal()));
//        std::cout<<"f = 0"<<std::endl;
        
        if (!f.isZero()) {
//            if (!visibility)    Li = Spectrum3d(0.0, 0.0, 0.0);
            if (!Li.isZero()) {
                if (light->isDeltaLight()){
                    Ld += f.cwiseProduct(Li) / lightPdf;
                }
                else {
                    double weight = powerHeuristic(1, lightPdf, 1, scatteringPdf);
                    Ld += f.cwiseProduct(Li) * weight / lightPdf;
                }
            }
            else{
                std::cout<<"Li = 0"<<std::endl;
            }
        }
        else {
//            std::cout<<"f = 0"<<std::endl;
        }
    }
    
    return Ld;
}

void PathIntegrator::generateNewRay(Intersection* it, Vector3d wi, Vector3d nW) {
    Point3d newO = it->getInterPoint() + nW * eps;
    ray->setRay(newO, wi, 0.0);
}


Spectrum3d PathIntegrator::Li() {
    Spectrum3d L(0.0, 0.0, 0.0), beta(1.0, 1.0, 1.0);
    bool specularBounce = false;
    Spectrum3d betaList[10];
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
            if (isInter && inter->getShape()->isEmmit) {
                Spectrum3d Le = inter->getShape()->getAreaLight()->L(inter->getInterPoint(), -ray->getDirection());
                L += beta.cwiseProduct(Le);
            }
            //⟨Addemittedlightatpathvertexorfromtheenvironment 877⟩ //(2)c
        }
//        if( bounces > 0)    std::cout<<bounces<<std::endl;
        if (!isInter || bounces >= maxDepth)  break; //(3)
        
        
        //⟨Compute scattering functions and skip over medium boundaries⟩  //(4)
        Material* material = inter->getMaterial();
        material->calcRotateMartix(inter->getLocalNormal());
        
        //⟨Sample illumination from lights to find path contribution⟩ //(5)
//        std::cout<<"here"<<std::endl;
        Spectrum3d beta_temp = beta;
        Spectrum3d Ld = beta_temp.cwiseProduct(uniformSampleOneLight(inter, scene, normalSampler,
                                                   -ray->getDirection(), false)); //wo = - ray.d
//        std::cout<<"Ld = "<<Ld<<std::endl;
        L += Ld;
        
        //⟨Sample BSDF to get new path direction⟩ //(6)
        Vector3d wi, wo = -ray->getDirection();
//        std::cout<<"woW = "<<wo.transpose()<<std::endl;
        double pdf;
        Spectrum3d f = material->sampleBSDF(wo, wi, pdf);
        if (f.isZero() || pdf == 0.f)   break;
        
//        bool judge = bounces == 1;
        bool judge = beta.norm() > sqrt(3);
        if(judge){
            std::cout<<bounces<<std::endl;
            std::cout<<"beta = "<<beta.transpose()<<", pdf = "<<pdf<<", f = "<<f.transpose()<<
            ", dot = "<<abs(wi.dot(inter->getLocalNormal()))<<std::endl;
        }
        beta = beta.cwiseProduct(f * abs(wi.dot(inter->getLocalNormal()))) / pdf;
        if(judge){
            std::cout<<"beta = "<<beta.transpose()<<", pdf = "<<pdf<<", f = "<<f.transpose()<<
            ", dot = "<<abs(wi.dot(inter->getLocalNormal()))<<", L = "<<L.transpose()<<std::endl<<std::endl;
        }
        betaList[bounces] = beta;
        generateNewRay(inter, wi, inter->getLocalNormal());
        
        //⟨Account for subsurface scattering, if applicable⟩ (7)
        
        //⟨Possibly terminate the path with Russian roulette⟩ (8)
        
//        L = Spectrum3d(1.0, 1.0, 1.0); // for test
    }
//    if (L.norm() > 10.0) {
//        std::cout<<"Big L = "<<L.transpose()<<std::endl;
//    }
    ray->setRadiance(L);
    return ray->getRadiance();
}


