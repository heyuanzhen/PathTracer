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
Material::Material(materialType mt, BSDF* bs) : mType(mt), bsdf(bs) {
    M.setZero();
    invM.setZero();
}

Material::~Material() {}

void Material::calcRotateMartix(const Vector3d nW){
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

Vector3d Material::rotateNormalToLocal(const Vector3d vW) const {
    if (M.isZero()) {
        std::cout<<"M is a zero matrix !"<<std::endl;
        exit(0);
    }
    return M * vW;
}

Vector3d Material::rotateNormalToWorld(const Vector3d vL) const {
    if (invM.isZero()) {
        std::cout<<"invM is a zero matrix !"<<std::endl;
        exit(0);
    }
    return invM * vL;
}

Vector3d Material::getGeometryNormal() const {
    return nG;
}

Matrix3d Material::getM() const {
    return M;
}


void Material::eval(const Vector3d woW, const Vector3d wiW,
                    Vector3d& woL, Vector3d& wiL,
                    Spectrum3d& f, double& pdf) const {
    if (!bsdf->getBuilt()) {
        std::cout<<"The BSDF is not build !"<<std::endl;
        return;
    }
    woL = rotateNormalToLocal(woW);
    wiL = rotateNormalToLocal(wiW);
//    std::cout<<"wo = "<<wo.transpose()<< ", wi = "<<wi.transpose()<<std::endl;
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
    for (int i = 0; i < bsdf->getBxDFCount(); i++) {
        pdf += bsdf->bxdfs[i]->calcPDF(woL, wiL);
        pdfCount++;
        if (bsdf->bxdfs[i]->getType() != BxDF::TRANSMISSION) {
            f += bsdf->bxdfs[i]->eval(woL, wiL);
        }
    }
//    std::cout<<"pdfCount = "<<pdfCount<<std::endl;
//    pdf /= (1.0 * pdfCount);
//    std::cout<<"f = "<<f.transpose()<< ", pdf = "<<pdf<<std::endl;
}

Spectrum3d Material::sampleBSDF(const Vector3d woW, Vector3d& wiW, double &pdf) const {
    Vector3d woL = rotateNormalToLocal(woW);
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
    int bxdfNum = std::min((int)(rsp.get1D() * BxDFCount), BxDFCount - 1);
    BxDF* bxdf = bsdf->bxdfs[bxdfNum];
    Vector3d wiL;
    f = bxdf->sampleWiAndEval(woL, wiL, rsp.get2D(), pdf);
    wiW = rotateNormalToWorld(wiL);
    
    int pdfCount = 0;
    for (int i = 0; i < BxDFCount; i++) {
        if (bsdf->bxdfs[i] != bxdf) {
            pdf += bsdf->bxdfs[i]->calcPDF(woL, wiL);
            pdfCount++;
            if (bsdf->bxdfs[i]->getType() != BxDF::TRANSMISSION) {
                f += bsdf->bxdfs[i]->eval(woL, wiL);
            }
        }
    }
    pdf /= (1.0 * pdfCount);
    
    return f;
}

////BlinnPhong







