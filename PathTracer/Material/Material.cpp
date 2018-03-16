//
//  Material.cpp
//  PathTracer
//
//  Created by HYZ on 2018/2/8.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "Material.h"
#include "Sampler.h"
#include <iostream>

////Material
Material::Material(materialType mt, BSDF* bs) : mType(mt), bsdf(bs) {}

Material::~Material() {}

Vector3d Material::rotateNormalToLocal(const Vector3d vW, const Matrix3d M) const {
    if (M.isZero()) {
        std::cout<<"M is a zero matrix !"<<std::endl;
        exit(0);
    }
    return M * vW;
}

Vector3d Material::rotateNormalToWorld(const Vector3d vL, const Matrix3d invM) const {
    if (invM.isZero()) {
        std::cout<<"invM is a zero matrix !"<<std::endl;
        exit(0);
    }
    return invM * vL;
}

Vector3d Material::getGeometryNormal() const {
    return nG;
}


int Material::decideWhichBxDFToSample() const {
    double u = (rand() * 1.0) / (RAND_MAX * 1.0);
    int ind = 0;
    while(u > 0) {
        u -= bsdf->bxdfs[ind]->getWeight() / bsdf->getWeightSum();
        if (u > 0) {
            ind++;
        }
    }
    return ind;
}


void Material::eval(const Vector3d woW, const Vector3d wiW,
                    Vector3d& woL, Vector3d& wiL, const Matrix3d M,
                    Spectrum3d& f, double& pdf) const {
    if (!bsdf->getBuilt()) {
        std::cout<<"The BSDF is not build !"<<std::endl;
        return;
    }
    woL = rotateNormalToLocal(woW, M);
    wiL = rotateNormalToLocal(wiW, M);
//    std::cout<<"M = "<<M<<std::endl;
//    std::cout<<"woL = "<<woL.transpose()<< ", wiL = "<<wiL.transpose()<<std::endl;
    if (!isSameHemisphere(woL, wiL)) {
//        std::cout<<"Back ray !"<<" woW = "<<woL.transpose()<< ", wiW = "<<wiL.transpose()<<std::endl;
        f = Spectrum3d(0.0, 0.0, 0.0);
        pdf = 0.0;
        return;
    }
    
    f = Spectrum3d(0.0, 0.0, 0.0);
    pdf = 0.0;
    
    if (!bsdf->getBxDFCount()) {
        std::cout<<"Empty material !"<<std::endl;
        return;
    }

    int pdfCount = 0;
//    std::cout<<bsdf->getBxDFCount()<<std::endl;
    for (int i = 0; i < bsdf->getBxDFCount(); i++) {
//        pdf += bsdf->bxdfs[i]->calcPDF(woL, wiL);
        pdf += bsdf->bxdfs[i]->getWeight() * bsdf->bxdfs[i]->calcPDF(woL, wiL) / bsdf->getWeightSum();
        pdfCount++;
        if (bsdf->bxdfs[i]->getType() != BxDF::TRANSMISSION) {
            f += bsdf->bxdfs[i]->eval(woL, wiL);
        }
    }
//    std::cout<<"pdfCount = "<<pdfCount<<std::endl;
//    pdf /= (1.0 * pdfCount);
//    std::cout<<"f = "<<f.transpose()<< ", pdf = "<<pdf<<std::endl;
}

Spectrum3d Material::sampleBSDF(const Vector3d woW, Vector3d& wiW, const Matrix3d M,
                                const Matrix3d invM, double &pdf, bool& specularBounces) const {
    Vector3d woL = rotateNormalToLocal(woW, M);
    Spectrum3d f = Spectrum3d(0.0, 0.0, 0.0);
    pdf = 0.0;
    if (!bsdf->getBuilt()) {
        std::cout<<"The BSDF is not build !"<<std::endl;
        return f;
    }
    if (!bsdf->getBxDFCount()) {
        std::cout<<"Empty material !"<<std::endl;
        return f;
    }
    
    RandomSampler rsp;
    int BxDFCount = bsdf->getBxDFCount();
    int bxdfNum = decideWhichBxDFToSample();
    BxDF *bxdf = bsdf->bxdfs[bxdfNum];
    
    if (!bxdf) {
        std::cout<<bxdfNum<<std::endl;
        std::cout<<"NULL BxDF !"<<std::endl;
        exit(0);
    }
    
    Vector3d wiL;
    f = bxdf->sampleWiAndEval(woL, wiL, rsp.get2D(), pdf);
    pdf *= bxdf->getWeight() / bsdf->getWeightSum();
    wiW = rotateNormalToWorld(wiL, invM);
//    if (bxdf->getType() == BxDF::SPECULAR) {
//        std::cout<<"wiW = "<<wiW.transpose()<<std::endl<<std::endl;
//    }
    
    specularBounces = bxdf->getType() == BxDF::SPECULAR ? true : false;
    
    for (int i = 0; i < BxDFCount; i++) {
        if (bsdf->bxdfs[i] != bxdf) {
            pdf += bsdf->bxdfs[i]->getWeight() * bsdf->bxdfs[i]->calcPDF(woL, wiL) / bsdf->getWeightSum();
            if (bsdf->bxdfs[i]->getType() != BxDF::TRANSMISSION) {
                f += bsdf->bxdfs[i]->eval(woL, wiL);
            }
        }
    }
    return f;
}

////BlinnPhong







