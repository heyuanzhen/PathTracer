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
ray(r), scene(sce), normalSampler(nsp), maxDepth(mD) {
    M.setZero();
    invM.setZero();
}

PathIntegrator::~PathIntegrator() {}

void PathIntegrator::calcRotateMartix(const Vector3d nW, const Vector3d nG){
    if ((abs(nW.x() - nG.x()) < eps) && (abs(nW.y() - nG.y()) < eps) && (abs(nW.z() - nG.z()) < eps)) {
        M.setIdentity();
        invM.setIdentity();
        return;
    }
    Vector3d v = nW.cross(nG);
    double s = v.norm();
    double c = nW.dot(nG);
    Matrix3d vx;
    vx << 0.0, -v.z(), v.y(),
    v.z(), 0.0, -v.x(),
    -v.y(), v.x(), 0.0;
    Matrix3d I = Eigen::Matrix3d::Identity();
    M = I + vx + vx * vx * (1.0 - c) / (s * s);
    invM = M.inverse();
}

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
    bool isSpecular = false;//need to be done
    return estimateDirectLightOnly(it, uScattering, light, uLight,
                                   scene, normalSampler, wo, false, isSpecular)/ lightPdf;
}


Spectrum3d PathIntegrator::estimateDirectLightOnly(const Intersection* it, const Point2d uScattering,
                                                   const Light* light, const Point2d uLight,
                                                   const Scene* scene, Sampler* sampler, Vector3d wo,
                                                   bool handleMedia, bool specular) {
    Spectrum3d Ld(0.0, 0.0, 0.0);
    Vector3d wi;
    double lightPdf = 0.0, scatteringPdf = 0.0;
    bool visibility;
    Spectrum3d f(0.0, 0.0, 0.0);
    Spectrum3d Li(0.0, 0.0, 0.0);
    Material* material = it->getMaterial();
    
    //⟨Sample light source with multiple importance sampling⟩
    //get Li, wi, lightPdf, visibility
    Li = light->Sample_Li(it, uLight, wi, lightPdf, visibility, scene);
//    std::cout<<"Li = "<<Li.transpose()<<", lightPdf = "<<lightPdf<<std::endl;
    if (!visibility) {
        return Spectrum3d(0.0, 0.0, 0.0);
    }

    if (lightPdf > 0 && !Li.isZero()) {
        if (!material) {
            std::cout<<"null material !"<<std::endl;
            return Spectrum3d(0.0, 0.0, 0.0);
        }
        //get f, scatteringPdf, woL, wiL
        Vector3d woL, wiL;
        material->eval(wo, -wi, woL, wiL, M, f, scatteringPdf); //wi := -wi
        f *= abs(wiL[2]);
//        std::cout<<"f = "<<f.transpose()<<std::endl;
        if (!f.isZero()) {
            if (!Li.isZero()) {
                if (light->isDeltaLight()){
//                    std::cout<<"f = "<<f.transpose()<<std::endl;
                    Ld += f.cwiseProduct(Li) / lightPdf;
                }
                else {
                    double weight = powerHeuristic(1, lightPdf, 1, scatteringPdf);
                    Ld += f.cwiseProduct(Li) * weight / lightPdf;
                }
            }
        }
    }
//    std::cout<<"Ld = "<<Ld.transpose()<<std::endl;
    
//    //⟨Sample BSDF with multiple importance sampling⟩
    if (!light->isDeltaLight()) {
        bool isInter = false;
        f = material->sampleBSDF(wo, wi, M, invM, scatteringPdf, specular, isInter);
        f *= abs(wi.dot(it->getLocalNormal()));
        if (!f.isZero() && scatteringPdf > 0.0) {
            double weight = 1.0;
            Spectrum3d Li(0.0, 0.0, 0.0);
            if (!specular && light->getShape()) {
                Ray rL;
                generateNewRay(it, wi, it->getLocalNormal(), false, rL);
                bool isInter = rL.findIntersection(scene);
                if (isInter) {
                    Intersection* interL = rL.getIntersection();
                    Light* al = interL->getShape()->getAreaLight();
                    if (al == light) {
//                        std::cout<<"here"<<std::endl;
                        Li = light->getShape()->getAreaLight()->L(interL->getInterPoint(), -wi);
                        lightPdf = interL->getShape()->pdf(it, interL->getInterPoint(), wi);
//                        std::cout<<"Li = "<<Li.transpose()<<"lightPdf = "<<lightPdf<<std::endl;
                        weight = powerHeuristic(1.0, scatteringPdf, 1.0, lightPdf);
                        Ld += f.cwiseProduct(Li) * weight / scatteringPdf;
//                        std::cout<<"scatteringPdf = "<<scatteringPdf<<std::endl;
//                        std::cout<<"weight = "<<weight<<", Ld = "<<Ld.transpose()<<std::endl<<std::endl;
                    }
                }

            }
        }
    }
    return Ld;
}

void PathIntegrator::generateNewRay(const Intersection* it, Vector3d wi, Vector3d nW, bool isEnter, Ray& r) {
    double flag = isEnter ? -10.0 : 10.0;
    Point3d newO = it->getInterPoint() + flag * nW * eps;
    r.setRay(newO, wi, 0.0);
}


Spectrum3d PathIntegrator::Li() {
    Spectrum3d L(0.0, 0.0, 0.0), beta(1.0, 1.0, 1.0);
    bool specularBounce = false;
    bool isEnter = false;

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
//        if(true){
            if (isInter && inter->getShape()->isEmmit) {
                Spectrum3d Le = inter->getShape()->getAreaLight()->L(inter->getInterPoint(), -ray->getDirection());
                L += beta.cwiseProduct(Le);
            }
            //⟨Addemittedlightatpathvertexorfromtheenvironment 877⟩ //(2)c
        }
        if (!isInter || bounces >= maxDepth)  break; //(3)
        
        //⟨Compute scattering functions and skip over medium boundaries⟩  //(4)
        Material* material = inter->getMaterial();
        calcRotateMartix(inter->getLocalNormal(), material->getGeometryNormal());
        
        //⟨Sample illumination from lights to find path contribution⟩ //(5)
        Spectrum3d beta_temp = beta;
        Spectrum3d Ld = beta_temp.cwiseProduct(uniformSampleOneLight(inter, scene, normalSampler,
                                                   -ray->getDirection(), false)); //wo = - ray.d
        L += Ld;
        
        //⟨Sample BSDF to get new path direction⟩ //(6)
        Vector3d wi(0.0, 0.0, 0.0), wo = -ray->getDirection(); //negative!
        double pdf;
        Spectrum3d f = material->sampleBSDF(wo, wi, M, invM, pdf, specularBounce, isEnter);
        if (f.isZero() || pdf == 0.f)   break;
        
        
        Spectrum3d betaOld = beta;
        beta = beta.cwiseProduct(f * abs(wi.dot(inter->getLocalNormal()))) / pdf;

        if(false){
            std::cout<<wi.dot(inter->getLocalNormal())<<std::endl;
            std::cout<<"beta_old = "<<betaOld.transpose()<<", beta = "<<beta.transpose()<<", pdf = "<<pdf
            <<", f = "<<f.transpose()<<", dot = "<<abs(wi.dot(inter->getLocalNormal()))
            <<", L = "<<L.transpose()<<", woW = "<<wo.transpose()<<", wiW = "<<wi.transpose()<<", lN = "<<inter->getLocalNormal().transpose()<<std::endl<<std::endl;
        }
        generateNewRay(inter, wi, inter->getLocalNormal(), isEnter, *ray);
        
        //⟨Account for subsurface scattering, if applicable⟩ (7)
        
        //⟨Possibly terminate the path with Russian roulette⟩ (8)
        
    }
    ray->setRadiance(L);
    return ray->getRadiance();
}


